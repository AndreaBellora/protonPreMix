#include "protonPreMix/protonPreMix/interface/CTPPSPreMixProducer.h"

CTPPSPreMixProducer::CTPPSPreMixProducer(const edm::ParameterSet &conf)
    : verbosity_(conf.getUntrackedParameter<int>("Verbosity")),
      includePixels_(conf.getParameter<bool>("includePixels")),
      includeStrips_(conf.getParameter<bool>("includeStrips")),
      simPixelSrc_(conf.getParameter<edm::InputTag>("Sim_CTPPSPixelRecHitTag")),

      simStripsSrc_(conf.getParameter<edm::InputTag>("Sim_TotemRPRecHitTag")),
      puFileReader_(conf.getParameter<std::vector<std::string>>("PUFilesList"),
                    puPixelSrc_ = conf.getParameter<edm::InputTag>(
                        "PU_CTPPSPixelRecHitTag"),
                    puStripsSrc_ = conf.getParameter<edm::InputTag>(
                        "PU_TotemRPRecHitTag")),
      puEntries_(puFileReader_.GetEntries()) {
  if (includePixels_) {
    tokenCTPPSPixelRecHit_ =
        consumes<edm::DetSetVector<CTPPSPixelRecHit>>(simPixelSrc_);
    produces<edm::DetSetVector<CTPPSPixelRecHit>>();
  }
  if (includeStrips_) {
    tokenTotemRPRecHit_ =
        consumes<edm::DetSetVector<TotemRPRecHit>>(simStripsSrc_);
    produces<edm::DetSetVector<TotemRPRecHit>>();
  }

  const std::vector<edm::LuminosityBlockRange> lumiTemp =
      conf.getUntrackedParameter<std::vector<edm::LuminosityBlockRange>>(
          "lumisToProcess");
  lumisToProcess_.resize(lumiTemp.size());
  copy(lumiTemp.begin(), lumiTemp.end(), lumisToProcess_.begin());

  puFileReader_.setLumisToProcess(lumisToProcess_);

  if (verbosity_ > 0)
    edm::LogInfo("PPS") << "PU files contain " << puEntries_ << " events";
}

CTPPSPreMixProducer::~CTPPSPreMixProducer() {}

void CTPPSPreMixProducer::fillDescriptions(
    edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<int>("Verbosity", 0);
  desc.add<std::vector<std::string>>("PUFilesList", std::vector<std::string>());
  desc.add<edm::InputTag>("Sim_CTPPSPixelRecHitTag",
                          edm::InputTag("ctppsDirectProtonSimulation"));
  desc.add<edm::InputTag>("PU_CTPPSPixelRecHitTag",
                          edm::InputTag("ctppsPixelRecHits"));
  desc.add<edm::InputTag>("Sim_TotemRPRecHitTag",
                          edm::InputTag("ctppsDirectProtonSimulation"));
  desc.add<edm::InputTag>("PU_TotemRPRecHitTag",
                          edm::InputTag("totemRPRecHitProducer"));
  desc.add<bool>("includePixels", true);
  desc.add<bool>("includeStrips", true);
  desc.addUntracked<std::vector<edm::LuminosityBlockRange>>(
      "lumisToProcess", std::vector<edm::LuminosityBlockRange>());
  descriptions.add("ctppsPreMixProducer", desc);
}

void CTPPSPreMixProducer::produce(edm::Event &iEvent,
                                  const edm::EventSetup &iSetup) {
  using namespace std;

  // Retrieve an event from PU files
  const edm::Service<edm::RandomNumberGenerator> rng;
  if (!rng.isAvailable()) {
    throw cms::Exception("Configuration")
        << "CTPPSPreMixProducer requires the RandomNumberGeneratorService,\n"
           "which is not present in the configuration file. You must add\n"
           "the service in the configuration file.\n";
  }

  edm::DetSetVector<CTPPSPixelRecHit> pixelsOutput;
  edm::DetSetVector<TotemRPRecHit> stripsOutput;

  bool isPUinError = false;
  do {
    CLHEP::HepRandomEngine &engine = rng->getEngine(iEvent.streamID());
    int entryNumber = CLHEP::RandFlat::shootInt(&engine, 0, puEntries_);

    if (verbosity_ > 0) {
      edm::LogInfo("PPS") << "Picking PU event number: " << entryNumber;
    }

    isPUinError = false;
    if (!puFileReader_.getAndCheckEvent(entryNumber))
      continue;

    if (includePixels_) {
      edm::Handle<edm::DetSetVector<CTPPSPixelRecHit>> simPixelRpRecHits;
      iEvent.getByToken(tokenCTPPSPixelRecHit_, simPixelRpRecHits);

      edm::DetSetVector<CTPPSPixelRecHit> puPixelRpRecHits;
      if (!puFileReader_.getRecHitsDsv(
              puPixelRpRecHits, CTPPSDetId::SubDetector::sdTrackingPixel))
        isPUinError = true;

      if (simPixelRpRecHits.isValid())
        merge(*simPixelRpRecHits, puPixelRpRecHits, pixelsOutput);
      else
        std::cout << "Pixel rechits collection not valid" << std::endl;
    }

    if (includeStrips_) {
      edm::Handle<edm::DetSetVector<TotemRPRecHit>> simStripsRpRecHits;
      iEvent.getByToken(tokenTotemRPRecHit_, simStripsRpRecHits);

      edm::DetSetVector<TotemRPRecHit> puStripsRpRecHits;
      if (!puFileReader_.getRecHitsDsv(
              puStripsRpRecHits, CTPPSDetId::SubDetector::sdTrackingStrip))
        isPUinError = true;

      if (simStripsRpRecHits.isValid())
        merge(*simStripsRpRecHits, puStripsRpRecHits, stripsOutput);
      else
        std::cout << "Strips rechits collection not valid" << std::endl;
    }
  } while (isPUinError);

  if (includePixels_)
    iEvent.put(
        std::make_unique<edm::DetSetVector<CTPPSPixelRecHit>>(pixelsOutput));
  if (includeStrips_)
    iEvent.put(
        std::make_unique<edm::DetSetVector<TotemRPRecHit>>(stripsOutput));
}

template <class T>
void CTPPSPreMixProducer::merge(const edm::DetSetVector<T> &simRecHits,
                                const edm::DetSetVector<T> &puRecHits,
                                edm::DetSetVector<T> &output) {

  // Merge the two inputs
  for (auto simRecHits_ds : simRecHits) {
    edm::DetSet<T> &outputRecHits_ds = output.find_or_insert(simRecHits_ds.id);
    for (auto simRecHit : simRecHits_ds.data) {
      outputRecHits_ds.push_back(simRecHit);
    }
  }

  for (auto puRecHits_ds : puRecHits) {
    edm::DetSet<T> &outputRecHits_ds = output.find_or_insert(puRecHits_ds.id);
    for (auto puRecHit : puRecHits_ds.data) {
      outputRecHits_ds.push_back(puRecHit);
    }
  }
}

// define this as a plugin
DEFINE_FWK_MODULE(CTPPSPreMixProducer);
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
  descriptions.add("ctppsPreMixProducer", desc);
}

void CTPPSPreMixProducer::produce(edm::Event &iEvent,
                                  const edm::EventSetup &iSetup) {
  using namespace std;

  // Retrieve an event from PU files
  edm::Service<edm::RandomNumberGenerator> rng;
  if (!rng.isAvailable()) {
    throw cms::Exception("Configuration")
        << "CTPPSPreMixProducer requires the RandomNumberGeneratorService,\n"
           "which is not present in the configuration file. You must add\n"
           "the service in the configuration file.\n";
  }

  CLHEP::HepRandomEngine &engine = rng->getEngine(iEvent.streamID());
  double entryNumber = CLHEP::RandFlat::shootInt(&engine, 0, puEntries_);

  if (verbosity_ > 0) {
    edm::LogInfo("PPS") << "Picking PU event number: " << entryNumber;
    puFileReader_.PrintEvent(entryNumber);
  }

  if (includePixels_) {
    // std::cerr << "CHECK 1" << std::endl;
    edm::Handle<edm::DetSetVector<CTPPSPixelRecHit>> simPixelRpRecHits;
    iEvent.getByToken(tokenCTPPSPixelRecHit_, simPixelRpRecHits);

    edm::DetSetVector<CTPPSPixelRecHit> pixelsOutput;

    // std::cerr << "CHECK 2" << std::endl;
    edm::DetSetVector<CTPPSPixelRecHit> puPixelRpRecHits =
        puFileReader_.getPixelRecHitsDsv(entryNumber);

    // std::cerr << "CHECK 3" << std::endl;
    if(simPixelRpRecHits.isValid())
      mergePixels(*simPixelRpRecHits, puPixelRpRecHits, pixelsOutput);

    // std::cerr << "CHECK 4" << std::endl;
    iEvent.put(
        std::make_unique<edm::DetSetVector<CTPPSPixelRecHit>>(pixelsOutput));
    // std::cerr << "CHECK 5" << std::endl;
  }

  if (includeStrips_) {
    // std::cerr << "CHECK 6" << std::endl;
    edm::Handle<edm::DetSetVector<TotemRPRecHit>> simStripsRpRecHits;
    iEvent.getByToken(tokenTotemRPRecHit_, simStripsRpRecHits);

    edm::DetSetVector<TotemRPRecHit> stripsOutput;

    // std::cerr << "CHECK 7" << std::endl;
    edm::DetSetVector<TotemRPRecHit> puStripsRpRecHits =
        puFileReader_.getStripsRecHitsDsv(entryNumber);

    // std::cerr << "CHECK 8" << std::endl;
    if (simStripsRpRecHits.isValid())
      mergeStrips(*simStripsRpRecHits, puStripsRpRecHits, stripsOutput);
    
    // std::cerr << "CHECK 9" << std::endl;
    iEvent.put(
        std::make_unique<edm::DetSetVector<TotemRPRecHit>>(stripsOutput));
    // std::cerr << "CHECK 10" << std::endl;
  }
}

void CTPPSPreMixProducer::mergePixels(
    const edm::DetSetVector<CTPPSPixelRecHit> &simRpRecHits,
    const edm::DetSetVector<CTPPSPixelRecHit> &puRpRecHits,
    edm::DetSetVector<CTPPSPixelRecHit> &output) {

  // Merge the two inputs
  for (auto simRpRecHits_ds : simRpRecHits) {
    edm::DetSet<CTPPSPixelRecHit> &outputRecHits_ds =
        output.find_or_insert(simRpRecHits_ds.id);
    for (auto simRpRecHit : simRpRecHits_ds.data) {
      outputRecHits_ds.push_back(simRpRecHit);
    }
  }

  for (auto puRpRecHits_ds : puRpRecHits) {
    edm::DetSet<CTPPSPixelRecHit> &outputRecHits_ds =
        output.find_or_insert(puRpRecHits_ds.id);
    for (auto puRpRecHit : puRpRecHits_ds.data) {
      outputRecHits_ds.push_back(puRpRecHit);
    }
  }
}

void CTPPSPreMixProducer::mergeStrips(
    const edm::DetSetVector<TotemRPRecHit> &simRpRecHits,
    const edm::DetSetVector<TotemRPRecHit> &puRpRecHits,
    edm::DetSetVector<TotemRPRecHit> &output) {

  // Merge the two inputs
  for (auto simRpRecHits_ds : simRpRecHits) {
    edm::DetSet<TotemRPRecHit> &outputRecHits_ds =
        output.find_or_insert(simRpRecHits_ds.id);
    for (auto simRpRecHit : simRpRecHits_ds.data) {
      outputRecHits_ds.push_back(simRpRecHit);
    }
  }

  for (auto puRpRecHits_ds : puRpRecHits) {
    edm::DetSet<TotemRPRecHit> &outputRecHits_ds =
        output.find_or_insert(puRpRecHits_ds.id);
    for (auto puRpRecHit : puRpRecHits_ds.data) {
      outputRecHits_ds.push_back(puRpRecHit);
    }
  }
}

// define this as a plugin
DEFINE_FWK_MODULE(CTPPSPreMixProducer);
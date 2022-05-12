#include "protonPreMix/protonPreMix/interface/CTPPSProtonMixer.h"

CTPPSProtonMixer::CTPPSProtonMixer(const edm::ParameterSet &conf)
    : verbosity_(conf.getUntrackedParameter<int>("Verbosity")),
      simProtonsSrc_(conf.getParameter<edm::InputTag>("Sim_ProtonsTag")),
      puProtonsSrc_(conf.getParameter<edm::InputTag>("PU_ProtonsTag")),
      puFileReader_(conf.getParameter<std::vector<std::string>>("PUFilesList"),
                    edm::InputTag(""),edm::InputTag("")),
      puEntries_(puFileReader_.getEntries()) {
    tokenProtons_ =
        consumes<reco::ForwardProtonCollection>(simProtonsSrc_);
    produces<reco::ForwardProtonCollection>();
  
  const std::vector<edm::LuminosityBlockRange> lumiTemp =
      conf.getUntrackedParameter<std::vector<edm::LuminosityBlockRange>>(
          "lumisToProcess");
  lumisToProcess_.resize(lumiTemp.size());
  copy(lumiTemp.begin(), lumiTemp.end(), lumisToProcess_.begin());

  puFileReader_.setLumisToProcess(lumisToProcess_);
  puFileReader_.setProtonTag(puProtonsSrc_);
  if (verbosity_ > 0)
    edm::LogInfo("PPS") << "PU files contain " << puEntries_ << " events";
}

CTPPSProtonMixer::~CTPPSProtonMixer() {}

void CTPPSProtonMixer::fillDescriptions(
    edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<int>("Verbosity", 0);
  desc.add<std::vector<std::string>>("PUFilesList", std::vector<std::string>());
  desc.add<edm::InputTag>("Sim_ProtonsTag",
                          edm::InputTag("ctppsProtons","multiRP"));
  desc.add<edm::InputTag>("PU_ProtonsTag",
                          edm::InputTag("ctppsProtons","multiRP"));
  desc.addUntracked<std::vector<edm::LuminosityBlockRange>>(
      "lumisToProcess", std::vector<edm::LuminosityBlockRange>());
  descriptions.add("ctppsProtonMixer", desc);
}

void CTPPSProtonMixer::produce(edm::Event &iEvent,
                               const edm::EventSetup &iSetup) {

  // Retrieve an event from PU files
  const edm::Service<edm::RandomNumberGenerator> rng;
  if (!rng.isAvailable()) {
    throw cms::Exception("Configuration")
        << "CTPPSProtonMixer requires the RandomNumberGeneratorService,\n"
           "which is not present in the configuration file. You must add\n"
           "the service in the configuration file.\n";
  }

  reco::ForwardProtonCollection protonsOutput;

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

    edm::Handle<reco::ForwardProtonCollection> simProtons;
    iEvent.getByToken(tokenProtons_, simProtons);

    reco::ForwardProtonCollection puProtons;
    if (!puFileReader_.getProtonCollection(puProtons))
      isPUinError = true;

    if (simProtons.isValid())
      merge(*simProtons, puProtons, protonsOutput);

  } while (isPUinError);

  iEvent.put(
    std::make_unique<reco::ForwardProtonCollection>(protonsOutput));
}

void CTPPSProtonMixer::merge(const reco::ForwardProtonCollection &simProtons,
                             const reco::ForwardProtonCollection &puProtons,
                             reco::ForwardProtonCollection &outputProtons) {
  // Merge the two inputs
  for (const auto &collection : {simProtons, puProtons})
    for (const auto &proton : collection)
      outputProtons.push_back(proton);
}

// define this as a plugin
DEFINE_FWK_MODULE(CTPPSProtonMixer);
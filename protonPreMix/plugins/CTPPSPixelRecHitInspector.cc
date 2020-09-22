#include "protonPreMix/protonPreMix/interface/CTPPSPixelRecHitInspector.h"
#include "protonPreMix/protonPreMix/interface/PUFileReader.h"

CTPPSPixelRecHitInspector::CTPPSPixelRecHitInspector(
    const edm::ParameterSet &conf) {
  src_ = conf.getUntrackedParameter<edm::InputTag>("recHitTag");
  initialMsg_ = conf.getUntrackedParameter<std::string>("initialMsg");
  pixelRecHitToken_ = consumes<edm::DetSetVector<CTPPSPixelRecHit>>(src_);
}

void CTPPSPixelRecHitInspector::analyze(const edm::Event &iEvent,
                                        const edm::EventSetup &iSetup) {
  using namespace edm;

  LogInfo("CTPPSPixelRecHitInspector") << initialMsg_;

  Handle<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHits;
  iEvent.getByToken(pixelRecHitToken_, pixelRecHits);

  for (auto &recHitDs : *pixelRecHits) {
    // Print ID
    CTPPSPixelDetId recHitId = CTPPSPixelDetId(recHitDs.id);
    // Print list of RecHits
    std::string planeSummary;
    int i = 1;
    for (auto &recHit : recHitDs.data) {
      planeSummary += " RecHit number " + std::to_string(i) + ": " +
                      "minPixelCol = " + std::to_string(recHit.minPixelCol()) +
                      " minPixelRow = " + std::to_string(recHit.minPixelRow()) +
                      "\n";
    }
    if (planeSummary.empty())
      planeSummary = "No RecHits";
    LogInfo("CTPPSPixelRecHitInspector") << recHitId << "\n" << planeSummary;
  }

  std::vector<std::string> fileNames = {
      "/eos/project-c/ctpps/subsystems/Pixel/RPixTracking/"
      "EfficiencyCalculation2017/ReRecoEras_CMSSW_10_6_10/SingleElectron/"
      "ctpps_2017B_pixelefficiency_singleele/200423_112331/0000/"
      "ReReco_2017B_1.root"};
  PUFileReader reader(fileNames);
  reader.PrintEvent(1);
}

void CTPPSPixelRecHitInspector::fillDescriptions(
    edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<edm::InputTag>("recHitTag",
                                   edm::InputTag("ctppsPixelRecHits", ""));
  desc.addUntracked<std::string>("initialMsg", "");
  descriptions.add("CTPPSPixelRecHitInspector", desc);
}

// define this as a plugin
DEFINE_FWK_MODULE(CTPPSPixelRecHitInspector);
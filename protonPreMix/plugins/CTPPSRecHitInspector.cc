#include "protonPreMix/protonPreMix/interface/CTPPSRecHitInspector.h"

CTPPSRecHitInspector::CTPPSRecHitInspector(
    const edm::ParameterSet &conf) {
  pixelSrc_ = conf.getUntrackedParameter<edm::InputTag>("pixelRecHitTag");
  stripsSrc_ = conf.getUntrackedParameter<edm::InputTag>("stripsRecHitTag");
  initialMsg_ = conf.getUntrackedParameter<std::string>("initialMsg");
  pixelRecHitToken_ = consumes<edm::DetSetVector<CTPPSPixelRecHit>>(pixelSrc_);
  stripsRecHitToken_ = consumes<edm::DetSetVector<TotemRPRecHit>>(stripsSrc_);
}

void CTPPSRecHitInspector::analyze(const edm::Event &iEvent,
                                        const edm::EventSetup &iSetup) {
  using namespace edm;

  LogInfo("CTPPSRecHitInspector") << initialMsg_;

  Handle<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHits;
  iEvent.getByToken(pixelRecHitToken_, pixelRecHits);

  std::string detectorSummary;
  std::string planeSummary;
  for (auto &recHitDs : *pixelRecHits) {
    // Print ID
    CTPPSPixelDetId recHitId = CTPPSPixelDetId(recHitDs.id);
    // Print list of RecHits
    int j = 1;
    for (auto &recHit : recHitDs.data) {
      planeSummary += " RecHit number " + std::to_string(j) + ": " +
                      "minPixelCol = " + std::to_string(recHit.minPixelCol()) +
                      " minPixelRow = " + std::to_string(recHit.minPixelRow()) +
                      "\n";
      j++;
    }
    if (planeSummary.empty())
      planeSummary = "No RecHits";

    detectorSummary +=
        std::string("subDet=") + recHitId.subdetId() + std::string(" arm=") +
        recHitId.arm() + std::string(" station=") + recHitId.station() +
        std::string(" rp=") + recHitId.rp() + std::string(" plane=") +
        recHitId.plane() + "\n" + planeSummary + "\n";
    planeSummary.clear();
  }


  Handle<edm::DetSetVector<TotemRPRecHit>> stripsRecHits;
  iEvent.getByToken(stripsRecHitToken_, stripsRecHits);
  
  for (auto &recHitDs : *stripsRecHits) {
    // Print ID
    TotemRPDetId recHitId = TotemRPDetId(recHitDs.id);
    // Print list of RecHits
    int j = 1;
    for (auto &recHit : recHitDs.data) {
      planeSummary += " RecHit number " + std::to_string(j) + ": " +
                      "position = " + std::to_string(recHit.getPosition()) +
                      " sigma = " + std::to_string(recHit.getSigma()) +
                      "\n";
      j++;
    }
    if (planeSummary.empty())
      planeSummary = "No RecHits";

    detectorSummary +=
        std::string("subDet=") + recHitId.subdetId() + std::string(" arm=") +
        recHitId.arm() + std::string(" station=") + recHitId.station() +
        std::string(" rp=") + recHitId.rp() + std::string(" plane=") +
        recHitId.plane() + "\n" + planeSummary + "\n";
    planeSummary.clear();
  }

  LogInfo("CTPPSRecHitInspector") << detectorSummary;
}

void CTPPSRecHitInspector::fillDescriptions(
    edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<edm::InputTag>("pixelRecHitTag",
                                   edm::InputTag("ctppsPixelRecHits", ""));
  desc.addUntracked<edm::InputTag>("stripsRecHitTag",
                                   edm::InputTag("totemRPRecHitProducer", ""));
  desc.addUntracked<std::string>("initialMsg", "");
  descriptions.add("CTPPSRecHitInspector", desc);
}

// define this as a plugin
DEFINE_FWK_MODULE(CTPPSRecHitInspector);
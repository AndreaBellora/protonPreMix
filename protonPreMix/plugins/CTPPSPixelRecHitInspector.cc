#include "protonPreMix/protonPreMix/interface/CTPPSPixelRecHitInspector.h"

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
    LogInfo("CTPPSPixelRecHitInspector") << recHitId;

    // Print list of RecHits

    for (auto &recHit : recHitDs.data)
      LogInfo("CTPPSPixelRecHitInspector")
          << "minPixelCol = " << recHit.minPixelCol()
          << " minPixelRow = " << recHit.minPixelRow();
  }
}

void CTPPSPixelRecHitInspector::fillDescriptions(edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<edm::InputTag>("recHitTag", edm::InputTag("ctppsPixelRecHits", ""));
  desc.addUntracked<std::string>("initialMsg", "");
  descriptions.add("CTPPSPixelRecHitInspector", desc);
}

// define this as a plugin
DEFINE_FWK_MODULE(CTPPSPixelRecHitInspector);
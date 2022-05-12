#include "protonPreMix/protonPreMix/interface/CTPPSProductsInspector.h"

CTPPSProductsInspector::CTPPSProductsInspector(const edm::ParameterSet &conf)
    : pixelRHTag_(conf.getUntrackedParameter<edm::InputTag>("pixelRecHitTag")),
      stripsRHTag_(
          conf.getUntrackedParameter<edm::InputTag>("stripsRecHitTag")),
      trackLiteTag_(conf.getUntrackedParameter<edm::InputTag>("trackLiteTag")),
      singleRPProtonTag_(
          conf.getUntrackedParameter<edm::InputTag>("singleRPProtonTag")),
      multiRPProtonTag_(
          conf.getUntrackedParameter<edm::InputTag>("multiRPProtonTag")),
      pixelRecHitToken_(
          consumes<edm::DetSetVector<CTPPSPixelRecHit>>(pixelRHTag_)),
      stripsRecHitToken_(
          consumes<edm::DetSetVector<TotemRPRecHit>>(stripsRHTag_)),
      trackLiteToken_(consumes<CTPPSLocalTrackLiteCollection>(trackLiteTag_)),
      singleRPProtonToken_(
          consumes<reco::ForwardProtonCollection>(singleRPProtonTag_)),
      multiRPProtonToken_(
          consumes<reco::ForwardProtonCollection>(multiRPProtonTag_)),
      initialMsg_(conf.getUntrackedParameter<std::string>("initialMsg")),
      usePixelRH_(true), useStripsRH_(true), useTrackLite_(true),
      useSingleRPProton_(true), useMultiRPProton_(true) {}

void CTPPSProductsInspector::analyze(const edm::Event &iEvent,
                                     const edm::EventSetup &iSetup) {
  using namespace edm;
  using namespace std;

  usePixelRH_ = true;
  useStripsRH_ = true;
  useTrackLite_ = true;
  useSingleRPProton_ = true;
  useMultiRPProton_ = true;

  TString message;

  Handle<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHits;
  Handle<edm::DetSetVector<TotemRPRecHit>> stripsRecHits;
  Handle<CTPPSLocalTrackLiteCollection> trackLites;
  Handle<reco::ForwardProtonCollection> singleRPProtons, multiRPProtons;

  if (usePixelRH_)
    iEvent.getByToken(pixelRecHitToken_, pixelRecHits);
  if (pixelRecHits.failedToGet()) {
    usePixelRH_ = false;
    message += "Pixel RecHits not found\n";
  }

  if (useStripsRH_)
    iEvent.getByToken(stripsRecHitToken_, stripsRecHits);
  if (stripsRecHits.failedToGet()) {
    useStripsRH_ = false;
    message += "Strips RecHits not found\n";
  }

  if (useTrackLite_)
    iEvent.getByToken(trackLiteToken_, trackLites);
  if (trackLites.failedToGet()) {
    useTrackLite_ = false;
    message += "TrackLites not found\n";
  }

  if (useSingleRPProton_)
    iEvent.getByToken(singleRPProtonToken_, singleRPProtons);
  if (singleRPProtons.failedToGet()) {
    useSingleRPProton_ = false;
    message += "SingleRP protons not found\n";
  }

  if (useMultiRPProton_)
    iEvent.getByToken(multiRPProtonToken_, multiRPProtons);
  if (multiRPProtons.failedToGet()) {
    useMultiRPProton_ = false;
    message += "MultiRP protons not found\n";
  }

  if (usePixelRH_) {
    TString detectorSummary;
    TString planeSummary;

    for (auto &recHitDs : *pixelRecHits) {
      // Print ID
      CTPPSPixelDetId recHitId = CTPPSPixelDetId(recHitDs.id);
      // Print list of RecHits
      int j = 1;
      for (auto &recHit : recHitDs.data) {
        planeSummary += " \t  RecHit number " + to_string(j) + ": " +
                        "minPixelCol = " + to_string(recHit.minPixelCol()) +
                        " minPixelRow = " + to_string(recHit.minPixelRow()) +
                        "\n";
        j++;
      }
      if (planeSummary.IsNull())
        planeSummary = "No RecHits";

      detectorSummary += "\tsubDet=" + to_string(recHitId.subdetId()) +
                         " arm=" + to_string(recHitId.arm()) +
                         " station=" + to_string(recHitId.station()) +
                         " rp=" + to_string(recHitId.rp()) +
                         " plane=" + to_string(recHitId.plane()) + "\n" +
                         planeSummary;
      planeSummary.Clear();
    }
    if (!detectorSummary.IsNull())
      message += "***Pixel RecHits***\n";
    message += detectorSummary + "\n";
  }

  if (useStripsRH_) {
    TString detectorSummary;
    TString planeSummary;

    for (auto &recHitDs : *stripsRecHits) {
      // Print ID
      TotemRPDetId recHitId = TotemRPDetId(recHitDs.id);
      // Print list of RecHits
      int j = 1;
      for (auto &recHit : recHitDs.data) {
        planeSummary += "\t  RecHit number " + to_string(j) + ": " +
                        "position = " + to_string(recHit.getPosition()) +
                        " sigma = " + to_string(recHit.getSigma()) + "\n";
        j++;
      }
      if (planeSummary.IsNull())
        planeSummary = "No RecHits";

      detectorSummary += "\tsubDet=" + to_string(recHitId.subdetId()) +
                         " arm=" + to_string(recHitId.arm()) +
                         " station=" + to_string(recHitId.station()) +
                         " rp=" + to_string(recHitId.rp()) +
                         " plane=" + to_string(recHitId.plane()) + "\n" +
                         planeSummary + "\n";
      planeSummary.Clear();
    }
    if (!detectorSummary.IsNull())
      message += "***Strips RecHits***\n";
    message += detectorSummary;
  }

  if (useTrackLite_) {
    TString trackSummary;
    int idx = 0;
    for (auto &track : *trackLites) {
      CTPPSDetId rpId(track.getRPId());
      trackSummary += "\tTrack number [" + to_string(idx) +
                      "]: arm=" + to_string(rpId.arm()) +
                      " station=" + to_string(rpId.station()) +
                      " x=" + to_string(track.getX()) +
                      " y=" + to_string(track.getY()) + "\n";
      idx++;
    }
    if (!trackSummary.IsNull())
      message += "***TrackLites***\n";
    message += trackSummary;
  }

  if (useSingleRPProton_) {
    TString protonSummary;
    int i = 0;
    for (auto &proton : *singleRPProtons) {
      auto track = *(proton.contributingLocalTracks()[0]);
      CTPPSDetId rpId(track.getRPId());
      protonSummary += "\tProton number " + to_string(i) +
                       ": arm=" + to_string(rpId.arm()) +
                       " station=" + to_string(rpId.station()) +
                       " xi=" + to_string(proton.xi()) +
                       " thetaX=" + to_string(proton.thetaX()) +
                       " thetaY=" + to_string(proton.thetaY()) + "\n";
      // Find track used for proton reco
      if (useTrackLite_) {
        int idx = 0;
        TString trackSummary;
        for (auto &trackLite : *trackLites) {
          // Look for the track in the collection
          if (trackLite.getRPId() == rpId &&
              fabs(trackLite.getX() - track.getX()) < 0.01 &&
              fabs(trackLite.getY() - track.getY()) < 0.01)
            trackSummary += "\t  Track used: [" + to_string(idx) + "]\n";
          idx++;
        }
        if (trackSummary.IsNull())
          protonSummary += "\t  Track used NOT FOUND\n";
        else
          protonSummary += trackSummary;
      }
      i++;
    }
    if (!protonSummary.IsNull())
      message += "***SingleRP protons***\n";
    message += protonSummary;
  }

  if (useMultiRPProton_) {
    TString protonSummary;
    int i = 0;
    for (auto &proton : *multiRPProtons) {
      protonSummary += "\tProton number " + to_string(i) +
                       ": xi=" + to_string(proton.xi()) +
                       " thetaX=" + to_string(proton.thetaX()) +
                       " thetaY=" + to_string(proton.thetaY()) + "\n";
      // Find tracks used for proton reco
      if (useTrackLite_) {
        TString trackSummary = "\t  Track used: \n";
        for (auto track : proton.contributingLocalTracks()) {
          int idx = 0;
          CTPPSDetId rpId((*track).getRPId());
          for (auto &trackLite : *trackLites) {
            // Look for the track in the collection
            if (trackLite.getRPId() == rpId &&
                fabs(trackLite.getX() - (*track).getX()) < 0.01 &&
                fabs(trackLite.getY() - (*track).getY()) < 0.01)
              trackSummary += "\t    [" + to_string(idx) + "] ";
            idx++;
          }
          trackSummary +=
              " arm=" + to_string(rpId.arm()) +
              " station=" + to_string(rpId.station()) +
              " x=" + to_string((*track).getX()) +
              " y=" + to_string((*track).getY()) + "\n";
        }
        protonSummary += trackSummary + "\n";
      }
      i++;
    }
    if (!protonSummary.IsNull())
      message += "***MultiRP protons***\n";
    else 
      message += "No MultiRP protons\n";
    message += protonSummary;
  }
  LogInfo("PPS") << message;
}

void CTPPSProductsInspector::fillDescriptions(
    edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<edm::InputTag>(
      "pixelRecHitTag", edm::InputTag("ctppsDirectProtonSimulation", ""));
  desc.addUntracked<edm::InputTag>(
      "stripsRecHitTag", edm::InputTag("ctppsDirectProtonSimulation", ""));
  desc.addUntracked<edm::InputTag>(
      "trackLiteTag", edm::InputTag("ctppsLocalTrackLiteProducer", ""));
  desc.addUntracked<edm::InputTag>("singleRPProtonTag",
                                   edm::InputTag("ctppsProtons", "singleRP"));
  desc.addUntracked<edm::InputTag>("multiRPProtonTag",
                                   edm::InputTag("ctppsProtons", "multiRP"));
  desc.addUntracked<std::string>("initialMsg", "");
  descriptions.add("ctppsProductsInspector", desc);
}

// define this as a plugin
DEFINE_FWK_MODULE(CTPPSProductsInspector);
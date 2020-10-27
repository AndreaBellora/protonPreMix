#include "protonPreMix/protonPreMix/interface/PUFileReader.h"

using namespace std;

PUFileReader::PUFileReader(vector<string> fileNames, edm::InputTag pixelTag,
                           edm::InputTag stripsTag)
    : fileNames_(fileNames), pixelTag_(pixelTag), stripsTag_(stripsTag),
      pixelLabel_(pixelTag_.label()), pixelInstance_(pixelTag_.instance()),
      stripsLabel_(stripsTag_.label()), stripsInstance_(stripsTag_.instance()) {

  for (auto &fileName : fileNames_) {
    if (fileName.find("file:") == string::npos &&
        fileName.find("root:") == string::npos) {
      fileName = "root://cms-xrd-global.cern.ch//" + fileName;
    }
  }
  ev_ = make_unique<fwlite::ChainEvent>(fileNames_);
}

void PUFileReader::PrintEvent(int i) {
  fwlite::Handle<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHits;
  fwlite::Handle<edm::DetSetVector<TotemRPRecHit>> stripsRecHits;

  if (ev_->to(i)) {
    ev_->eventAuxiliary().write(cout);
    cout << endl;
    pixelRecHits.getByLabel(*ev_, pixelLabel_.data(), pixelInstance_.data());
    edm::DetSetVector<CTPPSPixelRecHit> pixelRecHitsDsv = *(pixelRecHits.ptr());
    // cout << "Size: " << pixelRecHits.ptr()->size() << endl;
    for (auto pixelRecHitsDs : pixelRecHitsDsv) {
      CTPPSPixelDetId recHitId = CTPPSPixelDetId(pixelRecHitsDs.id);
      cout << "subDet=" << recHitId.subdetId() << " arm=" << recHitId.arm()
           << " station=" << recHitId.station() << " rp=" << recHitId.rp()
           << " plane=" << recHitId.plane() << endl;
      int j = 1;
      for (auto pixelRecHit : pixelRecHitsDs.data) {
        cout << " RecHit number " << j
             << ": minPixelCol = " << pixelRecHit.minPixelCol()
             << " minPixelRow = " << pixelRecHit.minPixelRow() << "\n";
        j++;
      }
      cout << endl;
    }

    stripsRecHits.getByLabel(*ev_, stripsLabel_.data(), stripsInstance_.data());
    edm::DetSetVector<TotemRPRecHit> stripsRecHitsDsv = *(stripsRecHits.ptr());
    // cout << "Size: " << pixelRecHits.ptr()->size() << endl;
    for (auto stripsRecHitsDs : stripsRecHitsDsv) {
      TotemRPDetId recHitId = TotemRPDetId(stripsRecHitsDs.id);
      cout << "subDet=" << recHitId.subdetId() << " arm=" << recHitId.arm()
           << " station=" << recHitId.station() << " rp=" << recHitId.rp()
           << " plane=" << recHitId.plane() << endl;
      int j = 1;
      for (auto stripsRecHit : stripsRecHitsDs.data) {
        cout << " RecHit number " << j
             << ": position = " << stripsRecHit.getPosition()
             << " sigma = " << stripsRecHit.getSigma() << "\n";
        j++;
      }
      cout << endl;
    }
  } 
}

edm::DetSetVector<CTPPSPixelRecHit> PUFileReader::getPixelRecHitsDsv(int i) {

  fwlite::Handle<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHits;

  if (ev_->to(i)) {
    pixelRecHits.getByLabel(*ev_, pixelLabel_.data(), pixelInstance_.data());
    edm::DetSetVector<CTPPSPixelRecHit> pixelRecHitsDsv = *(pixelRecHits.ptr());
    return pixelRecHitsDsv;
  } else {
    edm::LogWarning("PPS") << "Event not found in the PU files";
    return edm::DetSetVector<CTPPSPixelRecHit>();
  }
}

edm::DetSetVector<TotemRPRecHit> PUFileReader::getStripsRecHitsDsv(int i) {

  fwlite::Handle<edm::DetSetVector<TotemRPRecHit>> stripsRecHits;

  if (ev_->to(i)) {
    stripsRecHits.getByLabel(*ev_, stripsLabel_.data(), stripsInstance_.data());
    edm::DetSetVector<TotemRPRecHit> stripsRecHitsDsv = *(stripsRecHits.ptr());
    return stripsRecHitsDsv;
  } else {
    edm::LogWarning("PPS") << "Event not found in the PU files";
    return edm::DetSetVector<TotemRPRecHit>();
  }
}
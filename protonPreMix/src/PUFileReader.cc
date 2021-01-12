#include "protonPreMix/protonPreMix/interface/PUFileReader.h"

using namespace std;

PUFileReader::PUFileReader(vector<string> fileNames, edm::InputTag pixelTag,
                           edm::InputTag stripsTag)
    : fileNames_(fileNames), pixelTag_(pixelTag), stripsTag_(stripsTag),
      pixelLabel_(pixelTag_.label()), pixelInstance_(pixelTag_.instance()),
      stripsLabel_(stripsTag_.label()), stripsInstance_(stripsTag_.instance()),
      eventNumber_(-1) {

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
    ev_->eventAuxiliary().write(cerr);
    cerr << endl;
    pixelRecHits.getByLabel(*ev_, pixelLabel_.data(), pixelInstance_.data());
    edm::DetSetVector<CTPPSPixelRecHit> pixelRecHitsDsv = *(pixelRecHits.ptr());
    // cerr << "Size: " << pixelRecHits.ptr()->size() << endl;
    for (auto pixelRecHitsDs : pixelRecHitsDsv) {
      CTPPSPixelDetId recHitId = CTPPSPixelDetId(pixelRecHitsDs.id);
      cerr << "subDet=" << recHitId.subdetId() << " arm=" << recHitId.arm()
           << " station=" << recHitId.station() << " rp=" << recHitId.rp()
           << " plane=" << recHitId.plane() << endl;
      int j = 1;
      for (auto pixelRecHit : pixelRecHitsDs.data) {
        cerr << " RecHit number " << j
             << ": minPixelCol = " << pixelRecHit.minPixelCol()
             << " minPixelRow = " << pixelRecHit.minPixelRow() << "\n";
        j++;
      }
      cerr << endl;
    }

    stripsRecHits.getByLabel(*ev_, stripsLabel_.data(), stripsInstance_.data());
    edm::DetSetVector<TotemRPRecHit> stripsRecHitsDsv = *(stripsRecHits.ptr());
    // cerr << "Size: " << pixelRecHits.ptr()->size() << endl;
    for (auto stripsRecHitsDs : stripsRecHitsDsv) {
      TotemRPDetId recHitId = TotemRPDetId(stripsRecHitsDs.id);
      cerr << "subDet=" << recHitId.subdetId() << " arm=" << recHitId.arm()
           << " station=" << recHitId.station() << " rp=" << recHitId.rp()
           << " plane=" << recHitId.plane() << endl;
      int j = 1;
      for (auto stripsRecHit : stripsRecHitsDs.data) {
        cerr << " RecHit number " << j
             << ": position = " << stripsRecHit.getPosition()
             << " sigma = " << stripsRecHit.getSigma() << "\n";
        j++;
      }
      cerr << endl;
    }
  }
}

edm::DetSetVector<CTPPSPixelRecHit> PUFileReader::getPixelRecHitsDsv(int i) {
  if (eventNumber_ == i) {
    edm::LogWarning("PPS") << "an exception was already caught for this event, "
                              "returning empty vector";
    return edm::DetSetVector<CTPPSPixelRecHit>();
  }

  fwlite::Handle<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHits;
  try {
    if (ev_->to(i) && ev_->isValid()) {
      // std::cerr << "Accessing pixel file: " << ev_->getTFile()->GetName()
      //           << " at event " << i << std::endl;
      // std::cerr << pixelLabel_.data() << " " << pixelInstance_.data()
      //           << std::endl;
      pixelRecHits.getByLabel(*ev_, pixelLabel_.data(), pixelInstance_.data());
      // std::cerr << "CHECK PIXEL 1" << std::endl;
      if (pixelRecHits.isValid()) {
        edm::DetSetVector<CTPPSPixelRecHit> pixelRecHitsDsv =
            edm::DetSetVector<CTPPSPixelRecHit>(*(pixelRecHits.ptr()));
        // std::cerr << "CHECK PIXEL 2" << std::endl;
        return pixelRecHitsDsv;
      } else {
        edm::LogWarning("PPS")
            << "For some reason, the recHit pointer is not valid";
        return edm::DetSetVector<CTPPSPixelRecHit>();
      }
    } else {
      edm::LogWarning("PPS") << "Event not found in the PU files";
      return edm::DetSetVector<CTPPSPixelRecHit>();
    }
  } catch (cms::Exception &e) {
    edm::LogWarning("PPS")
        << "Careful, exception caught, returning empty collection\n"
        << "Find below the exception\n"
        << e.what();
    eventNumber_ = i;
    return edm::DetSetVector<CTPPSPixelRecHit>();
  }
}

edm::DetSetVector<TotemRPRecHit> PUFileReader::getStripsRecHitsDsv(int i) {
  if (eventNumber_ == i) {
    edm::LogWarning("PPS") << "an exception was already caught for this event, "
                              "returning empty vector";
    return edm::DetSetVector<TotemRPRecHit>();
  }

  fwlite::Handle<edm::DetSetVector<TotemRPRecHit>> stripsRecHits;
  try {
    if (ev_->to(i) && ev_->isValid()) {
      // std::cerr << "Accessing strips file: " << ev_->getTFile()->GetName()
      //           << " at event " << i << std::endl;
      // std::cerr << stripsLabel_.data() << " " << stripsLabel_.data()
      //           << "endline" << std::endl;
      stripsRecHits.getByLabel(*ev_, stripsLabel_.data(),
                               stripsInstance_.data());
      // std::cerr << "CHECK STRIPS 1" << std::endl;

      if (stripsRecHits.isValid()) {
        edm::DetSetVector<TotemRPRecHit> stripsRecHitsDsv =
            edm::DetSetVector<TotemRPRecHit>(*(stripsRecHits.ptr()));
        // std::cerr << "CHECK STRIPS 2" << std::endl;
        return stripsRecHitsDsv;
      } else {
        edm::LogWarning("PPS")
            << "For some reason, the recHit pointer is not valid";
        return edm::DetSetVector<TotemRPRecHit>();
      }
    } else {
      edm::LogWarning("PPS") << "Event not found in the PU files";
      return edm::DetSetVector<TotemRPRecHit>();
    }
  } catch (cms::Exception &e) {
    edm::LogWarning("PPS")
        << "Careful, exception caught, returning empty collection\n"
        << e.what();
    eventNumber_ = i;
    return edm::DetSetVector<TotemRPRecHit>();
  }
}
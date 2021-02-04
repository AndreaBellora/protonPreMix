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

bool PUFileReader::getPixelRecHitsDsv(
    int i, edm::DetSetVector<CTPPSPixelRecHit> &pixelRecHitsDsv) {
  if (eventNumber_ == i) {
    edm::LogWarning("PPS") << "an exception was already caught for this event, "
                              "choosing another event";
    return false;
  }
  fwlite::Handle<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHits;
  try {
    if (ev_->to(i) && ev_->isValid()) {
      if (!jsonContainsEvent(*ev_))
        return false;

      pixelRecHits.getByLabel(*ev_, pixelLabel_.data(), pixelInstance_.data());
      if (pixelRecHits.isValid()) {
        pixelRecHitsDsv =
            edm::DetSetVector<CTPPSPixelRecHit>(*(pixelRecHits.ptr()));
        return true;
      } else {
        edm::LogWarning("PPS")
            << "For some reason, the recHit pointer is not valid";
        return false;
      }
    } else {
      edm::LogWarning("PPS") << "Event not found in the PU files";
      return false;
    }
  } catch (cms::Exception &e) {
    edm::LogWarning("PPS")
        << "Careful, exception caught, choosing another event\n"
        << "Find below the exception\n"
        << e.what();
    eventNumber_ = i;
    return false;
  }
}

bool PUFileReader::getStripsRecHitsDsv(
    int i, edm::DetSetVector<TotemRPRecHit> &stripsRecHitsDsv) {
  if (eventNumber_ == i) {
    edm::LogWarning("PPS") << "an exception was already caught for this event, "
                              "choosing another event";
    return false;
  }
  fwlite::Handle<edm::DetSetVector<TotemRPRecHit>> stripsRecHits;
  try {
    if (ev_->to(i) && ev_->isValid()) {
      if (!jsonContainsEvent(*ev_))
        return false;

      stripsRecHits.getByLabel(*ev_, stripsLabel_.data(),
                               stripsInstance_.data());

      if (stripsRecHits.isValid()) {
        stripsRecHitsDsv =
            edm::DetSetVector<TotemRPRecHit>(*(stripsRecHits.ptr()));
        return true;
      } else {
        edm::LogWarning("PPS")
            << "For some reason, the recHit pointer is not valid";
        return false;
      }
    } else {
      edm::LogWarning("PPS") << "Event not found in the PU files";
      return false;
    }
  } catch (cms::Exception &e) {
    edm::LogWarning("PPS")
        << "Careful, exception caught, choosing another event\n"
        << e.what();
    eventNumber_ = i;
    return false;
  }
}

bool PUFileReader::jsonContainsEvent(const edm::EventBase &event) {
  // if the jsonVec is empty, then no JSON file was provided so all
  // events should pass
  if (jsonVector_.empty())
    return true;

  bool (*funcPtr)(edm::LuminosityBlockRange const &,
                  edm::LuminosityBlockID const &) = &edm::contains;
  edm::LuminosityBlockID lumiID(event.id().run(), event.id().luminosityBlock());
  vector<edm::LuminosityBlockRange>::const_iterator iter = find_if(
      jsonVector_.begin(), jsonVector_.end(), boost::bind(funcPtr, _1, lumiID));
  return jsonVector_.end() != iter;
}
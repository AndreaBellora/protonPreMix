/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef PUFileReader_H
#define PUFileReader_H

#include <boost/bind.hpp>
#include <iostream>

#include "TFile.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/Provenance/interface/LuminosityBlockRange.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"

#include "FWCore/Common/interface/TriggerNames.h"

class PUFileReader {
public:
  PUFileReader(std::vector<std::string> fileNames, edm::InputTag pixelTag,
               edm::InputTag stripsTag);
  ~PUFileReader() {}
  inline int GetEntries() { return ev_->size(); };
  bool getAndCheckEvent(const int i);
  template <class T>
  bool getRecHitsDsv(edm::DetSetVector<T> &recHitsDsv, CTPPSDetId::SubDetector subDetector);
  inline void
  setLumisToProcess(std::vector<edm::LuminosityBlockRange> jsonVector) {
    jsonVector_ = jsonVector;
  };

private:
  bool jsonContainsEvent(const edm::EventBase &event);

  std::vector<std::string> fileNames_;
  std::unique_ptr<fwlite::ChainEvent> ev_;

  edm::InputTag pixelTag_;
  edm::InputTag stripsTag_;
  int errorEventNumber_;

  std::vector<edm::LuminosityBlockRange> jsonVector_;
};

template <class T>
bool PUFileReader::getRecHitsDsv(edm::DetSetVector<T> &recHitsDsv,
                                 CTPPSDetId::SubDetector subDetector) {

  fwlite::Handle < edm::DetSetVector<T>> recHits;
  edm::InputTag recHitInputTag;
  if(ev_->eventIndex() == errorEventNumber_)
    edm::LogWarning("PPS") << "An error was already caught for this event, skipping";
  if (subDetector == CTPPSDetId::SubDetector::sdTrackingStrip)
    recHitInputTag = stripsTag_;
  else if (subDetector == CTPPSDetId::SubDetector::sdTrackingPixel)
    recHitInputTag = pixelTag_;
  else
    throw cms::Exception("PPS")
        << "PPS subdetector type not compatible with RecHit mixing";

  try {
    recHits.getByLabel(*ev_, recHitInputTag.label().data(),
                       recHitInputTag.instance().data());

    if (recHits.isValid()) {
      recHitsDsv = edm::DetSetVector<T>(*(recHits.ptr()));
      return true;
    } else {
      edm::LogWarning("PPS") << "RecHit pointer is not valid";
      return false;
    }
  } catch (cms::Exception &e) {
    edm::LogWarning("PPS")
        << "Exception caught while getting RecHit vector for subDetector "
        << subDetector << "\n"
        << "Find the exception below:\n"
        << e.what();
    errorEventNumber_ = ev_->eventIndex();
    return false;
  }
}

#endif
/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef PUFileReader_H
#define PUFileReader_H

#include <iostream>
#include <boost/bind.hpp>

#include "TFile.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
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
  void PrintEvent(int i);
  inline int GetEntries() { return ev_->size(); };
  bool getPixelRecHitsDsv(int i,edm::DetSetVector<CTPPSPixelRecHit> &pixelRecHitsDsv);
  bool getStripsRecHitsDsv(int i,edm::DetSetVector<TotemRPRecHit> &stripsRecHitsDsv);
  inline void setLumisToProcess(std::vector<edm::LuminosityBlockRange> jsonVector){jsonVector_ = jsonVector;};

private:
  bool jsonContainsEvent(const edm::EventBase &event);

  std::vector<std::string> fileNames_;
  std::unique_ptr<fwlite::ChainEvent> ev_;
  
  edm::InputTag pixelTag_;
  edm::InputTag stripsTag_;

  std::string pixelLabel_;
  std::string pixelInstance_;
  std::string stripsLabel_;
  std::string stripsInstance_;
  std::vector<edm::LuminosityBlockRange> jsonVector_;
  int eventNumber_;
};

#endif
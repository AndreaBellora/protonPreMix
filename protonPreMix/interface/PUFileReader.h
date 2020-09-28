/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef PUFileReader_H
#define PUFileReader_H

#include <iostream>

#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"

class PUFileReader {
public:
  PUFileReader(std::vector<std::string> fileNames, edm::InputTag pixelTag,
               edm::InputTag stripsTag);
  ~PUFileReader() {}
  void PrintEvent(int i);
  inline int GetEntries() { return ev_->size(); };
  edm::DetSetVector<CTPPSPixelRecHit> getPixelRecHitsDsv(int i);
  edm::DetSetVector<TotemRPRecHit> getStripsRecHitsDsv(int i);

private:
  std::vector<std::string> fileNames_;
  std::unique_ptr<fwlite::ChainEvent> ev_;
  
  edm::InputTag pixelTag_;
  edm::InputTag stripsTag_;

  std::string pixelLabel_;
  std::string pixelInstance_;
  std::string stripsLabel_;
  std::string stripsInstance_;

};

#endif
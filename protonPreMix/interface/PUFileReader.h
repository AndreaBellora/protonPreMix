/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef PUFileReader_H
#define PUFileReader_H

#include <iostream>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"


class PUFileReader {
public:
  PUFileReader(std::vector<std::string> fileNames);
  ~PUFileReader(){}
  void PrintEvent(int i);

private:
  std::vector<std::string> fileNames_;
  fwlite::ChainEvent ev_;
};

#endif
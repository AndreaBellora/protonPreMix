/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef protonPreMix_CTPPSPixelRecHitInspector_H
#define protonPreMix_CTPPSPixelRecHitInspector_H


#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/Framework/interface/MakerMacros.h"

class CTPPSPixelRecHitInspector : public edm::one::EDAnalyzer<> {

public:
  explicit CTPPSPixelRecHitInspector(const edm::ParameterSet &conf);

  ~CTPPSPixelRecHitInspector(){};


  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  virtual void beginJob(){};
  virtual void analyze(const edm::Event &, const edm::EventSetup &) override;
  virtual void endJob(){};

  edm::InputTag src_;
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHitToken_;

  std::string initialMsg_;

};

#endif
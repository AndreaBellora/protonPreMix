/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef protonPreMix_CTPPSRecHitInspector_H
#define protonPreMix_CTPPSRecHitInspector_H


#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"

class CTPPSRecHitInspector : public edm::one::EDAnalyzer<> {

public:
  explicit CTPPSRecHitInspector(const edm::ParameterSet &conf);

  ~CTPPSRecHitInspector(){};


  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  virtual void beginJob(){};
  virtual void analyze(const edm::Event &, const edm::EventSetup &) override;
  virtual void endJob(){};

  edm::InputTag pixelSrc_;
  edm::InputTag stripsSrc_;

  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHitToken_;
  edm::EDGetTokenT<edm::DetSetVector<TotemRPRecHit>> stripsRecHitToken_;

  std::string initialMsg_;

};

#endif
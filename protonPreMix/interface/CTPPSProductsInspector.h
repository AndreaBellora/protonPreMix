/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef protonPreMix_CTPPSProductsInspector_H
#define protonPreMix_CTPPSProductsInspector_H

#include "TString.h"

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
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLiteFwd.h"
#include "DataFormats/ProtonReco/interface/ForwardProton.h"
#include "DataFormats/ProtonReco/interface/ForwardProtonFwd.h"

class CTPPSProductsInspector : public edm::one::EDAnalyzer<> {

public:
  explicit CTPPSProductsInspector(const edm::ParameterSet &conf);

  ~CTPPSProductsInspector(){};


  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  virtual void beginJob(){};
  virtual void analyze(const edm::Event &, const edm::EventSetup &) override;
  virtual void endJob(){};

  edm::InputTag pixelRHTag_,stripsRHTag_,trackLiteTag_,singleRPProtonTag_,multiRPProtonTag_;

  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHitToken_;
  edm::EDGetTokenT<edm::DetSetVector<TotemRPRecHit>> stripsRecHitToken_;
  edm::EDGetTokenT<CTPPSLocalTrackLiteCollection> trackLiteToken_;
  edm::EDGetTokenT<reco::ForwardProtonCollection> singleRPProtonToken_, multiRPProtonToken_;

  std::string initialMsg_;

  bool usePixelRH_,useStripsRH_,useTrackLite_,useSingleRPProton_,useMultiRPProton_;

};

#endif
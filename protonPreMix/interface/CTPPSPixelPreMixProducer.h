/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef protonPreMix_CTPPSPixelPreMixProducer_H
#define protonPreMix_CTPPSPixelPreMixProducer_H

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/Framework/interface/MakerMacros.h"

class CTPPSPixelPreMixProducer : public edm::stream::EDProducer<> {

public:
  explicit CTPPSPixelPreMixProducer(const edm::ParameterSet &param);

  ~CTPPSPixelPreMixProducer() override;

  void produce(edm::Event &, const edm::EventSetup &) override;

  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  edm::ParameterSet param_;
  int verbosity_;

  edm::InputTag simSrc_;
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit>> tokenCTPPSPixelRecHit_;

  std::string preMixSrc_;

  void run(const edm::DetSetVector<CTPPSPixelRecHit> &input, edm::DetSetVector<CTPPSPixelRecHit> &output);
};

#endif
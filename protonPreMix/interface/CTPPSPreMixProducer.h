/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef protonPreMix_CTPPSPreMixProducer_H
#define protonPreMix_CTPPSPreMixProducer_H

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"

#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"

#include "protonPreMix/protonPreMix/interface/PUFileReader.h"

class CTPPSPreMixProducer : public edm::stream::EDProducer<> {

public:
  explicit CTPPSPreMixProducer(const edm::ParameterSet &param);

  ~CTPPSPreMixProducer() override;

  void produce(edm::Event &, const edm::EventSetup &) override;

  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  int verbosity_;

  bool includePixels_,includeStrips_;

  edm::InputTag simPixelSrc_, puPixelSrc_;
  edm::InputTag simStripsSrc_, puStripsSrc_;

  PUFileReader puFileReader_;
  std::vector<std::string> puFilesList_;
  int puEntries_;

  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit>> tokenCTPPSPixelRecHit_;
  edm::EDGetTokenT<edm::DetSetVector<TotemRPRecHit>> tokenTotemRPRecHit_;

  void mergePixels(const edm::DetSetVector<CTPPSPixelRecHit> &simRPRecHits, const edm::DetSetVector<CTPPSPixelRecHit> &puRPRecHits, edm::DetSetVector<CTPPSPixelRecHit> &output);
  void mergeStrips(const edm::DetSetVector<TotemRPRecHit> &simRPRecHits, const edm::DetSetVector<TotemRPRecHit> &puRPRecHits, edm::DetSetVector<TotemRPRecHit> &output);
  // Copy-constructor for CTPPSPixelRecHits
};

#endif
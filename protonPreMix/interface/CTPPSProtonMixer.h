/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#ifndef protonPreMix_CTPPSProtonMixer_H
#define protonPreMix_CTPPSProtonMixer_H

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

#include "DataFormats/ProtonReco/interface/ForwardProton.h"
#include "DataFormats/ProtonReco/interface/ForwardProtonFwd.h"

#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"

#include "protonPreMix/protonPreMix/interface/PUFileReader.h"

class CTPPSProtonMixer : public edm::stream::EDProducer<> {

public:
  explicit CTPPSProtonMixer(const edm::ParameterSet &param);

  ~CTPPSProtonMixer() override;

  void produce(edm::Event &, const edm::EventSetup &) override;

  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  int verbosity_;

  edm::InputTag simProtonsSrc_, puProtonsSrc_;

  PUFileReader puFileReader_;
  std::vector<std::string> puFilesList_;
  int puEntries_;

  edm::EDGetTokenT<reco::ForwardProtonCollection> tokenProtons_;
  std::vector<edm::LuminosityBlockRange> lumisToProcess_;

  void merge(const reco::ForwardProtonCollection &simProtons, const reco::ForwardProtonCollection &puProtons, reco::ForwardProtonCollection &output);
};

#endif
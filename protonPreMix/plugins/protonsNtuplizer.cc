// system include files
#include <memory>

#include "TTree.h"

#include <iostream> // std::cout
#include <string>   // std::string

// user include files
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/ProtonReco/interface/ForwardProton.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/RunInfo/interface/LHCInfo.h"
#include "CondFormats/DataRecord/interface/LHCInfoRcd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

class protonsNtuplizer
    : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit protonsNtuplizer(const edm::ParameterSet &);
  ~protonsNtuplizer();

  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event &, const edm::EventSetup &) override;
  virtual void endJob() override;

  edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite>> pps_token_;
  edm::EDGetTokenT<std::vector<reco::ForwardProton>> recoProtonsSingleRPToken_;
  edm::EDGetTokenT<std::vector<reco::ForwardProton>> recoProtonsMultiRPToken_;
  edm::EDGetTokenT<edm::TriggerResults> hlt_token_;

  TTree *tree_;

  std::vector<float> *pps_track_x_;
  std::vector<float> *pps_track_y_;
  std::vector<int> *pps_track_rpid_;
  std::vector<float> *proton_xi_;
  std::vector<float> *proton_thy_;
  std::vector<float> *proton_thx_;
  std::vector<float> *proton_y_;
  std::vector<float> *proton_x_;
  std::vector<float> *proton_t_;
  std::vector<int> *proton_ismultirp_;
  std::vector<int> *proton_rpid_;
  std::vector<int> *proton_arm_;
  std::vector<float> *proton_trackx1_;
  std::vector<float> *proton_tracky1_;
  std::vector<float> *proton_trackx2_;
  std::vector<float> *proton_tracky2_;
  std::vector<int> *proton_trackpixshift1_;
  std::vector<int> *proton_trackpixshift2_;
  std::vector<int> *proton_rpid1_;
  std::vector<int> *proton_rpid2_;
  std::vector<int> *l1prescale;
  std::vector<int> *hltprescale;

  std::vector<string> * hlt_;

  int *run_;
  long int *ev_;
  int *lumiblock_;
  float *crossingangle_;

  HLTConfigProvider hltConfig_;
  HLTPrescaleProvider hltPrescaleProvider_;
};

protonsNtuplizer::protonsNtuplizer(const edm::ParameterSet &iConfig)
    : pps_token_(consumes<std::vector<CTPPSLocalTrackLite>>(
          edm::InputTag(("ctppsLocalTrackLiteProducer")))),
      recoProtonsSingleRPToken_(consumes<std::vector<reco::ForwardProton>>(
          iConfig.getParameter<edm::InputTag>("ppsRecoProtonSingleRPTag"))),
      recoProtonsMultiRPToken_(consumes<std::vector<reco::ForwardProton>>(
          iConfig.getParameter<edm::InputTag>("ppsRecoProtonMultiRPTag"))),
      hlt_token_(consumes<edm::TriggerResults>(
          edm::InputTag("TriggerResults", "", "HLT"))),
      hltPrescaleProvider_(iConfig, consumesCollector(), *this) {
  // now do what ever initialization is needed
  usesResource("TFileService");
}

// ------------ method called for each event  ------------
void protonsNtuplizer::analyze(const edm::Event &iEvent,
                               const edm::EventSetup &iSetup) {
  using namespace edm;

  // HLT
  bool first = true;
  edm::Handle<edm::TriggerResults> hltResults;
  iEvent.getByToken(hlt_token_, hltResults);
  const edm::TriggerNames &trigNames = iEvent.triggerNames(*hltResults);
  for (unsigned int i = 0; i < trigNames.size(); i++) {
    int prescale_value =
        hltPrescaleProvider_
            .prescaleValues(iEvent, iSetup, trigNames.triggerName(i))
            .second;
    int l1prescale_value =
        hltPrescaleProvider_
            .prescaleValues(iEvent, iSetup, trigNames.triggerName(i))
            .first;
    std::string triggerName = trigNames.triggerName(i);
    if (first) {
      std::cout << "Trigger name: " << triggerName << " l1 prescale "
                << l1prescale_value << " hlt prescale" << prescale_value
                << std::endl;
    }
    if ((hltResults->accept(i) > 0)) {
      (*hlt_).push_back(triggerName);
      (*l1prescale).push_back(l1prescale_value);
      (*hltprescale).push_back(prescale_value);
    }
  }
  first = false;

  // Proton lite tracks
  edm::Handle<std::vector<CTPPSLocalTrackLite>> ppsTracks;
  iEvent.getByToken(pps_token_, ppsTracks);

  for (const auto &trk : *ppsTracks) {
    const CTPPSDetId detid(trk.getRPId());

    // transform the raw, 32-bit unsigned integer detId into the TOTEM
    // "decimal" notation
    const unsigned short raw_id =
        100 * detid.arm() + 10 * detid.station() + detid.rp();

    (*pps_track_x_).push_back(trk.getX());
    (*pps_track_y_).push_back(trk.getY());
    (*pps_track_rpid_).push_back(raw_id);
  }

  // Full reco protons
  edm::Handle<vector<reco::ForwardProton>> recoMultiRPProtons;
  iEvent.getByToken(recoProtonsMultiRPToken_, recoMultiRPProtons);
  edm::Handle<vector<reco::ForwardProton>> recoSingleRPProtons;
  iEvent.getByToken(recoProtonsSingleRPToken_, recoSingleRPProtons);

  int ismultirp = -999;
  unsigned int decRPId = -999;
  unsigned int armId = -999;
  float th_y = -999;
  float th_x = -999;
  float x_star = -999;
  float y_star = -999;
  float t = -999;
  float xi = -999;
  float trackx1 = -999.;
  float tracky1 = -999.;
  float trackx2 = -999.;
  float tracky2 = -999.;
  unsigned int trackrpid1 = -999;
  unsigned int trackrpid2 = -999;
  int pixshift1 = -999;
  int pixshift2 = -999;

  // Single-RP
  for (const auto &proton : *recoSingleRPProtons) {
    if (proton.validFit()) {
      th_y = proton.thetaY();
      th_x = proton.thetaX();
      x_star = proton.vx();
      y_star = proton.vy();
      xi = proton.xi();
      t = proton.t();

      trackx1 = (*proton.contributingLocalTracks().begin())->getX();
      tracky1 = (*proton.contributingLocalTracks().begin())->getY();

      CTPPSpixelLocalTrackReconstructionInfo pixtrackinfo1 =
          (*proton.contributingLocalTracks().begin())->getPixelTrackRecoInfo();
      if (pixtrackinfo1 ==
              CTPPSpixelLocalTrackReconstructionInfo::notShiftedRun ||
          pixtrackinfo1 ==
              CTPPSpixelLocalTrackReconstructionInfo::noShiftedPlanes ||
          pixtrackinfo1 == CTPPSpixelLocalTrackReconstructionInfo::invalid)
        pixshift1 = 0;
      else
        pixshift1 = 1;

      CTPPSDetId rpId((*proton.contributingLocalTracks().begin())->getRPId());
      decRPId = rpId.arm() * 100 + rpId.station() * 10 + rpId.rp();
      ismultirp = 0;

      (*proton_xi_).push_back(xi);
      (*proton_thy_).push_back(th_y);
      (*proton_thx_).push_back(th_x);
      (*proton_y_).push_back(y_star);
      (*proton_x_).push_back(x_star);
      (*proton_t_).push_back(t);
      (*proton_ismultirp_).push_back(ismultirp);
      (*proton_rpid_).push_back(decRPId);
      (*proton_arm_).push_back(armId);
      (*proton_trackx1_).push_back(trackx1);
      (*proton_tracky1_).push_back(tracky1);
      (*proton_trackpixshift1_).push_back(pixshift1);
      (*proton_rpid1_).push_back(decRPId);
    }
  }

  // Multi-RP
  for (const auto &proton : *recoMultiRPProtons) {
    if (proton.validFit()) {
      th_y = proton.thetaY();
      th_x = proton.thetaX();
      x_star = proton.vx();
      y_star = proton.vy();
      xi = proton.xi();
      t = proton.t();

      int ij = 0;
      for (const auto &tr : proton.contributingLocalTracks()) {
        CTPPSDetId rpIdJ(tr->getRPId());
        unsigned int rpDecIdJ =
            rpIdJ.arm() * 100 + rpIdJ.station() * 10 + rpIdJ.rp();

        CTPPSpixelLocalTrackReconstructionInfo pixtrackinfo =
            (*proton.contributingLocalTracks().begin())
                ->getPixelTrackRecoInfo();

        if (ij == 0) {
          trackx1 = tr->getX();
          tracky1 = tr->getY();
          trackrpid1 = rpDecIdJ;
          armId = rpIdJ.arm();
          if (pixtrackinfo ==
                  CTPPSpixelLocalTrackReconstructionInfo::notShiftedRun ||
              pixtrackinfo ==
                  CTPPSpixelLocalTrackReconstructionInfo::noShiftedPlanes ||
              pixtrackinfo == CTPPSpixelLocalTrackReconstructionInfo::invalid)
            pixshift1 = 0;
          else
            pixshift1 = 1;
        }
        if (ij == 1) {
          trackx2 = tr->getX();
          tracky2 = tr->getY();
          trackrpid2 = rpDecIdJ;
          if (pixtrackinfo ==
                  CTPPSpixelLocalTrackReconstructionInfo::notShiftedRun ||
              pixtrackinfo ==
                  CTPPSpixelLocalTrackReconstructionInfo::noShiftedPlanes ||
              pixtrackinfo == CTPPSpixelLocalTrackReconstructionInfo::invalid)
            pixshift2 = 0;
          else
            pixshift2 = 1;
        }
        ij++;
      }

      ismultirp = 1;

      (*proton_xi_).push_back(xi);
      (*proton_thy_).push_back(th_y);
      (*proton_thx_).push_back(th_x);
      (*proton_y_).push_back(y_star);
      (*proton_x_).push_back(x_star);
      (*proton_t_).push_back(t);
      (*proton_ismultirp_).push_back(ismultirp);
      (*proton_rpid_).push_back(decRPId);
      (*proton_arm_).push_back(armId);
      (*proton_trackx1_).push_back(trackx1);
      (*proton_tracky1_).push_back(tracky1);
      (*proton_trackx2_).push_back(trackx2);
      (*proton_tracky2_).push_back(tracky2);
      (*proton_trackpixshift1_).push_back(pixshift1);
      (*proton_trackpixshift2_).push_back(pixshift2);
      (*proton_rpid1_).push_back(trackrpid1);
      (*proton_rpid2_).push_back(trackrpid2);
    }
  }

  // Run and vertex multiplicity info
  *run_ = iEvent.id().run();
  *ev_ = iEvent.id().event();
  *lumiblock_ = iEvent.luminosityBlock();

  // Crossing angle information from DB - available for all years in legacy
  // re-RECO
  edm::ESHandle<LHCInfo> pSetup;
  const string label = "";
  iSetup.get<LHCInfoRcd>().get(label, pSetup);

  // re-initialise algorithm upon crossing-angle change
  const LHCInfo *pInfo = pSetup.product();
  *crossingangle_ = pInfo->crossingAngle();
  tree_->Fill();

  (*pps_track_x_).clear();
  (*pps_track_y_).clear();
  (*pps_track_rpid_).clear();

  (*proton_xi_).clear();
  (*proton_thy_).clear();
  (*proton_thx_).clear();
  (*proton_y_).clear();
  (*proton_x_).clear();
  (*proton_t_).clear();
  (*proton_ismultirp_).clear();
  (*proton_rpid_).clear();
  (*proton_arm_).clear();
  // New for legacy
  (*proton_trackx1_).clear();
  (*proton_tracky1_).clear();
  (*proton_trackx2_).clear();
  (*proton_tracky2_).clear();
  (*proton_trackpixshift1_).clear();
  (*proton_trackpixshift2_).clear();
  (*proton_rpid1_).clear();
  (*proton_rpid2_).clear();
  (*l1prescale).clear();
  (*hltprescale).clear();

  (*hlt_).clear();
}

protonsNtuplizer::~protonsNtuplizer() {

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

// ------------ method called once each job just before starting event loop
// ------------
void protonsNtuplizer::beginJob() {

  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("ntp1", "ntp1");

  pps_track_x_ = new std::vector<float>;
  pps_track_y_ = new std::vector<float>;
  pps_track_rpid_ = new std::vector<int>;

  proton_xi_ = new std::vector<float>;
  proton_thy_ = new std::vector<float>;
  proton_thx_ = new std::vector<float>;
  proton_y_ = new std::vector<float>;
  proton_x_ = new std::vector<float>;
  proton_t_ = new std::vector<float>;
  proton_ismultirp_ = new std::vector<int>;
  proton_rpid_ = new std::vector<int>;
  proton_arm_ = new std::vector<int>;
  // New for legacy
  proton_trackx1_ = new std::vector<float>;
  proton_tracky1_ = new std::vector<float>;
  proton_trackx2_ = new std::vector<float>;
  proton_tracky2_ = new std::vector<float>;
  proton_trackpixshift1_ = new std::vector<int>;
  proton_trackpixshift2_ = new std::vector<int>;
  proton_rpid1_ = new std::vector<int>;
  proton_rpid2_ = new std::vector<int>;
  l1prescale = new std::vector<int>;
  hltprescale = new std::vector<int>;

  hlt_ = new std::vector<string>;

  ev_ = new long int;
  run_ = new int;
  lumiblock_ = new int;
  crossingangle_ = new float;

  tree_->Branch("pps_track_x", &pps_track_x_);
  tree_->Branch("pps_track_y", &pps_track_y_);
  tree_->Branch("pps_track_rpid", &pps_track_rpid_);
  tree_->Branch("proton_xi", &proton_xi_);
  tree_->Branch("proton_thy", &proton_thy_);
  tree_->Branch("proton_thx", &proton_thx_);
  tree_->Branch("proton_y", &proton_y_);
  tree_->Branch("proton_x", &proton_x_);
  tree_->Branch("proton_t", &proton_t_);
  tree_->Branch("proton_ismultirp", &proton_ismultirp_);
  tree_->Branch("proton_rpid", &proton_rpid_);
  tree_->Branch("proton_arm", &proton_arm_);
  // New for legacy
  tree_->Branch("proton_trackx1", &proton_trackx1_);
  tree_->Branch("proton_tracky1", &proton_tracky1_);
  tree_->Branch("proton_trackx2", &proton_trackx2_);
  tree_->Branch("proton_tracky2", &proton_tracky2_);
  tree_->Branch("proton_trackpixshift1", &proton_trackpixshift1_);
  tree_->Branch("proton_trackpixshift2", &proton_trackpixshift2_);
  tree_->Branch("proton_rpid1", &proton_rpid1_);
  tree_->Branch("proton_rpid2", &proton_rpid2_);
  tree_->Branch("hltprescale", &hltprescale);
  tree_->Branch("l1prescale", &l1prescale);

  tree_->Branch("hlt", &hlt_);

  tree_->Branch("run", run_, "run/I");
  tree_->Branch("event", ev_, "event/L");
  tree_->Branch("lumiblock", lumiblock_, "lumiblock/I");
  tree_->Branch("crossingangle", crossingangle_, "crossingangle/F");
}

// ------------ method called once each job just after ending the event loop
// ------------
void protonsNtuplizer::endJob() {

  delete pps_track_x_;
  delete pps_track_y_;
  delete pps_track_rpid_;
  delete proton_xi_;
  delete proton_thy_;
  delete proton_thx_;
  delete proton_y_;
  delete proton_x_;
  delete proton_t_;
  delete proton_ismultirp_;
  delete proton_rpid_;
  delete proton_arm_;
  // New for legacy
  delete proton_trackx1_;
  delete proton_tracky1_;
  delete proton_trackx2_;
  delete proton_tracky2_;
  delete proton_trackpixshift1_;
  delete proton_trackpixshift2_;
  delete proton_rpid1_;
  delete proton_rpid2_;
  delete l1prescale;
  delete hltprescale;

  delete hlt_;
}

// ------------ method fills 'descriptions' with the allowed parameters for the
// module  ------------
void protonsNtuplizer::fillDescriptions(
    edm::ConfigurationDescriptions &descriptions) {
  // The following says we do not know what parameters are allowed so do no
  // validation
  // Please change this to state exactly what you do use, even if it is no
  // parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// define this as a plug-in
DEFINE_FWK_MODULE(protonsNtuplizer);

/*************************************************************
 *
 * Author: A. Bellora - INFN Torino
 *
 *************************************************************/
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ProtonReco/interface/ForwardProton.h"

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandomEngine.h"

#include "TFile.h"
#include "TH2D.h"

#include "protonPreMix/protonPreMix/interface/PUFileReader.h"

using namespace std;

class PPSEfficiencyProducer : public edm::stream::EDProducer<> {

public:
  explicit PPSEfficiencyProducer(const edm::ParameterSet &param);

  ~PPSEfficiencyProducer() override;

  void produce(edm::Event &, const edm::EventSetup &) override;

  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  double maxDelta_xi = 0.1;
  double maxDelta_t = 0.7;
  int surviving_protons = 0;
  int recoProton_counter = 0;
  int matches_counter = 0;
  int multiMatch_counter = 0;
  int total_genProtons = 0;
  int nEvents = 0;

  int verbosity_;
  bool outputSummary_;

  string efficiencyFileName_Near_, efficiencyFileName_Far_;
  edm::InputTag mixedProtonsSrc_, genParticlesSrc_;

  int year_;
  string era_;

  edm::EDGetTokenT<vector<reco::ForwardProton>> tokenCTPPSProtons_;
  edm::EDGetTokenT<reco::GenParticleCollection> tokenGenParticles_;

  unique_ptr<TFile> efficiencyFile_Near_, efficiencyFile_Far_;

  map<CTPPSDetId, TH2D *> efficiencyMaps_;

  TFile *outputFile_;

  // Output histograms
  map<int, TH1D *> h1DeltaXi;
  map<int, TH1D *> h1DeltaT;
};

PPSEfficiencyProducer::PPSEfficiencyProducer(const edm::ParameterSet &conf)
    : verbosity_(conf.getUntrackedParameter<int>("verbosity")),
      outputSummary_(conf.getUntrackedParameter<bool>("outputSummary")),
      efficiencyFileName_Near_(
          conf.getParameter<string>("efficiencyFileName_Near")),
      efficiencyFileName_Far_(
          conf.getParameter<string>("efficiencyFileName_Far")),
      mixedProtonsSrc_(conf.getParameter<edm::InputTag>("mixedProtonsSrc")),
      genParticlesSrc_(conf.getParameter<edm::InputTag>("genParticlesSrc")),
      year_(conf.getParameter<int>("year")),
      era_(conf.getParameter<string>("era")) {

  tokenCTPPSProtons_ = consumes<vector<reco::ForwardProton>>(mixedProtonsSrc_);
  tokenGenParticles_ = consumes<reco::GenParticleCollection>(genParticlesSrc_);

  // Create smart pointers to files and open them
  unique_ptr<TFile> efficiencyFile_Near_ =
      unique_ptr<TFile>(TFile::Open(efficiencyFileName_Near_.data()));
  unique_ptr<TFile> efficiencyFile_Far_ =
      unique_ptr<TFile>(TFile::Open(efficiencyFileName_Far_.data()));

  vector<TString> sectors = {"45", "56"};

  // Save pointers to efficiency maps for later use
  if (efficiencyFile_Near_) {
    for (auto sector : sectors) {
      int arm = (sector == "45") ? 0 : 1;
      string map_name;
      CTPPSDetId detId(3, 0, 0, 0); // dummy detId

      if (year_ == 2016) {
        string rpid = (sector == "45") ? "2" : "102";
        detId = CTPPSDetId(3, arm, 0, 2);
        map_name = "Strips/" + to_string(year_) + "/" + to_string(year_) +
                   era_ + "/h" + sector + "_" + to_string(year_) + era_ +
                   "_all_2D_RP" + rpid;
      }else if (year_ == 2017) {
        detId = CTPPSDetId(3, arm, 0, 3);
        map_name = "Strips/" + to_string(year_) + "/" + to_string(year_) +
                   era_ + "/h" + sector + "_" + to_string(year_) + era_ +
                   "_all_2D";
      }else if (year_ == 2018) {
        detId = CTPPSDetId(4, arm, 0, 3);
        map_name = "Pixel/" + to_string(year_) + "/" + to_string(year_) + era_ +
                   "/" + "h" + sector + "_" + 210 + "_" + to_string(year_) +
                   era_ + "_all_2D";
      }
      else throw cms::Exception("PPSEfficiencyProducer")
            << "PPSEfficiencyProducer requires year to be defined";

      if (efficiencyFile_Near_->Get(map_name.data()) != NULL) {
        efficiencyMaps_[detId] =
            (TH2D *)efficiencyFile_Near_->Get(map_name.data());
      } else {
        edm::LogError("PPS")
            << "Efficiency map: " << map_name << " NOT FOUND in "
            << efficiencyFile_Near_->GetName();
        throw cms::Exception("PPS")
            << "PPSEfficiencyProducer requires efficiency maps to be defined";

      }
    }
  }

  if (efficiencyFile_Far_) {
    for (auto sector : sectors) {
      int arm = (sector == "45") ? 0 : 1;
      string map_name;
      CTPPSDetId detId(3, arm, 2, 0); // dummy detId

      if (year_ == 2016) {
        string rpid = (sector == "45") ? "2" : "102";
        detId = CTPPSDetId(3, arm, 0, 3);
        map_name = "Strips/" + to_string(year_) + "/" + to_string(year_) +
                   era_ + "/h" + sector + "_" + to_string(year_) + era_ +
                   "_all_2D_RP" + rpid;
      }
      if (year_ == 2017) {
        detId = CTPPSDetId(4, arm, 2, 3);
        map_name = "Pixel/" + to_string(year_) + "/" + to_string(year_) + era_ +
                   "/" + "h" + sector + "_" + 220 + "_" + to_string(year_) +
                   era_ + "_all_2D";
      }
      if (year_ == 2018) {
        detId = CTPPSDetId(4, arm, 2, 3);
        map_name = "Pixel/" + to_string(year_) + "/" + to_string(year_) + era_ +
                   "/" + "h" + sector + "_" + 220 + "_" + to_string(year_) +
                   era_ + "_all_2D";
      }
      if (efficiencyFile_Far_->Get(map_name.data()) != NULL) {
        efficiencyMaps_[detId] =
            (TH2D *)efficiencyFile_Far_->Get(map_name.data());
      } else {
        edm::LogError("PPS")
            << "Efficiency map: " << map_name << " NOT FOUND in "
            << efficiencyFile_Far_->GetName();
        throw cms::Exception("PPS")
            << "PPSEfficiencyProducer requires efficiency maps to be defined";
      }
    }
  }

  produces<vector<reco::ForwardProton>>("multiRP");

  if (verbosity_ > 0)
    edm::LogInfo("PPS") << "Creating...";
  if (outputSummary_)
    outputFile_ = new TFile("PPSEfficiencyProducer.root", "RECREATE");
}

PPSEfficiencyProducer::~PPSEfficiencyProducer() {
  if (outputSummary_) {
    outputFile_->cd();
    vector<int> arms = {0, 1};
    for (auto it : h1DeltaXi) {
      h1DeltaXi[it.first]->Write();
      h1DeltaT[it.first]->Write();
      delete h1DeltaXi[it.first];
      delete h1DeltaT[it.first];
    }
    outputFile_->Close();
    delete outputFile_;
  }
  if (verbosity_)
    edm::LogInfo("PPS")
        << "Reconstructed protons:" << recoProton_counter << "\n"
        << "Matches found:" << matches_counter << "\n"
        << "Multiple matches found:" << multiMatch_counter << "\n"
        << "Surviving matched protons:" << surviving_protons << "\n"
        << "Total generated protons:" << total_genProtons << "\n"
        << "Events processed:" << nEvents << "\n";
}

void PPSEfficiencyProducer::fillDescriptions(
    edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<int>("verbosity", 0);
  desc.addUntracked<bool>("outputSummary", false);
  desc.add<string>("efficiencyFileName_Near", "");
  desc.add<string>("efficiencyFileName_Far", "");
  desc.add<edm::InputTag>("mixedProtonsSrc",
                          edm::InputTag("ctppsProtons", "multiRP"));
  desc.add<edm::InputTag>("genParticlesSrc", edm::InputTag("genParticles"));
  desc.add<int>("year", 0);
  desc.add<string>("era", "X");
  descriptions.add("ppsEfficiencyProducer", desc);
}

void PPSEfficiencyProducer::produce(edm::Event &iEvent,
                                    const edm::EventSetup &iSetup) {
  nEvents++;
  // Need rng to add efficiency effects
  edm::Service<edm::RandomNumberGenerator> rng;
  if (!rng.isAvailable()) {
    throw cms::Exception("Configuration")
        << "PPSEfficiencyProducer requires the "
           "RandomNumberGeneratorService,\n"
           "which is not present in the configuration file. You must add\n"
           "the service in the configuration file.\n";
  }
  CLHEP::HepRandomEngine &engine = rng->getEngine(iEvent.streamID());

  edm::Handle<vector<reco::ForwardProton>> mixedProtons;
  iEvent.getByToken(tokenCTPPSProtons_, mixedProtons);

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByToken(tokenGenParticles_, genParticles);

  vector<reco::ForwardProton> outputProtons;

  // Select only protons and store them
  vector<reco::GenParticle> genProtons;
  map<CTPPSDetId, int> matches;
  for (auto genParticle : *genParticles) {
    if (genParticle.pdgId() == 2212 && fabs(genParticle.pz()) > 3000 &&
        genParticle.status() == 1) {
      genProtons.push_back(genParticle);
      total_genProtons++;
    }
  }
  if (genProtons.size() > 2)
    edm::LogError("PPS")
        << "Found more than two outgoing gen-level protons";

  if (genProtons.size() < 2)
    edm::LogError("PPS")
        << "Found less than two outgoing gen-level protons";

  vector<reco::ForwardProton> genMatchProtons;
  // For each reco proton, try to match it with the gen proton
  for (auto &proton : *mixedProtons) {
    recoProton_counter++;

    double xi = proton.xi();
    double t = proton.t();
    CTPPSDetId detId =
        CTPPSDetId((*(proton.contributingLocalTracks().at(0))).getRPId());

    if (verbosity_ > 1)
      edm::LogInfo("PPS") << "Trying to match proton with: \n"
                                            << "ID: " << detId << "\n"
                                            << "Xi = " << xi << "\n"
                                            << "t = " << t << "\n"
                                            << "Pz = " << proton.pz();
    for (auto &genProton : genProtons) {
      // Check that the two protons are on the same side
      if (genProton.pz() * proton.pz() > 0) {
        double gen_xi = 1 - fabs(genProton.pz() / 6500.);
        double gen_t;
        if (genProton.pz() > 0)
          gen_t = (6500. - genProton.energy()) * (6500. - genProton.energy()) -
                  genProton.px() * genProton.px() -
                  genProton.py() * genProton.py() -
                  (6500. - genProton.pz()) * (6500. - genProton.pz());
        else
          gen_t = (6500. - genProton.energy()) * (6500. - genProton.energy()) -
                  genProton.px() * genProton.px() -
                  genProton.py() * genProton.py() -
                  (-6500. - genProton.pz()) * (-6500. - genProton.pz());
        if (verbosity_ > 1)
          edm::LogInfo("PPS") << "Gen proton kinematics: \n"
                                                << "Xi = " << gen_xi << "\n"
                                                << "t = " << gen_t << "\n"
                                                << "Pz = " << genProton.pz();

        double delta_xi = gen_xi - xi;
        double delta_t = gen_t - t;

        int arm = detId.arm();

        // Book the delta histograms
        if (outputSummary_) {
          if (h1DeltaXi.find(arm) == h1DeltaXi.end()) {
            h1DeltaXi[arm] =
                new TH1D(Form("h1DeltaXi_arm%i", arm),
                         Form("h1DeltaXi_arm%i", arm), 200, -0.1, 0.1);
            h1DeltaT[arm] = new TH1D(Form("h1DeltaT_arm%i", arm),
                                     Form("h1DeltaT_arm%i", arm), 200, -1, 1);
          }
          h1DeltaXi[arm]->Fill(delta_xi);
          if (fabs(delta_xi) < maxDelta_xi)
            h1DeltaT[arm]->Fill(delta_t);
        }

        // Find genMatch for proton
        if (fabs(delta_xi) > maxDelta_xi || fabs(delta_t) > maxDelta_t) {
          outputProtons.push_back(proton);
          if (verbosity_ > 1)
            edm::LogInfo("PPS") << "Proton comes from PU";
        } else {
          matches_counter++;
          genMatchProtons.push_back(proton);
          matches[detId]++;
          if (verbosity_ > 1)
            edm::LogInfo("PPS") << "GenMatch found";
        }
      }
    }
  }

  // Check for multiple matches
  for (auto match : matches) {
    if (match.second > 1) {
      multiMatch_counter++;
      edm::LogWarning("PPS")
          << "Found more than one match in " << match.first;
    }
  }

  // Extract the efficiency and throw an rng to see if the proton survives
  for (auto &proton : genMatchProtons) {
    CTPPSLocalTrackLite track_Near = *(proton.contributingLocalTracks().at(0));
    CTPPSLocalTrackLite track_Far = *(proton.contributingLocalTracks().at(1));
    CTPPSDetId detId_Near = CTPPSDetId(track_Near.getRPId());
    CTPPSDetId detId_Far = CTPPSDetId(track_Far.getRPId());
    if (verbosity_ > 1)
      edm::LogInfo("PPS") << "Accessing efficiency maps...";
    double eff_Near = efficiencyMaps_[detId_Near]->GetBinContent(
        efficiencyMaps_[detId_Near]->FindBin(track_Near.getX(),
                                             track_Near.getY()));
    double eff_Far = efficiencyMaps_[detId_Far]->GetBinContent(
        efficiencyMaps_[detId_Far]->FindBin(track_Far.getX(),
                                            track_Far.getY()));
    if (verbosity_)
      edm::LogInfo("PPS")
          << "Efficiency for " << detId_Near << "= " << eff_Near
          << "\nEfficiency for " << detId_Far << "= " << eff_Far;

    // Compute the total efficiency
    double efficiency = eff_Near * eff_Far;

    // Throw a random number and save the proton if it survives
    if (engine.flat() < efficiency) {
      if (verbosity_)
        edm::LogInfo("PPS") << "Proton lives gloriously";
      surviving_protons++;
      outputProtons.push_back(proton);
    } else if (verbosity_)
      edm::LogInfo("PPS") << "Proton dies miserably";
  }

  // Save the final protons
  iEvent.put(std::make_unique<vector<reco::ForwardProton>>(outputProtons),
             "multiRP");
}

// define this as a plugin
DEFINE_FWK_MODULE(PPSEfficiencyProducer);
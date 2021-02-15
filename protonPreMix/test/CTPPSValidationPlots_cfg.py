import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from Configuration.ProcessModifiers.run2_miniAOD_UL_cff import run2_miniAOD_UL
#process = cms.Process('TEST', eras.Run2_2016, run2_miniAOD_UL)
# process = cms.Process('TEST', eras.Run2_2017, run2_miniAOD_UL)                   # !!! to adapt depending on the year
process = cms.Process('TEST', eras.Run2_2018, run2_miniAOD_UL)

outFile_suffix = "_pileup_EGamma_2018A_ReMiniAOD"
## direct proton simulation

import Validation.CTPPS.simu_config.year_2018_cff as config               # !!! to adapt depending on the year
process.load("Validation.CTPPS.simu_config.year_2018_cff")                # !!! to adapt depending on the y
config.SetDefaults(process)

# track distribution plotter
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
  tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),

  rpId_45_F = process.rpIds.rp_45_F,
  rpId_45_N = process.rpIds.rp_45_N,
  rpId_56_N = process.rpIds.rp_56_N,
  rpId_56_F = process.rpIds.rp_56_F,

  outputFile = cms.string("output_tracks"+outFile_suffix+".root")
)

# reconstruction plotter (analysis example)
process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
  tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
  tagRecoProtonsSingleRP = cms.InputTag("ctppsProtons", "singleRP"),
  # tagRecoProtonsMultiRP = cms.InputTag("ppsEfficiencyProducer", "multiRP"),
  tagRecoProtonsMultiRP = cms.InputTag("ctppsProtons", "multiRP"),
  rpId_45_F = cms.uint32(23),
  rpId_45_N = cms.uint32(3),
  rpId_56_N = cms.uint32(103),
  rpId_56_F = cms.uint32(123),

  association_cuts_45 = process.ctppsProtons.association_cuts_45,
  association_cuts_56 = process.ctppsProtons.association_cuts_56,

  outputFile = cms.string("output_protons"+outFile_suffix+".root")
)

# reconstruction plotter (analysis example)
process.ctppsLHCInfoPlotter = cms.EDAnalyzer("CTPPSLHCInfoPlotter",
  lhcInfoLabel = cms.string(''),
  outputFile = cms.string("output_lhcInfo"+outFile_suffix+".root")
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
 # 2017 EraB SingleEle
 # "file:/eos/cms/store/group/phys_pps/ReMiniAODSkimForEfficiencies/2017ReMiniAODSkimForEff_SingleEle_2017B/SingleElectron/ReMiniAODSkimForEff_SingleEle_2017B/201021_082238/0000/ReMiniAOD_SkimForEfficiency_1.root"
 # 2017 EraD SingleEle
 # "file:/eos/cms/store/group/phys_pps/ReMiniAODSkimForEfficiencies/2018ReMiniAODSkimForEff_SingleEle_2018D/EGamma/ReMiniAODSkimForEff_SingleEle_2018D/200925_172800/0000/ReMiniAOD_SkimForEfficiency_92.root"
 # 2018 EraA SingleEle
 "file:/afs/cern.ch/work/a/abellora/Work/PPtoPPWWjets_analysis/newInstall/CMSSW_10_6_17/src/protonPreMix/protonPreMix/test/ReMiniAOD_SkimForEfficiency_EGamma_2018A.root"
 # 2018 EraA SingleMuon
 # "file:/afs/cern.ch/work/a/abellora/Work/PPtoPPWWjets_analysis/newInstall/CMSSW_10_6_17/src/protonPreMix/protonPreMix/test/ReMiniAOD_SkimForEfficiency_SingleMuon_2018A.root"
  ),
  dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
  inputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_*_*_ReMiniAOD',
  )
)

# add some logs
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring('cout')
process.MessageLogger.cout = cms.untracked.PSet( 
  threshold = cms.untracked.string('INFO'),
  FwkReport = cms.untracked.PSet(
    optionalPSet = cms.untracked.bool(True),
    reportEvery = cms.untracked.int32(10),
    limit = cms.untracked.int32(50000000)
  ),
)
process.MessageLogger.statistics = cms.untracked.vstring()
process.options = cms.untracked.PSet(
      wantSummary = cms.untracked.bool(True),
)
        

# number of events
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(10000)
)

process.path = cms.Path(
  # example of analysis
  process.ctppsLHCInfoPlotter
  * process.ctppsTrackDistributionPlotter
  * process.ctppsProtonReconstructionPlotter
)


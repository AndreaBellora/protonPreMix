import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from Configuration.Eras.Modifier_ctpps_2018_cff import ctpps_2018
from Configuration.ProcessModifiers.run2_miniAOD_UL_cff import run2_miniAOD_UL
process = cms.Process('TEST', ctpps_2018, run2_miniAOD_UL)

# outFile_suffix = "_mix_noEff_SingleMuon_2018A"
outFile_suffix = "_mix_noEff_EGamma_2018A"
## direct proton simulation

process.load('Configuration.EventContent.EventContent_cff')

import Validation.CTPPS.simu_config.year_2018_cff as config               # !!! to adapt depending on the year
process.load("Validation.CTPPS.simu_config.year_2018_cff")                # !!! to adapt depending on the y
config.SetDefaults(process)

process.ctppsRPAlignmentCorrectionsDataESSourceXML.MisalignedFiles = ["protonPreMix/protonPreMix/test/PPS_2018_Alignments/2018_preTS1.xml"]
process.ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = ["protonPreMix/protonPreMix/test/PPS_2018_Alignments/2018_preTS1.xml"]
# process.ctppsRPAlignmentCorrectionsDataESSourceXML.MisalignedFiles = ["protonPreMix/protonPreMix/test/PPS_2018_Alignments/2018_TS1_TS2.xml"]
# process.ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = ["protonPreMix/protonPreMix/test/PPS_2018_Alignments/2018_TS1_TS2.xml"]
# process.ctppsRPAlignmentCorrectionsDataESSourceXML.MisalignedFiles = ["protonPreMix/protonPreMix/test/PPS_2018_Alignments/2018_postTS2.xml"]
# process.ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = ["protonPreMix/protonPreMix/test/PPS_2018_Alignments/2018_postTS2.xml"]

process.ctppsOpticalFunctionsESSource.configuration[0].opticalFunctions = cms.VPSet(
  cms.PSet( xangle = cms.double(120), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version6/120urad.root") ),
  cms.PSet( xangle = cms.double(130), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version6/130urad.root") ),
  cms.PSet( xangle = cms.double(140), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version6/140urad.root") )
)

# # override LHCInfo source
# process.load("CalibPPS.ESProducers.ctppsLHCInfoRandomXangleESSource_cfi")
# process.ctppsLHCInfoRandomXangleESSource.generateEveryNEvents = 1
# process.ctppsLHCInfoRandomXangleESSource.xangleHistogramFile = "CrossingAngles2017.root"
# process.ctppsLHCInfoRandomXangleESSource.xangleHistogramObject = "hxang"
# process.ctppsLHCInfoRandomXangleESSource.beamEnergy = 6500.
# process.ctppsLHCInfoRandomXangleESSource.betaStar = 0.40

# process.esPreferLHCInfo = cms.ESPrefer("CTPPSLHCInfoRandomXangleESSource", "ctppsLHCInfoRandomXangleESSource")

# update settings of beam-smearing module                                                                                                                                                                
process.beamDivergenceVtxGenerator.src = cms.InputTag("")
process.beamDivergenceVtxGenerator.srcGenParticle = cms.VInputTag(
    cms.InputTag("genParticles","","SIM")
)

# re-shift CMS vtx                                                                                                                                                           
process.load("CalibPPS.ESProducers.ctppsBeamParametersFromLHCInfoESSource_cfi")
process.ctppsBeamParametersFromLHCInfoESSource.beamDivX45 = process.ctppsBeamParametersESSource.beamDivX45
process.ctppsBeamParametersFromLHCInfoESSource.beamDivX56 = process.ctppsBeamParametersESSource.beamDivX56
process.ctppsBeamParametersFromLHCInfoESSource.beamDivY45 = process.ctppsBeamParametersESSource.beamDivY45
process.ctppsBeamParametersFromLHCInfoESSource.beamDivY56 = process.ctppsBeamParametersESSource.beamDivY56

process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetX45 = process.ctppsBeamParametersESSource.vtxOffsetX45
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetX56 = process.ctppsBeamParametersESSource.vtxOffsetX56
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetY45 = process.ctppsBeamParametersESSource.vtxOffsetY45
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetY56 = process.ctppsBeamParametersESSource.vtxOffsetY56
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetZ45 = process.ctppsBeamParametersESSource.vtxOffsetZ45
process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetZ56 = process.ctppsBeamParametersESSource.vtxOffsetZ56

process.ctppsBeamParametersFromLHCInfoESSource.vtxStddevX = process.ctppsBeamParametersESSource.vtxStddevX
process.ctppsBeamParametersFromLHCInfoESSource.vtxStddevY = process.ctppsBeamParametersESSource.vtxStddevY
process.ctppsBeamParametersFromLHCInfoESSource.vtxStddevZ = process.ctppsBeamParametersESSource.vtxStddevZ

# do not apply vertex smearing again
process.ctppsBeamParametersESSource.vtxStddevX = 0
process.ctppsBeamParametersESSource.vtxStddevY = 0
process.ctppsBeamParametersESSource.vtxStddevZ = 0

# undo CMS vertex shift
#process.ctppsBeamParametersESSource.vtxOffsetX45 = +0.2475 * 1E-1
#process.ctppsBeamParametersESSource.vtxOffsetY45 = -0.6924 * 1E-1
#process.ctppsBeamParametersESSource.vtxOffsetZ45 = -8.1100 * 1E-1
# These should be the vtx shift values for 2017...
process.ctppsBeamParametersESSource.vtxOffsetX45 = +0.024793
process.ctppsBeamParametersESSource.vtxOffsetY45 = -0.0692861
process.ctppsBeamParametersESSource.vtxOffsetZ45 = -7.89895

# processing path                                                                                                                                                                                        
process.simProtonStep = cms.Sequence(
    process.beamDivergenceVtxGenerator
  * process.ctppsDirectProtonSimulation
)

## PreMix protons

process.load("protonPreMix.protonPreMix.ctppsPreMixProducer_cfi")
process.ctppsPreMixProducer.PUFilesList = cms.vstring(
 # 2017 EraB SingleEle
 # "file:/eos/cms/store/group/phys_pps/ReMiniAODSkimForEfficiencies/2017ReMiniAODSkimForEff_SingleEle_2017B/SingleElectron/ReMiniAODSkimForEff_SingleEle_2017B/201021_082238/0000/ReMiniAOD_SkimForEfficiency_1.root"
 # 2017 EraD SingleEle
 # "file:/eos/cms/store/group/phys_pps/ReMiniAODSkimForEfficiencies/2018ReMiniAODSkimForEff_SingleEle_2018D/EGamma/ReMiniAODSkimForEff_SingleEle_2018D/200925_172800/0000/ReMiniAOD_SkimForEfficiency_92.root"
 # 2018 EraA EGamma
 "file:/afs/cern.ch/work/a/abellora/Work/PPtoPPWWjets_analysis/newInstall/CMSSW_10_6_17/src/protonPreMix/protonPreMix/test/ReMiniAOD_SkimForEfficiency_EGamma_2018A.root"
 # 2018 EraA SingleMuon
 # "file:/afs/cern.ch/work/a/abellora/Work/PPtoPPWWjets_analysis/newInstall/CMSSW_10_6_17/src/protonPreMix/protonPreMix/test/ReMiniAOD_SkimForEfficiency_SingleMuon_2018A.root"
)
# rng service for premixing
process.RandomNumberGeneratorService.ctppsPreMixProducer = cms.PSet(initialSeed = cms.untracked.uint32(42))

process.ctppsPreMixProducer.Verbosity = 0
process.ctppsPreMixProducer.Sim_CTPPSPixelRecHitTag = cms.InputTag("ctppsDirectProtonSimulation")
process.ctppsPreMixProducer.Sim_TotemRPRecHitTag = cms.InputTag("ctppsDirectProtonSimulation")

# Disable strips, because they are missing in 2018 PU samples
process.ctppsPreMixProducer.includeStrips = False # 2018
process.protonMixingStep = cms.Sequence(process.ctppsPreMixProducer)

## RECO config

# point the track producers to the correct products
process.ctppsPixelLocalTracks.label = "ctppsPreMixProducer"
# process.totemRPUVPatternFinder.tagRecHit = cms.InputTag("ctppsPreMixProducer")

# remove timing tracks from trackLites, they are not produced by protonPreMix
process.ctppsLocalTrackLiteProducer.includeDiamonds = False

# remove strips tracks from trackLites, they are disabled protonPreMix 
# process.ctppsLocalTrackLiteProducer.includeStrips = True # 2016-2017
process.ctppsLocalTrackLiteProducer.includeStrips = False # 2018

process.load("protonPreMix.protonPreMix.ppsEfficiencyProducer_cfi")

process.RandomNumberGeneratorService.ppsEfficiencyProducer = cms.PSet(initialSeed = cms.untracked.uint32(43))
process.ppsEfficiencyProducer.year = cms.int32(2018)
process.ppsEfficiencyProducer.era = cms.string("D1")
process.ppsEfficiencyProducer.efficiencyFileName_Near = cms.string("/eos/project/c/ctpps/subsystems/Pixel/RPixTracking/pixelEfficiencies_radiation.root")
process.ppsEfficiencyProducer.efficiencyFileName_Far = cms.string("/eos/project/c/ctpps/subsystems/Pixel/RPixTracking/pixelEfficiencies_radiation.root")


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

# lhcInfo plotter (analysis example)
process.ctppsLHCInfoPlotter = cms.EDAnalyzer("CTPPSLHCInfoPlotter",
  lhcInfoLabel = cms.string(''),
  outputFile = cms.string("output_lhcInfo"+outFile_suffix+".root")
)

# reconstruction sequence
process.recoStep = cms.Sequence(
  process.reco_local
  * process.ctppsProtons
)

# data source

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring("/store/mc/RunIIFall17DRPremix/GGToWW_bSM-ACW2e-5_13TeV-fpmc-herwig6/AODSIM/PU2017_94X_mc2017_realistic_v11-v2/80000/F6BDDE1E-AA27-E911-A06C-FA163EA9A90B.root"),

  dropDescendantsOfDroppedBranches = cms.untracked.bool(False),

  inputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_genParticles_*_*',
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
# process.theout = cms.OutputModule( "PoolOutputModule",
#                                    # SelectEvents = cms.untracked.PSet(
#                                    #     SelectEvents = cms.vstring(
#                                    #         'hltsel'
#                                    #     )
#                                    # ),
#      outputCommands = cms.untracked.vstring("keep *"),
#     # fileName = cms.untracked.string('ReMiniAOD_SkimForEfficiency_SingleMuon_2017B.root')
#     fileName = cms.untracked.string('output_data.root')
#     # fileName = cms.untracked.string('ReMiniAOD_SkimForEfficiency_EGamma_2018A.root')
# )

process.path = cms.Path(
  # proton simulation
  process.simProtonStep

  # proton mixing
  * process.protonMixingStep

  # standard reconstruction
  * process.recoStep

  # add efficiency
  # * process.ppsEfficiencyProducer 

  # example of analysis
  * process.ctppsLHCInfoPlotter
  * process.ctppsTrackDistributionPlotter
  * process.ctppsProtonReconstructionPlotter
)

# process.outpath = cms.EndPath(process.theout)

import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from Configuration.ProcessModifiers.run2_miniAOD_UL_cff import run2_miniAOD_UL
#process = cms.Process('TEST', eras.Run2_2016, run2_miniAOD_UL)
process = cms.Process('TEST', eras.Run2_2017, run2_miniAOD_UL)                   # !!! to adapt depending on the year
# process = cms.Process('TEST', eras.Run2_2018, run2_miniAOD_UL)

## direct proton simulation

process.load('Configuration.EventContent.EventContent_cff')

import Validation.CTPPS.simu_config.year_2017_preTS2_cff as config               # !!! to adapt depending on the year
process.load("Validation.CTPPS.simu_config.year_2017_preTS2_cff")                # !!! to adapt depending on the y
config.SetDefaults(process)

# override LHCInfo source
process.load("CalibPPS.ESProducers.ctppsLHCInfoRandomXangleESSource_cfi")
process.ctppsLHCInfoRandomXangleESSource.generateEveryNEvents = 1
process.ctppsLHCInfoRandomXangleESSource.xangleHistogramFile = "CrossingAngles2017.root"
process.ctppsLHCInfoRandomXangleESSource.xangleHistogramObject = "hxang"
process.ctppsLHCInfoRandomXangleESSource.beamEnergy = 6500.
process.ctppsLHCInfoRandomXangleESSource.betaStar = 0.40

process.esPreferLHCInfo = cms.ESPrefer("CTPPSLHCInfoRandomXangleESSource", "ctppsLHCInfoRandomXangleESSource")

# update settings of beam-smearing module                                                                                                                                                                
process.beamDivergenceVtxGenerator.src = cms.InputTag("")
process.beamDivergenceVtxGenerator.srcGenParticle = cms.VInputTag(
    cms.InputTag("genParticles","","SIM")
)

# processing path                                                                                                                                                                                        
process.simProtonStep = cms.Sequence(
    process.beamDivergenceVtxGenerator
  * process.ctppsDirectProtonSimulation
)

## PreMix protons

process.load("protonPreMix.protonPreMix.CTPPSPreMixProducer_cfi")
process.CTPPSPreMixProducer.PUFilesList = cms.vstring(
	"file:/eos/project-c/ctpps/subsystems/Pixel/RPixTracking/EfficiencyCalculation2017/ReRecoEras_CMSSW_10_6_10/SingleElectron/ctpps_2017B_pixelefficiency_singleele/200423_112331/0000/ReReco_2017B_1.root",
)
# rng service for premixing
process.RandomNumberGeneratorService.CTPPSPreMixProducer = cms.PSet(initialSeed = cms.untracked.uint32(42))

process.CTPPSPreMixProducer.Sim_CTPPSPixelRecHitTag = cms.InputTag("ctppsDirectProtonSimulation")
process.CTPPSPreMixProducer.Sim_TotemRPRecHitTag = cms.InputTag("ctppsDirectProtonSimulation")


process.protonMixingStep = cms.Sequence(process.CTPPSPreMixProducer)

## RECO config

# point the track producers to the correct products
process.ctppsPixelLocalTracks.label = "CTPPSPreMixProducer"
process.totemRPUVPatternFinder.tagRecHit = cms.InputTag("CTPPSPreMixProducer")

# remove timing tracks from trackLites, they are not produced by protonPreMix
process.ctppsLocalTrackLiteProducer.includeDiamonds = False

# reconstruction (if 2016 data, remove modules for RPs which did not exist at that time)
def RemoveModules(pr):
  pr.reco_local.remove(pr.ctppsPixelLocalTracks)
  pr.ctppsLocalTrackLiteProducer.includePixels = False
  pr.CTPPSPreMixProducer.includePixels = False

from Configuration.Eras.Modifier_ctpps_2016_cff import ctpps_2016
from Configuration.Eras.Modifier_ctpps_2017_cff import ctpps_2017
from Configuration.Eras.Modifier_ctpps_2018_cff import ctpps_2018
(eras.ctpps_2016 & ~ctpps_2017 & ~ctpps_2018).toModify(process, RemoveModules)


# track distribution plotter
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
  tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),

  rpId_45_F = process.rpIds.rp_45_F,
  rpId_45_N = process.rpIds.rp_45_N,
  rpId_56_N = process.rpIds.rp_56_N,
  rpId_56_F = process.rpIds.rp_56_F,

  outputFile = cms.string("output_tracks.root")
)

# reconstruction plotter (analysis example)
process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
  tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
  tagRecoProtonsSingleRP = cms.InputTag("ctppsProtons", "singleRP"),
  tagRecoProtonsMultiRP = cms.InputTag("ctppsProtons", "multiRP"),
  rpId_45_F = cms.uint32(23),
  rpId_45_N = cms.uint32(3),
  rpId_56_N = cms.uint32(103),
  rpId_56_F = cms.uint32(123),

  association_cuts_45 = process.ctppsProtons.association_cuts_45,
  association_cuts_56 = process.ctppsProtons.association_cuts_56,

  outputFile = cms.string("output_protons.root")
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
        

# number of events
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(5000)
)

# output module
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("test.root"),
    outputCommands = cms.untracked.vstring("keep *",
    )
)

process.path = cms.Path(
  # proton simulation
  process.simProtonStep

  # proton mixing
  * process.protonMixingStep

  # standard reconstruction
  * process.recoStep

  # example of analysis
  * process.ctppsTrackDistributionPlotter
  * process.ctppsProtonReconstructionPlotter
)

process.endPath = cms.EndPath(process.output)
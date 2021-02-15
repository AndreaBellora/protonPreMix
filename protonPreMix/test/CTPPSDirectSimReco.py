import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from Configuration.Eras.Modifier_ctpps_2018_cff import ctpps_2018
from Configuration.ProcessModifiers.run2_miniAOD_UL_cff import run2_miniAOD_UL
process = cms.Process('TEST', ctpps_2018, run2_miniAOD_UL)

# outFile_suffix = "_mix_noEff_SingleMuon_2018A"
outFile_suffix = "_EGamma_2018B2"
## direct proton simulation

process.load('Configuration.EventContent.EventContent_cff')

# for simulation

# import Validation.CTPPS.simu_config.year_2018_cff as config               # !!! to adapt depending on the year
# process.load("Validation.CTPPS.simu_config.year_2018_cff")                # !!! to adapt depending on the y
# config.SetDefaults(process)

# process.ctppsRPAlignmentCorrectionsDataESSourceXML.MisalignedFiles = ["protonPreMix/protonPreMix/test/PPS_2018_Alignments/2018_TS1_TS2.xml"]
# process.ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = ["protonPreMix/protonPreMix/test/PPS_2018_Alignments/2018_TS1_TS2.xml"]

# process.ctppsOpticalFunctionsESSource.configuration[0].opticalFunctions = cms.VPSet(
#   cms.PSet( xangle = cms.double(120), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version6/120urad.root") ),
#   cms.PSet( xangle = cms.double(130), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version6/130urad.root") ),
#   cms.PSet( xangle = cms.double(140), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version6/140urad.root") )
# )

# # # override LHCInfo source
# # process.load("CalibPPS.ESProducers.ctppsLHCInfoRandomXangleESSource_cfi")
# # process.ctppsLHCInfoRandomXangleESSource.generateEveryNEvents = 1
# # process.ctppsLHCInfoRandomXangleESSource.xangleHistogramFile = "CrossingAngles2017.root"
# # process.ctppsLHCInfoRandomXangleESSource.xangleHistogramObject = "hxang"
# # process.ctppsLHCInfoRandomXangleESSource.beamEnergy = 6500.
# # process.ctppsLHCInfoRandomXangleESSource.betaStar = 0.40

# # process.esPreferLHCInfo = cms.ESPrefer("CTPPSLHCInfoRandomXangleESSource", "ctppsLHCInfoRandomXangleESSource")

# # update settings of beam-smearing module                                                                                                                                                                
# process.beamDivergenceVtxGenerator.src = cms.InputTag("")
# process.beamDivergenceVtxGenerator.srcGenParticle = cms.VInputTag(
#     cms.InputTag("genParticles","","SIM")
# )

# # re-shift CMS vtx                                                                                                                                                           
# process.load("CalibPPS.ESProducers.ctppsBeamParametersFromLHCInfoESSource_cfi")
# process.ctppsBeamParametersFromLHCInfoESSource.beamDivX45 = process.ctppsBeamParametersESSource.beamDivX45
# process.ctppsBeamParametersFromLHCInfoESSource.beamDivX56 = process.ctppsBeamParametersESSource.beamDivX56
# process.ctppsBeamParametersFromLHCInfoESSource.beamDivY45 = process.ctppsBeamParametersESSource.beamDivY45
# process.ctppsBeamParametersFromLHCInfoESSource.beamDivY56 = process.ctppsBeamParametersESSource.beamDivY56

# process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetX45 = process.ctppsBeamParametersESSource.vtxOffsetX45
# process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetX56 = process.ctppsBeamParametersESSource.vtxOffsetX56
# process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetY45 = process.ctppsBeamParametersESSource.vtxOffsetY45
# process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetY56 = process.ctppsBeamParametersESSource.vtxOffsetY56
# process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetZ45 = process.ctppsBeamParametersESSource.vtxOffsetZ45
# process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetZ56 = process.ctppsBeamParametersESSource.vtxOffsetZ56

# process.ctppsBeamParametersFromLHCInfoESSource.vtxStddevX = process.ctppsBeamParametersESSource.vtxStddevX
# process.ctppsBeamParametersFromLHCInfoESSource.vtxStddevY = process.ctppsBeamParametersESSource.vtxStddevY
# process.ctppsBeamParametersFromLHCInfoESSource.vtxStddevZ = process.ctppsBeamParametersESSource.vtxStddevZ

# # do not apply vertex smearing again
# process.ctppsBeamParametersESSource.vtxStddevX = 0
# process.ctppsBeamParametersESSource.vtxStddevY = 0
# process.ctppsBeamParametersESSource.vtxStddevZ = 0

# # undo CMS vertex shift
# #process.ctppsBeamParametersESSource.vtxOffsetX45 = +0.2475 * 1E-1
# #process.ctppsBeamParametersESSource.vtxOffsetY45 = -0.6924 * 1E-1
# #process.ctppsBeamParametersESSource.vtxOffsetZ45 = -8.1100 * 1E-1
# # These should be the vtx shift values for 2017...
# process.ctppsBeamParametersESSource.vtxOffsetX45 = +0.024793
# process.ctppsBeamParametersESSource.vtxOffsetY45 = -0.0692861
# process.ctppsBeamParametersESSource.vtxOffsetZ45 = -7.89895

# # processing path                                                                                                                                                                                        
# process.simProtonStep = cms.Sequence(
#     process.beamDivergenceVtxGenerator
#   * process.ctppsDirectProtonSimulation
# )

# ## RECO config

# # point the track producers to the correct products
# process.ctppsPixelLocalTracks.label = "ctppsPixelRecHits"
# # process.ctppsPixelLocalTracks.label = "ctppsPreMixProducer"
# # process.totemRPUVPatternFinder.tagRecHit = cms.InputTag("ctppsPreMixProducer")

# # remove timing tracks from trackLites, they are not produced by protonPreMix
# process.ctppsLocalTrackLiteProducer.includeDiamonds = False

# # remove strips tracks from trackLites, they are disabled protonPreMix 
# # process.ctppsLocalTrackLiteProducer.includeStrips = True # 2016-2017
# process.ctppsLocalTrackLiteProducer.includeStrips = False # 2018

# reconstruction sequence
# process.recoStep = cms.Sequence(
#   process.reco_local
#   * process.ctppsProtons
# )

# end for simulation

# for data
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
# Global tag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v28')
process.load('RecoCTPPS.Configuration.recoCTPPS_cff')
# end for data
print("Association cuts 45")
print(process.ctppsProtons.association_cuts_45)
print("Association cuts 56")
print(process.ctppsProtons.association_cuts_56)

# track distribution plotter
from Validation.CTPPS.simu_config.year_2018_cff import rpIds
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
  tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer","","ReMiniAOD"),

  rpId_45_F = rpIds.rp_45_F,
  rpId_45_N = rpIds.rp_45_N,
  rpId_56_N = rpIds.rp_56_N,
  rpId_56_F = rpIds.rp_56_F,

  outputFile = cms.string("output_tracks"+outFile_suffix+".root")
)

# reconstruction plotter (analysis example)
process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
  tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
  tagRecoProtonsSingleRP = cms.InputTag("ctppsProtons", "singleRP","ReMiniAOD"),
  # tagRecoProtonsMultiRP = cms.InputTag("ppsEfficiencyProducer", "multiRP"),
  tagRecoProtonsMultiRP = cms.InputTag("ctppsProtons", "multiRP","ReMiniAOD"),
  rpId_45_F = rpIds.rp_45_F,
  rpId_45_N = rpIds.rp_45_N,
  rpId_56_N = rpIds.rp_56_N,
  rpId_56_F = rpIds.rp_56_F,

  association_cuts_45 = process.ctppsProtons.association_cuts_45,
  association_cuts_56 = process.ctppsProtons.association_cuts_56,

  outputFile = cms.string("output_protons"+outFile_suffix+".root")
)

# lhcInfo plotter (analysis example)
process.ctppsLHCInfoPlotter = cms.EDAnalyzer("CTPPSLHCInfoPlotter",
  lhcInfoLabel = cms.string(''),
  outputFile = cms.string("output_lhcInfo"+outFile_suffix+".root")
)

# data source
import FWCore.Utilities.FileUtils as FileUtils
fileList = FileUtils.loadListFromFile ("EGamma_2018B.dat") 
inputFiles = cms.untracked.vstring( *fileList)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(inputFiles),

  dropDescendantsOfDroppedBranches = cms.untracked.bool(False),

  inputCommands = cms.untracked.vstring(
    'keep *',
    'keep *_ctppsPixelRecHits_*_*',
    'keep *_totemRPRecHitProducer_*_*',
  )
)

import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = "/eos/project-c/ctpps/Operations/DataExternalConditions/2018/CMSgolden_2RPGood_anyarms_EraB2.json").getVLuminosityBlockRange()
    
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
  input = cms.untracked.int32(1000000)
)

process.path = cms.Path(
  # proton simulation
  # process.simProtonStep

  # proton mixing
  # * process.protonMixingStep

  # standard reconstruction
  # process.recoStep

  # add efficiency
  # * process.ppsEfficiencyProducer 

  # example of analysis
  process.ctppsLHCInfoPlotter
  * process.ctppsTrackDistributionPlotter
  * process.ctppsProtonReconstructionPlotter
)

# process.outpath = cms.EndPath(process.theout)

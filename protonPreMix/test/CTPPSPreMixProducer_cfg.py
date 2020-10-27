import FWCore.ParameterSet.Config as cms

process = cms.Process("INSPECTOR")

import FWCore.ParameterSet.VarParsing as VarParsing
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False),
    FailPath = cms.untracked.vstring('ProductNotFound','Type Mismatch')
    )

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring('cout')
process.MessageLogger.cout = cms.untracked.PSet( threshold = cms.untracked.string('INFO'))
process.MessageLogger.statistics = cms.untracked.vstring()

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
    # Local file with EraB PPS data
    fileNames = cms.untracked.vstring(
    	'file:/eos/project-c/ctpps/subsystems/Pixel/RPixTracking/EfficiencyCalculation2017/ReRecoEras_CMSSW_10_6_10/SingleElectron/ctpps_2017B_pixelefficiency_singleele/200423_112331/0000/ReReco_2017B_1.root',
    	# 'file:/eos/project-c/ctpps/subsystems/Pixel/RPixTracking/EfficiencyCalculation2018/ReRecoEras_CMSSW_10_6_10/EGamma/AOD/ctpps_2018A_pixelefficiency_singleele/200630_073503/0000/ReReco_2018A_1.root',
    	)
)

process.inspector_beforeMixing = cms.EDAnalyzer('CTPPSRecHitInspector',
    pixelRecHitTag = cms.untracked.InputTag("ctppsPixelRecHits", ""),
    stripsRecHitTag = cms.untracked.InputTag("totemRPRecHitProducer", ""),
    initialMsg = cms.untracked.string("Running CTPPSRecHitInspector before mixing")
)


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    preMixProducer = cms.PSet(
        initialSeed = cms.untracked.uint32(42)
    )
)

process.preMixProducer = cms.EDProducer('ctppsPreMixProducer',
	Verbosity = cms.untracked.int32(1),
	Sim_CTPPSPixelRecHitTag = cms.InputTag("ctppsPixelRecHits", ""),
	PU_CTPPSPixelRecHitTag = cms.InputTag("ctppsPixelRecHits", ""),
	Sim_TotemRPRecHitTag = cms.InputTag("totemRPRecHitProducer", ""),
	PU_TotemRPRecHitTag = cms.InputTag("totemRPRecHitProducer", ""),
	PUFilesList = cms.vstring(
		"file:/eos/project-c/ctpps/subsystems/Pixel/RPixTracking/EfficiencyCalculation2017/ReRecoEras_CMSSW_10_6_10/SingleElectron/ctpps_2017B_pixelefficiency_singleele/200423_112331/0000/ReReco_2017B_1.root",
		# "/store/data/Run2018A/EGamma/AOD/12Nov2019_UL2018-v2/710003/972DEEB4-9368-7F4B-A06C-E4D808DD9DEC.root",
		)
)


process.inspector_afterMixing = cms.EDAnalyzer('CTPPSRecHitInspector',
    pixelRecHitTag = cms.untracked.InputTag("preMixProducer", "ctppsPixelRecHits"),
    stripsRecHitTag = cms.untracked.InputTag("preMixProducer", "totemRPRecHits"),
    initialMsg = cms.untracked.string("Running CTPPSRecHitInspector after mixing")
)

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("test.root"),
    outputCommands = cms.untracked.vstring("keep *",
    )
)

process.p = cms.Path(
	process.inspector_beforeMixing *
	process.preMixProducer *
	process.inspector_afterMixing
)

process.output_step = cms.EndPath(  
    process.output
)
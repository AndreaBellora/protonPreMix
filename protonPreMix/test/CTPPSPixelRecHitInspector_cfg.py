import FWCore.ParameterSet.Config as cms

process = cms.Process("INSPECTOR")

import FWCore.ParameterSet.VarParsing as VarParsing
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    FailPath = cms.untracked.vstring('ProductNotFound','Type Mismatch')
    )

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring('cout')
process.MessageLogger.cout = cms.untracked.PSet( threshold = cms.untracked.string('INFO'))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
    # Local file with EraB PPS data
    fileNames = cms.untracked.vstring('file:/eos/project-c/ctpps/subsystems/Pixel/RPixTracking/EfficiencyCalculation2017/ReRecoEras_CMSSW_10_6_10/SingleElectron/ctpps_2017B_pixelefficiency_singleele/200423_112331/0000/ReReco_2017B_1.root',)
)

process.inspector = cms.EDAnalyzer('CTPPSPixelRecHitInspector',
    recHitTag = cms.untracked.InputTag("ctppsPixelRecHits", ""),
    initialMsg = cms.untracked.string("Running CTPPSPixelRecHitInspector!")
)

process.p = cms.Path(process.inspector)
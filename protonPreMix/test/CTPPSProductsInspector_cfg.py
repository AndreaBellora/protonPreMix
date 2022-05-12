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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # Local file with EraB PPS data
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/a/abellora/Work/PPtoPPWWjets_analysis/newInstall/CMSSW_10_6_17/src/PPtoPPWWjets/PPtoPPWWjets/python/testrun/output_data.root',),
    inputCommands = cms.untracked.vstring("keep *")
)

process.load("protonPreMix.protonPreMix.ctppsProductsInspector_cfi")
process.ctppsProductsInspector.pixelRecHitTag = cms.untracked.InputTag("ctppsDirectProtonSimulation","","CTPPSTestAcceptance")
process.ctppsProductsInspector.trackLiteTag = cms.untracked.InputTag("ctppsLocalTrackLiteProducer","","CTPPSTestAcceptance")
process.ctppsProductsInspector.multiRPProtonTag = cms.untracked.InputTag("ctppsProtonMixer", "","CTPPSTestAcceptance")


process.p = cms.Path(process.ctppsProductsInspector)
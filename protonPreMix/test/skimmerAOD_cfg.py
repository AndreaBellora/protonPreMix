import FWCore.ParameterSet.Config as cms
import re
import copy

YEAR=2017

from Configuration.StandardSequences.Eras import eras
from Configuration.ProcessModifiers.run2_miniAOD_UL_cff import run2_miniAOD_UL

if YEAR == 2016:
    process = cms.Process('ReMiniAOD', eras.Run2_2016, run2_miniAOD_UL)
if YEAR == 2017:
    process = cms.Process('ReMiniAOD', eras.Run2_2017, run2_miniAOD_UL)
if YEAR == 2018:
    process = cms.Process('ReMiniAOD', eras.Run2_2018, run2_miniAOD_UL)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# process.source = cms.Source("PoolSource",
#     fileNames = cms.untracked.vstring(
#         # SingleMuon 2017 - 315512
#         # '/store/data/Run2018B/EGamma/AOD/12Nov2019_UL2018-v1/260000/CA49C790-FEA2-6147-90F5-91E4A208A5CD.root'
#         # '/store/data/Run2017D/SingleElectron/AOD/09Aug2019_UL2017-v1/270000/A4D47566-AA32-B044-AF11-5181B23BB5A3.root'
#         # '/store/data/Run2017B/SingleMuon/AOD/17Nov2017-v1/510000/34B37296-FEDB-E711-A272-0CC47A7E68AA.root'

#         # EGamma 2018 - 315512 89-90
#         # "/store/data/Run2018A/EGamma/AOD/12Nov2019_UL2018-v2/100013/160DBEF0-C57A-AC4B-AE08-04A7462DB3CD.root",
#         # "/store/data/Run2018A/EGamma/AOD/12Nov2019_UL2018-v2/100013/06375BF3-6647-CB47-A294-87F1EAEB7D85.root"

#         # SingleMuon 2018 - 315512 89-90
#         "/store/data/Run2018A/SingleMuon/AOD/17Sep2018-v2/100002/CD55FD53-DCA3-8F4C-ABBE-CF24B954457D.root",
#         "/store/data/Run2018A/SingleMuon/AOD/17Sep2018-v2/100001/EF912C50-8F70-8241-842F-1B2298F5E646.root"
#     )
#                             #,
#                             #duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
# )


import FWCore.Utilities.FileUtils as FileUtils
fileList = FileUtils.loadListFromFile("files2017H.dat")
inputFiles = cms.untracked.vstring( *fileList)
process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = inputFiles, 
    # skipEvents = cms.untracked.uint32(600000)
)

import FWCore.PythonUtilities.LumiList as LumiList
# process.source.lumisToProcess = LumiList.LumiList(filename = "/afs/cern.ch/work/a/abellora/Work/PPtoPPWWjets_analysis/newInstall/CMSSW_10_6_17/src/protonPreMix/protonPreMix/test/early2018A.json").getVLuminosityBlockRange()

# 2017H
process.source.lumisToProcess = LumiList.LumiList(filename = "/afs/cern.ch/work/a/abellora/Work/PPtoPPWWjets_analysis/newInstall/CMSSW_10_6_17/src/protonPreMix/protonPreMix/test/runs2017H.json").getVLuminosityBlockRange()

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
from CondCore.CondDB.CondDB_cfi import *

# Trigger                                                                                                                                                                                    
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltFilter = copy.deepcopy(hltHighLevel)
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
# process.hltFilter.HLTPaths = ['HLT_Ele27_WPTight_Gsf_v*']
# 2017
# process.hltFilter.HLTPaths = ['HLT_IsoMu27_v*']
# 2018
# process.hltFilter.HLTPaths = ['HLT_IsoMu24_v*']

# 2017H
process.hltFilter.HLTPaths = [
    "HLT_HIMu15_v*",
    # "HLT_HIEle15_WPLoose_Gsf_v*",
    # "HLT_HIPFJet140_v*",
    # "HLT_HIPFJetFwd140_v*"
]

# Global tag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v28')

# Begin overrides for 2020 re-MiniAOD conditions                                                                                                          
# Alignment                                                                                                                                              
process.CondDBAlignment = CondDB.clone( connect = "frontier://FrontierProd/CMS_CONDITIONS" )
process.PoolDBESSourceAlignment = cms.ESSource("PoolDBESSource",
                                            process.CondDBAlignment,
                                            toGet = cms.VPSet(cms.PSet(
                                                record = cms.string('RPRealAlignmentRecord'),
                                                tag = cms.string("CTPPSRPAlignment_real_offline_v8")
                                            ))
)

process.esPreferDBFileAlignment = cms.ESPrefer("PoolDBESSource", "PoolDBESSourceAlignment")

# Optics                                                                                                                                                 
process.CondDBOptics = CondDB.clone( connect = "frontier://FrontierProd/CMS_CONDITIONS" )
process.PoolDBESSourceOptics = cms.ESSource("PoolDBESSource",
                                            process.CondDBOptics,
                                            DumpStat = cms.untracked.bool(False),
                                        toGet = cms.VPSet(cms.PSet(
                                            record = cms.string('CTPPSOpticsRcd'),
                                        tag = cms.string("PPSOpticalFunctions_offline_v7")
                                        )),
)

process.esPreferDBFileOptics = cms.ESPrefer("PoolDBESSource", "PoolDBESSourceOptics")
# End of overrides for testing 2020 re-MiniAOD conditions                                                                                                 


#################################
# If using full proton re-reco - local RP reconstruction chain with standard settings                                                             
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")
process.ctppsLocalTrackLiteProducer.includePixels = cms.bool(True)
process.ctppsLocalTrackLiteProducer.includeStrips = cms.bool(True)
process.ctppsLocalTrackLiteProducer.includeDiamonds = cms.bool(True)
process.ctppsProtons.doSingleRPReconstruction = cms.bool(True)
process.ctppsProtons.doMultiRPReconstruction = cms.bool(True)
if YEAR == 2016:
    process.ctppsLocalTrackLiteProducer.includePixels = cms.bool(False)
if YEAR == 2018:
    process.ctppsLocalTrackLiteProducer.includeStrips = cms.bool(False)

process.hltsel = cms.Path(process.hltFilter)

process.theout = cms.OutputModule( "PoolOutputModule",
                                   # SelectEvents = cms.untracked.PSet(
                                   #     SelectEvents = cms.vstring(
                                   #         'hltsel'
                                   #     )
                                   # ),
     outputCommands = cms.untracked.vstring("drop *",
#         "keep *_ctppsProtons_*_ReMiniAOD",
#         "keep *_ctppsLocalTrackLiteProducer_*_ReMiniAOD",
#         "keep *_ctppsPixelLocalTracks_*_ReMiniAOD"
        # "keep *_totemRPRecHitProducer_*_*",
        # "keep *_ctppsPixelRecHits_*_*",
        "keep *_ctppsPixelLocalTracks_*_*",
        "keep *_ctppsLocalTrackLiteProducer_*_*",
        "keep *_ctppsProtons_*_*",
        # "keep *_TriggerResults_*_*"                                            
                                        ),
    # fileName = cms.untracked.string('ReMiniAOD_SkimForEfficiency_SingleMuon_2017B.root')
    # fileName = cms.untracked.string('ReMiniAOD_SkimForEfficiency_SingleMuon_2018A.root')
    # fileName = cms.untracked.string('ReMiniAOD_SkimForEfficiency_EGamma_2018A.root')

    fileName = cms.untracked.string('ReMiniAOD_SkimForEfficiency_SingleMuon_2017H.root')

)

if YEAR == 2016:
    process.p = cms.Path(
        # process.hltFilter *
                         # Legacy re-reco from AOD sequence
                         process.totemRPUVPatternFinder *
                         process.totemRPLocalTrackFitter *
#                         process.ctppsDiamondRecHits *
#                         process.ctppsDiamondLocalTracks *
                         process.ctppsLocalTrackLiteProducer *
                         process.ctppsProtons)

if YEAR == 2017:
    process.p = cms.Path(
        # process.hltFilter *           
                         # Legacy re-reco from AOD sequence
                         process.totemRPUVPatternFinder *
                         process.totemRPLocalTrackFitter *
#                         process.ctppsDiamondRecHits *
#                         process.ctppsDiamondLocalTracks *
                         process.ctppsPixelLocalTracks *
                         process.ctppsLocalTrackLiteProducer *
                         process.ctppsProtons)

if YEAR == 2018:
    process.p = cms.Path(
        # process.hltFilter *
                         # Legacy re-reco from AOD sequence                                                                                                             
#                         process.ctppsDiamondRecHits *
#                         process.ctppsDiamondLocalTracks *
                         process.ctppsPixelLocalTracks *
                         process.ctppsLocalTrackLiteProducer *
                         process.ctppsProtons)

process.outpath = cms.EndPath(process.theout)


print process.dumpPython()


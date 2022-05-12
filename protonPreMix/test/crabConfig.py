from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'ctpps_2017H_pixelefficiency'
config.General.workArea = 'skim_2017H'
config.General.transferOutputs = True
config.General.transferLogs = True


config.JobType.pluginName = 'analysis'
config.JobType.psetName = 'skimmerAOD_cfg.py'

config.Data.inputDataset = '/SingleMuon/Run2017H-09Aug2019_UL2017_LowPU-v1/AOD'
# '/SingleElectron/Run2017F-09Aug2019_UL2017_rsb-v2/AOD'
# '/SingleElectron/Run2017C-17Nov2017-v1/AOD'
# /SingleElectron/Run2017E-09Aug2019_UL2017-v1/AOD
config.Data.allowNonValidInputDataset = True # CAREFUL WITH THIS

config.Data.inputDBS = 'global'

config.Data.splitting = 'FileBased'      
# config.Data.splitting = 'Automatic'                                                                                         
config.Data.unitsPerJob = 20                                                                                              
config.Data.lumiMask = '/afs/cern.ch/work/a/abellora/Work/PPtoPPWWjets_analysis/newInstall/CMSSW_10_6_17/src/protonPreMix/protonPreMix/test/runs2017H.json'
# config.Data.totalUnits = 10

config.Data.outLFNDirBase = '/store/group/cmst3/group/top/exclusive_top/Efficiency_2017H'
# config.Data.outLFNDirBase = '/eos/home-a/abellora/2017H_data'
config.Data.publication = False
config.Data.outputDatasetTag = 'ctpps_2017H_pixelefficiency'

config.Site.storageSite = 'T2_CH_CERN'
# config.Site.storageSite = 'T3_CH_CERNBOX'
config.Site.blacklist = ['T3_UK_London_QMUL','T3_UK_London_RHUL','T2_PL_Swierk','T2_FR_GRIF_IRFU']

config.JobType.allowUndistributedCMSSW = True

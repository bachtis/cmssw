import FWCore.ParameterSet.Config as cms
process = cms.Process("L1KMTF")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')   



process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )



process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/scratch3/MTF/data/data.root'
#        '/store/data/Run2017F/MinimumBias/RAW/v1/000/305/038/00000/4623B44F-44B0-E711-9EC9-02163E011DDB.root'

    )
)

process.load("L1Trigger.L1KalmanMuonTrackFinder.l1KalmanMuons_cff")


##Stage 2 BMTF emulator####
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '90X_upgrade2023_realistic_v9', '')






process.load('EventFilter.L1TRawToDigi.l1tRawtoDigiBMTF_cfi')

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('test.root'),
    outputCommands = cms.untracked.vstring("drop *_*_*_*",
                                 "keep *_*_*_L1KMTF",
                                 "keep *_muons_*_*")
                               
)

process.l1KalmanInputStubs.src = cms.InputTag("BMTFStage2Digis") 

process.p = cms.Path(process.BMTFStage2Digis*process.l1KalmanMuons)
process.e = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.p,process.e)



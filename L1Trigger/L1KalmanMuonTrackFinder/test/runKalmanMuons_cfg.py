import FWCore.ParameterSet.Config as cms

process = cms.Process("L1MUONKF")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/scratch3/MTF/data/singleNu140.root'
    )
)


process.load("L1Trigger.L1KalmanMuonTrackFinder.l1KalmanMuons_cff")


process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('singleNeutrino140.root')
)

  
process.p = cms.Path(process.l1KalmanMuons)
process.e = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.p,process.e)

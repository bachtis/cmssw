import FWCore.ParameterSet.Config as cms

process = cms.Process("L1MUONKF")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/scratch3/MTF/data/singleMu140.root'
    )
)


process.load("L1Trigger.L1KalmanMuonTrackFinder.l1KalmanMuons_cff")


process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('singleMuon140.root')
)

  
process.p = cms.Path(process.l1KalmanMuons)
process.e = cms.EndPath(process.out)

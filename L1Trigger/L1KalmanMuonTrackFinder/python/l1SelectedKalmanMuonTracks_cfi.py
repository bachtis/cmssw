import FWCore.ParameterSet.Config as cms
from L1Trigger.L1KalmanMuonTrackFinder.l1MuonKalmanFilter_cfi import l1MuonKalmanFilterSettings

l1SelectedKalmanMuonTracks = cms.EDProducer('L1KalmanMuonTrackSelector',
                                          verbose=cms.bool(False),
                                          src = cms.InputTag("l1KalmanMuonTracks"),
                                          maxCurvature = cms.int32(2621), #pt>2.5 GeV
                                          maxLocalChi2 = cms.int32(600), #Chi2 cut
                                          chiSquareOffset = cms.int32(5) #remogve hit if another track has 5 times better chi2
)

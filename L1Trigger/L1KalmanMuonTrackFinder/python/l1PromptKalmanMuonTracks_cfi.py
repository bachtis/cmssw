import FWCore.ParameterSet.Config as cms
from L1Trigger.L1KalmanMuonTrackFinder.l1MuonKalmanFilter_cfi import l1MuonKalmanFilterSettings

l1PromptKalmanMuonTracks = cms.EDProducer('L1KalmanMuonTrackSelector',
                                          verbose=cms.bool(True),
                                          src = cms.InputTag("l1KalmanMuonTracks"),
                                          maxCurvature = cms.int32(655), #pt>2.5 GeV
                                          maxLocalChi2 = cms.int32(4480), #Chi2<35.0
                                          chiSquareOffset = cms.int32(256) #remogve hit if DeltaChi2>2
)

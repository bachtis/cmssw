import FWCore.ParameterSet.Config as cms
from L1Trigger.L1KalmanMuonTrackFinder.l1MuonKalmanFilter_cfi import l1MuonKalmanFilterSettings

l1KalmanMuonTracks = cms.EDProducer('L1KalmanMuonTrackProducer',
                                    src = cms.InputTag("l1KalmanInputStubs"),
                                    algo = l1MuonKalmanFilterSettings
)





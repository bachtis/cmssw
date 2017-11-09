import FWCore.ParameterSet.Config as cms
from L1Trigger.L1KalmanMuonTrackFinder.l1MuonKalmanFilter_cfi import l1MuonKalmanFilterSettings

l1SelectedKalmanMuonTracks = cms.EDProducer('L1KalmanMuonTrackSelector',
                   verbose=cms.bool(True),
                   src = cms.InputTag("l1KalmanMuonTracks"),
                   maxChi2 = cms.uint32(1300), #Chi2/DOF cut at 99.7%
                   rankFactorSeg = cms.uint32(90), #Chi2 allowance per segment
                   rankFactorQual = cms.uint32(30) #Chi2 allowance per quality
)

import FWCore.ParameterSet.Config as cms
from L1Trigger.L1KalmanMuonTrackFinder.l1KalmanInputStubs_cfi import  l1KalmanInputStubs
from L1Trigger.L1KalmanMuonTrackFinder.l1KalmanMuonTracks_cfi import  l1KalmanMuonTracks

l1KalmanMuons = cms.Sequence(l1KalmanInputStubs+l1KalmanMuonTracks)


import FWCore.ParameterSet.Config as cms
from L1Trigger.L1KalmanMuonTrackFinder.l1KalmanInputStubs import  l1KalmanInputStubs
from L1Trigger.L1KalmanMuonTrackFinder.l1KalmanMuonTracks import  l1KalmanMuonTracks

l1KalmanMuons = cms.Path(l1KalmanInputStubs*l1KalmanMuonTracks)


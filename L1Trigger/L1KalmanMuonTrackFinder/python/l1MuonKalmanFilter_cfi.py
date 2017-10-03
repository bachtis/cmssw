import FWCore.ParameterSet.Config as cms

l1MuonKalmanFilterSettings =  cms.PSet(
    verbose = cms.bool(True),
    eLoss = cms.vint32(),
    aPhi  = cms.vint32(),
    bPhi  = cms.vint32(),
    aPhiB =  cms.vint32(),
    bPhiB =  cms.vint32(),
    denominator =  cms.int32(4096),
    useOfflineAlgo = cms.bool(True),
    mScatteringPhi = cms.vint32(),
    mScatteringPhiB = cms.vint32(),
    pointResolutionPhi = cms.vint32(),
    pointResolutionPhiB = cms.vint32(),
    pointResolutionVertex = cms.int32()
)

import FWCore.ParameterSet.Config as cms

l1MuonKalmanFilterSettings =  cms.PSet(
    verbose = cms.bool(True),
    eLoss = cms.vint32(-21,-8,-9,-8), #divide this by 16384
    aPhi  = cms.vint32(-29556,-286,-332,-245),   #divide this by 4096
    bPhi  = cms.vint32(4096,-631,-709,-602),         # divide this by 512 
    aPhiB =  cms.vint32(1847,286,331,264),#divide this by 4096
    bPhiB =  cms.vint32(-512,591,601,587), #divide this by 512
    denominator =  cms.int32(4096),
    useOfflineAlgo = cms.bool(True),
    ###Only for the offline algo -not in firmware -floating point 
    mScatteringPhi = cms.vdouble(1.5e-3,1.1e-3,9.0e-4,4.5e-4),
    mScatteringPhiB = cms.vdouble(1.9e-3,9.1e-4,1.5e-3,1.5e-3),
    pointResolutionPhi = cms.double(6.0),
    pointResolutionPhiB = cms.double(15.0),
    pointResolutionVertex = cms.double(0.1)
)



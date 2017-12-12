import FWCore.ParameterSet.Config as cms

l1MuonKalmanFilterSettings =  cms.PSet(
    verbose = cms.bool(False),
    makePattern = cms.uint32(0), #Tool to make patterns for hardware tests 
    eLoss = cms.vdouble(-2.85e-4,0,0,0), 
    aPhi = cms.vdouble(1.942,0.032,0.028,0.016),
    aPhiB = cms.vdouble(-0.833,-0.123,-0.174,-0.144),
    aPhiBNLO = cms.vdouble(1.758e-4,0,0,0),
    bPhi = cms.vdouble(-1,0.154,0.173,0.147),
    bPhiB = cms.vdouble(-1,1.154,1.173,1.147),
    chiSquare = cms.vdouble(0.0,0.095,0.232,0.356),   
    chiSquareCut = cms.int32(800),
    useOfflineAlgo = cms.bool(True),

    ###Only for the offline algo -not in firmware --------------------
    mScatteringPhi = cms.vdouble(0.9322,8.384e-5,5.406e-5,1.786e-5),
    mScatteringPhiB = cms.vdouble(4.821e-3,3.580e-3,6.634e-3,5.478e-3),
    pointResolutionPhi = cms.double(1.),
    pointResolutionPhiB = cms.double(500.),
    pointResolutionVertex = cms.double(100.),
    ###---------------------------------------------------------------
    #coarse eta look up table

    etaLUTAddr=cms.vint32(1152,129,258,1040,513,520,521,
                      1168,130,2336,272,17,146,2322,
                      1032,2194,288,2178,34,585,274,
                      2066,2320,584,1033,2306,577,9,
                      2080,576,65,1042,2082,1025,1170,
                      72,73,2176,2050,290,2304,2192,2064,
                      144,1026,1154,1056,1153,18,2338),

    etaLUTValue=cms.vint32(32,27,73,37,8,9,8,37,48,78,68,25,
                       48,66,22,57,86,56,80,8,66,61,65,8,
                       22,71,8,16,78,9,11,43,76,24,44,12,
                       12,56,66,78,72,57,61,41,42,43,26,26
                       ,53,77)




)

import FWCore.ParameterSet.Config as cms

l1MuonKalmanFilterSettings =  cms.PSet(
    verbose = cms.bool(False),
    eLoss = cms.vdouble(-1.3e-3,-2.92e-4,-5.53e-4,-4.99e-4), #Implemented in LUT
    aPhi  = cms.vint32(-29556,-286,-332,-245),   #divide this by 4096
    bPhi  = cms.vint32(4096,-631,-709,-602),         # divide this by 512 
    aPhiB =  cms.vint32(1847,286,331,264),#divide this by 4096
    bPhiB =  cms.vint32(-8192,631,709,602), #divide this by 4096
    wordSize =  cms.vint32(11,8,11), #one for phi one for phiB and one for K
    denominator =  cms.int32(4096), #common denominator for precision
    chiSquareVertexA = cms.vint32(585,532,479,428),   #divide this by 64
    chiSquareVertexB = cms.vint32(715,2000,2300,2700),
    chiSquareVertexC = cms.vint32(133,187,184,177), #dvide this by 128
    chiSquareVertexDenominator =  cms.int32(128), #common denominator for precision on Chi2 calculations
    useOfflineAlgo = cms.bool(True),
    ###Only for the offline algo -not in firmware -floating point 
    mScatteringPhi = cms.vdouble(1.5e-3,0.5e-4,4.4e-4,2.2e-4),
    mScatteringPhiB = cms.vdouble(2.0e-3,0.5e-4,0.75e-3,0.75e-3),
    pointResolutionPhi = cms.double(6),
    pointResolutionPhiB = cms.double(15),
    pointResolutionVertex = cms.double(10),

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

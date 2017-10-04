import FWCore.ParameterSet.Config as cms

l1MuonKalmanFilterSettings =  cms.PSet(
    verbose = cms.bool(True),
    eLoss = cms.vint32(-21,-8,-9,-8), #divide this by 16384
    aPhi  = cms.vint32(-29556,-286,-332,-245),   #divide this by 4096
    bPhi  = cms.vint32(4096,-631,-709,-602),         # divide this by 512 
    aPhiB =  cms.vint32(1847,286,331,264),#divide this by 4096
    bPhiB =  cms.vint32(-8192,631,709,602), #divide this by 4096
    wordSize =  cms.vint32(11,8,11), #one for phi one for phiB and one for K
    denominator =  cms.int32(4096), #common denominator for precision
    useOfflineAlgo = cms.bool(True),
    ###Only for the offline algo -not in firmware -floating point 
    mScatteringPhi = cms.vdouble(1.5e-3,0.5e-4,4.4e-4,2.2e-4),
    mScatteringPhiB = cms.vdouble(2.0e-3,0.5e-4,0.75e-3,0.75e-3),
    pointResolutionPhi = cms.double(6),
    pointResolutionPhiB = cms.double(15),
    pointResolutionVertex = cms.double(10),

    #coarse eta look up table
    etaLUTAddr=cms.vint32(2176,1168,584,36,144,272,130,2194,65,1026,2322,
                          2320,1056,2324,2304,2306,288,520,521,1154,1032,
                          1033,2178,146,1096,585,2192,1170,577,258,1156,
                          148,1152,9,260,73,72,129,1153,1184,1025,1042,
                          1041,1040,17,2336,2082,2064,2080,1028,34,2338,
                          576,290,18,274,513,292,2050,2066),
    etaLUTValue=cms.vint32(143,91,21,241,108,167,
                           121,144,27,107,168,
                           168,66,165,184,182,214,
                           21,21,107,55,55,144,
                           124,54,21,143,109,21,
                           183,72,140,82,41,225,
                           33,30,65,66,83,60,107,81,
                           95,64,199,195,156,199,68,
                           198,196,21,202,135,168,21,
                           224,169,155)

)


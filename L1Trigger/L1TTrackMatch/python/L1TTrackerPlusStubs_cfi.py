import FWCore.ParameterSet.Config as cms
import math
from math import pi


l1TPSStubs = cms.EDProducer("L1TMuCorrelatorHitProducer",
    verbose = cms.int32(0),
    srcCSC = cms.InputTag("simCscTriggerPrimitiveDigis"),
    srcDT = cms.InputTag("simDtTriggerPrimitiveDigis"),
    srcRPC = cms.InputTag("simMuonRPCDigis"),
    CSC =cms.PSet(                            
        verbose = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),         
        phiLSB             = cms.double(0.000340885), 
        etaLSB             = cms.double(0.00585938), 
        ),
   DT = cms.PSet(                         
        verbose            = cms.int32(0),
        minPhiQuality      = cms.int32(0),
        minThetaQuality    = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),                           
        disableMasks       = cms.bool(True),                               
        phiLSB             = cms.double(0.000340885),
        cotTheta_1         = cms.vint32(105,101,97,93,88,84,79,69,64,58,52,46,40,34,21,14,7,0,-7,-14,-21,-34,-40,-46,-52,-58,-64,-69,-79,-84,-88,-93,-97,-101,-105),
        cotTheta_2         = cms.vint32(93,89,85,81,77,73,68,60,55,50,45,40,34,29,17,12,6,0,-6,-12,-17,-29,-34,-40,-45,-50,-55,-60,-68,-73,-77,-81,-85,-89,-93),
        cotTheta_3         = cms.vint32(81,77,74,70,66,62,58,51,46,42,38,33,29,24,15,10,5,0,-5,-10,-15,-24,-29,-33,-38,-42,-46,-51,-58,-62,-66,-70,-74,-77,-81)
   ),
   RPCBarrel = cms.PSet(                         
        verbose            = cms.int32(0),
        minPhiQuality      = cms.int32(0),
        minThetaQuality    = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),                           
        disableMasks       = cms.bool(False),                               
        phiLSB             = cms.double(0.000340885),
        etaLSB             = cms.double(0.00585938), 
        cotTheta_1         = cms.vint32(105,101,97,93,88,84,79,69,64,58,52,46,40,34,21,14,7,0,-7,-14,-21,-34,-40,-46,-52,-58,-64,-69,-79,-84,-88,-93,-97,-101,-105),
        cotTheta_2         = cms.vint32(93,89,85,81,77,73,68,60,55,50,45,40,34,29,17,12,6,0,-6,-12,-17,-29,-34,-40,-45,-50,-55,-60,-68,-73,-77,-81,-85,-89,-93),
        cotTheta_3         = cms.vint32(81,77,74,70,66,62,58,51,46,42,38,33,29,24,15,10,5,0,-5,-10,-15,-24,-29,-33,-38,-42,-46,-51,-58,-62,-66,-70,-74,-77,-81),
   ),
   RPCEndcap = cms.PSet(                         
        verbose            = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),                           
        phiLSB             = cms.double(0.000340885),
        etaLSB             = cms.double(0.00585938), 

   )


)

tpsAlgoSettings = cms.PSet(
    verbose = cms.int32(0),
    phiLSB = cms.double(0.000340885),
    etaLSB = cms.double(0.00585938),
    etaShift = cms.uint32(5),
    curvLSB  = cms.double(0.00585938),
    matchType = cms.vuint32(
        1,0,0,1,1,0,0,1,1,0,1,0,  #0
        1,0,0,1,1,0,0,1,1,0,1,0,  #1
        1,0,0,1,1,0,0,1,1,0,1,0,  #2
        1,0,0,1,1,0,0,1,1,0,1,0,  #3
        1,0,0,1,1,0,0,1,1,0,1,0,  #4
        1,0,0,1,1,0,0,1,1,0,1,0,  #5
        1,0,0,1,1,0,0,1,1,0,2,9,  #6
        1,0,0,1,1,0,0,1,2,3,3,9,  #7
        1,0,0,1,2,3,3,2,2,9,9,9,  #8
        1,0,0,1,2,3,2,2,3,2,9,9,  #9
        2,3,3,2,2,3,2,3,9,9,9,9,  #10
        2,3,3,2,2,3,2,3,9,9,9,9,  #11
        2,2,3,2,3,2,3,9,9,9,9,9,  #12
        2,2,3,2,3,9,9,9,9,9,9,9,  #13
        2,2,2,3,9,9,9,9,9,9,9,9,  #14
        2,2,2,2,9,9,9,9,9,9,9,9   #15
        ),
    matchEtaRegion = cms.vuint32(
        0,0,0,0,0,0,0,0,0,0,0,0,  #0
        0,0,0,0,1,1,1,1,1,1,1,1,  #1
        1,1,1,1,1,1,1,1,1,1,1,1,  #2
        1,1,1,1,1,1,1,1,1,1,2,2,  #3
        1,1,1,1,1,1,1,1,2,2,2,2,  #4
        1,1,1,1,2,2,2,2,2,2,2,2,  #5
        2,2,2,2,2,2,2,2,2,2,3,9,  #6
        2,2,2,2,2,2,2,2,3,3,3,9,  #7
        2,2,2,2,3,3,3,4,4,9,9,9,  #8
        2,2,2,2,4,3,4,4,3,4,9,9,  #9
        2,2,2,2,4,3,4,4,3,4,9,9,  #10
        4,4,4,4,4,4,4,4,9,9,9,9,  #11
        5,4,4,4,4,4,4,9,9,9,9,9,  #12
        5,5,4,4,4,9,9,9,9,9,9,9,  #13
        5,5,5,4,9,9,9,9,9,9,9,9,  #14
        5,5,5,5,9,9,9,9,9,9,9,9   #15
        ),
    matchDepthRegion = cms.vuint32(
        1,1,1,1,2,2,2,2,3,3,4,4,  #0
        1,1,1,1,2,2,2,2,3,3,4,4,  #1
        1,1,1,1,2,2,2,2,3,3,4,4,  #2
        1,1,1,1,2,2,2,2,3,3,4,4,  #3
        1,1,1,1,2,2,2,2,3,3,4,4,  #4
        1,1,1,1,2,2,2,2,3,3,4,4,  #5
        1,1,1,1,2,2,2,2,3,3,1,9,  #6
        1,1,1,1,2,2,2,2,1,1,2,9,  #7
        1,1,1,1,1,1,2,2,3,9,9,9,  #8
        1,1,1,1,1,2,2,3,3,4,9,9,  #9
        1,1,2,2,3,3,4,4,9,9,9,9,  #10
        1,1,2,2,3,3,4,4,9,9,9,9,  #11
        1,1,1,3,3,4,4,9,9,9,9,9,  #12
        1,2,3,4,4,9,9,9,9,9,9,9,  #13
        1,2,3,4,9,9,9,9,9,9,9,9,  #14
        1,2,3,4,9,9,9,9,9,9,9,9  #15
    )
)
        

        



l1StubMatchedMuons = cms.EDProducer("L1TTPSProducer",
    srcStubs = cms.InputTag("l1TPSStubs"),
    srcTracks = cms.InputTag("TTTracksFromTracklet:Level1TTTracks"),
    maxChi2 = cms.double(100),
    verbose = cms.int32(0),                                    
    sectors = cms.VPSet(
        cms.PSet(
            sectorNumber = cms.uint32(0),
            barrelSectors= cms.vuint32(4,5,6,7),
            csc10DegreeChambers=cms.vuint32(33,34,35,36,1,2,3,4),
            csc20DegreeChambers=cms.vuint32(18,1,2,3),
            rpcEndcapChambers=cms.vuint32(33,34,35,36,1,2,3,4),
            phiLowerBound = cms.int32(-1024),
            phiUpperBound = cms.int32(1024),
            phiOffset=cms.int32(-256),
            algoSettings = tpsAlgoSettings
        ),
        cms.PSet(
            sectorNumber = cms.uint32(1),
            barrelSectors= cms.vuint32(6,7,8,9),
            csc10DegreeChambers=cms.vuint32(1,2,3,4,5,6,7,8),
            csc20DegreeChambers=cms.vuint32(2,3,4,5),
            rpcEndcapChambers=cms.vuint32(1,2,3,4,5,6,7,8),
            phiLowerBound = cms.int32(1024),
            phiUpperBound = cms.int32(3072),
            phiOffset=cms.int32(1792),
            algoSettings = tpsAlgoSettings
        ),
        cms.PSet(
            sectorNumber = cms.uint32(2),
            barrelSectors= cms.vuint32(7,8,9,10),
            csc10DegreeChambers=cms.vuint32(5,6,7,8,9,10,11,12),
            csc20DegreeChambers=cms.vuint32(4,5,6,7),
            rpcEndcapChambers=cms.vuint32(5,6,7,8,9,10,11,12),
            phiLowerBound = cms.int32(3072),
            phiUpperBound = cms.int32(5120),
            phiOffset=cms.int32(3840),
            algoSettings = tpsAlgoSettings
        ),
        cms.PSet(
            sectorNumber = cms.uint32(3),
            barrelSectors= cms.vuint32(8,9,10,11),
            csc10DegreeChambers=cms.vuint32(9,10,11,12,13,14,15,16),
            csc20DegreeChambers=cms.vuint32(6,7,8,9),
            rpcEndcapChambers=cms.vuint32(9,10,11,12,13,14,15,16),
            phiLowerBound = cms.int32(5120),
            phiUpperBound = cms.int32(7168),
            phiOffset=cms.int32(5888),
            algoSettings = tpsAlgoSettings
        ),
        cms.PSet(
            sectorNumber = cms.uint32(4),
            barrelSectors= cms.vuint32(10,11,0,1),
            csc10DegreeChambers=cms.vuint32(13,14,15,16,17,18,19,20),
            csc20DegreeChambers=cms.vuint32(8,9,10,11),
            rpcEndcapChambers=cms.vuint32(13,14,15,16,17,18,19,20),
            phiLowerBound = cms.int32(7168),
            phiUpperBound = cms.int32(9216),
            phiOffset=cms.int32(7936),
            algoSettings = tpsAlgoSettings
        ),
        cms.PSet(
            sectorNumber = cms.uint32(5),
            barrelSectors= cms.vuint32(11,0,1,2),
            csc10DegreeChambers=cms.vuint32(17,18,19,20,21,22,23,24),
            csc20DegreeChambers=cms.vuint32(10,11,12,13),
            rpcEndcapChambers=cms.vuint32(17,18,19,20,21,22,23,24),
            phiLowerBound = cms.int32(9216),
            phiUpperBound = cms.int32(-7168),
            phiOffset=cms.int32(-8448),
            algoSettings = tpsAlgoSettings
        ),
        cms.PSet(
            sectorNumber = cms.uint32(6),
            barrelSectors= cms.vuint32(0,1,2,3),
            csc10DegreeChambers=cms.vuint32(21,22,23,24,25,26,27,28),
            csc20DegreeChambers=cms.vuint32(12,13,14,15),
            rpcEndcapChambers=cms.vuint32(21,22,23,24,25,26,27,28),
            phiLowerBound = cms.int32(-7168),
            phiUpperBound = cms.int32(-5120),
            phiOffset=cms.int32(-6400),
            algoSettings = tpsAlgoSettings
        ),
        cms.PSet(
            sectorNumber = cms.uint32(7),
            barrelSectors= cms.vuint32(1,2,3,4),
            csc10DegreeChambers=cms.vuint32(25,26,27,28,29,30,31,32),
            csc20DegreeChambers=cms.vuint32(14,15,16,17),
            rpcEndcapChambers=cms.vuint32(25,26,27,28,29,30,31,32),
            phiLowerBound = cms.int32(-5120),
            phiUpperBound = cms.int32(-3072),
            phiOffset=cms.int32(-4352),
            algoSettings = tpsAlgoSettings
        ),
        cms.PSet(
            sectorNumber = cms.uint32(8),
            barrelSectors= cms.vuint32(3,4,5,6),
            csc10DegreeChambers=cms.vuint32(29,30,31,32,33,34,35,36),
            csc20DegreeChambers=cms.vuint32(16,17,18,1),
            rpcEndcapChambers=cms.vuint32(29,30,31,32,33,34,35,36),
            phiLowerBound = cms.int32(-3072),
            phiUpperBound = cms.int32(-1024),
            phiOffset=cms.int32(-2304),
            algoSettings = tpsAlgoSettings
            )
        )
)

l1TrackerPlusStubsSequence = cms.Sequence(l1TPSStubs*l1StubMatchedMuons)

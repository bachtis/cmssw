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
        phiLSB             = cms.double(0.00019174760), 
        etaLSB             = cms.double(0.00585938), 
        ),
   DT = cms.PSet(                         
        verbose            = cms.int32(0),
        minPhiQuality      = cms.int32(0),
        minThetaQuality    = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),                           
        disableMasks       = cms.bool(True),                               
        phiLSB             = cms.double(0.00019174760),
        cotTheta_1         = cms.vint32(197,189,182,173,165,156,147,129,120,108,97,86,74,63,38,26,12,0,-12,-25,-38,-62,-74,-86,-98,-108,-120,-129,-148,-157,-165,-172,-182,-189,-197),
        cotTheta_2         = cms.vint32(174,167,159,153,144,136,128,112,103,93,83,73,63,53,33,21,10,0,-10,-21,-32,-54,-63,-73,-84,-93,-102,-111,-128,-136,-144,-153,-160,-167,-175),
        cotTheta_3         = cms.vint32(0,141,137,131,123,117,109,94,86,78,70,61,53,44,27,18,8,0,-8,-18,-26,-45,-53,-61,-70,-77,-86,-94,-109,-116,-123,-131,-137,-141,0),
   ),
   RPCBarrel = cms.PSet(                         
        verbose            = cms.int32(0),
        minPhiQuality      = cms.int32(0),
        minThetaQuality    = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),                           
        disableMasks       = cms.bool(False),                               
        phiLSB             = cms.double(0.00019174760),
        etaLSB             = cms.double(0.00585938), 
        cotTheta_1         = cms.vint32(197,189,182,173,165,156,147,129,120,108,97,86,74,63,38,26,12,0,-12,-25,-38,-62,-74,-86,-98,-108,-120,-129,-148,-157,-165,-172,-182,-189,-197),
        cotTheta_2         = cms.vint32(174,167,159,153,144,136,128,112,103,93,83,73,63,53,33,21,10,0,-10,-21,-32,-54,-63,-73,-84,-93,-102,-111,-128,-136,-144,-153,-160,-167,-175),
        cotTheta_3         = cms.vint32(0,141,137,131,123,117,109,94,86,78,70,61,53,44,27,18,8,0,-8,-18,-26,-45,-53,-61,-70,-77,-86,-94,-109,-116,-123,-131,-137,-141,0),
   ),
   RPCEndcap = cms.PSet(                         
        verbose            = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),                           
        phiLSB             = cms.double(0.00019174760),
        etaLSB             = cms.double(0.00585938), 

   )


)

tpsAlgoSettings = cms.PSet(
    verbose = cms.int32(1),
    phiLSB = cms.double(0.00019174760),
    etaLSB = cms.double(0.000366211),
    etaShift = cms.uint32(4),
    curvLSB  = cms.double(0.000122070),
    matchType = cms.vuint32(
        1,0,0,1,1,0,0,1,1,0,1,0,9,  #0
        1,0,0,1,1,0,0,1,1,0,1,0,9,  #1
        1,0,0,1,1,0,0,1,1,2,3,3,9,  #2
        1,0,0,1,1,0,0,1,2,3,3,2,2,  #3
        1,0,0,1,2,3,3,2,2,3,2,3,9,  #4
        2,3,3,2,2,3,2,3,9,9,9,9,9,  #5
        2,2,3,3,2,2,3,2,3,9,9,9,9,  #6
        2,2,2,3,2,3,9,9,9,9,9,9,9  #7
        ),
    matchEtaRegion = cms.vuint32(
        0,0,0,0,0,0,0,0,0,0,0,0,9,  #0
        1,1,1,1,1,1,1,1,1,1,1,1,9,  #1
        1,1,1,1,1,1,1,1,1,3,3,3,9,  #2
        1,1,1,1,1,1,1,1,3,3,3,4,4,  #3
        1,1,1,1,3,3,3,4,4,3,4,3,9,  #4
        4,4,3,4,4,3,4,4,9,9,9,9,9,  #5
        5,4,4,4,4,4,4,4,3,9,9,9,9,  #6
        5,5,4,4,4,4,9,9,9,9,9,9,9   #7
        ),
    matchEtaRegion2 = cms.vuint32(
        1,1,1,1,1,1,1,1,1,1,1,1,9,  #0
        2,2,2,2,2,2,2,2,2,2,2,2,9,  #1
        2,2,2,2,2,2,2,2,2,9,9,9,9,  #2
        2,2,2,2,2,2,2,2,9,9,9,9,9,  #3
        2,2,2,2,4,4,4,9,9,9,9,9,9,  #4
        9,9,4,9,9,4,9,9,9,9,9,9,9,  #5
        9,9,9,9,5,5,9,9,4,9,9,9,9,  #6
        9,9,5,5,5,5,9,9,9,9,9,9,9,  #7
        ),
    matchDepthRegion = cms.vuint32(
        1,1,1,1,2,2,2,2,3,3,4,4,9,  #0
        1,1,1,1,2,2,2,2,3,3,4,4,9,  #1
        1,1,1,1,2,2,2,2,3,1,1,2,9,  #2
        1,1,1,1,2,2,2,2,1,1,2,2,3,  #3
        1,1,1,1,1,1,2,2,3,3,4,4,9,  #4
        1,1,2,2,3,3,4,4,9,9,9,9,9,  #5
        1,1,1,2,2,3,3,4,4,9,9,9,9,  #6
        1,2,3,3,4,4,9,9,9,9,9,9,9  #7
    ),
    matchID = cms.vuint32(
        0,9,9,1,0,9,9,1,9,9,9,9,9,  #0
        0,9,9,1,0,9,9,1,9,9,9,9,9,  #1
        0,9,9,1,0,9,9,1,9,9,9,9,9,  #2
        0,9,9,1,0,9,9,1,9,9,9,9,9,  #3
        0,9,9,1,9,9,9,9,9,9,9,9,9,  #4
        9,9,9,9,9,9,9,9,9,9,9,9,9,  #5
        9,9,9,9,9,9,9,9,9,9,9,9,9,  #6
        9,9,9,9,9,9,9,9,9,9,9,9,9   #7
        ),

)
        

        



l1StubMatchedMuons = cms.EDProducer("L1TTPSProducer",
    srcStubs = cms.InputTag("l1TPSStubs"),
    srcTracks = cms.InputTag("TTTracksFromTracklet:Level1TTTracks"),
    maxChi2 = cms.double(100000000),
    minStubs = cms.uint32(4),
    verbose = cms.int32(1),                                    
    sectors = cms.VPSet(
        cms.PSet(
            sectorNumber = cms.uint32(0),
            barrelSectors= cms.vuint32(0,1,2,3,4,5,6,7,8,9,10,11),
            csc10DegreeChambers=cms.vuint32(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36),
            csc20DegreeChambers=cms.vuint32(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18),
            rpcEndcapChambers=cms.vuint32(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36),
            iRpcChambers=cms.vuint32(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36),
            phiLowerBound = cms.int32(-16000),
            phiUpperBound = cms.int32(16000),
            phiOffset=cms.int32(0),
            algoSettings = tpsAlgoSettings
        )
    )
)

l1TrackerPlusStubsSequence = cms.Sequence(l1TPSStubs*l1StubMatchedMuons)

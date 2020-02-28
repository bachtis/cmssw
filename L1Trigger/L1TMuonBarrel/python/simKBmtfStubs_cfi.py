import FWCore.ParameterSet.Config as cms


simKBmtfStubs = cms.EDProducer("L1TMuCorrelatorHitProducer",
    useDTPhase2   = cms.bool(False),                        
    verbose = cms.int32(0),
    srcCSC = cms.InputTag("simCscTriggerPrimitiveDigis"),
    srcDT = cms.InputTag("simTwinMuxDigis"),
    srcDTTheta = cms.InputTag("simTwinMuxDigis"),
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
        minBX              = cms.int32(-2),                           
        maxBX              = cms.int32(2),                           
        disableMasks       = cms.bool(False),                               
        phiLSB             = cms.double(0.00019174760),
        eta_1=cms.vint32(105,101,97,93,88,84,79,69,64,58,52,46,40,34,21,14,7,0,-7,-14,-21,-34,-40,-46,-52,-58,-64,-69,-79,-84,-88,-93,-97,-101,-105),
        eta_2=cms.vint32(93,89,85,81,77,73,68,60,55,50,45,40,34,29,17,12,6,0,-6,-12,-17,-29,-34,-40,-45,-50,-55,-60,-68,-73,-77,-81,-85,-89,-93),
        eta_3=cms.vint32(81,77,74,70,66,62,58,51,46,42,38,33,29,24,15,10,5,0,-5,-10,-15,-24,-29,-33,-38,-42,-46,-51,-58,-62,-66,-70,-74,-77,-81),
        coarseEta_1 = cms.vint32(0,-102,-171),
        coarseEta_2 = cms.vint32(0,-85,-154),
        coarseEta_3 = cms.vint32(0,-68,-137),
        coarseEta_4 = cms.vint32(0,-68,-119),
        bendingScale       = cms.vdouble(8.0*1.7370158,8.0*2.6109661,8.0*4.4111160,8.0*10.56),
        phiOffset          = cms.vint32(17,-12,8,2)
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
        coarseEta_1 = cms.vint32(0,-102,-171),
        coarseEta_2 = cms.vint32(0,-85,-154),
        coarseEta_3 = cms.vint32(0,-68,-137),
        coarseEta_4 = cms.vint32(0,-68,-119),
        phiOffset          = cms.vint32(17,-12,8,2)
   ),
   RPCEndcap = cms.PSet(                         
        verbose            = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),                           
        phiLSB             = cms.double(0.00019174760),
        etaLSB             = cms.double(0.00585938), 
   )


)


simPhase2KBmtfStubs = cms.EDProducer("L1TMuCorrelatorHitProducer",
    useDTPhase2   = cms.bool(True),                        
    verbose = cms.int32(0),
    srcCSC = cms.InputTag("simCscTriggerPrimitiveDigis"),
    srcDT = cms.InputTag("dtTriggerPhase2PrimitiveDigis"),
    srcDTTheta = cms.InputTag("simTwinMuxDigis"),
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
        minBX              = cms.int32(-2),                           
        maxBX              = cms.int32(2),                           
        disableMasks       = cms.bool(True),                               
        phiLSB             = cms.double(0.00019174760),
        eta_1         = cms.vint32(197,189,182,173,165,156,147,129,120,108,97,86,74,63,38,26,12,0,-12,-25,-38,-62,-74,-86,-98,-108,-120,-129,-148,-157,-165,-172,-182,-189,-197),
        eta_2         = cms.vint32(174,167,159,153,144,136,128,112,103,93,83,73,63,53,33,21,10,0,-10,-21,-32,-54,-63,-73,-84,-93,-102,-111,-128,-136,-144,-153,-160,-167,-175),
        eta_3         = cms.vint32(0,141,137,131,123,117,109,94,86,78,70,61,53,44,27,18,8,0,-8,-18,-26,-45,-53,-61,-70,-77,-86,-94,-109,-116,-123,-131,-137,-141,0),
        coarseEta_1 = cms.vint32(0,-102,-171),
        coarseEta_2 = cms.vint32(0,-85,-154),
        coarseEta_3 = cms.vint32(0,-68,-137),
        coarseEta_4 = cms.vint32(0,-68,-119),
        bendingScale       = cms.vdouble(1.84,2.96,5.52,15.28),
        phiOffset          = cms.vint32(0,0,0,0)
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
        coarseEta_1 = cms.vint32(0,-102,-171),
        coarseEta_2 = cms.vint32(0,-85,-154),
        coarseEta_3 = cms.vint32(0,-68,-137),
        coarseEta_4 = cms.vint32(0,-68,-119),
        phiOffset          = cms.vint32(17,-12,8,2)
   ),
   RPCEndcap = cms.PSet(                         
        verbose            = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),                           
        phiLSB             = cms.double(0.00019174760),
        etaLSB             = cms.double(0.00585938), 
   )


)

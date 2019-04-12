import FWCore.ParameterSet.Config as cms
import math
from math import pi


l1DTStubsForTPS = cms.EDProducer("L1TMuonBarrelKalmanStubProducer",
    verbose = cms.int32(0),
    srcPhi = cms.InputTag("simDtTriggerPrimitiveDigis"),
    srcTheta = cms.InputTag("simDtTriggerPrimitiveDigis"),
    minPhiQuality = cms.int32(0),
    minThetaQuality = cms.int32(0),
    minBX = cms.int32(0),                           
    maxBX = cms.int32(0),                           
    disableMasks = cms.bool(False),                               
    cotTheta_1=cms.vint32(105,101,97,93,88,84,79,69,64,58,52,46,40,34,21,14,7,0,-7,-14,-21,-34,-40,-46,-52,-58,-64,-69,-79,-84,-88,-93,-97,-101,-105),
    cotTheta_2=cms.vint32(93,89,85,81,77,73,68,60,55,50,45,40,34,29,17,12,6,0,-6,-12,-17,-29,-34,-40,-45,-50,-55,-60,-68,-73,-77,-81,-85,-89,-93),
    cotTheta_3=cms.vint32(81,77,74,70,66,62,58,51,46,42,38,33,29,24,15,10,5,0,-5,-10,-15,-24,-29,-33,-38,-42,-46,-51,-58,-62,-66,-70,-74,-77,-81)
)


l1NewStubs = cms.EDProducer("L1TMuCorrelatorHitProducer",
    verbose = cms.int32(0),
    srcCSC = cms.InputTag("simCscTriggerPrimitiveDigis"),
    srcDT = cms.InputTag("simDtTriggerPrimitiveDigis"),
    srcRPC = cms.InputTag("simMuonRPCDigis"),
    CSC =cms.PSet(                            
        verbose = cms.int32(0),
        minBX            = cms.int32(0),                           
        maxBX            = cms.int32(0),         
        ),
   DT = cms.PSet(                         
        verbose = cms.int32(0),
        minPhiQuality = cms.int32(0),
        minThetaQuality = cms.int32(0),
        minBX = cms.int32(0),                           
        maxBX = cms.int32(0),                           
        disableMasks = cms.bool(True),                               
        cotTheta_1=cms.vint32(105,101,97,93,88,84,79,69,64,58,52,46,40,34,21,14,7,0,-7,-14,-21,-34,-40,-46,-52,-58,-64,-69,-79,-84,-88,-93,-97,-101,-105),
        cotTheta_2=cms.vint32(93,89,85,81,77,73,68,60,55,50,45,40,34,29,17,12,6,0,-6,-12,-17,-29,-34,-40,-45,-50,-55,-60,-68,-73,-77,-81,-85,-89,-93),
        cotTheta_3=cms.vint32(81,77,74,70,66,62,58,51,46,42,38,33,29,24,15,10,5,0,-5,-10,-15,-24,-29,-33,-38,-42,-46,-51,-58,-62,-66,-70,-74,-77,-81)
   ),
   RPCBarrel = cms.PSet(                         
    verbose = cms.int32(0),
    minPhiQuality = cms.int32(0),
    minThetaQuality = cms.int32(0),
    minBX = cms.int32(0),                           
    maxBX = cms.int32(0),                           
    disableMasks = cms.bool(False),                               
    cotTheta_1=cms.vint32(105,101,97,93,88,84,79,69,64,58,52,46,40,34,21,14,7,0,-7,-14,-21,-34,-40,-46,-52,-58,-64,-69,-79,-84,-88,-93,-97,-101,-105),
    cotTheta_2=cms.vint32(93,89,85,81,77,73,68,60,55,50,45,40,34,29,17,12,6,0,-6,-12,-17,-29,-34,-40,-45,-50,-55,-60,-68,-73,-77,-81,-85,-89,-93),
    cotTheta_3=cms.vint32(81,77,74,70,66,62,58,51,46,42,38,33,29,24,15,10,5,0,-5,-10,-15,-24,-29,-33,-38,-42,-46,-51,-58,-62,-66,-70,-74,-77,-81),
   ),
   RPCEndcap = cms.PSet(                         
   verbose = cms.int32(0),
   minBX = cms.int32(0),                           
   maxBX = cms.int32(0)                           
   )


)







l1StubMatchedMuons = cms.EDProducer("L1TTrackerPlusStubsProducer",
    srcStubs = cms.InputTag("l1DTStubsForTPS"),
    srcTracks = cms.InputTag("TTTracksFromTracklet:Level1TTTracks"),
    maxChi2 = cms.double(100),
    trackMatcherSettings = cms.PSet(
        sectorsToProcess = cms.vint32(0,1,2,3,4,5,6,7,8,9,10,11),
	verbose = cms.int32(0),
        geomPi = cms.double(pi),
        propagationConstants = cms.vdouble(-1.14441e0,-1.24939e0,-1.31598e0,-1.34792e0),
	sectorSettings = cms.PSet(
            verbose = cms.int32(0),
            geomPi = cms.double(pi),
	    stationsToProcess = cms.vint32(1,2,3,4),
	    tolerance = cms.double(3),
	    toleranceB = cms.double(3),
	    toleranceQ = cms.int32(2),
	    phi1 = cms.vdouble(11*pi/12,-11*pi/12,-9*pi/12,-7*pi/12,-5*pi/12,-3*pi/12,-pi/12,pi/12,3*pi/12,5*pi/12,7*pi/12,9*pi/12),
	    phi2 = cms.vdouble(-11*pi/12,-9*pi/12,-7*pi/12,-5*pi/12,-3*pi/12,-pi/12,pi/12,3*pi/12,5*pi/12,7*pi/12,9*pi/12,11*pi/12),
            propagationConstants = cms.vdouble(-1.14441e0,-1.24939e0,-1.31598e0,-1.34792e0),
	    propagationConstantsB = cms.vdouble(-9.05378e-2,-6.34508e-2,-3.22425e-2,-9.16026e-3),
	    etaHigh2 = cms.vdouble(1.25,1.1,0.95,0.835),
	    etaHigh1 = cms.vdouble(0.85,0.725,0.615,0.54),
	    etaHigh0 = cms.vdouble(0.3,0.25,0.21,0.175),
	    etaHighm1 = cms.vdouble(-0.325,-0.25,-0.225,-0.19),
	    etaHighm2 = cms.vdouble(-0.815,-0.72,-0.6,-0.525),
	    etaLow2 = cms.vdouble(0.815,0.72,0.6,0.525),
	    etaLow1 = cms.vdouble(0.325,0.25,0.225,0.19),
	    etaLow0 = cms.vdouble(-0.3,-0.25,-0.21,-0.175),
	    etaLowm1 = cms.vdouble(-0.85,-0.725,-0.615,-0.54),
	    etaLowm2 = cms.vdouble(-1.25,-1.1,-0.95,-0.835),
	    alpha = cms.vdouble(2.50016e-2,3.10230e-2,3.65267e-2,3.46650e-2),
	    beta = cms.vdouble(2*2.13888e1,2*2.15320e1,2*2.09371e1,2*2.98381e1),
	    alphaB = cms.vdouble(5.56536e-3,6.74712e-3,8.22339e-3,1.04466e-2),
	    betaB = cms.vdouble(1.68262e0,1.61702e0,1.52894e0,1.25928e0)
        )
        
    )
)

l1TrackerPlusStubsSequence = cms.Sequence(l1DTStubsForTPS*l1StubMatchedMuons)

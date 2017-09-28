import FWCore.ParameterSet.Config as cms

l1kalmanInputStubs = cms.EDProducer('L1BarrelStubSelector',
                                    onlyBX0 = cms.bool(True)
)

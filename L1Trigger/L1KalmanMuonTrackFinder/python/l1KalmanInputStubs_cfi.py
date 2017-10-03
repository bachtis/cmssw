import FWCore.ParameterSet.Config as cms
l1kalmanInputStubs = cms.EDProducer('L1BarrelStubSelector',
                                    src = cms.InputTag("simTwinMuxDigis"),
                                    onlyBX0 = cms.bool(True)
)



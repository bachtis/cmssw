import FWCore.ParameterSet.Config as cms

bmtfDigis = cms.EDProducer(
    "L1TRawToDigi",
    Setup = cms.string("stage2::BMTFSetup"),#what module creates the unpackers
    InputLabel = cms.InputTag("rawDataCollector"),#input to unpack
    FedIds = cms.vint32(1376,1377),#bmtf feds
    FWId = cms.uint32(1),#bmtf algo related info
    lenSlinkHeader = cms.untracked.int32(8),#length of the link's header
    lenSlinkTrailer = cms.untracked.int32(8),#length of the link's trailer
    lenAMCHeader = cms.untracked.int32(8),#AMC(MP7) header length
    lenAMCTrailer = cms.untracked.int32(0),#AMC(MP7) trailer length
    lenAMC13Header = cms.untracked.int32(8),#AMC13 header length
    lenAMC13Trailer = cms.untracked.int32(8)#AMC13 trailer length
)

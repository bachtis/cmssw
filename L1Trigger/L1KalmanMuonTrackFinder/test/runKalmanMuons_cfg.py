import FWCore.ParameterSet.Config as cms

process = cms.Process("L1MUONKF")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Services_cff')




process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )




process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
#        'file:/scratch3/MTF/data/singleMu140.root'
#        'file:/scratch3/MTF/data/singleNu140.root'
        'file:/scratch3/MTF/data/hzz4mu140.root'
    )
)

process.load("L1Trigger.L1KalmanMuonTrackFinder.l1KalmanMuons_cff")


##Stage 2 BMTF emulator####
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '90X_upgrade2023_realistic_v9', '')

#Emulator Parameter
process.load('L1Trigger.L1TMuonBarrel.fakeBmtfParams_cff')
process.esProd = cms.EDAnalyzer("EventSetupRecordDataGetter",
   toGet = cms.VPSet(
      cms.PSet(record = cms.string('L1TMuonBarrelParamsRcd'),
               data = cms.vstring('L1TMuonBarrelParams'))
                   ),
   verbose = cms.untracked.bool(True)
)
process.fakeBmtfParams.fwVersion = cms.uint32(2)
#process.fakeBmtfParams.EtaTrackFinder = cms.bool(False)
process.fakeBmtfParams.BX_max = cms.int32(2)
process.fakeBmtfParams.BX_min = cms.int32(-2)

#enable stations here-each digit corresponds to a sector
maskenable      = '000000000000'
maskdisable     = '111111111111'
process.fakeBmtfParams.mask_phtf_st1        = cms.vstring(maskdisable, maskenable, maskenable, maskenable, maskenable, maskenable, maskdisable)
process.fakeBmtfParams.mask_phtf_st2        = cms.vstring(maskenable,  maskenable, maskenable, maskenable, maskenable, maskenable, maskenable)
process.fakeBmtfParams.mask_phtf_st3        = cms.vstring(maskenable,  maskenable, maskenable, maskenable, maskenable, maskenable, maskenable)
process.fakeBmtfParams.mask_phtf_st4        = cms.vstring(maskenable,  maskenable, maskenable, maskenable, maskenable, maskenable, maskenable)

process.fakeBmtfParams.mask_ettf_st1        = cms.vstring(maskdisable, maskenable, maskenable, maskenable, maskenable, maskenable, maskdisable)
process.fakeBmtfParams.mask_ettf_st2        = cms.vstring(maskenable,  maskenable, maskenable, maskenable, maskenable, maskenable, maskenable)
process.fakeBmtfParams.mask_ettf_st3        = cms.vstring(maskenable,  maskenable, maskenable, maskenable, maskenable, maskenable, maskenable)
####BMTF Emulator is loaded here
process.load('L1Trigger.L1TMuonBarrel.simTwinMuxDigis_cfi')
process.load('L1Trigger.L1TMuonBarrel.simBmtfDigis_cfi')
process.simBmtfDigis.Debug = cms.untracked.int32(0)
process.simBmtfDigis.DTDigi_Source = cms.InputTag("simDtTriggerPrimitiveDigis")
process.simBmtfDigis.DTDigi_Theta_Source = cms.InputTag("simDtTriggerPrimitiveDigis")










process.out = cms.OutputModule("PoolOutputModule",
     fileName = cms.untracked.string('higgsFourMuons140.root')
#    fileName = cms.untracked.string('singleNeutrino140.root')
#    fileName = cms.untracked.string('singleMuon140.root')

)

  
process.p = cms.Path(process.simBmtfDigis*process.l1KalmanMuons)
process.e = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.p,process.e)

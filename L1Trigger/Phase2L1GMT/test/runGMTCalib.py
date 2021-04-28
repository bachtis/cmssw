# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step1 --conditions 111X_mcRun4_realistic_T15_v3 -n 2 --era Phase2C9 --eventcontent FEVTDEBUGHLT --runUnscheduled file:/eos/cms/store/relval/CMSSW_11_0_0/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_110X_mcRun4_realistic_v3_2026D49PU200-v2/10000/01054EE2-1B51-C449-91A2-5202A60D16A3.root -s RAW2DIGI,L1TrackTrigger,L1 --datatier FEVTDEBUGHLT --customise SLHCUpgradeSimulations/Configuration/aging.customise_aging_1000,L1Trigger/Configuration/customisePhase2TTNoMC.customisePhase2TTNoMC,Configuration/DataProcessing/Utils.addMonitoring --geometry Extended2026D49 --fileout file:/tmp/step1_Reprocess_TrackTrigger_L1.root --no_exec --nThreads 8 --python step1_L1_ProdLike.py --filein das:/TT_TuneCP5_14TeV-powheg-pythia8/Phase2HLTTDRWinter20DIGI-PU200_110X_mcRun4_realistic_v3-v2/GEN-SIM-DIGI-RAW
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Phase2C9_cff import Phase2C9

process = cms.Process('L1',Phase2C9)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D49Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1TrackTrigger_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( (

     ) ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(

        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(1)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step1 nevts:2'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('FEVTDEBUGHLT'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('reprocess.root'),
    outputCommands = cms.untracked.vstring(
        "drop *_*_*_*",
        "keep *_gmtMuons_*_*",
        "keep *_gmtStubs_*_*",
        "keep *_genParticles_*_*",
        "keep *_TTTracksFromTrackletEmulation_Level1TTTracks_*",
        "keep *_L1TkMuons_*_*"
    ),
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '111X_mcRun4_realistic_T15_v3', '')

##
#Calibrate Digis
process.load("L1Trigger.DTTriggerPhase2.CalibratedDigis_cfi")
process.CalibratedDigis.dtDigiTag = "simMuonDTDigis" 
process.CalibratedDigis.scenario = 0

#DTTriggerPhase2
process.load("L1Trigger.DTTriggerPhase2.dtTriggerPhase2PrimitiveDigis_cfi")
process.dtTriggerPhase2PrimitiveDigis.debug = False
process.dtTriggerPhase2PrimitiveDigis.dump = False
process.dtTriggerPhase2PrimitiveDigis.scenario = 0

process.load("L1Trigger.Phase2L1GMT.gmt_cff")
process.gmtMuons.trackMatching.verbose=0
process.gmtMuons.verbose=0
process.gmtMuons.trackConverter.verbose=0

process.gmtStubs = cms.EDProducer("Phase2L1TGMTStubProducer",
    verbose = cms.int32(0),
    srcCSC = cms.InputTag("simCscTriggerPrimitiveDigis"),
    srcDT = cms.InputTag("dtTriggerPhase2PrimitiveDigis"),
    srcDTTheta = cms.InputTag("simDtTriggerPrimitiveDigis"),
    srcRPC = cms.InputTag("simMuonRPCDigis"),
    Endcap =cms.PSet(                            
        verbose              = cms.uint32(0),
        minBX                = cms.int32(0),                           
        maxBX                = cms.int32(0),         
        coord1LSB            = cms.double(9.5873799e-05), 
        eta1LSB              = cms.double(7.68334e-04), 
        coord2LSB            = cms.double(9.5873799e-05), 
        eta2LSB              = cms.double(7.68334e-04),
        phiMatch             = cms.double(0.05),
        etaMatch             = cms.double(0.1)
    ),
    Barrel = cms.PSet(                         
        verbose            = cms.int32(0),
        minPhiQuality      = cms.int32(0),
        minThetaQuality    = cms.int32(0),
        minBX              = cms.int32(0),                           
        maxBX              = cms.int32(0),                           
        phiLSB             = cms.double(9.5873799e-05),
        phiBDivider        = cms.int32(1),
        etaLSB             = cms.double(7.68334e-04), 
        eta_1              = cms.vint32(-1503,-1446,-1387,-1327,-1266,-1194,-1125,-985,-916,-839,-752,-670,-582,-489,-315,-213,-115,-49,49, 115, 213, 315, 489, 582, 670, 752, 839, 916, 985, 1125, 1194, 1266, 1327, 1387, 1446, 1503),
        eta_2              = cms.vint32(-1334,-1279,-1227,-1168,-1109,-1044,-982,-861,-793,-720,-648,-577,-496,-425,-268,-185,-97,-51,51, 97, 185, 268, 425, 496, 577, 648, 720, 793, 861, 982, 1044, 1109, 1168, 1227, 1279, 1334),
        eta_3              = cms.vint32(-1148,-1110,-1051,-1004,-947,-895,-839,-728,-668,-608,-546,-485,-425,-366,-222,-155,-87,-40,40, 87, 155, 222, 366, 425, 485, 546, 608, 668, 728, 839, 895, 947, 1004, 1051, 1110, 1148),
        coarseEta_1        = cms.vint32(0,758,1336),
        coarseEta_2        = cms.vint32(0,653,1168),
        coarseEta_3        = cms.vint32(0,552,1001),
        coarseEta_4        = cms.vint32(0,478,878),
        phiOffset          = cms.vint32(33,-15,+14,0)    
   )

)


#process.schedule = cms.Schedule(process.L1TrackTrigger_step,process.pL1TMuonTPS,process.endjob_step,process.e) # Adding MuonTPS


# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1TrackTrigger_step = cms.Path(process.L1TrackTrigger)
#process.pL1TkPrimaryVertex = cms.Path(process.L1TkPrimaryVertex)
#process.pL1TkPhotonsCrystal = cms.Path(process.L1TkPhotonsCrystal)
#process.pL1TkIsoElectronsCrystal = cms.Path(process.L1TkIsoElectronsCrystal)
#process.pL1TkElectronsLooseCrystal = cms.Path(process.L1TkElectronsLooseCrystal)
#process.pL1TkElectronsLooseHGC = cms.Path(process.L1TkElectronsLooseHGC)
#process.pL1TkElectronsHGC = cms.Path(process.L1TkElectronsHGC)
process.pL1TkMuon = cms.Path(process.L1TkMuons+process.L1TkMuonsTP)
#process.pL1TkElectronsEllipticMatchHGC = cms.Path(process.L1TkElectronsEllipticMatchHGC)
#process.pL1TkElectronsCrystal = cms.Path(process.L1TkElectronsCrystal)
#process.pL1TkPhotonsHGC = cms.Path(process.L1TkPhotonsHGC)
#process.pL1TkIsoElectronsHGC = cms.Path(process.L1TkIsoElectronsHGC)
#process.pL1TkElectronsEllipticMatchCrystal = cms.Path(process.L1TkElectronsEllipticMatchCrystal)
#process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.testpath=cms.Path(process.CalibratedDigis*process.dtTriggerPhase2PrimitiveDigis*process.phase2GMT)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1TrackTrigger_step,process.testpath,process.endjob_step,process.FEVTDEBUGHLToutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#Setup FWK for multithreaded
#process.options.numberOfThreads=cms.untracked.uint32(1)
#process.options.numberOfStreams=cms.untracked.uint32(0)
#process.options.numberOfConcurrentLuminosityBlocks=cms.untracked.uint32(1)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.aging
from SLHCUpgradeSimulations.Configuration.aging import customise_aging_1000 

#call to customisation function customise_aging_1000 imported from SLHCUpgradeSimulations.Configuration.aging
#process = customise_aging_1000(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customisePhase2TTNoMC
from L1Trigger.Configuration.customisePhase2TTNoMC import customisePhase2TTNoMC 

#call to customisation function customisePhase2TTNoMC imported from L1Trigger.Configuration.customisePhase2TTNoMC
process = customisePhase2TTNoMC(process)

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

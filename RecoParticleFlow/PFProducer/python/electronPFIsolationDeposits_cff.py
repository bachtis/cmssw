import FWCore.ParameterSet.Config as cms

from CommonTools.ParticleFlow.Isolation.tools_cfi import *

#Now prepare the iso deposits
elPFIsoDepositCharged=isoDepositReplace('pfSelectedElectrons','pfAllChargedHadrons')
elPFIsoDepositChargedAll=isoDepositReplace('pfSelectedElectrons','pfAllChargedParticles')
elPFIsoDepositNeutral=isoDepositReplace('pfSelectedElectrons','pfWeightedNeutralHadrons')
elPFIsoDepositGamma=isoDepositReplace('pfSelectedElectrons','pfWeightedPhotons')
elPFIsoDepositPU=isoDepositReplace('pfSelectedElectrons','pfPileUpAllChargedParticles')

electronPFIsolationDepositsSequence = cms.Sequence(
    elPFIsoDepositCharged+
    elPFIsoDepositChargedAll+
    elPFIsoDepositGamma+
    elPFIsoDepositNeutral+
    elPFIsoDepositPU
    )

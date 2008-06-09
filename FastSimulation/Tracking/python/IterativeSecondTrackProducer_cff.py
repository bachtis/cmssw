import FWCore.ParameterSet.Config as cms

import RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi
iterativeSecondTracksWithTriplets = RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi.ctfWithMaterialTracks.clone()
import RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi
iterativeSecondTracksWithPlTriplets = RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi.ctfWithMaterialTracks.clone()
iterativeSecondTracks = cms.Sequence(iterativeSecondTracksWithTriplets+iterativeSecondTracksWithPlTriplets)
iterativeSecondTracksWithTriplets.src = 'iterativeSecondTrackCandidatesWithTriplets'
iterativeSecondTracksWithTriplets.TTRHBuilder = 'WithoutRefit'
iterativeSecondTracksWithTriplets.Fitter = 'KFFittingSmoother'
iterativeSecondTracksWithTriplets.Propagator = 'PropagatorWithMaterial'
iterativeSecondTracksWithPlTriplets.src = 'iterativeSecondTrackCandidatesWithPlTriplets'
iterativeSecondTracksWithPlTriplets.TTRHBuilder = 'WithoutRefit'
iterativeSecondTracksWithPlTriplets.Fitter = 'KFFittingSmootherWithOutliersRejectionAndRK'
iterativeSecondTracksWithPlTriplets.Propagator = 'PropagatorWithMaterial'



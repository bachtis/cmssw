#ifndef PHASE2GMT_TRACKCONVERTER
#define PHASE2GMT_TRACKCONVERTER

#include "L1Trigger/Phase2L1GMT/interface/ConvertedTTTrack.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace Phase2L1GMT {

  class TrackConverter {

  public:
  TrackConverter(const edm::ParameterSet& iConfig):
    verbose_(iConfig.getParameter<int>("verbose"))
      {}
    ~TrackConverter() {}

    std::vector<ConvertedTTTrack> convertTracks(const std::vector<edm::Ptr< l1t::TrackerMuon::L1TTTrackType > >& tracks) {
      std::vector<ConvertedTTTrack> out;
      for (const auto& t :  tracks)
	out.push_back(convert(t));
      return out;
    }

  private:
    int verbose_;

    uint generateQuality(const edm::Ptr<TTTrack<Ref_Phase2TrackerDigi_> >& track) {
      return 1;
    }

    ConvertedTTTrack convert(const edm::Ptr<TTTrack<Ref_Phase2TrackerDigi_> >& track) {
      uint                charge      = (track->rInv()<0) ? 1 : 0;
      int                 curvature   = track->rInv()*(1<<(BITSTTCURV-1))/maxCurv_;
      int                 phi         = track->phi()*(1<<(BITSPHI-1))/(M_PI);
      int                 tanLambda   = track->tanL()*(1<<(BITSTTTANL-1))/maxTanl_;
      int                 z0          = track->z0()*(1<<(BITSZ0-1))/maxZ0_;
      int                 d0          = track->d0()*(1<<(BITSD0-1))/maxD0_;
      //calculate pt
      uint  absCurv  = curvature>0 ? (curvature) : (-curvature);
      uint pt = ptLUT[absCurv>>3];
      uint quality = generateQuality(track);
      uint absTanL = tanLambda >0 ? (tanLambda) : (-tanLambda);
      uint absEta = etaLUT[absTanL>>4];
      int eta = tanLambda >0 ?  (absEta) : (-absEta);

      ap_uint<96> word = twos_complement(curvature,BITSTTCURV);
      ap_int<BITSPHI> phiSec = ap_int<BITSPHI>(phi)-ap_int<BITSPHI>((track->phiSector()*40*M_PI/180.)*(1<<(BITSPHI-1))/(M_PI));

      word = word |(twos_complement(phiSec,BITSTTPHI)<<BITSTTCURV);
      word = word |(twos_complement(tanLambda,BITSTTTANL)<<(BITSTTCURV+BITSTTPHI));
      word = word |(twos_complement(z0,BITSZ0)<<(BITSTTCURV+BITSTTPHI+BITSTTTANL));
      word = word |(twos_complement(d0,BITSD0)<<(BITSTTCURV+BITSTTPHI+BITSTTTANL+BITSZ0));
      word=  word |(twos_complement(uint(track->chi2()),4)<<(BITSTTCURV+BITSTTPHI+BITSTTTANL+BITSZ0+BITSD0)); 

      ap_int<BITSPHI> phiCorrected = ap_int<BITSPHI>(phiSec+track->phiSector()*910);
      ConvertedTTTrack convertedTrack(charge,curvature,absEta,pt,eta,phiCorrected.to_int(),z0,d0,quality,word);
      convertedTrack.setOfflineQuantities(track->momentum().transverse(),track->eta(),track->phi());
      if (verbose_)
	convertedTrack.print();
      convertedTrack.setTrkPtr(track);
      return convertedTrack;
    }
  };
}

#endif




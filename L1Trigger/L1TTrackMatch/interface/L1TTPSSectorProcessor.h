#ifndef L1TTPSSECTORPROCESSOR_H
#define L1TTPSSECTORPROCESSOR_H

#include "DataFormats/L1TMuon/interface/L1MuCorrelatorHit.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkMuonParticle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class L1TTPSSectorProcessor {
 public:
  typedef std::vector<edm::Ptr< l1t::L1TkMuonParticle::L1TTTrackType > > TrackPtrVector;

  L1TTPSSectorProcessor(const edm::ParameterSet&);
  ~L1TTPSSectorProcessor();

  std::vector<l1t::L1TkMuonParticle> process(const TrackPtrVector& ,const L1MuCorrelatorHitRefVector&);
  uint sector() const {
    return sectorNumber_;
  }


 private:
  typedef struct {
    int propagatedValue;
    int propagatedPhi; //only in software
    int propagatedEta;
    uint propagationVar; //only in software
    uint propagatedSigma;
    int etaIndex;   
    bool valid;
    uint propagatorIndex;
  } PropagationInfo;

  int deltaPhi(int phi1,int phi2);

  int stubPhi(const L1MuCorrelatorHitRef&);
  int trackPhi(const l1t::L1TkMuonParticle&);
  int trackEta(const l1t::L1TkMuonParticle&);
  int trackCurv(const l1t::L1TkMuonParticle&);
  PropagationInfo propagate(const l1t::L1TkMuonParticle&,uint);
  bool match(l1t::L1TkMuonParticle&,const PropagationInfo&,const L1MuCorrelatorHitRefVector&);
  void processTrack(l1t::L1TkMuonParticle&,const L1MuCorrelatorHitRefVector&);
  std::vector<l1t::L1TkMuonParticle> clean(const std::vector<l1t::L1TkMuonParticle>&);
  uint sectorNumber_;
  std::vector<uint> barrelSectors_;
  std::vector<uint> csc10DegreeChambers_;
  std::vector<uint> csc20DegreeChambers_;
  std::vector<uint> rpcEndcapChambers_;
  std::vector<uint> iRpcChambers_;
  int trackPhiLowerBound_;
  int trackPhiUpperBound_;
  int phiOffset_;


  int verbose_;
  double phiLSB_;
  double trackEtaLSB_;
  uint trackEtaShift_;
  double trackCurvLSB_;
  std::vector<uint> matchType_;
  std::vector<uint> matchEtaRegion_;
  std::vector<uint> matchEtaRegion2_;
  std::vector<uint> matchDepthRegion_;
  std::vector<uint> matchID_;







  

};

#endif

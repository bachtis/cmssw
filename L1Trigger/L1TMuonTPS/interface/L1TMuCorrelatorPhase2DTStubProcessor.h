#ifndef L1TMUCORRELATORPHASE2DTSTUBPROCESSOR
#define L1TMUCORRELATORPHASE2DTSTUBPROCESSOR

#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"
#include "DataFormats/L1TMuon/interface/L1MuCorrelatorHit.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/L1TObjects/interface/L1TMuonBarrelParams.h"
#include "CondFormats/DataRecord/interface/L1TMuonBarrelParamsRcd.h"

class L1MuDTTFMasks;


class L1TMuCorrelatorPhase2DTStubProcessor {
 public:
  L1TMuCorrelatorPhase2DTStubProcessor();
  L1TMuCorrelatorPhase2DTStubProcessor(const edm::ParameterSet&);
  
  ~L1TMuCorrelatorPhase2DTStubProcessor();


  L1MuCorrelatorHitCollection makeStubs(const L1Phase2MuDTPhContainer*,const L1MuDTChambThContainer*,const L1TMuonBarrelParams&);
  
 private:
  bool isGoodPhiStub(const L1Phase2MuDTPhDigi*); 
  L1MuCorrelatorHit buildStub(const L1Phase2MuDTPhDigi&,const L1MuDTChambThDigi*,const L1MuCorrelatorHitCollection&);
  L1MuCorrelatorHit buildStubNoEta(const L1Phase2MuDTPhDigi&,const L1MuCorrelatorHitCollection&);

  int calculateEta(uint, int,uint,uint);  
  int minPhiQuality_;
  int minBX_;
  int maxBX_;
  std::vector<int> eta1_;
  std::vector<int> eta2_;
  std::vector<int> eta3_;
  std::vector<int> coarseEta1_;
  std::vector<int> coarseEta2_;
  std::vector<int> coarseEta3_;
  std::vector<int> coarseEta4_;
  int verbose_;
  double phiLSB_;
  std::vector<double> bendingScale_;

  //    edm::ESHandle< L1TMuonBarrelParams > bmtfParamsHandle;
  //    L1MuDTTFMasks       masks_;





};


#endif

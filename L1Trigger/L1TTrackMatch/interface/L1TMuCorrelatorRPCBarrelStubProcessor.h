#ifndef L1TMUCORRELATORRPCBARRELSTUBPROCESSOR
#define L1TMUCORRELATORRPCBARRELSTUBPROCESSOR

#include "DataFormats/L1TMuon/interface/L1MuCorrelatorHit.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "L1Trigger/L1TTwinMux/interface/RPCtoDTTranslator.h"

class L1TMuCorrelatorRPCBarrelStubProcessor {
 public:
  L1TMuCorrelatorRPCBarrelStubProcessor();
  L1TMuCorrelatorRPCBarrelStubProcessor(const edm::ParameterSet&);  
  ~L1TMuCorrelatorRPCBarrelStubProcessor();

  L1MuCorrelatorHitCollection makeStubs(const L1MuDTChambPhContainer&);
  
 private:
  L1MuCorrelatorHit buildStubNoEta(const L1MuDTChambPhDigi&);
  int minBX_;
  int maxBX_;
  int verbose_;
  double phiLSB_;


};


#endif

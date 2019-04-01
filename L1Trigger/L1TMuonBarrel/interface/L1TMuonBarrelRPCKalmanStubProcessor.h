#ifndef L1TMUONBARRELRPCKALMANSTUBPROCESSOR
#define L1TMUONBARRELRPCKALMANSTUBPROCESSOR

#include "DataFormats/L1TMuon/interface/L1MuKBMTCombinedStub.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "L1Trigger/L1TTwinMux/interface/RPCtoDTTranslator.h"

class L1TMuonBarrelRPCKalmanStubProcessor {
 public:
  L1TMuonBarrelRPCKalmanStubProcessor();
  L1TMuonBarrelRPCKalmanStubProcessor(const edm::ParameterSet&);
  
  ~L1TMuonBarrelRPCKalmanStubProcessor();


  L1MuKBMTCombinedStubCollection makeStubs(const L1MuDTChambPhContainer&);

  
 private:
  L1MuKBMTCombinedStub buildStubNoEta(const L1MuDTChambPhDigi&);
  int minBX_;
  int maxBX_;
  int verbose_;


  //    edm::ESHandle< L1TMuonBarrelParams > bmtfParamsHandle;
  //    L1MuDTTFMasks       masks_;





};


#endif

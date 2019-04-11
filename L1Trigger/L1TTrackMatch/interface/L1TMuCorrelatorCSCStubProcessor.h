//
// Combined correlator Hit producer
//Michalis Bachtis (UCLA)
//

#ifndef L1TMUCORRELATORCSCSTUBPROCESSOR
#define L1TMUCORRELATORCSCSTUBPROCESSOR

#include "DataFormats/L1TMuon/interface/L1MuCorrelatorHit.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigi.h"
#include "DataFormats/MuonData/interface/MuonDigiCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"

class L1TMuCorrelatorCSCStubProcessor {


 public:
  L1TMuCorrelatorCSCStubProcessor();
  L1TMuCorrelatorCSCStubProcessor(const edm::ParameterSet&);
    ~L1TMuCorrelatorCSCStubProcessor();


  L1MuCorrelatorHitCollection makeStubs(const MuonDigiCollection<CSCDetId,CSCCLCTDigi>&,const MuonDigiCollection<CSCDetId,CSCALCTDigi>&);

    
 private:
  L1MuCorrelatorHit buildStub(const CSCDetId& , const CSCCLCTDigi&,const MuonDigiCollection<CSCDetId,CSCALCTDigi>&);
  int minBX_;
  int maxBX_;

  std::vector<int> halfStrips_;
  std::vector<int> chambers_;
  std::vector<int> wires_;
  std::vector<int> etaLUT1_;
  std::vector<int> etaLUT2_;
  std::vector<int> etaLUT3_;
  std::vector<int> etaLUT4_;
  uint getIndex(const CSCDetId& id);


};


#endif

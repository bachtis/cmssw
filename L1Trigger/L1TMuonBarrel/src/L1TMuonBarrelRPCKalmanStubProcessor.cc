
#include "L1Trigger/L1TMuonBarrel/interface/L1TMuonBarrelRPCKalmanStubProcessor.h"
#include <cmath>
#include "CondFormats/L1TObjects/interface/L1MuDTTFParameters.h"
#include "CondFormats/DataRecord/interface/L1MuDTTFParametersRcd.h"
#include "CondFormats/L1TObjects/interface/L1MuDTTFMasks.h"
#include "CondFormats/DataRecord/interface/L1MuDTTFMasksRcd.h"

#include <iostream> 
#include <string> 
#include <sstream> 

L1TMuonBarrelRPCKalmanStubProcessor::L1TMuonBarrelRPCKalmanStubProcessor():
  minBX_(-3),
  maxBX_(3)
{
 
} 



L1TMuonBarrelRPCKalmanStubProcessor::L1TMuonBarrelRPCKalmanStubProcessor(const edm::ParameterSet& iConfig):
  minBX_(iConfig.getParameter<int>("minBX")),
  maxBX_(iConfig.getParameter<int>("maxBX")),
  verbose_(iConfig.getParameter<int>("verbose"))
{

} 



L1TMuonBarrelRPCKalmanStubProcessor::~L1TMuonBarrelRPCKalmanStubProcessor() {}




L1MuKBMTCombinedStub 
L1TMuonBarrelRPCKalmanStubProcessor::buildStubNoEta(const L1MuDTChambPhDigi& phiS) {
  int wheel = phiS.whNum();
  int sector = phiS.scNum();
  int station = phiS.stNum();
  int phi = phiS.phi();
  int phiB = phiS.phiB();
  bool tag = (phiS.Ts2Tag()==1);
  int bx=phiS.bxNum();
  int quality=phiS.code();


  //Now full eta
  int qeta1=0;
  int qeta2=0;
  int eta1=7;
  int eta2=7; 
  L1MuKBMTCombinedStub stub(wheel,sector,station,phi,phiB,tag,
			    bx,quality,eta1,eta2,qeta1,qeta2,1);

  return stub;

}





L1MuKBMTCombinedStubCollection 
L1TMuonBarrelRPCKalmanStubProcessor::makeStubs(const L1MuDTChambPhContainer& phiContainer) {


  //get the masks from th standard BMTF setup!
  //    const L1TMuonBarrelParamsRcd& bmtfParamsRcd = setup.get<L1TMuonBarrelParamsRcd>();
  //  bmtfParamsRcd.get(bmtfParamsHandle);
  //  const L1TMuonBarrelParams& bmtfParams = *bmtfParamsHandle.product();
  //  masks_ =  bmtfParams.l1mudttfmasks;


  L1MuKBMTCombinedStubCollection  out;
  for (int bx=minBX_;bx<=maxBX_;bx++) {
    for (int wheel=-2;wheel<=2;wheel++) {
      for (uint sector=0;sector<12;sector++) {
	for (uint station=1;station<5;station++) {
	  L1MuDTChambPhDigi const* high = phiContainer.chPhiSegm1(wheel,station,sector,bx);
	  if (high) {
	      const L1MuDTChambPhDigi& stubPhi = *high;
		out.push_back(buildStubNoEta(stubPhi));
	  }
	  L1MuDTChambPhDigi const* low = phiContainer.chPhiSegm2(wheel,station,sector,bx-1);
	  if (low) {
	    const L1MuDTChambPhDigi& stubPhi = *low;
	    out.push_back(buildStubNoEta(stubPhi));
	  }
	}
      }
    }
  }

  return out;
}




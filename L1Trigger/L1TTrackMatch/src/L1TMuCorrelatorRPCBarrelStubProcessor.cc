#include "L1Trigger/L1TTrackMatch/interface/L1TMuCorrelatorRPCBarrelStubProcessor.h"
#include <cmath>
#include "CondFormats/L1TObjects/interface/L1MuDTTFParameters.h"
#include "CondFormats/DataRecord/interface/L1MuDTTFParametersRcd.h"
#include "CondFormats/L1TObjects/interface/L1MuDTTFMasks.h"
#include "CondFormats/DataRecord/interface/L1MuDTTFMasksRcd.h"

#include <iostream> 
#include <string> 
#include <sstream> 

L1TMuCorrelatorRPCBarrelStubProcessor::L1TMuCorrelatorRPCBarrelStubProcessor():
  minBX_(-3),
  maxBX_(3)
{
 
} 



L1TMuCorrelatorRPCBarrelStubProcessor::L1TMuCorrelatorRPCBarrelStubProcessor(const edm::ParameterSet& iConfig):
  minBX_(iConfig.getParameter<int>("minBX")),
  maxBX_(iConfig.getParameter<int>("maxBX")),
  verbose_(iConfig.getParameter<int>("verbose")),
  phiLSB_(iConfig.getParameter<double>("phiLSB"))

{

} 



L1TMuCorrelatorRPCBarrelStubProcessor::~L1TMuCorrelatorRPCBarrelStubProcessor() {}




L1MuCorrelatorHit
L1TMuCorrelatorRPCBarrelStubProcessor::buildStubNoEta(const L1MuDTChambPhDigi& phiS) {
  int wheel = phiS.whNum();
  int sector = phiS.scNum();
  int station = phiS.stNum();
  double globalPhi = (-180+sector*30)+phiS.phi()*30./2048.;
  if (globalPhi<-180)
    globalPhi+=360;
  if (globalPhi>180)
    globalPhi-=360;
  globalPhi = globalPhi*M_PI/180.;
  int phi = int(globalPhi/phiLSB_);
  int phiB = phiS.phiB();
  bool tag = (phiS.Ts2Tag()==1);
  int bx=phiS.bxNum();
  int quality=phiS.code();

  //Now full eta
  int qeta1=0;
  int qeta2=0;
  int eta1=7;
  int eta2=7; 
  L1MuCorrelatorHit stub(wheel,sector,station,phi,phiB,tag,
			    bx,quality,eta1,eta2,qeta1,qeta2,1);
  return stub;

}





L1MuCorrelatorHitCollection 
L1TMuCorrelatorRPCBarrelStubProcessor::makeStubs(const L1MuDTChambPhContainer& phiContainer) {


  //get the masks from th standard BMTF setup!
  //    const L1TMuonBarrelParamsRcd& bmtfParamsRcd = setup.get<L1TMuonBarrelParamsRcd>();
  //  bmtfParamsRcd.get(bmtfParamsHandle);
  //  const L1TMuonBarrelParams& bmtfParams = *bmtfParamsHandle.product();
  //  masks_ =  bmtfParams.l1mudttfmasks;


  L1MuCorrelatorHitCollection  out;
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




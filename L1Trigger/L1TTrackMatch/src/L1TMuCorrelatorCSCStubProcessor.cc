
#include "L1Trigger/L1TTrackMatch/interface/L1TMuCorrelatorCSCStubProcessor.h"
#include <cmath>
#include <iostream> 
#include <string> 
#include <sstream> 

L1TMuCorrelatorCSCStubProcessor::L1TMuCorrelatorCSCStubProcessor():
  minBX_(-3),
  maxBX_(3)
{
 
} 



L1TMuCorrelatorCSCStubProcessor::L1TMuCorrelatorCSCStubProcessor(const edm::ParameterSet& iConfig):
  minBX_(iConfig.getParameter<int>("minBX")),
  maxBX_(iConfig.getParameter<int>("maxBX")),
  halfStrips_(iConfig.getParameter<std::vector<int> >("halfStrips")),
  chambers_(iConfig.getParameter<std::vector<int> >("numberOfChambers")),
  wires_(iConfig.getParameter<std::vector<int> >("wireOffset")),
  etaLUT1_(iConfig.getParameter<std::vector<int> >("etaLUT1")),
  etaLUT2_(iConfig.getParameter<std::vector<int> >("etaLUT2")),
  etaLUT3_(iConfig.getParameter<std::vector<int> >("etaLUT3")),
  etaLUT4_(iConfig.getParameter<std::vector<int> >("etaLUT4"))
{

} 



L1TMuCorrelatorCSCStubProcessor::~L1TMuCorrelatorCSCStubProcessor() {}


uint
L1TMuCorrelatorCSCStubProcessor::getIndex(const CSCDetId& detid) {
  uint station = detid.station();
  uint ring = detid.ring();
  if (station==1) {
    if (ring==1)
      return 0;
    else if (ring==2)
      return 1;
    else if (ring==3)
      return 2;
  }
  else if (station==2) {
    if (ring==1)
      return 3;
    else if (ring==2)
      return 4;
  }
  else if (station==3) {
    if (ring==1)
      return 5;
    else if (ring==2)
      return 6;
  }
  else if (station==4) {
    if (ring==1)
      return 7;
    else if (ring==2)
      return 8;
  }
  return 0;
}


L1MuCorrelatorHit
L1TMuCorrelatorCSCStubProcessor::buildStub(const CSCDetId& detid,const CSCCLCTDigi& digi, const MuonDigiCollection<CSCDetId,CSCALCTDigi>& etadigis) {

  int endcap = detid.endcap();
  int station = detid.station();
  int chamber =detid.chamber();
  int ring = detid.ring();
  
  uint index = getIndex(detid);


  int numberOfChambers = chambers_[index];

  float globalPhi=0;
  float pitch = 360.0/numberOfChambers/(halfStrips_[index]-10);

  //ME1/3
  if(station==1 && ring==3)
    pitch = 360.0/numberOfChambers/(halfStrips_[index]);
  
  uint strip = digi.getKeyStrip();
  if (station==1 && ring==1) {
    if (strip>127) {//ME1/1a {
      strip=strip-127;
      pitch = 360.0/numberOfChambers/(halfStrips_[index]-10-127);
    }
    else {
      pitch = 360.0/numberOfChambers/(117);
    }

  }

  if ((endcap==1 && station >=3)||(endcap==2 && station<=2))
    globalPhi = -5.0+(chamber-1)*360.0/numberOfChambers+(halfStrips_[index]-strip)*pitch;
  if ((endcap==1 && station <=2)||(endcap==2 && station>=3))
    globalPhi = -5.0+(chamber-1)*360.0/numberOfChambers+(strip-1)*pitch;

  if (station ==1 && ring==1 && digi.getKeyStrip()<128)
    globalPhi = -5.0+(chamber-1)*360.0/numberOfChambers+(strip-1)*pitch;

  if (station ==1 && ring==1 && digi.getKeyStrip()>127)
    globalPhi = -5.0+(chamber-1)*360.0/numberOfChambers+(halfStrips_[index]-127-strip)*pitch;


  
  
    

  if (globalPhi<-180)
    globalPhi+=360;
  if (globalPhi>180)
    globalPhi-=360;

  int phi = int(globalPhi*2048/30.0);
  
   

  

  int type=2;
  int wheel=0;;

  
  int sign = endcap==1 ? +1 : -1;

  if (ring==3)
    wheel = sign*3;
  if (ring==2)
    wheel = sign*4;
  if (ring==1)
    wheel = sign*5;

  int sector = fabs(chamber);
  int phiB =0;
  bool tag = 0;
  int bx=digi.getBX()-6;
  int quality=0;



  int eta1=0;
  int eta2=0;
  int qeta1=0;
  int qeta2=0;



   auto etachamber = etadigis.begin();
   auto etachend  = etadigis.end();
   int found=0;
   for( ; etachamber != etachend; ++etachamber ) {
     auto etadigi = (*etachamber).second.first;
     auto etadend = (*etachamber).second.second;    
     for( ; etadigi != etadend; ++etadigi ) {
       if((*etachamber).first==detid) {
	 if (found==0) {
	   eta1 =wires_[index]+(*etadigi).getKeyWG(); 	 
	   eta2=eta1;
	   found=1;
	 }
	 if (found==1) {
	   eta2 =wires_[index]+(*etadigi).getKeyWG(); 	 
	   found=2;
	 }


       }
     }
   }

   if (found>0) {
     if (station==1) {
       eta1=sign*etaLUT1_[eta1];
       eta2=sign*etaLUT1_[eta2];
     }
     if (station==2) {
       eta1=sign*etaLUT2_[eta1];
       eta2=sign*etaLUT2_[eta2];
     }
     if (station==3) {
       eta1=sign*etaLUT3_[eta1];
       eta2=sign*etaLUT3_[eta2];
     }
     if (station==4) {
       eta1=sign*etaLUT4_[eta1];
       eta2=sign*etaLUT4_[eta2];
     }
    
   }


  //Now full eta
 if(bx==0 &&station==1)
   printf("etaRegion=%d phiRegion=%d depthRegion=%d phi=%d strip=%d\n",wheel,sector,station,phi,digi.getKeyStrip());

  L1MuCorrelatorHit stub(wheel,sector,station,phi,phiB,tag,
			    bx,quality,eta1,eta2,qeta1,qeta2,type); 
  return stub;

  }






L1MuCorrelatorHitCollection 
L1TMuCorrelatorCSCStubProcessor::makeStubs(const MuonDigiCollection<CSCDetId,CSCCLCTDigi>& digis,const MuonDigiCollection<CSCDetId,CSCALCTDigi>& etaDigis) {
  L1MuCorrelatorHitCollection out;
   auto chamber = digis.begin();
   auto chend  = digis.end();
   for( ; chamber != chend; ++chamber ) {
     auto digi = (*chamber).second.first;
     auto dend = (*chamber).second.second;    
     for( ; digi != dend; ++digi ) {
       L1MuCorrelatorHit stub = buildStub((*chamber).first,*digi,etaDigis);
       if (stub.bxNum()>=minBX_&&stub.bxNum()<=maxBX_)
	 out.push_back(stub);
     }
   }
  return out;
}


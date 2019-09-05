#include "L1Trigger/L1TMuonTPS/interface/L1TMuCorrelatorPhase2DTStubProcessor.h"
#include <cmath>
#include "CondFormats/L1TObjects/interface/L1MuDTTFParameters.h"
#include "CondFormats/DataRecord/interface/L1MuDTTFParametersRcd.h"
#include "CondFormats/L1TObjects/interface/L1MuDTTFMasks.h"
#include "CondFormats/DataRecord/interface/L1MuDTTFMasksRcd.h"

#include <iostream> 
#include <string> 
#include <sstream> 

L1TMuCorrelatorPhase2DTStubProcessor::L1TMuCorrelatorPhase2DTStubProcessor():
  minPhiQuality_(0),
  minBX_(-3),
  maxBX_(3)
{
 
} 



L1TMuCorrelatorPhase2DTStubProcessor::L1TMuCorrelatorPhase2DTStubProcessor(const edm::ParameterSet& iConfig):
  minPhiQuality_(iConfig.getParameter<int>("minPhiQuality")),
  minBX_(iConfig.getParameter<int>("minBX")),
  maxBX_(iConfig.getParameter<int>("maxBX")),
  eta1_(iConfig.getParameter<std::vector<int> >("eta_1")),
  eta2_(iConfig.getParameter<std::vector<int> >("eta_2")),
  eta3_(iConfig.getParameter<std::vector<int> >("eta_3")),
  coarseEta1_(iConfig.getParameter<std::vector<int> >("coarseEta_1")),
  coarseEta2_(iConfig.getParameter<std::vector<int> >("coarseEta_2")),
  coarseEta3_(iConfig.getParameter<std::vector<int> >("coarseEta_3")),
  coarseEta4_(iConfig.getParameter<std::vector<int> >("coarseEta_4")),
  verbose_(iConfig.getParameter<int>("verbose")),
  phiLSB_(iConfig.getParameter<double>("phiLSB")),
  bendingScale_(iConfig.getParameter<std::vector<double> >("bendingScale"))
{

} 



L1TMuCorrelatorPhase2DTStubProcessor::~L1TMuCorrelatorPhase2DTStubProcessor() {}



bool L1TMuCorrelatorPhase2DTStubProcessor::isGoodPhiStub(const L1Phase2MuDTPhDigi * stub) {
  if (stub->quality()<minPhiQuality_)
    return false;
  return true;
}




L1MuCorrelatorHit 
L1TMuCorrelatorPhase2DTStubProcessor::buildStub(const L1Phase2MuDTPhDigi& phiS,const L1MuDTChambThDigi* etaS,const L1MuCorrelatorHitCollection& stubs) {
  
  L1MuCorrelatorHit stub = buildStubNoEta(phiS,stubs);


  //Now full eta
  int qeta1=0;
  int qeta2=0;
  int eta1=255;
  int eta2=255; 


  bool hasEta=false;
  for (uint i=0;i<7;++i) {
    if (etaS->position(i)==0)
      continue;
    if (!hasEta) {
      hasEta=true;
      eta1=calculateEta(i,etaS->whNum(),etaS->scNum(),etaS->stNum());
      if (etaS->quality(i)==1)
	qeta1=2;
      else
	qeta1=1;
    }
    else {
      eta2=calculateEta(i,etaS->whNum(),etaS->scNum(),etaS->stNum());
      if (etaS->quality(i)==1)
	qeta2=2;
      else
	qeta2=1;
    }
  }

  int eta=eta1;
  int qeta=qeta1;

  if (qeta2>0) {//both stubs->average
    eta=(eta1+eta2)>>1;
    qeta=0;
    stub.setEta(eta,qeta);
  }
  else if (qeta1>0) {//Good single stub
    eta=eta1;
    qeta=qeta1;
    stub.setEta(eta,qeta);
  }
 
  return stub;

}





L1MuCorrelatorHit
L1TMuCorrelatorPhase2DTStubProcessor::buildStubNoEta(const L1Phase2MuDTPhDigi& phiS,const L1MuCorrelatorHitCollection& stubs) {
  int wheel = phiS.whNum();
  int abswheel = fabs(phiS.whNum());
  int sign  = wheel>0 ? 1: -1;
  int sector = phiS.scNum();
  int station = phiS.stNum();

  bool tag=0;
  for (const auto& s : stubs){
    if (s.etaRegion()==wheel && s.phiRegion()==sector && s.depthRegion() ==station) {
      tag=1;
      break;
    }
  }


  double globalPhi = (-180+sector*30)+phiS.phi()*45.8366/65536.;
  if (globalPhi<-180)
    globalPhi+=360;
  if (globalPhi>180)
    globalPhi-=360;
  globalPhi = globalPhi*M_PI/180.;
  int phi = int(globalPhi/phiLSB_);
  double normPhiB = bendingScale_[phiS.stNum()-1]*phiS.phiBend()*1.4/2048.;
  int phiB = int(normPhiB/phiLSB_);

  int bx=phiS.bxNum();
  int quality=phiS.quality();
  uint tfLayer=0;
  int eta=-255;
  if (station==1) {
    tfLayer=1; 
    eta=coarseEta1_[abswheel];
  }
  else if (station==2) {
    tfLayer=5;
    eta=coarseEta2_[abswheel];

  }
  else if (station==3) {
    tfLayer=8;
    eta=coarseEta3_[abswheel];

  }
  else if (station==4) {
    tfLayer=10;
    eta=coarseEta4_[abswheel];

  }
  else {
    tfLayer=0;
    eta=-255;
  }
  //Now full eta

  eta=eta*sign;
  L1MuCorrelatorHit stub(wheel,sector,station,tfLayer,phi,phiB,tag,
			 bx,quality,eta,0);
  return stub;

}





L1MuCorrelatorHitCollection 
L1TMuCorrelatorPhase2DTStubProcessor::makeStubs(const L1Phase2MuDTPhContainer* phiContainer,const L1MuDTChambThContainer* etaContainer,const L1TMuonBarrelParams& params) {

  L1MuCorrelatorHitCollection out;

  for (const auto & phiS : *phiContainer->getContainer()) {
    if (phiS.bxNum()<minBX_ || phiS.bxNum()>maxBX_)
      continue;

    if (phiS.quality()<minPhiQuality_)
      continue;

    //Check for eta

    L1MuDTChambThDigi const*  tseta   = etaContainer->chThetaSegm(phiS.whNum(),phiS.stNum(),phiS.scNum(),phiS.bxNum());
    if (tseta) {
      out.push_back(buildStub(phiS,tseta,out));
    }
    else {
      out.push_back(buildStubNoEta(phiS,out));
    }
  }
  return out;
}



int L1TMuCorrelatorPhase2DTStubProcessor::calculateEta(uint i, int wheel,uint sector,uint station) {
  int eta=0;
  if (wheel>0) {
	eta=7*wheel+3-i;
      }
  else if (wheel<0) {
	eta=7*wheel+i-3;
  }
  else {
    if (sector==0 || sector==3 ||sector==4 ||sector==7 ||sector==8 ||sector==11)
      eta=i-3;
    else
      eta=3-i;
  }


  if (station==1)
    eta=eta1_[eta+17];
  else if (station==2)
    eta=eta2_[eta+17];
  else 
    eta=eta3_[eta+17];

  return eta;



}




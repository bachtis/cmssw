#include "L1Trigger/L1TTrackMatch/interface/L1TTPSSectorProcessor.h"
#include "L1Trigger/L1TTrackMatch/interface/L1TPSLUTs.h"

L1TTPSSectorProcessor::L1TTPSSectorProcessor(const edm::ParameterSet& iConfig): 
  sectorNumber_(iConfig.getParameter<unsigned int>("sectorNumber")),
  barrelSectors_(iConfig.getParameter<std::vector<unsigned int> >("barrelSectors")),
  csc10DegreeChambers_(iConfig.getParameter<std::vector<unsigned int> >("csc10DegreeChambers")),
  csc20DegreeChambers_(iConfig.getParameter<std::vector<unsigned int> >("csc20DegreeChambers")),
  rpcEndcapChambers_(iConfig.getParameter<std::vector<unsigned int> >("rpcEndcapChambers")),
  trackPhiLowerBound_(iConfig.getParameter<int>("phiLowerBound")),
  trackPhiUpperBound_(iConfig.getParameter<int>("phiUpperBound")),
  phiOffset_(iConfig.getParameter<int>("phiOffset"))
{
  //For a more readable python cfg file
  const edm::ParameterSet& algoSettings = iConfig.getParameter<edm::ParameterSet>("algoSettings");
  verbose_          = algoSettings.getParameter<int>("verbose");
  phiLSB_           = algoSettings.getParameter<double>("phiLSB");
  trackEtaLSB_      = algoSettings.getParameter<double>("etaLSB");
  trackEtaShift_    = algoSettings.getParameter<unsigned int>("etaShift");
  trackCurvLSB_     = algoSettings.getParameter<double>("curvLSB");
  matchType_        = algoSettings.getParameter<std::vector<unsigned int> >("matchType");
  matchEtaRegion_   = algoSettings.getParameter<std::vector<unsigned int> >("matchEtaRegion");
  matchDepthRegion_ = algoSettings.getParameter<std::vector<unsigned int> >("matchDepthRegion");
}

L1TTPSSectorProcessor::~L1TTPSSectorProcessor() {}

std::vector<l1t::L1TkMuonParticle> L1TTPSSectorProcessor::process(const TrackPtrVector& tracks,const L1MuCorrelatorHitRefVector& stubsAll) {
  //Output collection
  std::vector<l1t::L1TkMuonParticle> out;

  //First filter the stubs:
  L1MuCorrelatorHitRefVector stubsInSector;
  for (const auto& stub : stubsAll) {
    if (stub->type()<=1) {//barrel 
      for (const auto& sector : barrelSectors_) {
	if (stub->phiRegion()==int(sector)) {
	  stubsInSector.push_back(stub);
	  break;
	}
      }
    }
    else if (stub->type()==2 && (stub->depthRegion()==1 || (stub->depthRegion()>1 && fabs(stub->etaRegion())==5))) {//CSC 
      for (const auto& sector : csc10DegreeChambers_) {
	if (stub->phiRegion()==int(sector)) {
	  stubsInSector.push_back(stub);
	  break;
	}
      }
   }
    else if (stub->type()==2 && (stub->depthRegion()>1 && fabs(stub->etaRegion())==5)) {//CSC 
      for (const auto& sector : csc20DegreeChambers_) {
	if (stub->phiRegion()==int(sector)) {
	  stubsInSector.push_back(stub);
	  break;
	}
      }
    }
    else if (stub->type()==3) {//RPC
      for (const auto& sector : rpcEndcapChambers_) {
	if (stub->phiRegion()==int(sector)) {
	  stubsInSector.push_back(stub);
	  break;
	}
      }
    }
  }

  //Now loop on the tracks
  for (const auto& track: tracks) {
    l1t::L1TkMuonParticle::LorentzVector vec(track->getMomentum().x(),
					track->getMomentum().y(),
					track->getMomentum().z(),
					track->getMomentum().mag());
    l1t::L1TkMuonParticle muon (vec,track);   
    //Set muon charge
    int charge=1;
    if (track->getRInv()<0)
      charge=-1;
    muon.setCharge(charge);

    //If not in nonant kill
    int globalPhi = muon.phi()/phiLSB_;
    if (deltaPhi(globalPhi,trackPhiLowerBound_)<0 || deltaPhi(globalPhi,trackPhiUpperBound_)>0)
      continue;
    processTrack(muon,stubsInSector);
    if (muon.getMatchedStubs().size()>0)
      out.push_back(muon);
  }
  return out;
}

int L1TTPSSectorProcessor::deltaPhi(int phi1, int phi2) {
  //add offset code here 
  int delta = phi1-phi2;
  int pi = int(3.14159/phiLSB_);
  if (delta>pi)
    return delta-2*pi;
  if (delta<-pi)
    return delta+2*pi;
  return delta;
}


int L1TTPSSectorProcessor::stubPhi(const L1MuCorrelatorHitRef& stub ) {
  //add offset code here 
  return deltaPhi(stub->phi(),phiOffset_);
}

int L1TTPSSectorProcessor::trackPhi(const l1t::L1TkMuonParticle& track) {
  //add offset code here 
  return deltaPhi(int(track.phi()/phiLSB_),phiOffset_);
}

int L1TTPSSectorProcessor::trackEta(const l1t::L1TkMuonParticle& track) {
  //add offset code here 
  return int(track.eta()/trackEtaLSB_);
}

int L1TTPSSectorProcessor::trackCurv(const l1t::L1TkMuonParticle& track) {
  //add offset code here 
  return int(track.getTrkPtr()->getRInv()/trackCurvLSB_);
}



L1TTPSSectorProcessor::PropagationInfo L1TTPSSectorProcessor::propagate(const l1t::L1TkMuonParticle& track,uint propIndex) {
  L1TTPSSectorProcessor::PropagationInfo out;
  //first calculate the index
  int eta = trackEta(track);
  uint absEta = uint(fabs(eta))>>trackEtaShift_;
  uint etaIndex = L1TTPS::etaIndex[absEta];
  double slope,eLoss,resa,resb;
  uint propVar;
  bool valid;

  switch(propIndex) {
  case 0:
    slope=L1TTPS::slope_0[absEta];
    eLoss=L1TTPS::eLoss_0[absEta];
    resa =L1TTPS::resa_0[absEta];
    resb =L1TTPS::resb_0[absEta];
    propVar = L1TTPS::var_0[etaIndex];
    valid = L1TTPS::valid_0[etaIndex];
    break;
  case 1:
    slope=L1TTPS::slope_1[absEta];
    eLoss=L1TTPS::eLoss_1[absEta];
    resa =L1TTPS::resa_1[absEta];
    resb =L1TTPS::resb_1[absEta];
    propVar = L1TTPS::var_1[etaIndex];
    valid = L1TTPS::valid_1[etaIndex];
    break;
  case 2:
    slope=L1TTPS::slope_2[absEta];
    eLoss=L1TTPS::eLoss_2[absEta];
    resa =L1TTPS::resa_2[absEta];
    resb =L1TTPS::resb_2[absEta];
    propVar = L1TTPS::var_2[etaIndex];
    valid = L1TTPS::valid_2[etaIndex];
    break;
  case 3:
    slope=L1TTPS::slope_3[absEta];
    eLoss=L1TTPS::eLoss_3[absEta];
    resa =L1TTPS::resa_3[absEta];
    resb =L1TTPS::resb_3[absEta];
    propVar = L1TTPS::var_3[etaIndex];
    valid = L1TTPS::valid_3[etaIndex];
    break;
  case 4:
    slope=L1TTPS::slope_4[absEta];
    eLoss=L1TTPS::eLoss_4[absEta];
    resa =L1TTPS::resa_4[absEta];
    resb =L1TTPS::resb_4[absEta];
    propVar = L1TTPS::var_4[etaIndex];
    valid = L1TTPS::valid_4[etaIndex];
    break;
  case 5:
    slope=L1TTPS::slope_5[absEta];
    eLoss=L1TTPS::eLoss_5[absEta];
    resa =L1TTPS::resa_5[absEta];
    resb =L1TTPS::resb_5[absEta];
    propVar = L1TTPS::var_5[etaIndex];
    valid = L1TTPS::valid_5[etaIndex];
    break;
  case 6:
    slope=L1TTPS::slope_6[absEta];
    eLoss=L1TTPS::eLoss_6[absEta];
    resa =L1TTPS::resa_6[absEta];
    resb =L1TTPS::resb_6[absEta];
    propVar = L1TTPS::var_6[etaIndex];
    valid = L1TTPS::valid_6[etaIndex];
    break;
  case 7:
    slope=L1TTPS::slope_7[absEta];
    eLoss=L1TTPS::eLoss_7[absEta];
    resa =L1TTPS::resa_7[absEta];
    resb =L1TTPS::resb_7[absEta];
    propVar = L1TTPS::var_7[etaIndex];
    valid = L1TTPS::valid_7[etaIndex];
    break;
  case 8:
    slope=L1TTPS::slope_8[absEta];
    eLoss=L1TTPS::eLoss_8[absEta];
    resa =L1TTPS::resa_8[absEta];
    resb =L1TTPS::resb_8[absEta];
    propVar = L1TTPS::var_8[etaIndex];
    valid = L1TTPS::valid_8[etaIndex];
    break;
  case 9:
    slope=L1TTPS::slope_9[absEta];
    eLoss=L1TTPS::eLoss_9[absEta];
    resa =L1TTPS::resa_9[absEta];
    resb =L1TTPS::resb_9[absEta];
    propVar = L1TTPS::var_9[etaIndex];
    valid = L1TTPS::valid_9[etaIndex];
    break;
  case 10:
    slope=L1TTPS::slope_10[absEta];
    eLoss=L1TTPS::eLoss_10[absEta];
    resa =L1TTPS::resa_10[absEta];
    resb =L1TTPS::resb_10[absEta];
    propVar = L1TTPS::var_10[etaIndex];
    valid = L1TTPS::valid_10[etaIndex];
    break;
  case 11:
    slope=L1TTPS::slope_11[absEta];
    eLoss=L1TTPS::eLoss_11[absEta];
    resa =L1TTPS::resa_11[absEta];
    resb =L1TTPS::resb_11[absEta];
    propVar = L1TTPS::var_11[etaIndex];
    valid = L1TTPS::valid_11[etaIndex];
    break;
  default:
    slope=0;
    eLoss=0;
    resa =0;
    resb =0;
    propVar = 0;
    valid = 0;
  }

  
  int phi = trackPhi(track);
  int curv = trackCurv(track);
  if (propVar==1)
    phi=0;
  out.propagatedValue = int(phi+slope*curv/(1+eLoss*fabs(curv)));
  out.propagatedPhi = int(phi+slope*curv/(1+eLoss*fabs(curv)));
  out.propagatorIndex = propIndex;
  out.propagatedSigma = uint(resa+resb*abs(curv));
  out.etaIndex=etaIndex;
  out.valid = valid;
  out.propagationVar = propVar;

  if (verbose_==1) {
    printf("Propagating index=%d phi=%d curvature=%d value=%d sigma=%d eta index=%d valid=%d var=%d\n",propIndex,phi,curv,out.propagatedValue,out.propagatedSigma,out.etaIndex,out.valid,out.propagationVar);
  }

  return out;
}


void L1TTPSSectorProcessor::match(l1t::L1TkMuonParticle& muon,const L1TTPSSectorProcessor::PropagationInfo& prop,const L1MuCorrelatorHitRefVector& stubs) {
  if (!prop.valid)
    return;
  //retrieve eta information for matching
  uint index = prop.propagatorIndex*16+prop.etaIndex;
  int sign= trackEta(muon)>=0 ? 1 : -1;
  int etaRegion = sign*matchEtaRegion_[index];
  uint depthRegion = matchDepthRegion_[index];
  uint type = matchType_[index];
  int bestStub=-1;
  uint maxDPhi=10000;

  for (uint i=0;i<stubs.size();++i) {
    if ((stubs[i]->etaRegion()!=int(etaRegion))|| (stubs[i]->depthRegion()!=int(depthRegion)) || (stubs[i]->type()!=int(type)))
      continue;

    uint deltaPhi=fabs(prop.propagatedPhi-stubPhi(stubs[i]));
    if (deltaPhi<maxDPhi){
      maxDPhi=deltaPhi;
      bestStub=i;
    } 
  }
  if (bestStub!=-1) {
    uint delta=100000;
    if (prop.propagationVar==0)
      delta=maxDPhi;
    else
      delta=fabs(stubs[bestStub]->phiB()-prop.propagatedValue);

    if (delta<=prop.propagatedSigma) {
      muon.addStub(stubs[bestStub]);
    }
  }

}



void L1TTPSSectorProcessor::processTrack(l1t::L1TkMuonParticle& muon,const L1MuCorrelatorHitRefVector& stubs) {
  //Propagate 12 times maximum and match
  for (uint i=0;i<12;++i) {
    //propagate
    L1TTPSSectorProcessor::PropagationInfo prop = propagate(muon,i);
    //fix (not needed in hardware: if we have phiB propagate the previous one to get phi
    if (prop.propagationVar==1) {
      L1TTPSSectorProcessor::PropagationInfo propTmp = propagate(muon,i-1);
      prop.propagatedPhi = propTmp.propagatedPhi;
    }
    //Now match
    match(muon,prop,stubs);    
  }
  //estimate the quality- for now just number of matches
  muon.setQuality(muon.getMatchedStubs().size());
}





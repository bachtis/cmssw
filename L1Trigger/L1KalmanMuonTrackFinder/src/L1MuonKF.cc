#include <math.h>
#include "L1Trigger/L1KalmanMuonTrackFinder/interface/L1MuonKF.h"


L1MuonKF::L1MuonKF(const edm::ParameterSet& settings):
  eLoss_(settings.getParameter<std::vector<int> >("eLoss")),
  aPhi_(settings.getParameter<std::vector<int> >("aPhi")),
  bPhi_(settings.getParameter<std::vector<int> >("bPhi")),
  aPhiB_(settings.getParameter<std::vector<int> >("aPhiB")),
  bPhiB_(settings.getParameter<std::vector<int> >("aPhiB")),
  denominator_(settings.getParameter<int>("denominator")),
  useOfflineAlgo_(settings.getParameter<bool>("useOfflineAlgo")),
  mScatteringPhi_(settings.getParameter<std::vector<int> >("mScatteringPhi")),
  mScatteringPhiB_(settings.getParameter<std::vector<int> >("mScatteringPhiB")),
  pointResolutionPhi_(settings.getParameter<int>("pointResolutionPhi")),
  pointResolutionPhiB_(settings.getParameter<int>("pointResolutionPhiB")),
  pointResolutionVertex_(settings.getParameter<int>("pointResolutionVertex"))
{



}


L1MuonKF::~L1MuonKF() {

}

int L1MuonKF::correctedPhi(const StubRef& stub,int sector) {
  if (stub->scNum()==sector)
    return stub->phi();
  else if ((stub->scNum()==sector-1) || (stub->scNum()==11 && sector==0))
    return stub->phi()-2144;
  else if ((stub->scNum()==sector+1) || (stub->scNum()==0 && sector==11))
    return stub->phi()+2144;
  return 0;
} 


int L1MuonKF::phiBitmask(const L1KalmanMuTrack& track)  {
  unsigned int mask = 0;
  for (const auto& stub : track.stubs()) {
    mask = mask+round(pow(2,stub->stNum()-1));
  }
  return mask;
}

int L1MuonKF::etaBitmask(const L1KalmanMuTrack& track)  {
  int bitmask = 0;
  for (const auto& stub : track.stubs()) {
    if (stub->stNum()==1)
      bitmask+=pow(2,abs(stub->whNum()));
    if (stub->stNum()==2)
      bitmask+=pow(2,3+abs(stub->whNum()));
    if (stub->stNum()==3)
      bitmask+=pow(2,6+abs(stub->whNum()));
    if (stub->stNum()==4)
      bitmask+=pow(2,9+abs(stub->whNum()));
  }
  return bitmask;
}


int etaLookup(const L1KalmanMuTrack& track) {
  return 0;
}


int L1MuonKF::customBitmask(bool bit1,bool bit2,bool bit3,bool bit4)  {
  return bit1*1+bit2*2+bit3*4*bit4*8;
}

bool L1MuonKF::getBit(int bitmask,int pos)  {
  return (bitmask & ( 1 << pos )) >> pos;
}



void L1MuonKF::propagate(L1KalmanMuTrack& track) {
  int K = track.curvature();
  int phi = track.positionAngle();
  int phiB = track.bendingAngle();
  unsigned int step = track.step();

  int charge=1;
  if (K!=0) 
    charge = K/abs(K);


  //energy loss
  int KNew = K+charge*eLoss_[step-1]*K*K/denominator_;
  //phi propagation
  int phiNew = phi+aPhi_[step-1]*K/denominator_-bPhi_[step-1]*phiB/denominator_;
  //phiB propagation
  int phiBNew = +aPhiB_[step-1]*K/denominator_ +bPhiB_[step-1]*phiB/denominator_;
  track.setCoordinates(step-1,KNew,phiNew,phiBNew);

  ///////////////////////////////////////////////////////
  //Rest of the stuff  is for the offline version only 
  //where we want to check what is happening in the covariaznce matrix 


  //Create the transformation matrix
  double a[9];
  a[0] = 1;
  a[1] = 0;
  a[2] = 0;
  a[3] = aPhi_[step-1];
  a[4] = 1;
  a[5] = bPhi_[step-1];
  a[6] = aPhiB_[step-1];
  a[7] = 0;
  a[8] = bPhiB_[step-1];


  ROOT::Math::SMatrix<double,3> P(a,9);
  
  track.covariance = ROOT::Math::Similarity(P,track.covariance);

  //Add the multiple scattering
  int phiRMS = mScatteringPhi_[step-1]*K*K;
  int phiBRMS = mScatteringPhi_[step-1]*K*K;
  int crossTerm=0.5*sqrt(phiRMS*phiBRMS);

  std::vector<double> b(6);
  b[0] = 0;
  b[1] = 0;
  b[2] =phiRMS;
  b[3] =0;
  b[4] = crossTerm;
  b[5] = phiBRMS;

  reco::Candidate::CovarianceMatrix MS(b.begin(),b.end());
  track.covariance = track.covariance+MS;
}

bool L1MuonKF::update(L1KalmanMuTrack& track,const StubRefVector& stubs,int sector) {
  if (useOfflineAlgo_)
    return updateOffline(track,stubs,sector);
  else
    return false;
}

bool L1MuonKF::updateOffline(L1KalmanMuTrack& track,const StubRefVector& stubs,int sector) {
    int trackK = track.curvature();
    int trackPhi = track.positionAngle();
    int trackPhiB = track.bendingAngle();


    //Find the nearest stub:

    int bestStub=-1;
    int distance = 100000;

    for( unsigned int i=0;i<stubs.size();++i) {
      int phi = correctedPhi(stubs[i],sector);
      int d = abs(phi-trackPhi);
      if (d<distance) {
        distance = d;
        bestStub=i;
      }

    }

    if (bestStub<0)
      return false;
    
    StubRef stub = stubs[bestStub];
    int phi  = correctedPhi(stub,sector);
    int phiB = stub->phiB();


    Vector2 residual;
    residual[0] = trackPhi-phi;
    residual[1] = trackPhiB-phiB;
    
    Matrix23 H;
    H(0,0)=0.0;
    H(0,1)=1.0;
    H(0,2)=0.0;
    H(1,0)=0.0;
    H(1,1)=0.0;
    H(1,2)=1.0;

    
    CovarianceMatrix2 R;
    R(0,0) = pointResolutionPhi_;
    R(0,1) = 0.0;
    R(1,0) = 0.0;
    R(1,1) = pointResolutionPhiB_;
    CovarianceMatrix2 S = ROOT::Math::Similarity(H,track.covariance)+R;
    if (!S.Invert())
      return false;
    
    Matrix32 Gain = track.covariance*ROOT::Math::Transpose(H)*S;
   
    int KNew  = round(trackK+Gain(0,0)*residual(0)+Gain(0,1)*residual(1));
    int phiNew  = round(trackPhi+Gain(1,0)*residual(0)+Gain(1,1)*residual(1));
    int phiBNew  = round(trackPhiB+Gain(2,0)*residual(0)+Gain(2,1)*residual(1));

    track.setCoordinates(track.step(),KNew,phiNew,phiBNew);
    track.covariance = track.covariance - Gain*(H*track.covariance);
    track.addStub(stub);
    return true;
}

void L1MuonKF::vertexConstraint(L1KalmanMuTrack& track) {
  if (useOfflineAlgo_)
    vertexConstraintOffline(track);
}


void L1MuonKF::vertexConstraintOffline(L1KalmanMuTrack& track) {
  double residual = -track.dxy();
  Matrix13 H;
  H(0,0)=0;
  H(0,1)=0;
  H(0,2)=1;
  
  double S = ROOT::Math::Similarity(H,track.covariance)(0,0)+pointResolutionVertex_;
  S=1.0/S;
  
  Matrix31 Gain = track.covariance*(ROOT::Math::Transpose(H))*S;
  
  int KNew = round(track.curvature()+Gain(0,1)*residual);
  int phiNew = round(track.positionAngle()+Gain(1,1)*residual);
  track.setCoordinatesAtVertex(KNew,phiNew,track.dxy());
  track.covariance = track.covariance - Gain*H*track.covariance;
}



L1MuonKF::TrackVector L1MuonKF::process(const StubRef& seed, const StubRefVector& stubs) {
  TrackVector output;
  std::vector<int> combinatorics;
  switch(seed->stNum()) {
  case 4:
    combinatorics.push_back(customBitmask(0,0,1,1));
    combinatorics.push_back(customBitmask(0,1,0,1));
    combinatorics.push_back(customBitmask(1,0,0,1));
    combinatorics.push_back(customBitmask(0,1,1,1));
    combinatorics.push_back(customBitmask(1,0,1,1));
    combinatorics.push_back(customBitmask(1,1,0,1));
    combinatorics.push_back(customBitmask(1,1,1,1));
  case 3:
    combinatorics.push_back(customBitmask(0,1,1,0));
    combinatorics.push_back(customBitmask(1,0,1,0));
    combinatorics.push_back(customBitmask(1,1,1,0));
  case 2:
    combinatorics.push_back(customBitmask(1,1,0,0));    
  default:
    return output;
  }

  for( const auto& mask : combinatorics) {
    L1KalmanMuTrack track(seed);
    while(track.step()>0) {
      if (track.step()==1) {
	track.setHitPattern(phiBitmask(track));
	track.setCoarseEta(etaLookup(track),etaBitmask(track));
	setFloatingPointValues(track,false);
      }
      propagate(track);
      if (track.step()>0)
	if (getBit(mask,track.step()-1))
	  if (!update(track,stubs,seed->scNum()))
	    break;
      if (track.step()==0) {
	track.setCoordinatesAtVertex(track.curvature(),track.positionAngle(),track.bendingAngle());
	vertexConstraint(track);
	track.setCoordinatesAtVertex(track.curvature(),track.positionAngle(),track.dxy());
	setFloatingPointValues(track,true);
	output.push_back(track);
      }
    }
  }
  return output;
}       
    





#include "DataFormats/L1KalmanMuonTrigger/interface/L1KalmanMuTrack.h"

L1KalmanMuTrack::L1KalmanMuTrack() {

}

L1KalmanMuTrack::~L1KalmanMuTrack() {

}

L1KalmanMuTrack::L1KalmanMuTrack(const L1KalmanMuTrack::StubRef& stub):
  curv_(0),
  phi_(stub->phi()),
  phiB_(stub->phiB()),
  coarseEta_(0),
  approxChi2_(0),
  hitPattern_(0),
  etaCoarsePattern_(0),
  step_(stub->stNum())
{
  
  stubs_.push_back(stub);
}



int L1KalmanMuTrack::curvatureAtVertex() const {
  return curvVertex_;
}

int L1KalmanMuTrack::phiAtVertex() const {
  return phiVertex_;
}
int L1KalmanMuTrack::dxy() const{
   return dxy_;
}

int L1KalmanMuTrack::curvature() const {
    return curv_;
}

int L1KalmanMuTrack::positionAngle() const {
  return phi_;
}

int L1KalmanMuTrack::bendingAngle() const{
   return phiB_;
}

int L1KalmanMuTrack::coarseEta() const{
   return coarseEta_;
}

int L1KalmanMuTrack::approxChi2() const{
   return approxChi2_;
}

int L1KalmanMuTrack::phiPattern() const{
   return hitPattern_;
}

int L1KalmanMuTrack::etaCoarsePattern() const{
   return etaCoarsePattern_;
}

int L1KalmanMuTrack::step() const{
   return step_;
} 

const  L1KalmanMuTrack::PolarLorentzVector& L1KalmanMuTrack::unconstrainedP4() const{
   return unconstrainedP4_;
}

const L1KalmanMuTrack::StubRefVector& L1KalmanMuTrack::stubs() const {
  return stubs_;
}




void L1KalmanMuTrack::setCoordinates(int step,int curv,int phi,int phiB) {
  step_=step;
  curv_ = curv;
  phiB_=phiB;
  phi_=phi;

}

void L1KalmanMuTrack::setCoordinatesAtVertex(int curv,int phi,int dxy) {
  curvVertex_ = curv;
  phiVertex_  = phi;
  dxy_ = dxy;
}

void L1KalmanMuTrack::setCoarseEta(int eta,int pattern) {
  coarseEta_ = eta;
  etaCoarsePattern_ = pattern;
}

void L1KalmanMuTrack::setHitPattern(int pattern) {
  hitPattern_ = pattern;
}

void L1KalmanMuTrack::setApproxChi2(int chi) {
  approxChi2_ = chi;
}

void L1KalmanMuTrack::setPtEtaPhi(double pt ,double eta, double phi,bool atVertex) {
  if (atVertex) {
    PolarLorentzVector v(pt,eta,phi,0.105);
    setP4(v);
  }
  else {
    unconstrainedP4_.SetPt(pt);
    unconstrainedP4_.SetEta(eta);
    unconstrainedP4_.SetPhi(phi);
    unconstrainedP4_.SetM(0.105);

  }
}


void L1KalmanMuTrack::addStub(const L1KalmanMuTrack::StubRef& stub) {
  stubs_.push_back(stub);
}


void L1KalmanMuTrack::setKalmanGain(unsigned int step,float a1, float a2,float a3,float a4,float a5,float a6) {
  switch(step) {
  case 3: 
    kalmanGain3_.push_back(a1);
    kalmanGain3_.push_back(a2);
    kalmanGain3_.push_back(a3);
    kalmanGain3_.push_back(a4);
    kalmanGain3_.push_back(a5);
    kalmanGain3_.push_back(a6);
    break;
  case 2: 
    kalmanGain2_.push_back(a1);
    kalmanGain2_.push_back(a2);
    kalmanGain2_.push_back(a3);
    kalmanGain2_.push_back(a4);
    kalmanGain2_.push_back(a5);
    kalmanGain2_.push_back(a6);
    break;
  case 1: 
    kalmanGain1_.push_back(a1);
    kalmanGain1_.push_back(a2);
    kalmanGain1_.push_back(a3);
    kalmanGain1_.push_back(a4);
    kalmanGain1_.push_back(a5);
    kalmanGain1_.push_back(a6);
    break;
  case 0:
    kalmanGain0_.push_back(a1);
    kalmanGain0_.push_back(a2);
    kalmanGain0_.push_back(a3);
    break;

 default:
   printf("Critical ERROR on setting the Klamn gain\n"); 
  }
}


std::vector<float> L1KalmanMuTrack::kalmanGain(unsigned int step) {
  switch(step) {
  case 3: 
    return kalmanGain3_;
  case 2: 
    return kalmanGain2_;
  case 1: 
    return kalmanGain1_;
  case 0:
    return kalmanGain0_;
  default:
   return std::vector<float>();
  }
}

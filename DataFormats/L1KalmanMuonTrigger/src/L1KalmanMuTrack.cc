#include "DataFormats/L1KalmanMuonTrigger/interface/L1KalmanMuTrack.h"

L1KalmanMuTrack::L1KalmanMuTrack(): 
  reco::LeafCandidate(-1,reco::LeafCandidate::PolarLorentzVector(0.1,0.0,0.0,0.105))
{
}

L1KalmanMuTrack::~L1KalmanMuTrack() {

}

L1KalmanMuTrack::L1KalmanMuTrack(const L1KalmanMuTrack::StubRef& stub):
  reco::LeafCandidate(-1,reco::LeafCandidate::PolarLorentzVector(0.1,0.0,0.0,0.105)),
  covariance_(std::vector<double>(6,0.0)),
  curv_(0),
  phi_(stub->phi()),
  phiB_(8*stub->phiB()),
  coarseEta_(0),
  approxChi2_(0),
  hitPattern_(0),
  etaCoarsePattern_(0),
  step_(stub->stNum()),
  sector_(stub->scNum()),
  quality_(stub->code()),
  deltaK_(0)
{


  switch(stub->stNum()) {
  case 1:
    curv_ = -int(round(0.47*phiB_));
    break;
  case 2:
    curv_ = -int(round(1.69*phiB_));
    break;
  case 3:
    curv_ = -int(round(2.45*phiB_));
    break;
  case 4:
    curv_ = -int(round(3.56*phiB_));
    break;
  default:
    curv_ = 0;
    break;
    
  }


  stubs_.push_back(stub);
}



int L1KalmanMuTrack::curvatureAtVertex() const {
  return curvVertex_;
}
int L1KalmanMuTrack::curvatureAtMuon() const {
  return curvMuon_;
}

int L1KalmanMuTrack::phiAtVertex() const {
  return phiVertex_;
}
int L1KalmanMuTrack::phiAtMuon() const {
  return phiMuon_;
}
int L1KalmanMuTrack::phiBAtMuon() const {
  return phiBMuon_;
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
int L1KalmanMuTrack::sector() const{
   return sector_;
} 
int L1KalmanMuTrack::quality() const{
   return quality_;
} 
uint L1KalmanMuTrack::deltaK() const{
   return deltaK_;
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

void L1KalmanMuTrack::setCoordinatesAtMuon(int curv,int phi,int phiB) {
  curvMuon_ = curv;
  phiMuon_  = phi;
  phiBMuon_ = phiB;
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
  if (stub->code()<quality_)
    quality_ = stub->code();
}


void L1KalmanMuTrack::setKalmanGain(unsigned int step,unsigned int K,float a1, float a2,float a3,float a4,float a5,float a6) {
  switch(step) {
  case 3: 
    kalmanGain3_.push_back(K);
    kalmanGain3_.push_back(a1);
    kalmanGain3_.push_back(a2);
    kalmanGain3_.push_back(a3);
    kalmanGain3_.push_back(a4);
    kalmanGain3_.push_back(a5);
    kalmanGain3_.push_back(a6);
    break;
  case 2: 
    kalmanGain2_.push_back(K);
    kalmanGain2_.push_back(a1);
    kalmanGain2_.push_back(a2);
    kalmanGain2_.push_back(a3);
    kalmanGain2_.push_back(a4);
    kalmanGain2_.push_back(a5);
    kalmanGain2_.push_back(a6);
    break;
  case 1: 
    kalmanGain1_.push_back(K);
    kalmanGain1_.push_back(a1);
    kalmanGain1_.push_back(a2);
    kalmanGain1_.push_back(a3);
    kalmanGain1_.push_back(a4);
    kalmanGain1_.push_back(a5);
    kalmanGain1_.push_back(a6);
    break;
  case 0:
    kalmanGain0_.push_back(K);
    kalmanGain0_.push_back(a1);
    kalmanGain0_.push_back(a2);
    kalmanGain0_.push_back(a3);
    break;

 default:
   printf("Critical ERROR on setting the Klamn gain\n"); 
  }
}


const std::vector<float>&  L1KalmanMuTrack::kalmanGain(unsigned int step) const{
  switch(step) {
  case 3: 
    return kalmanGain3_;
  case 2: 
    return kalmanGain2_;
  case 1: 
    return kalmanGain1_;
  case 0:
    return kalmanGain0_;
  }
  return kalmanGain0_;
}

const std::vector<double>& L1KalmanMuTrack::covariance() const {
  return covariance_;
}



bool L1KalmanMuTrack::overlap( const L1KalmanMuTrack& other) const{
 
  for (const auto& s1 : stubs_) {
    for (const auto& s2 : other.stubs()) {
      if (s1->scNum()== s2->scNum() && 
	  s1->whNum()==s2->whNum() &&
	  s1->stNum()==s2->stNum() &&
	  s1->Ts2Tag()==s2->Ts2Tag())
	return true;
    }
  }
  return false;
}



void L1KalmanMuTrack::setCovariance(const CovarianceMatrix& c) {
  covariance_[0] = c(0,0);
  covariance_[1] = c(0,1);
  covariance_[2] = c(1,1);
  covariance_[3] = c(0,2);
  covariance_[4] = c(1,2);
  covariance_[5] = c(2,2);
}



void L1KalmanMuTrack::setDeltaK(uint deltaK) {
  deltaK_ = deltaK;
}

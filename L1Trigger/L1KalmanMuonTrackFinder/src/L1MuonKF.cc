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
  pointResolutionPhiB_(settings.getParameter<int>("pointResolutionPhiB"))
{



}


L1MuonKF::~L1MuonKF() {

}

int correctedPhi(const StubRef& stub,int sector) {
  if (stub->scNum()==sector)
    return stub->phi();
  else if ((stub->scNum()==sector-1) || (stub->scNum()==11 && sector==0))
    return stub->phi()-2144;
  else if ((stub->scNum()==sector+1) || (stub->scNum()==0 && sector==11))
    return stub->phi()+2144;
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
  int KNew = K+charge*eLoss_[step]*K*K/denominator_;
  //phi propagation
  int phiNew = phi+aPhi_[step]*K/denominator_ -bPhi_[step]*phiB/denominator;
  //phiB propagation
  int phiBNew = +aPhiB_[step]*K/denominator_ +bPhiB_[step]*phiB/denominator;
  track.setCoordinates(step-1,KNew,phiNew,phiBNew);

  ///////////////////////////////////////////////////////
  //Rest of the stuff  is for the offline version only 
  //where we want to check what is happening in the covariaznce matrix 


  //Create the transformation matrix
  double a[9];
  a[0] = 1;
  a[1] = 0;
  a[2] = 0;
  a[3] = aPhi_[step];
  a[4] = 1;
  a[5] = bPhi_[step];
  a[6] = aPhiB_[step];
  a[7] = 0;
  a[8] = bPhiB_[step];


  ROOT::Math::SMatrix<double,3> P(a,9);
  
  track.covariance = ROOT::Math::Similarity(P,track.covariance);

  //Add the multiple scattering
  int phiRMS = mScatteringPhi_[step]*K*K;
  int phiBRMS = mScatteringPhi_[step]*K*K;
  int crossTerm=0.5*sqrt(phiRMS*phiBRMS);

  std::vector<double> b(6);
  b[0] = 0;
  b[1] = 0;
  b[2] =phiRMS;
  b[3] =0;
  b[4] = crossTerm;
  b[5] = phiBRMS;

  CovarianceMatrix MS(b.begin(),b.end());
  track.covariance = track.covariance+MS;
}



bool L1MuonKF::updateOffline(L1KalmanMuTrack& track,const StubRefVector& stubs,int sector) {

    int trackPhi = track.positionAngle();
    int trackPhiB = track.bendingAngle();


    //Find the nearest stub:

    unsigned int bestStub=-1;
    unsigned int distance = 100000;

    for( unsigned int i=0;i<stubs.size();++i) {
      int phi = correctedPhi(stub,sector);
      int d = abs(phi-trackPhi);
      if (d<distance) {
        distance = d;
        bestStub=i;
      }

    }

    if (bestStub<0)
      return false;
    
    StubRef stub = stubs[i];
    int phi  = correctedPhi(stub,sector);
    int phiB = stub->phiB();


    ROOT::Math::SVector<double,2> residual;
    residual[0] = trackPhi-phi;
    residual[1] = trackPhiB-phiB;
    
    ///




}

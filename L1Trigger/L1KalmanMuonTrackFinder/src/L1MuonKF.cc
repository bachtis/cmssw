#include <math.h>
#include "L1Trigger/L1KalmanMuonTrackFinder/interface/L1MuonKF.h"




L1MuonKF::L1MuonKF(const edm::ParameterSet& settings):
  verbose_(settings.getParameter<bool>("verbose")),
  eLoss_(settings.getParameter<std::vector<int> >("eLoss")),
  aPhi_(settings.getParameter<std::vector<int> >("aPhi")),
  bPhi_(settings.getParameter<std::vector<int> >("bPhi")),
  aPhiB_(settings.getParameter<std::vector<int> >("aPhiB")),
  bPhiB_(settings.getParameter<std::vector<int> >("bPhiB")),
  etaLUTAddr_(settings.getParameter<std::vector<int> >("etaLUTAddr")),
  etaLUTVal_(settings.getParameter<std::vector<int> >("etaLUTValue")),
  bitWidth_(settings.getParameter<std::vector<int> >("wordSize")),
  denominator_(settings.getParameter<int>("denominator")),
  chiSquareVertexA_(settings.getParameter<std::vector<int> >("chiSquareVertexA")),
  chiSquareVertexB_(settings.getParameter<std::vector<int> >("chiSquareVertexB")),
  chiSquareVertexC_(settings.getParameter<std::vector<int> >("chiSquareVertexC")),
  chiSquareVertexDenominator_(settings.getParameter<int>("chiSquareVertexDenominator")),
  useOfflineAlgo_(settings.getParameter<bool>("useOfflineAlgo")),
  mScatteringPhi_(settings.getParameter<std::vector<double> >("mScatteringPhi")),
  mScatteringPhiB_(settings.getParameter<std::vector<double> >("mScatteringPhiB")),
  pointResolutionPhi_(settings.getParameter<double>("pointResolutionPhi")),
  pointResolutionPhiB_(settings.getParameter<double>("pointResolutionPhiB")),
  pointResolutionVertex_(settings.getParameter<double>("pointResolutionVertex"))
{



}


int L1MuonKF::correctedPhiB(const StubRef& stub) {
  //place holder
  return stub->phiB();

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
      bitmask+= (1<<abs(stub->whNum()));
    if (stub->stNum()==2)
      bitmask+= (1<<(3+abs(stub->whNum())));
    if (stub->stNum()==3)
      bitmask+= (1<<(6+abs(stub->whNum())));
    if (stub->stNum()==4)
      bitmask+=(1<<(9+abs(stub->whNum())));
  }
  return bitmask;
}


bool L1MuonKF::etaLookup(L1KalmanMuTrack& track) {
  int etaMask = etaBitmask(track);
 
  int sign=0;
  for (const auto& stub :track.stubs()) {
    if (stub->whNum()!=0)
      sign=sign+stub->whNum()/abs(stub->whNum());
  }


  for (unsigned int i=0;i<etaLUTAddr_.size();++i) {
    int addr = etaLUTAddr_[i];
    if (etaMask==addr) {
      if (sign>0) {
	track.setCoarseEta(etaLUTVal_[i],etaMask);
	return true;
      }
      else if (sign<0) {
	track.setCoarseEta(-etaLUTVal_[i],etaMask);
	return true;
      }
      else {
	track.setCoarseEta(0,etaMask);
	return true;
      }

    }
  }
  return false;
}


int L1MuonKF::customBitmask(unsigned int bit1,unsigned int bit2,unsigned int bit3,unsigned int bit4)  {
  return bit1*1+bit2*2+bit3*4+bit4*8;
}

bool L1MuonKF::getBit(int bitmask,int pos)  {
  return (bitmask & ( 1 << pos )) >> pos;
}



void L1MuonKF::propagate(L1KalmanMuTrack& track) {
  int K = track.curvature();
  int phi = track.positionAngle();
  int phiB = track.bendingAngle();
  unsigned int step = track.step();


  int charge=0;
  if (K!=0) 
    charge = K/abs(K);


  
  //energy loss
  int KNew = K+charge*eLoss_[step-1]*K*K/(4*denominator_);
  //phi propagation
  int phiNew = phi+(aPhi_[step-1]*K+bPhi_[step-1]*8*phiB)/denominator_;
  //phiB propagation
  int phiBNew = phiB+(aPhiB_[step-1]*K+bPhiB_[step-1]*phiB)/denominator_;
  ///////////////////////////////////////////////////////
  //Rest of the stuff  is for the offline version only 
  //where we want to check what is happening in the covariaznce matrix 


  //Create the transformation matrix
  double a[9];
  a[0] = 1.0;
  a[1] = 0.0;
  a[2] = 0.0;
  a[3] = aPhi_[step-1]*1.0/float(denominator_);
  a[4] = 1.0;
  a[5] = bPhi_[step-1]*8.0/float(denominator_);
  a[6] = aPhiB_[step-1]*1.0/float(denominator_);
  a[7] = 0.0;
  a[8] = 1.0+bPhiB_[step-1]/float(denominator_);


  ROOT::Math::SMatrix<double,3> P(a,9);
 
  track.covariance = ROOT::Math::Similarity(P,track.covariance);

  //Add the multiple scattering
  int phiRMS = mScatteringPhi_[step-1]*K*K;
  int phiBRMS = mScatteringPhiB_[step-1]*K*K;
  int crossTerm=sqrt(phiRMS*phiBRMS);

  std::vector<double> b(6);
  b[0] = 0;
  b[1] = 0;
  b[2] =phiRMS;
  b[3] =0;
  b[4] = crossTerm;
  b[5] = phiBRMS;

  reco::Candidate::CovarianceMatrix MS(b.begin(),b.end());
  track.covariance = track.covariance+MS;
  track.setCoordinates(step-1,KNew,phiNew,phiBNew);

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
      if (stubs[i]->stNum()!=track.step())
	continue;
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
    int phiB = correctedPhiB(stub);


    Vector2 residual;
    residual[0] = phi-trackPhi;
    residual[1] = phiB-trackPhiB;
    
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
    track.setKalmanGain(track.step(),Gain(0,0),Gain(0,1),Gain(1,0),Gain(1,1),Gain(2,0),Gain(2,1));
    int KNew  = round(trackK+Gain(0,0)*residual(0)+Gain(0,1)*residual(1));
    int phiNew  = round(trackPhi+Gain(1,0)*residual(0)+Gain(1,1)*residual(1));
    int phiBNew  = round(trackPhiB+Gain(2,0)*residual(0)+Gain(2,1)*residual(1));

    track.setCoordinates(track.step(),KNew,phiNew,phiBNew);
    Matrix33 cov = track.covariance - Gain*(H*track.covariance);
    track.covariance(0,0)=cov(0,0); 
    track.covariance(0,1)=cov(0,1); 
    track.covariance(0,2)=cov(0,2); 
    track.covariance(1,0)=cov(1,0); 
    track.covariance(1,1)=cov(1,1); 
    track.covariance(1,2)=cov(1,2); 
    track.covariance(2,0)=cov(2,0); 
    track.covariance(2,1)=cov(2,1); 
    track.covariance(2,2)=cov(2,2); 

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
  
  double S = (ROOT::Math::Similarity(H,track.covariance))(0,0)+pointResolutionVertex_;
  S=1.0/S;
  
  Matrix31 Gain = track.covariance*(ROOT::Math::Transpose(H))*S;
  track.setKalmanGain(track.step(),Gain(0,0),Gain(1,0),Gain(2,0));

  int KNew = round(track.curvature()+Gain(0,0)*residual);
  int phiNew = round(track.positionAngle()+Gain(1,0)*residual);
  track.setCoordinatesAtVertex(KNew,phiNew,track.dxy());
  Matrix33 cov = track.covariance - Gain*(H*track.covariance);
  track.covariance(0,0)=cov(0,0); 
  track.covariance(0,1)=cov(0,1); 
  track.covariance(0,2)=cov(0,2); 
  track.covariance(1,0)=cov(1,0); 
  track.covariance(1,1)=cov(1,1); 
  track.covariance(1,2)=cov(1,2); 
  track.covariance(2,0)=cov(2,0); 
  track.covariance(2,1)=cov(2,1); 
  track.covariance(2,2)=cov(2,2); 

    //  track.covariance = track.covariance - Gain*H*track.covariance;

}



void L1MuonKF::setFloatingPointValues(L1KalmanMuTrack& track,bool vertex) {
  int K,phiINT;

  int coarseEta=track.coarseEta();
  if (vertex) {
    K  = track.curvatureAtVertex();
    phiINT = track.phiAtVertex();
  }
  else {
    K=track.curvature();
    phiINT=track.positionAngle();
  }

  double lsb = 1.25/float(1 << bitWidth_[2]);
  double pt = 1.0/(lsb*K); 
  double eta = float(coarseEta)/256.0;

  double phi= -M_PI+track.stubs()[0]->scNum()*M_PI/6.0+phiINT*M_PI/(12.0*2048.);
  if (phi<-M_PI)
    phi=phi+2*M_PI;

  if (phi>M_PI)
    phi=phi-2*M_PI;
  track.setPtEtaPhi(pt,eta,phi,vertex);
}



L1MuonKF::TrackVector L1MuonKF::process(const StubRef& seed, const StubRefVector& stubs) {
  TrackVector output;
  std::vector<int> combinatorics;
  switch(seed->stNum()) {
  case 4:
    combinatorics.push_back(customBitmask(1,1,1,1));
    combinatorics.push_back(customBitmask(0,1,1,1));
    combinatorics.push_back(customBitmask(1,0,1,1));
    combinatorics.push_back(customBitmask(1,1,0,1));
    combinatorics.push_back(customBitmask(0,0,1,1));
    combinatorics.push_back(customBitmask(0,1,0,1));
    combinatorics.push_back(customBitmask(1,0,0,1));
    break;
  case 3:
    combinatorics.push_back(customBitmask(1,1,1,0));
    combinatorics.push_back(customBitmask(1,0,1,0));
    combinatorics.push_back(customBitmask(0,1,1,0));

    break;
  case 2:
    combinatorics.push_back(customBitmask(1,1,0,0));    
    break;
  default:
    return output;
  }

  for( const auto& mask : combinatorics) {
    L1KalmanMuTrack track(seed);
    //set covariance
    track.covariance(0,0)=4096*4096;
    track.covariance(0,1)=0;
    track.covariance(0,2)=0;
    track.covariance(1,0)=0;
    track.covariance(1,1)=pointResolutionPhi_;
    track.covariance(1,2)=0;
    track.covariance(2,0)=0;
    track.covariance(2,1)=0;
    track.covariance(2,2)=pointResolutionPhiB_;
    //
    if (verbose_) {
      printf("New Kalman fit staring at step=%d station = %d, phi=%d,phiB=%d\n",track.step(),seed->stNum(),track.positionAngle(),track.bendingAngle());
      printf("BITMASK:");
      for (unsigned int i=0;i<4;++i)
	printf("%d",getBit(mask,i));
      printf("\n");
      printf("------------------------------------------------------\n");
      printf("------------------------------------------------------\n");
      printf("------------------------------------------------------\n");
      printf("stubs:\n");
      for (const auto& stub: stubs) 
	printf("station=%d phi=%d phiB=%d\n",stub->stNum(),correctedPhi(stub,seed->scNum()),correctedPhiB(stub)); 
      printf("------------------------------------------------------\n");
      printf("------------------------------------------------------\n");

    }
    while(track.step()>0) {
      if (track.step()==1) {
	track.setHitPattern(phiBitmask(track));
	if(!etaLookup(track)) {
	  if (verbose_)
	    printf("Failed to assign coarse eta\n");
	  break;

	}
	setFloatingPointValues(track,false);
	if (verbose_) 
	  printf ("Floating point coordinates in Muon System: pt=%f, eta=%f phi=%f\n",track.unconstrainedP4().pt(),track.unconstrainedP4().eta(),track.unconstrainedP4().phi());
      }
      propagate(track);
      if (verbose_)
	printf("propagated Coordinates step:%d,phi=%d,phiB=%d,K=%d\n",track.step(),track.positionAngle(),track.bendingAngle(),track.curvature());

      if (track.step()>0) 
	if (getBit(mask,track.step()-1)) {
	  if (!update(track,stubs,seed->scNum()))
	    break;
	  if (verbose_)
	    printf("updated Coordinates step:%d,phi=%d,phiB=%d,K=%d\n",track.step(),track.positionAngle(),track.bendingAngle(),track.curvature());
	}
      if (track.step()==0) {
	track.setCoordinatesAtVertex(track.curvature(),track.positionAngle(),track.bendingAngle());
	if (verbose_)
	  printf(" Coordinates before vertex constraint step:%d,phi=%d,dxy=%d,K=%d\n",track.step(),track.phiAtVertex(),track.dxy(),track.curvatureAtVertex());
	vertexConstraint(track);
	estimateChiSquareVertex(track);
	if (verbose_) {
	  printf(" Coordinates after vertex constraint step:%d,phi=%d,dxy=%d,K=%d  maximum local chi2=%d\n",track.step(),track.phiAtVertex(),track.dxy(),track.curvatureAtVertex(),track.approxChi2());
	  printf("------------------------------------------------------\n");
	  printf("------------------------------------------------------\n");
	}
	setFloatingPointValues(track,true);
	if (verbose_)
	  printf ("Floating point coordinates at vertex: pt=%f, eta=%f phi=%f\n",track.pt(),track.eta(),track.phi());

	
	output.push_back(track);
      }
    }
  }
  return output;
}       
    





void L1MuonKF::estimateChiSquareVertex(L1KalmanMuTrack& track) {
  //here we have a simplification of the algorithm for the sake of the emulator - rsult is identical
  // we apply cuts on the firmware as |u -u'|^2 < a+b *K^2 
  int phi = track.phiAtVertex();
  int K = track.curvatureAtVertex();
  float maxChi2 = 0;
  for (const auto& stub: track.stubs()) {
    float p = correctedPhi(stub,track.stubs()[0]->scNum())+8*stub->phiB()-phi-chiSquareVertexA_[stub->stNum()-1]*2*K/chiSquareVertexDenominator_;
    p = p*p/(chiSquareVertexB_[stub->stNum()-1]+chiSquareVertexC_[stub->stNum()-1]*K*K/chiSquareVertexDenominator_);
    if (p>maxChi2)
      maxChi2=p;     
  }
  track.setApproxChi2(int(maxChi2*128));
}


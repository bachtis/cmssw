#include <math.h>
#include "L1Trigger/L1KalmanMuonTrackFinder/interface/L1MuonKF.h"



L1MuonKF::L1MuonKF(const edm::ParameterSet& settings):
  verbose_(settings.getParameter<bool>("verbose")),
  eLoss_(settings.getParameter<std::vector<double> >("eLoss")),
  aPhi_(settings.getParameter<std::vector<double> >("aPhi")),
  aPhiB_(settings.getParameter<std::vector<double> >("aPhiB")),
  aPhiBNLO_(settings.getParameter<std::vector<double> >("aPhiBNLO")),
  bPhi_(settings.getParameter<std::vector<double> >("bPhi")),
  bPhiB_(settings.getParameter<std::vector<double> >("bPhiB")),
  etaLUTAddr_(settings.getParameter<std::vector<int> >("etaLUTAddr")),
  etaLUTVal_(settings.getParameter<std::vector<int> >("etaLUTValue")),
  chiSquare_(settings.getParameter<std::vector<double> >("chiSquare")),
  chiSquareCut_(settings.getParameter<int>("chiSquareCut")),
  useOfflineAlgo_(settings.getParameter<bool>("useOfflineAlgo")),
  mScatteringPhi_(settings.getParameter<std::vector<double> >("mScatteringPhi")),
  mScatteringPhiB_(settings.getParameter<std::vector<double> >("mScatteringPhiB")),
  pointResolutionPhi_(settings.getParameter<double>("pointResolutionPhi")),
  pointResolutionPhiB_(settings.getParameter<double>("pointResolutionPhiB")),
  pointResolutionVertex_(settings.getParameter<double>("pointResolutionVertex"))
{



}


int L1MuonKF::correctedPhiB(const StubRef& stub) {
  //Promote phiB to 12 bits
  return 8*stub->phiB();

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


  int charge=1;
  if (K!=0) 
    charge = K/abs(K);



  //energy loss term only for MU->VERTEX
  int offset=int(charge*eLoss_[step-1]*K*K);
  if (abs(offset)>1024)
    offset=1024*offset/abs(offset);
  int KNew =wrapAround(K+offset,8192);

  //phi propagation
  int phiNew =wrapAround(phi+int(aPhi_[step-1]*K)-int(bPhi_[step-1]*phiB),8192);

  //phiB propagation
  int phiBNew = (int(aPhiB_[step-1]*K)  +int(bPhiB_[step-1]*phiB));

  //Only for the propagation to vertex we use the LUT for better precision
  phiBNew = wrapAround(phiBNew+charge*aPhiBNLO_[step-1]*K*K,2048);

  ///////////////////////////////////////////////////////
  //Rest of the stuff  is for the offline version only 
  //where we want to check what is happening in the covariaznce matrix 


  //Create the transformation matrix
  double a[9];
  a[0] = 1.0;
  a[1] = 0.0;
  a[2] = 0.0;
  a[3] = aPhi_[step-1];
  a[4] = 1.0;
  a[5] = -bPhi_[step-1];
  a[6] = aPhiB_[step-1];
  a[7] = 0.0;
  a[8] = bPhiB_[step-1];


  ROOT::Math::SMatrix<double,3> P(a,9);

  const std::vector<double>& covLine = track.covariance();
  L1KalmanMuTrack::CovarianceMatrix cov(covLine.begin(),covLine.end());
  cov = ROOT::Math::Similarity(P,cov);

  
  //Add the multiple scattering
  double phiRMS = mScatteringPhi_[step-1]*K*K;
  double phiBRMS = mScatteringPhiB_[step-1]*K*K;


  std::vector<double> b(6);
  b[0] = 0;
  b[1] = 0;
  b[2] =phiRMS;
  b[3] =0;
  b[4] = 0;
  b[5] = phiBRMS;

  reco::Candidate::CovarianceMatrix MS(b.begin(),b.end());



  cov = cov+MS;
  track.setCovariance(cov);
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
      int phi = correctedPhi(stubs[i],sector)+correctedPhiB(stubs[i]);
      int d = abs(phi-trackPhi-trackPhiB);
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
    if (stub->code()<6)
      residual[1]=0;
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

    const std::vector<double>& covLine = track.covariance();
    L1KalmanMuTrack::CovarianceMatrix cov(covLine.begin(),covLine.end());


    CovarianceMatrix2 S = ROOT::Math::Similarity(H,cov)+R;
    if (!S.Invert())
      return false;
    Matrix32 Gain = cov*ROOT::Math::Transpose(H)*S;

    track.setKalmanGain(track.step(),abs(trackK),Gain(0,0),Gain(0,1),Gain(1,0),Gain(1,1),Gain(2,0),Gain(2,1));
    int KNew  = wrapAround(trackK+int(Gain(0,0)*residual(0)+Gain(0,1)*residual(1)),8192);
    int phiNew  = wrapAround(trackPhi+residual(0),8192);
    int phiBNew = wrapAround(trackPhiB+int(Gain(2,0)*residual(0)+Gain(2,1)*residual(1)),2048);
    track.setCoordinates(track.step(),KNew,phiNew,phiBNew);
    Matrix33 covNew = cov - Gain*(H*cov);
    L1KalmanMuTrack::CovarianceMatrix c;
 
    c(0,0)=covNew(0,0); 
    c(0,1)=covNew(0,1); 
    c(0,2)=covNew(0,2); 
    c(1,0)=covNew(1,0); 
    c(1,1)=covNew(1,1); 
    c(1,2)=covNew(1,2); 
    c(2,0)=covNew(2,0); 
    c(2,1)=covNew(2,1); 
    c(2,2)=covNew(2,2); 
    track.setCovariance(c);
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

  const std::vector<double>& covLine = track.covariance();
  L1KalmanMuTrack::CovarianceMatrix cov(covLine.begin(),covLine.end());
  
  double S = (ROOT::Math::Similarity(H,cov))(0,0)+pointResolutionVertex_;
  S=1.0/S;
  
  Matrix31 Gain = cov*(ROOT::Math::Transpose(H))*S;
  track.setKalmanGain(track.step(),abs(track.curvature()),Gain(0,0),Gain(1,0),Gain(2,0));

  int KNew = wrapAround(int(track.curvature()+Gain(0,0)*residual),8192);
  int phiNew = wrapAround(int(track.positionAngle()+Gain(1,0)*residual),8192);
  track.setCoordinatesAtVertex(KNew,phiNew,-residual);
  Matrix33 covNew = cov - Gain*(H*cov);
  L1KalmanMuTrack::CovarianceMatrix c;
  c(0,0)=covNew(0,0); 
  c(0,1)=covNew(0,1); 
  c(0,2)=covNew(0,2); 
  c(1,0)=covNew(1,0); 
  c(1,1)=covNew(1,1); 
  c(1,2)=covNew(1,2); 
  c(2,0)=covNew(2,0); 
  c(2,1)=covNew(2,1); 
  c(2,2)=covNew(2,2); 
  track.setCovariance(c);
  //  track.covariance = track.covariance - Gain*H*track.covariance;
}



void L1MuonKF::setFloatingPointValues(L1KalmanMuTrack& track,bool vertex) {
  int K,phiINT;

  int coarseEta=track.coarseEta();
  if (vertex) {
    K  = track.curvatureAtVertex();
    if (K==0)
      K=1;
    phiINT = track.phiAtVertex();
    if (K==0)
      track.setCharge(1);
    else
      track.setCharge(K/abs(K));
  }
  else {
    K=track.curvatureAtMuon();
    if (K==0)
      K=1;
    phiINT=track.positionAngle();
  }

  if (abs(K)<46)
    K=46*K/abs(K);
  


  double lsb = 1.25/float(1 << 13);
  //this will be in the final BRAM giving PT=1/K
  //  double corrK = 0.83*abs(K)+9.173+2.49e-4*K*K;

  //  K = 0.963*abs(K)+(1.035e-4)*K*K;
  K = 0.95*K; //calibration
  double pt = 1.0/(lsb*abs(K));
  double eta = float(coarseEta)/100.0;

  double phi= track.sector()*M_PI/6.0+phiINT*M_PI/(6*2048.)-2*M_PI;
  if (phi<-M_PI)
    phi=phi+2*M_PI;
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
    L1KalmanMuTrack::CovarianceMatrix covariance;  
    covariance(0,0)=4096*4096;
    covariance(0,1)=0;
    covariance(0,2)=0;
    covariance(1,0)=0;
    covariance(1,1)=pointResolutionPhi_;
    covariance(1,2)=0;
    covariance(2,0)=0;
    covariance(2,1)=0;
    covariance(2,2)=pointResolutionPhiB_;
    track.setCovariance(covariance);
    //
    if (verbose_) {
      printf("New Kalman fit staring at step=%d station = %d, phi=%d,phiB=%d with curvature=%d\n",track.step(),seed->stNum(),track.positionAngle(),track.bendingAngle(),track.curvature());
      printf("BITMASK:");
      for (unsigned int i=0;i<4;++i)
	printf("%d",getBit(mask,i));
      printf("\n");
      printf("------------------------------------------------------\n");
      printf("------------------------------------------------------\n");
      printf("------------------------------------------------------\n");
      printf("stubs:\n");
      for (const auto& stub: stubs) 
	printf("station=%d phi=%d phiB=%d qual=%d \n",stub->stNum(),correctedPhi(stub,seed->scNum()),correctedPhiB(stub),stub->code()); 
      printf("------------------------------------------------------\n");
      printf("------------------------------------------------------\n");

    }
    while(track.step()>0) {


      // muon station 1 
      if (track.step()==1) {
	track.setHitPattern(phiBitmask(track));
	if(!etaLookup(track)) {
	  if (verbose_)
	    printf("Failed to assign coarse eta\n");
	  break;
	}
	setFloatingPointValues(track,false);
	track.setCoordinatesAtMuon(track.curvature(),track.positionAngle(),track.bendingAngle());
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
	//SATURATE DXY
	int dxy = track.bendingAngle();
	if (abs(dxy)>4096)
	  dxy = 4096*dxy/abs(dxy);
 
	track.setCoordinatesAtVertex(track.curvature(),track.positionAngle(),dxy);
	if (verbose_)
	  printf(" Coordinates before vertex constraint step:%d,phi=%d,dxy=%d,K=%d\n",track.step(),track.phiAtVertex(),track.dxy(),track.curvatureAtVertex());
	int preconstrainedK = track.curvature();
	vertexConstraint(track);
	int postconstrainedK = track.curvatureAtVertex(); 
	track.setDeltaK(abs(preconstrainedK-postconstrainedK));
	estimateChiSquare(track);

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
    





void L1MuonKF::estimateChiSquare(L1KalmanMuTrack& track) {
  //here we have a simplification of the algorithm for the sake of the emulator - rsult is identical
  // we apply cuts on the firmware as |u -u'|^2 < a+b *K^2 

  int phi = track.phiAtMuon();
  int phiB = track.phiBAtMuon();
  int K = track.curvatureAtMuon();

  int chi=0;
   for (const auto& stub: track.stubs()) {
     uint delta=abs(correctedPhi(stub,track.sector())-phi +correctedPhiB(stub)-phiB -chiSquare_[stub->stNum()-1]*K);
     chi=chi+abs(delta);
   }
  track.setApproxChi2(chi);
}


int L1MuonKF::rank(const L1KalmanMuTrack& track) {
  if (phiBitmask(track)==customBitmask(0,0,1,1))
    return -8192;
  return (track.stubs().size()*2+track.quality())*80-track.approxChi2();
  //  return (track.stubs().size()*2+track.quality())*80-abs(track.dxy());

}


int L1MuonKF::wrapAround(int value,int maximum) {
  if (value>maximum-1)
    return value-2*maximum;
  if (value<-maximum)
    return value+2*maximum;
  return value;

}


L1MuonKF::TrackVector L1MuonKF::cleanAndSort(const L1MuonKF::TrackVector& tracks) {
  L1MuonKF::TrackVector out;


  if (verbose_) 
    printf(" -----Preselected Kalman Tracks-----\n");


  for(const auto& track1 : tracks) {
    if (verbose_)
       printf("Preselected Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA =%d stubs=%d chi2=%d pts=%f %f\n",track1.charge(),track1.pt(),track1.eta(),track1.phi(),track1.curvatureAtVertex(),track1.curvatureAtMuon(),int(track1.stubs().size()),track1.approxChi2(),track1.pt(),track1.unconstrainedP4().pt()); 

    bool keep=true;
    for(const auto& track2 : tracks) {
      if (track1==track2)
	continue;
      if (!track1.overlap(track2))
	continue;
      if (rank(track1)<rank(track2))
	keep=false;
    }
    if (keep && (track1.approxChi2()<chiSquareCut_*int(track1.stubs().size()))) 
      out.push_back(track1);
  }

  if (verbose_) {
    printf(" -----Final Kalman Tracks-----\n");
    for (const auto& track1 :out)
      printf("Final Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA =%d stubs=%d chi2=%d pts=%f %f\n",track1.charge(),track1.pt(),track1.eta(),track1.phi(),track1.curvatureAtVertex(),track1.curvatureAtMuon(),int(track1.stubs().size()),track1.approxChi2(),track1.pt(),track1.unconstrainedP4().pt()); 
      
  }

  return out;
}

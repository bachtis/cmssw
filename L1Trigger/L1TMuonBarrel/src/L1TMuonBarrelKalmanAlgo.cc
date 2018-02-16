#include <math.h>
#include "L1Trigger/L1TMuonBarrel/interface/L1TMuonBarrelKalmanAlgo.h"


L1TMuonBarrelKalmanAlgo::L1TMuonBarrelKalmanAlgo(const edm::ParameterSet& settings):
  verbose_(settings.getParameter<bool>("verbose")),
  lutService_(new L1TMuonBarrelKalmanLUTs(settings.getParameter<std::string>("lutFile"))),
  initK_(settings.getParameter<std::vector<double> >("initialK")),
  eLoss_(settings.getParameter<std::vector<double> >("eLoss")),
  aPhi_(settings.getParameter<std::vector<double> >("aPhi")),
  aPhiB_(settings.getParameter<std::vector<double> >("aPhiB")),
  aPhiBNLO_(settings.getParameter<std::vector<double> >("aPhiBNLO")),
  bPhi_(settings.getParameter<std::vector<double> >("bPhi")),
  bPhiB_(settings.getParameter<std::vector<double> >("bPhiB")),
  chiSquare_(settings.getParameter<std::vector<double> >("chiSquare")),
  chiSquareCut_(settings.getParameter<int>("chiSquareCut")),
  combos4_(settings.getParameter<std::vector<int> >("combos4")),
  combos3_(settings.getParameter<std::vector<int> >("combos3")),
  combos2_(settings.getParameter<std::vector<int> >("combos2")),
  combos1_(settings.getParameter<std::vector<int> >("combos1")),
  useOfflineAlgo_(settings.getParameter<bool>("useOfflineAlgo")),
  mScatteringPhi_(settings.getParameter<std::vector<double> >("mScatteringPhi")),
  mScatteringPhiB_(settings.getParameter<std::vector<double> >("mScatteringPhiB")),
  pointResolutionPhi_(settings.getParameter<double>("pointResolutionPhi")),
  pointResolutionPhiB_(settings.getParameter<double>("pointResolutionPhiB")),
  pointResolutionVertex_(settings.getParameter<double>("pointResolutionVertex"))  

{



}


std::pair<bool,uint> L1TMuonBarrelKalmanAlgo::match(const L1MuKBMTrack& track, const L1MuKBMTCombinedStubRefVector& stubs) {
  L1MuKBMTCombinedStubRefVector selected;

  bool found=false;
  uint best=0;
  int distance=100000;
  uint N=0;
  for (const auto& stub :stubs)  {
    N=N+1;
    if (stub->stNum()!=track.step()) 
      continue;
    if (abs(track.positionAngle()-correctedPhi(stub,track.sector()))<distance) {
      distance = abs(track.positionAngle()-correctedPhi(stub,track.sector()));
      best=N-1;
      found=true;
    }
  }
  return std::make_pair(found,best);
}



int L1TMuonBarrelKalmanAlgo::correctedPhiB(const L1MuKBMTCombinedStubRef& stub) {
  //Promote phiB to 12 bits
  return 8*stub->phiB();

}

int L1TMuonBarrelKalmanAlgo::correctedPhi(const L1MuKBMTCombinedStubRef& stub,int sector) {
  if (stub->scNum()==sector)
    return stub->phi();
  else if ((stub->scNum()==sector-1) || (stub->scNum()==11 && sector==0))
    return stub->phi()-2144;
  else if ((stub->scNum()==sector+1) || (stub->scNum()==0 && sector==11))
    return stub->phi()+2144;
  return 0;
} 


int L1TMuonBarrelKalmanAlgo::hitPattern(const L1MuKBMTrack& track)  {
  unsigned int mask = 0;
  for (const auto& stub : track.stubs()) {
    mask = mask+round(pow(2,stub->stNum()-1));
  }
  return mask;
}



int L1TMuonBarrelKalmanAlgo::customBitmask(unsigned int bit1,unsigned int bit2,unsigned int bit3,unsigned int bit4)  {
  return bit1*1+bit2*2+bit3*4+bit4*8;
}

bool L1TMuonBarrelKalmanAlgo::getBit(int bitmask,int pos)  {
  return (bitmask & ( 1 << pos )) >> pos;
}



void L1TMuonBarrelKalmanAlgo::propagate(L1MuKBMTrack& track) {
  int K = track.curvature();
  int phi = track.positionAngle();
  int phiB = track.bendingAngle();
  unsigned int step = track.step();

  int charge=1;
  if (K!=0) 
    charge = K/abs(K);



  //energy loss term only for MU->VERTEX
  //int offset=int(charge*eLoss_[step-1]*K*K);
  //  if (abs(offset)>4096)
  //      offset=4096*offset/abs(offset);
  int KNew =wrapAround(int(K/(1+charge*eLoss_[step-1]*K)),8192);

  //phi propagation
  int phiNew =wrapAround(phi+int(aPhi_[step-1]*K)-int(bPhi_[step-1]*phiB),8192);

  //phiB propagation
  int phiBNew = wrapAround(int(aPhiB_[step-1]*K)  +int(bPhiB_[step-1]*phiB),2048);
  
  //Only for the propagation to vertex we use the LUT for better precision and the full function
  if (step==1)
    phiBNew = wrapAround(int(aPhiB_[step-1]*atan(aPhiBNLO_[step-1]*K))+int(bPhiB_[step-1]*phiB),2048);

  ///////////////////////////////////////////////////////
  //Rest of the stuff  is for the offline version only 
  //where we want to check what is happening in the covariaznce matrix 
  
  //Create the transformation matrix
  double a[9];
  a[0] = 1./((1.0+charge*eLoss_[step-1]*K)*(1.0+charge*eLoss_[step-1]*K));
  a[1] = 0.0;
  a[2] = 0.0;
  a[3] = aPhi_[step-1];
  a[4] = 1.0;
  a[5] = -bPhi_[step-1];
  a[6] = aPhiB_[step-1]+2*charge*aPhiBNLO_[step-1]*K;
  if (step==1)
    a[6] = aPhiB_[step-1]*aPhiBNLO_[step-1]/((1+aPhiBNLO_[step-1]*K*aPhiBNLO_[step-1])*(1+aPhiBNLO_[step-1]*K*aPhiBNLO_[step-1]));

  a[7] = 0.0;
  a[8] = bPhiB_[step-1];


  ROOT::Math::SMatrix<double,3> P(a,9);

  const std::vector<double>& covLine = track.covariance();
  L1MuKBMTrack::CovarianceMatrix cov(covLine.begin(),covLine.end());
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


bool L1TMuonBarrelKalmanAlgo::update(L1MuKBMTrack& track,const L1MuKBMTCombinedStubRef& stub) {
  updateEta(track,stub);
  if (useOfflineAlgo_)
    return updateOffline(track,stub);
  else
    return updateLUT(track,stub);

}

bool L1TMuonBarrelKalmanAlgo::updateOffline(L1MuKBMTrack& track,const L1MuKBMTCombinedStubRef& stub) {
    int trackK = track.curvature();
    int trackPhi = track.positionAngle();
    int trackPhiB = track.bendingAngle();

    int phi  = correctedPhi(stub,track.sector());
    int phiB = correctedPhiB(stub);
    //Update eta
    track.setCoarseEta(int((track.coarseEta()+stub->coarseEta())/2.0));
    if (stub->quality()<6)//Michalios
      phiB=trackPhiB;


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

    const std::vector<double>& covLine = track.covariance();
    L1MuKBMTrack::CovarianceMatrix cov(covLine.begin(),covLine.end());


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
    L1MuKBMTrack::CovarianceMatrix c;
 
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
    track.setHitPattern(hitPattern(track));

    return true;
}


bool L1TMuonBarrelKalmanAlgo::updateLUT(L1MuKBMTrack& track,const L1MuKBMTCombinedStubRef& stub) {
    int trackK = track.curvature();
    int trackPhi = track.positionAngle();
    int trackPhiB = track.bendingAngle();

    //Update eta
    track.setCoarseEta(int((track.coarseEta()+stub->coarseEta())/2.0));

    int phi  = correctedPhi(stub,track.sector());
    int phiB = correctedPhiB(stub);
    if (stub->quality()<6)
      phiB=trackPhiB;

    Vector2 residual;
    residual[0] = phi-trackPhi;
    residual[1] = phiB-trackPhiB;

    uint absK = abs(trackK);
    if (absK>2047)
      absK = 2047;
    std::vector<float> GAIN = lutService_->trackGain(track.step(),track.hitPattern(),absK/4);
    track.setKalmanGain(track.step(),abs(trackK),GAIN[0],GAIN[1],1,0,GAIN[2],GAIN[3]);
    int KNew  = wrapAround(trackK+int(GAIN[0]*residual(0)+GAIN[1]*residual(1)),8192);
    int phiNew  = wrapAround(trackPhi+residual(0),8192);
    int phiBNew = wrapAround(trackPhiB+int(GAIN[2]*residual(0)+GAIN[3]*residual(1)),2048);
    track.setCoordinates(track.step(),KNew,phiNew,phiBNew);
    track.addStub(stub);
    track.setHitPattern(hitPattern(track));
    return true;
}




void L1TMuonBarrelKalmanAlgo::updateEta(L1MuKBMTrack& track,const L1MuKBMTCombinedStubRef& stub) {

    if (stub->qeta1()>=0) {
      if (track.hasFineEta()) {
	uint dist2=1000;
	uint dist1 = abs(track.fineEta()-stub->eta1());
	if (stub->qeta2()>=0)
	  dist2 = abs(track.fineEta()-stub->eta2());
	if (dist1<dist2)
	  track.setFineEta((stub->eta1()+track.fineEta())/2);
	else
	  track.setFineEta((stub->eta2()+track.fineEta())/2);
      }else {
	if (stub->qeta2()>=0)
	  track.setFineEta((stub->eta1()+stub->eta2())/2);
	else
	  track.setFineEta(stub->eta1());
      }

    }
}






void L1TMuonBarrelKalmanAlgo::vertexConstraint(L1MuKBMTrack& track) {
  if (useOfflineAlgo_)
    vertexConstraintOffline(track);
  else
    vertexConstraintLUT(track);
 
}


void L1TMuonBarrelKalmanAlgo::vertexConstraintOffline(L1MuKBMTrack& track) {
  double residual = -track.dxy();
  Matrix13 H;
  H(0,0)=0;
  H(0,1)=0;
  H(0,2)=1;

  const std::vector<double>& covLine = track.covariance();
  L1MuKBMTrack::CovarianceMatrix cov(covLine.begin(),covLine.end());
  
  double S = (ROOT::Math::Similarity(H,cov))(0,0)+pointResolutionVertex_;
  S=1.0/S;
  Matrix31 Gain = cov*(ROOT::Math::Transpose(H))*S;
  track.setKalmanGain(track.step(),abs(track.curvature()),Gain(0,0),Gain(1,0),Gain(2,0));

  int KNew = wrapAround(int(track.curvature()+Gain(0,0)*residual),8192);
  int phiNew = wrapAround(int(track.positionAngle()+Gain(1,0)*residual),8192);
  track.setCoordinatesAtVertex(KNew,phiNew,-residual);
  Matrix33 covNew = cov - Gain*(H*cov);
  L1MuKBMTrack::CovarianceMatrix c;
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



void L1TMuonBarrelKalmanAlgo::vertexConstraintLUT(L1MuKBMTrack& track) {
  double residual = -track.dxy();
  uint absK = abs(track.curvature());
  if (absK>2047)
    absK = 2047;

std::pair<float,float> GAIN = lutService_->vertexGain(track.hitPattern(),absK/2);
  track.setKalmanGain(track.step(),abs(track.curvature()),GAIN.first,GAIN.second,-1);

  int KNew = wrapAround(int(track.curvature()+GAIN.first*residual),8192);
  int phiNew = wrapAround(int(track.positionAngle()+GAIN.second*residual),8192);
  track.setCoordinatesAtVertex(KNew,phiNew,-residual);
}



void L1TMuonBarrelKalmanAlgo::setFloatingPointValues(L1MuKBMTrack& track,bool vertex) {
  int K,phiINT,etaINT;

  if (track.hasFineEta())
    etaINT=track.fineEta();
  else
    etaINT=track.coarseEta();


  double lsb = 1.25/float(1 << 13);
  double lsbEta = 0.010875;


  if (vertex) {
    K  = track.curvatureAtVertex();
    if (K==0)
      track.setCharge(1);
    else
      track.setCharge(K/abs(K));

    phiINT = track.phiAtVertex();
    double phi= track.sector()*M_PI/6.0+phiINT*M_PI/(6*2048.)-2*M_PI;
    double eta = etaINT*lsbEta;
    if (phi<-M_PI)
      phi=phi+2*M_PI;
    if (K==0)
      K=1;    
    if (abs(K)<46)
      K=46*K/abs(K);
    double pt = 1.0/(lsb*abs(0.85*K));
    track.setPtEtaPhi(pt,eta,phi);
  }
  else {
    K=track.curvatureAtMuon();
    if (K==0)
      K=1;
    if (abs(K)<46)
      K=46*K/abs(K);
    double pt = 1.0/(lsb*abs(K));
    track.setPtUnconstrained(pt);
  }
}



std::pair<bool,L1MuKBMTrack> L1TMuonBarrelKalmanAlgo::chain(const L1MuKBMTCombinedStubRef& seed, const L1MuKBMTCombinedStubRefVector& stubs) {
  L1MuKBMTrackCollection pretracks; 
  std::vector<int> combinatorics;
  switch(seed->stNum()) {
  case 1:
    combinatorics=combos1_;
    break;
  case 2:
    combinatorics=combos2_;
    break;

  case 3:
    combinatorics=combos3_;
    break;

  case 4:
    combinatorics=combos4_;
    break;

  default:
    printf("Something really bad happend\n");
  }

  L1MuKBMTrack nullTrack(seed,correctedPhi(seed,seed->scNum()),correctedPhiB(seed));

  for( const auto& mask : combinatorics) {
    L1MuKBMTrack track(seed,correctedPhi(seed,seed->scNum()),correctedPhiB(seed));
    int initialK = int(initK_[seed->stNum()-1]*correctedPhiB(seed));
    int initialPhiB = correctedPhiB(seed);

    track.setCoordinates(seed->stNum(),initialK,correctedPhi(seed,seed->scNum()),initialPhiB);
    track.setHitPattern(hitPattern(track));
    //Set eta coarse
    track.setCoarseEta(seed->coarseEta());
    //Set fine eta:
    if (seed->qeta1()>=0) {
      if (seed->qeta2()>=0) {
	track.setFineEta((seed->eta1()+seed->eta2())/2);
      }
      else {
	track.setFineEta(seed->eta1());
      }
    }

    //set covariance
    L1MuKBMTrack::CovarianceMatrix covariance;  
    covariance(0,0)=4096.*4096.;
    covariance(0,1)=0;
    covariance(0,2)=0;
    covariance(1,0)=0;
    covariance(1,1)=float(pointResolutionPhi_);
    covariance(1,2)=0;
    covariance(2,0)=0;
    covariance(2,1)=0;
    covariance(2,2)=float(pointResolutionPhiB_);
    track.setCovariance(covariance);
    //
    if (verbose_) {
      printf("New Kalman fit staring at step=%d, phi=%d,phiB=%d with curvature=%d\n",track.step(),track.positionAngle(),track.bendingAngle(),track.curvature());
      printf("BITMASK:");
      for (unsigned int i=0;i<4;++i)
	printf("%d",getBit(mask,i));
      printf("\n");
      printf("------------------------------------------------------\n");
      printf("------------------------------------------------------\n");
      printf("------------------------------------------------------\n");
      printf("stubs:\n");
      for (const auto& stub: stubs) 
	printf("station=%d phi=%d phiB=%d qual=%d \n",stub->stNum(),correctedPhi(stub,seed->scNum()),correctedPhiB(stub),stub->quality()); 
      printf("------------------------------------------------------\n");
      printf("------------------------------------------------------\n");

    }

    while(track.step()>0) {
      // muon station 1 
      if (track.step()==1) {
	track.setCoordinatesAtMuon(track.curvature(),track.positionAngle(),track.bendingAngle());
	setFloatingPointValues(track,false);
	if (verbose_) 
	  printf ("Unconstrained PT  in Muon System: pt=%f\n",track.ptUnconstrained());
      }
      propagate(track);
      if (verbose_)
	printf("propagated Coordinates step:%d,phi=%d,phiB=%d,K=%d\n",track.step(),track.positionAngle(),track.bendingAngle(),track.curvature());

      if (track.step()>0) 
	if (getBit(mask,track.step()-1)) {
	  std::pair<bool,uint> bestStub = match(track,stubs);
	  
	  if ((!bestStub.first) || (!update(track,stubs[bestStub.second])))
	    break;
	  if (verbose_)
	    printf("updated Coordinates step:%d,phi=%d,phiB=%d,K=%d\n",track.step(),track.positionAngle(),track.bendingAngle(),track.curvature());
	}
      if (track.step()==0) {
	track.setCoordinatesAtVertex(track.curvature(),track.positionAngle(),track.bendingAngle());
	if (verbose_)
	  printf(" Coordinates before vertex constraint step:%d,phi=%d,dxy=%d,K=%d\n",track.step(),track.phiAtVertex(),track.dxy(),track.curvatureAtVertex());
	vertexConstraint(track);
	estimateChiSquare(track);
	if (verbose_) {
	  printf(" Coordinates after vertex constraint step:%d,phi=%d,dxy=%d,K=%d  maximum local chi2=%d\n",track.step(),track.phiAtVertex(),track.dxy(),track.curvatureAtVertex(),track.approxChi2());
	  printf("------------------------------------------------------\n");
	  printf("------------------------------------------------------\n");
	}
	setFloatingPointValues(track,true);
	track.setRank(rank(track));
	if (verbose_)
	  printf ("Floating point coordinates at vertex: pt=%f, eta=%f phi=%f\n",track.pt(),track.eta(),track.phi());
	pretracks.push_back(track);
      }
    }
  }

  //Now for all the pretracks we need only one 
  L1MuKBMTrackCollection cleaned = cleanAndSort(pretracks,1);

  if (cleaned.size()>0)
    return std::make_pair(true,cleaned[0]);
  return std::make_pair(false,nullTrack);
}       
    





void L1TMuonBarrelKalmanAlgo::estimateChiSquare(L1MuKBMTrack& track) {
  //here we have a simplification of the algorithm for the sake of the emulator - rsult is identical
  // we apply cuts on the firmware as |u -u'|^2 < a+b *K^2 
  int K = track.curvatureAtMuon();

  int chi=0;

  for (const auto& stub: track.stubs()) {
     uint delta=abs(correctedPhi(stub,track.sector())-track.phiAtMuon()+correctedPhiB(stub)-track.phiBAtMuon()-chiSquare_[stub->stNum()-1]*K);
     if (stub->quality()<6)
       delta = abs(correctedPhi(stub,track.sector())-track.phiAtMuon()-chiSquare_[stub->stNum()-1]*K);
     chi=chi+abs(delta);
   }
   track.setApproxChi2(chi);
}


int L1TMuonBarrelKalmanAlgo::rank(const L1MuKBMTrack& track) {
  if (hitPattern(track)==customBitmask(0,0,1,1))
    return -8192;
  return (track.stubs().size()*2+track.quality())*80-track.approxChi2();
}


int L1TMuonBarrelKalmanAlgo::wrapAround(int value,int maximum) {
  if (value>maximum-1)
    return value-2*maximum;
  if (value<-maximum)
    return value+2*maximum;
  return value;

}




L1MuKBMTrackCollection L1TMuonBarrelKalmanAlgo::cleanAndSort(const L1MuKBMTrackCollection& tracks,uint keep) {
  L1MuKBMTrackCollection out;

  if (verbose_) 
    printf(" -----Preselected Kalman Tracks-----\n");
  for(const auto& track1 : tracks) {
    if (verbose_)
      printf("Preselected Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA =%d stubs=%d chi2=%d pts=%f %f\n",track1.charge(),track1.pt(),track1.eta(),track1.phi(),track1.curvatureAtVertex(),track1.curvatureAtMuon(),int(track1.stubs().size()),track1.approxChi2(),track1.pt(),track1.ptUnconstrained()); 

    bool keep=true;
    for(const auto& track2 : tracks) {
      if (track1==track2)
	continue;
      if (!track1.overlap(track2))
	continue;
      if (track1.rank()<track2.rank())
	keep=false;
    }
    if (keep) 
      out.push_back(track1);
  }

  if (verbose_) {
  printf(" -----Algo Result Kalman Tracks-----\n");
  for (const auto& track1 :out)
    printf("Final Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA =%d stubs=%d chi2=%d pts=%f %f\n",track1.charge(),track1.pt(),track1.eta(),track1.phi(),track1.curvatureAtVertex(),track1.curvatureAtMuon(),int(track1.stubs().size()),track1.approxChi2(),track1.pt(),track1.ptUnconstrained()); 
  }


  TrackSorter sorter;
  if (out.size()>0)
    std::sort(out.begin(),out.end(),sorter);


  L1MuKBMTrackCollection exported;
  for (uint i=0;i<out.size();++i)
    if (i<=keep)
      exported.push_back(out[i]);
  return exported;
}


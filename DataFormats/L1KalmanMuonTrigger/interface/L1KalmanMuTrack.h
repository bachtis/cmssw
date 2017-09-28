/*
Kalman Track class for 
Kalman Muon Track Finder
Michalis Bachtis (UCLA)
Sep. 2017
*/

#ifndef L1KalmanMuTrack_H
#define L1KalmanMuTrack_H

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"

class L1KalmanMuTrack : public reco::LeafCandidate
{
public:
  typedef math::PtEtaPhiMLorentzVector PolarLorentzVector;
  typedef edm::Ref<std::vector<L1MuDTChambPhDigi> > StubRef;
  typedef std::vector<StubRef> StubRefVector;

  L1KalmanMuTrack();
  ~L1KalmanMuTrack();
  L1KalmanMuTrack(const StubRef&);

  //Constrained curvature at vertex
  int curvatureAtVertex() const; 
  //constrained phi at the vertex
  int phiAtVertex() const;
  //Impact parameter as calculated from the muon track 
  int dxy() const;

  //Unconstrained curvature at the Muon systen 
  int curvature() const;
  //Unconstrained phi at the Muon systen 
  int positionAngle() const;
  //Unconstrained bending angle at the Muon systen 
  int bendingAngle() const;
  //Coarse eta caluclated only using phi segments 
  int coarseEta() const;
  //Approximate Chi2 metric
  int approxChi2() const;

  //Approximate Chi2 metric
  int phiPattern() const;

  //Approximate Chi2 metric
  int etaCoarsePattern() const;

  //step;
  int step() const;


  //Four vector for displaced particles
  const PolarLorentzVector& unconstrainedP4() const;

  //Associated stubs
  const StubRefVector& stubs() const;

  //Set coordinates in muon system
  void setCoordinates(int,int,int,int );

  //Set coordinates at vertex
  void setCoordinatesAtVertex(int,int,int );

  //Set eta coarse and pattern
  void setCoarseEta(int,int);

  //Set phi hit pattern
  void setHitPattern(int);

  //Set chi2 like metric
  void setApproxChi2(int);

  //Set floating point coordinates for studies
  void setPtEtaPhi(double,double,double,bool);

  //Add a stub
  void addStub(const StubRef&); 
  
  //For offline usage
  CovarianceMatrix covariance;

 private:
  //Floating point coordinates for studies
  PolarLorentzVector unconstrainedP4_;


  StubRefVector stubs_;

  //vertex coordinates
  int curvVertex_;
  int phiVertex_;
  int dxy_;

  //muon coordinates
  int curv_;
  int phi_;
  int phiB_; 
  //common coordinates
  int coarseEta_;

  //Approximate Chi2 metric
  int approxChi2_;

  //phi bitmask
  int hitPattern_;

  //eta pattern
  int etaCoarsePattern_;

  //propagation step
  int step_;

  
}; 

#endif

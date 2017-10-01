/*
Kalman Filter L1 Muon algorithm
Michalis Bachtis (UCLA)
Sep. 2017
*/

#ifndef L1MuonKF_H
#define L1MuonKF_H

#include "DataFormats/L1KalmanMuonTrigger/interface/L1KalmanMuTrack.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class L1MuonKF {
 public:
  typedef L1KalmanMuTrack::StubRefVector StubRefVector;
  typedef L1KalmanMuTrack::StubRef StubRef;
  typedef std::vector<L1KalmanMuTrack> TrackVector;
  typedef ROOT::Math::SVector<double,2> Vector2;
  typedef ROOT::Math::SMatrix<double,2,2,ROOT::Math::MatRepSym<double,2> > CovarianceMatrix2;
  typedef ROOT::Math::SMatrix<double,3,2> Matrix32;
  typedef ROOT::Math::SMatrix<double,2,3> Matrix23;
  typedef ROOT::Math::SMatrix<double,1,3> Matrix13;
  typedef ROOT::Math::SMatrix<double,3,1> Matrix31;

  L1MuonKF(const edm::ParameterSet& settings);
  ~L1MuonKF();
  TrackVector  process(const StubRef&, const StubRefVector&);  
 

 private:
  int correctedPhi(const StubRef&,int);
  void propagate(L1KalmanMuTrack&);
  bool update(L1KalmanMuTrack&,const StubRefVector&,int);
  bool updateOffline(L1KalmanMuTrack&,const StubRefVector&,int);
  void vertexConstraint(L1KalmanMuTrack&);
  void vertexConstraintOffline(L1KalmanMuTrack&);
  int etaBitmask(const L1KalmanMuTrack&);
  int etaLookup(const L1KalmanMuTrack&);
  int phiBitmask(const L1KalmanMuTrack&);
  int customBitmask(bool,bool,bool,bool);
  bool getBit(int,int);
  void setFloatingPointValues(L1KalmanMuTrack&,bool);
  //propagation coefficients
  const std::vector<int>& eLoss_;
  const std::vector<int>& aPhi_;
  const std::vector<int>& bPhi_;
  const std::vector<int>& aPhiB_;
  const std::vector<int>& bPhiB_;
  //Denominator for the precision used
  int denominator_;


  //STUFF NOT USED IN THE FIRMWARE BUT ONLY FOR DEBUGGING
  ///////////////////////////////////////////////////////

  bool useOfflineAlgo_;
  const std::vector<int>& mScatteringPhi_;
  const std::vector<int>& mScatteringPhiB_;
  //point resolution for phi
  int pointResolutionPhi_;
  //point resolution for phiB
  int pointResolutionPhiB_;
  //point resolution for vertex
  int pointResolutionVertex_;

 


};
#endif




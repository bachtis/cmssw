
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
  typedef ROOT::Math::SMatrix<double,3,3> Matrix33;

  L1MuonKF(const edm::ParameterSet& settings);

  TrackVector  process(const StubRef&, const StubRefVector&);  
  TrackVector  cleanAndSort(const TrackVector&);  

 

 private:
  bool verbose_;
  int correctedPhi(const StubRef&,int);
  int correctedPhiB(const StubRef&);
  void propagate(L1KalmanMuTrack&);
  bool update(L1KalmanMuTrack&,const StubRefVector&,int);
  bool updateOffline(L1KalmanMuTrack&,const StubRefVector&,int);
  void vertexConstraint(L1KalmanMuTrack&);
  void vertexConstraintOffline(L1KalmanMuTrack&);
  int etaBitmask(const L1KalmanMuTrack&);
  bool etaLookup(L1KalmanMuTrack&);
  int phiBitmask(const L1KalmanMuTrack&);
  int customBitmask(unsigned int,unsigned int,unsigned int,unsigned int);
  bool getBit(int,int);
  void setFloatingPointValues(L1KalmanMuTrack&,bool);
  void estimateChiSquare(L1KalmanMuTrack&);

  int rank(const L1KalmanMuTrack&);
  int wrapAround(int ,int);

  //propagation coefficients
  std::vector<double> eLoss_;
  std::vector<double> aPhi_;
  std::vector<double> aPhiB_;
  std::vector<double> aPhiBNLO_;
  std::vector<double> bPhi_;
  std::vector<double> bPhiB_;

  //coarse eta LUT
  std::vector<int> etaLUTAddr_;
  std::vector<int> etaLUTVal_;


  //Denominator for the precision used
  int denominator_;

  //Chi Square estimator input
  std::vector<double> chiSquare_;
  int chiSquareCut_;

  //STUFF NOT USED IN THE FIRMWARE BUT ONLY FOR DEBUGGING
  ///////////////////////////////////////////////////////

  bool useOfflineAlgo_;
  std::vector<double> mScatteringPhi_;
  std::vector<double> mScatteringPhiB_;
  //point resolution for phi
  double pointResolutionPhi_;
  //point resolution for phiB
  double pointResolutionPhiB_;
  //point resolution for vertex
  double pointResolutionVertex_;

  
  
 


};
#endif




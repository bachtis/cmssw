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

  L1KalmanKF(const edm::ParameterSet& settings);
  ~L1KalmanKF();



  TrackVector  process(const StubRef&, const StubRefVector&);  

  

 private:
  int correctedPhi(const StubRef&,int);
  void propagate(L1KalmanMuTrack&);
  bool updateOffline(L1KalmanMuTrack&,const StubRefVector&,int);
  void vertexConstraint(L1KalmanMuTrack&);

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
 


}
#endif




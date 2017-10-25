#include <DataFormats/L1KalmanMuonTrigger/interface/L1KalmanMuTrack.h>
#include <DataFormats/Common/interface/Wrapper.h>
#include "DataFormats/Common/interface/Ref.h"


namespace DataFormats_L1KalmanMuonTrigger {
  struct dictionary {
    L1KalmanMuTrack kf_trk;
    std::vector<L1KalmanMuTrack> kf_trackV;
    edm::Ref<std::vector<L1KalmanMuTrack> > kf_trackVRef;
    edm::reftobase::Holder<reco::Candidate,edm::Ref<std::vector<L1KalmanMuTrack> > > kf_rtbkalmantrack;
    edm::Wrapper<std::vector<L1KalmanMuTrack> > w_kf_trackV;

  };
}

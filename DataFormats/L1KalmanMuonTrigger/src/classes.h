#include <DataFormats/L1KalmanMuonTrigger/interface/L1KalmanMuTrack.h>
#include <DataFormats/Common/interface/Wrapper.h>
#include "DataFormats/Common/interface/Ref.h"


namespace DataFormats_L1KalmanMuonTrigger {
  struct dictionary {
    L1KalmanMuTrack kf_trk;

    std::vector<L1KalmanMuTrack> kf_trackV;
    edm::Wrapper<std::vector<L1KalmanMuTrack> > w_kf_trackV;
  };
}

#include "L1Trigger/L1TMuonBarrel/interface/L1TMuonBarrelKalmanLUTs.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

L1TMuonBarrelKalmanLUTs::L1TMuonBarrelKalmanLUTs(const std::string& filename) {
  edm::FileInPath path(filename);
  lutFile_ = new TFile(path.fullPath().c_str());
  lut_[3*64+8] = (TH1*)lutFile_->Get("gain_3_8");
  lut_[2*64+4] = (TH1*)lutFile_->Get("gain_2_4");
  lut_[2*64+8] = (TH1*)lutFile_->Get("gain_2_8");
  lut_[2*64+12] = (TH1*)lutFile_->Get("gain_2_12");
  lut_[1*64+2] = (TH1*)lutFile_->Get("gain_1_2");
  lut_[1*64+4] = (TH1*)lutFile_->Get("gain_1_4");
  lut_[1*64+6] = (TH1*)lutFile_->Get("gain_1_6");
  lut_[1*64+8] = (TH1*)lutFile_->Get("gain_1_8");
  lut_[1*64+10] = (TH1*)lutFile_->Get("gain_1_10");
  lut_[1*64+12] = (TH1*)lutFile_->Get("gain_1_12");
  lut_[1*64+14] = (TH1*)lutFile_->Get("gain_1_14");
  lut_[3] = (TH1*)lutFile_->Get("gain_0_3");
  lut_[5] = (TH1*)lutFile_->Get("gain_0_5");
  lut_[6] = (TH1*)lutFile_->Get("gain_0_6");
  lut_[7] = (TH1*)lutFile_->Get("gain_0_7");
  lut_[9] = (TH1*)lutFile_->Get("gain_0_9");
  lut_[10] = (TH1*)lutFile_->Get("gain_0_10");
  lut_[11] = (TH1*)lutFile_->Get("gain_0_11");
  lut_[12] = (TH1*)lutFile_->Get("gain_0_12");
  lut_[13] = (TH1*)lutFile_->Get("gain_0_13");
  lut_[14] = (TH1*)lutFile_->Get("gain_0_14");
  lut_[15] = (TH1*)lutFile_->Get("gain_0_15");

}

L1TMuonBarrelKalmanLUTs::~L1TMuonBarrelKalmanLUTs() {
  lutFile_->Close();
  if (lutFile_ !=0)
    delete lutFile_;
}


std::vector<float> L1TMuonBarrelKalmanLUTs::trackGain(uint step,uint bitmask,uint K) {
  std::vector<float> gain(4,0.0);
  const TH1* h = lut_[64*step+bitmask];
  gain[0] = h->GetBinContent(K+1);
  gain[1] = h->GetBinContent(512+K+1);
  gain[2] = -h->GetBinContent(2*512+K+1);
  gain[3] = h->GetBinContent(3*512+K+1);
  return gain;
}


std::pair<float,float> L1TMuonBarrelKalmanLUTs::vertexGain(uint bitmask,uint K) {
  const TH1* h = lut_[bitmask];
  std::pair<float,float> gain(-h->GetBinContent(K+1),-h->GetBinContent(1024+K+1) );
  return gain;
}

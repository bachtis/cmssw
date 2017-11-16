#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/L1KalmanMuonTrigger/interface/L1KalmanMuTrack.h"
//
// class declaration
//

class L1KalmanMuonTrackSelector : public edm::stream::EDProducer<> {
   public:
      explicit L1KalmanMuonTrackSelector(const edm::ParameterSet&);
      ~L1KalmanMuonTrackSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
  edm::EDGetTokenT<std::vector<L1KalmanMuTrack> > src_;
  uint maxChi2_;
  uint rankFactorSeg_;
  uint rankFactorQual_;
  bool verbose_;
  

  bool  keep(const L1KalmanMuTrack&, const L1KalmanMuTrack& );
  bool  cut(const L1KalmanMuTrack&);
  

};
L1KalmanMuonTrackSelector::L1KalmanMuonTrackSelector(const edm::ParameterSet& iConfig):
  src_(consumes<std::vector<L1KalmanMuTrack> >(iConfig.getParameter<edm::InputTag>("src"))),
  maxChi2_(iConfig.getParameter<uint>("maxChi2")),
  rankFactorSeg_(iConfig.getParameter<uint>("rankFactorSeg")),
  rankFactorQual_(iConfig.getParameter<uint>("rankFactorQual")),
  verbose_(iConfig.getParameter<bool>("verbose"))
{
  produces <std::vector<L1KalmanMuTrack> >();
}


L1KalmanMuonTrackSelector::~L1KalmanMuonTrackSelector()
{
}

 


//
// member functions
//



bool
L1KalmanMuonTrackSelector::cut(const L1KalmanMuTrack& track)
{
  if (track.approxChi2()>int(maxChi2_*track.stubs().size()))
    return false;
  return true;
}


bool
L1KalmanMuonTrackSelector::keep(const L1KalmanMuTrack& track1,const L1KalmanMuTrack& track2)
{
  int rank1  = track1.stubs().size()*rankFactorSeg_ + track1.quality()*rankFactorQual_ - track1.approxChi2();
  int rank2  = track2.stubs().size()*rankFactorSeg_ + track2.quality()*rankFactorQual_ - track2.approxChi2();
  if (rank1<rank2)
    return false;
  return true;
}




// ------------ method called to produce the data  ------------
void
L1KalmanMuonTrackSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   Handle<std::vector<L1KalmanMuTrack> >trackHandle;
   iEvent.getByToken(src_,trackHandle);
   if (verbose_) 
     printf("Preselected Kalman Tracks-------------\n");

   std::vector<L1KalmanMuTrack> cleaned;

   for (const auto&  track1 : (*trackHandle.product()))  {
     if (verbose_)
       printf("Preselected Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA =%d stubs=%d chi2=%d pts=%f %f\n",track1.charge(),track1.pt(),track1.eta(),track1.phi(),track1.curvatureAtVertex(),track1.curvatureAtMuon(),int(track1.stubs().size()),track1.approxChi2(),track1.pt(),track1.unconstrainedP4().pt()); 
     bool keepTrack=true;
     for (const auto&  track2 : (*trackHandle.product()))  {
       if ((track1 == track2) || (!track1.overlap(track2)))
	 continue;
       if (!keep(track1,track2))
	 keepTrack=false;
     }
     if (keepTrack)
       cleaned.push_back(track1);
   }

   std::vector<L1KalmanMuTrack> out;
   if (verbose_) 
     printf("Final Kalman Tracks-------------\n");

   for (const auto& track : cleaned)
     if (cut(track)) {
     if (verbose_)
       printf("Final Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA =%d stubs=%d chi2=%d pts=%f %f\n",track.charge(),track.pt(),track.eta(),track.phi(),track.curvatureAtVertex(),track.curvatureAtMuon(),int(track.stubs().size()),track.approxChi2(),track.pt(),track.unconstrainedP4().pt()); 

       out.push_back(track);
     }

   if (verbose_) {
     printf("-----------------------------------\n");
     printf("-----------------------------------\n");
     printf("-----------------------------------\n");
   }
   iEvent.put(std::make_unique<std::vector<L1KalmanMuTrack> >(out));
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
L1KalmanMuonTrackSelector::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
L1KalmanMuonTrackSelector::endStream() {
}

void
L1KalmanMuonTrackSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1KalmanMuonTrackSelector);

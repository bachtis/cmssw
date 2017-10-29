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
  int maxCurvature_;
  int maxLocalChi2_;
  int chiSquareOffset_;
  bool verbose_;

  bool  keep(const L1KalmanMuTrack&, const L1KalmanMuTrack& );
  void clean(std::vector<L1KalmanMuTrack>&, const std::vector<L1KalmanMuTrack>& );
  bool  preselect(const L1KalmanMuTrack&);
  

};
L1KalmanMuonTrackSelector::L1KalmanMuonTrackSelector(const edm::ParameterSet& iConfig):
  src_(consumes<std::vector<L1KalmanMuTrack> >(iConfig.getParameter<edm::InputTag>("src"))),
  maxCurvature_(iConfig.getParameter<int>("maxCurvature")),
  maxLocalChi2_(iConfig.getParameter<int>("maxLocalChi2")),
  chiSquareOffset_(iConfig.getParameter<int>("chiSquareOffset")),
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
L1KalmanMuonTrackSelector::preselect(const L1KalmanMuTrack& track)
{
  if (track.approxChi2()>maxLocalChi2_ || abs(track.curvatureAtVertex())>maxCurvature_)
    return false;
  return true;
}


bool
L1KalmanMuonTrackSelector::keep(const L1KalmanMuTrack& track1,const L1KalmanMuTrack& track2)
{
  int stubs1 = track1.stubs().size();
  int stubs2 = track2.stubs().size();
  if ((stubs1>stubs2) && track1.approxChi2()>chiSquareOffset_*track2.approxChi2())
    return false;
  else if ((stubs1<stubs2) && track1.approxChi2()*chiSquareOffset_>track2.approxChi2())
    return false;
  else if ((stubs1==stubs2) && (track1.approxChi2()>track2.approxChi2())) 
    return false;
  return true;
}


void 
L1KalmanMuonTrackSelector::clean(std::vector<L1KalmanMuTrack>& coll1, const std::vector<L1KalmanMuTrack>& coll2)
{
  std::vector<L1KalmanMuTrack> cleaned;
  for (const auto& t1 : coll1) {
    bool save=true;
    for (const auto& t2: coll2) {
      if (t1==t2)
	continue;
      if (!t1.overlap(t2))
	continue;  
      if(!keep(t1,t2)) {
	save=false;
	break;
      }
	
    }
    if (save)
      cleaned.push_back(t1);   
  }
  coll1.clear();
  for (const auto& track :cleaned)
    coll1.push_back(track);

}



// ------------ method called to produce the data  ------------
void
L1KalmanMuonTrackSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   Handle<std::vector<L1KalmanMuTrack> >trackHandle;
   iEvent.getByToken(src_,trackHandle);
   std::vector<L1KalmanMuTrack> tracks4;
   std::vector<L1KalmanMuTrack> tracks3;
   std::vector<L1KalmanMuTrack> tracks2;
   if (verbose_) 
     printf("Preselected Kalman Tracks-------------\n");
   for (const auto&  track : (*trackHandle.product()))  {
     if (!preselect(track))
       continue;

     if (verbose_)
       printf("Preselected Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA =%d stubs=%d chi2=%d pts=%f %f\n",track.charge(),track.pt(),track.eta(),track.phi(),track.curvatureAtVertex(),track.curvatureAtMuon(),int(track.stubs().size()),track.approxChi2(),track.pt(),track.unconstrainedP4().pt()); 


     switch(track.stubs().size()) {
     case 4:
       tracks4.push_back(track);
       break;
     case 3:
       tracks3.push_back(track);
       break;
     case 2:
       tracks2.push_back(track);
       break;
     default:
       printf("Less than 2 stubs?This should never happen\n");
     } 
   }

   if (verbose_) {
     printf("-----------------------------------\n");
     printf("-----------------------------------\n");
     printf("-----------------------------------\n");
   }

   clean(tracks4,tracks4);  
   clean(tracks2,tracks2);
   clean(tracks3,tracks3);



   clean(tracks4,tracks3);  
   clean(tracks4,tracks2);  

   clean(tracks3,tracks4);
   clean(tracks3,tracks2);

   clean(tracks2,tracks4);
   clean(tracks2,tracks3);

   std::vector<L1KalmanMuTrack> out;


   if (verbose_) 
     printf("Selected Kalman Tracks-------------\n");
   for (const auto& track : tracks4) {
     if (verbose_)
       printf("Final Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA=%d stubs=%d chi2=%d\n",track.charge(),track.pt(),track.eta(),track.phi(),track.curvatureAtVertex(),track.curvatureAtMuon(),int(track.stubs().size()),track.approxChi2()); 
     out.push_back(track);
   }
   for (const auto& track : tracks3) {
     if (verbose_)
       printf("Final Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA=%d stubs=%d chi2=%d\n",track.charge(),track.pt(),track.eta(),track.phi(),track.curvatureAtVertex(),track.curvatureAtMuon(),int(track.stubs().size()),track.approxChi2()); 
  


     out.push_back(track);
   }
   for (const auto& track : tracks2) {
     if (verbose_)
       printf("Final Kalman Track charge=%d pt=%f eta=%f phi=%f curvature=%d curvature STA=%d stubs=%d chi2=%d\n",track.charge(),track.pt(),track.eta(),track.phi(),track.curvatureAtVertex(),track.curvatureAtMuon(),int(track.stubs().size()),track.approxChi2()); 

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

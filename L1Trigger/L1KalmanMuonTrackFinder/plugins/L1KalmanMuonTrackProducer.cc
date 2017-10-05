#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
#include "DataFormats/L1KalmanMuonTrigger/interface/L1KalmanMuTrack.h"
#include "L1Trigger/L1KalmanMuonTrackFinder/interface/L1MuonKF.h"
//
// class declaration
//

class L1KalmanMuonTrackProducer : public edm::stream::EDProducer<> {
   public:
      explicit L1KalmanMuonTrackProducer(const edm::ParameterSet&);
      ~L1KalmanMuonTrackProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
  edm::EDGetTokenT<std::vector<L1MuDTChambPhDigi> > src_;
  L1MuonKF *kalmanFilter_;
  L1MuonKF::StubRefVector collect(const L1MuonKF::StubRefVector&,L1MuonKF::StubRefVector&, int, int );
  

};
L1KalmanMuonTrackProducer::L1KalmanMuonTrackProducer(const edm::ParameterSet& iConfig):
  src_(consumes<std::vector<L1MuDTChambPhDigi> >(iConfig.getParameter<edm::InputTag>("src"))),
  kalmanFilter_(new L1MuonKF(iConfig.getParameter<edm::ParameterSet>("algo")))
{
  produces <std::vector<L1KalmanMuTrack> >();
}


L1KalmanMuonTrackProducer::~L1KalmanMuonTrackProducer()
{
 
  if (kalmanFilter_!=0)
    delete kalmanFilter_;
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}




L1MuonKF::StubRefVector L1KalmanMuonTrackProducer::collect(const L1MuonKF::StubRefVector& in,L1MuonKF::StubRefVector& seeds,int sector,int wheel ){
  L1MuonKF::StubRefVector output;
  for (const auto& stub : in) {
     
    if ((stub->scNum()==sector) && (stub->whNum()==wheel) && (stub->stNum() !=1)) {
     seeds.push_back(stub);
    }
    if ( (abs(stub->scNum()-sector)<=1) ||(stub->scNum()==0 && sector==11) || (stub->scNum()==11 && sector==0)) {
      if (abs(stub->whNum()-wheel)<=1) {
	output.push_back(stub);
      }
    }
  }
  return output;
}
 


//
// member functions
//

// ------------ method called to produce the data  ------------
void
L1KalmanMuonTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   Handle<std::vector<L1MuDTChambPhDigi> >stubHandle;
   std::unique_ptr<std::vector<L1KalmanMuTrack> > out(new std::vector<L1KalmanMuTrack>);
   iEvent.getByToken(src_,stubHandle);

   L1MuonKF::StubRefVector stubs;
   for (unsigned int i=0;i<stubHandle->size();++i) {
     L1MuonKF::StubRef r(stubHandle,i);
     stubs.push_back(r);
   }
  
   for (int sector=0;sector<12;++sector) {
     for (int wheel=-2;wheel<3;++wheel) {
       L1MuonKF::StubRefVector seeds;
       L1MuonKF::StubRefVector inputs = collect(stubs,seeds,sector,wheel);
       for (const auto& seed: seeds) {
	 L1MuonKF::TrackVector tracks = kalmanFilter_->process(seed,inputs);
	 for (const auto& t : tracks) 
	   out->push_back(t);
       }
     }
   }
   iEvent.put(std::move(out));

}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
L1KalmanMuonTrackProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
L1KalmanMuonTrackProducer::endStream() {
}

void
L1KalmanMuonTrackProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1KalmanMuonTrackProducer);

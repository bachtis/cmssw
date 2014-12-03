#include "HLTrigger/special/interface/FakeVertexProducer.h"

//
// static data member definitions
//

//
// constructors and destructor
//
FakeVertexProducer::FakeVertexProducer(const edm::ParameterSet& iConfig)
{
  beamspotToken_ = consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("src"));
  produces<reco::VertexCollection>();
  
}


FakeVertexProducer::~FakeVertexProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
FakeVertexProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<reco::BeamSpot> beamspotH;
   iEvent.getByToken(beamspotToken_,beamspotH);

   std::auto_ptr<reco::VertexCollection> out(new reco::VertexCollection);

   out->push_back(reco::Vertex(beamspotH->position(),beamspotH->covariance3D()));
   iEvent.put(out);
 }

// ------------ method called once each job just before starting event loop  ------------
void 
FakeVertexProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
FakeVertexProducer::endJob() {
}
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FakeVertexProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(FakeVertexProducer);

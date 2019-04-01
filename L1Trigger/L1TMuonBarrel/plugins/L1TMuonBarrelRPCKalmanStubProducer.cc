#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"


#include "L1Trigger/L1TMuonBarrel/interface/L1TMuonBarrelRPCKalmanStubProcessor.h"

//For masks

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class L1TMuonBarrelRPCKalmanStubProducer : public edm::stream::EDProducer<> {
   public:
      explicit L1TMuonBarrelRPCKalmanStubProducer(const edm::ParameterSet&);
      ~L1TMuonBarrelRPCKalmanStubProducer() override;

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      void beginStream(edm::StreamID) override;
      void produce(edm::Event&, const edm::EventSetup&) override;
      void endStream() override;
  edm::EDGetTokenT<RPCDigiCollection> src_;
  L1TMuonBarrelRPCKalmanStubProcessor * proc_;
  int verbose_;



};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
L1TMuonBarrelRPCKalmanStubProducer::L1TMuonBarrelRPCKalmanStubProducer(const edm::ParameterSet& iConfig):
  src_(consumes<RPCDigiCollection>(iConfig.getParameter<edm::InputTag>("srcPhi"))),
  proc_(new L1TMuonBarrelRPCKalmanStubProcessor(iConfig)),
  verbose_(iConfig.getParameter<int>("verbose"))
{
  produces <L1MuKBMTCombinedStubCollection>();
}


L1TMuonBarrelRPCKalmanStubProducer::~L1TMuonBarrelRPCKalmanStubProducer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)
  if (proc_!=nullptr)
    delete proc_;
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
L1TMuonBarrelRPCKalmanStubProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   Handle<RPCDigiCollection> phiIn;
   iEvent.getByToken(src_,phiIn);

   RPCtoDTTranslator translator(*phiIn);
   translator.run(iSetup);

   L1MuKBMTCombinedStubCollection stubs = proc_->makeStubs(translator.getDTRPCHitsContainer());
   if (verbose_==1)
     for (const auto& stub : stubs) {
       printf("Stub: wheel=%d sector=%d station =%d tag=%d eta1=%d qeta1=%d eta2=%d qeta2=%d\n",stub.whNum(),stub.scNum(),stub.stNum(),stub.tag(),stub.eta1(),stub.qeta1(),stub.eta2(),stub.qeta2());  
     }
   iEvent.put(std::make_unique<L1MuKBMTCombinedStubCollection>(stubs));
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
L1TMuonBarrelRPCKalmanStubProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
L1TMuonBarrelRPCKalmanStubProducer::endStream() {
}

void
L1TMuonBarrelRPCKalmanStubProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TMuonBarrelRPCKalmanStubProducer);

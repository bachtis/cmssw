#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
//
// class declaration
//

class L1BarrelStubSelector : public edm::stream::EDProducer<> {
   public:
      explicit L1BarrelStubSelector(const edm::ParameterSet&);
      ~L1BarrelStubSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
  edm::EDGetTokenT<L1MuDTChambPhContainer> src_;
  int minQuality_;
  bool onlyBX0_;
  

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
L1BarrelStubSelector::L1BarrelStubSelector(const edm::ParameterSet& iConfig):
  src_(consumes<L1MuDTChambPhContainer>(iConfig.getParameter<edm::InputTag>("src"))),
  minQuality_(iConfig.getParameter<int>("minQuality")),
  onlyBX0_(iConfig.getParameter<bool>("onlyBX0"))
{
  produces <std::vector<L1MuDTChambPhDigi>>();
}


L1BarrelStubSelector::~L1BarrelStubSelector()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
L1BarrelStubSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   Handle<L1MuDTChambPhContainer> phiIn;
   iEvent.getByToken(src_,phiIn);

   std::vector<L1MuDTChambPhDigi> out;

   for (const auto& seg : *(phiIn.product()->getContainer())) {
     if (seg.code()<minQuality_)
       continue;
     if ((seg.bxNum()==0 && onlyBX0_) || (!onlyBX0_)) {
       //we also do not want stubs at st = 1 and wheel +=2 due to overlap
       if (abs(seg.whNum())==2 && seg.stNum()==1)
	 continue;
       out.push_back(seg);
     }
   }


   iEvent.put(std::make_unique<std::vector<L1MuDTChambPhDigi> >(out));
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
L1BarrelStubSelector::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
L1BarrelStubSelector::endStream() {
}

void
L1BarrelStubSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1BarrelStubSelector);

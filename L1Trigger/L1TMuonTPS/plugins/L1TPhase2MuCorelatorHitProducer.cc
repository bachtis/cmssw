
#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "L1Trigger/L1TMuonTPS/interface/L1TMuCorrelatorPhase2DTStubProcessor.h"
#include "L1Trigger/L1TMuonTPS/interface/L1TMuCorrelatorRPCBarrelStubProcessor.h"
#include "L1Trigger/L1TMuonTPS/interface/L1TMuCorrelatorRPCEndcapStubProcessor.h"
#include "L1Trigger/L1TMuonTPS/interface/L1TMuCorrelatorCSCStubProcessor.h"

//For masks
#include "CondFormats/L1TObjects/interface/L1TMuonBarrelParams.h"
#include "CondFormats/DataRecord/interface/L1TMuonBarrelParamsRcd.h"
#include "CondFormats/L1TObjects/interface/L1MuDTTFMasks.h"
#include "CondFormats/DataRecord/interface/L1MuDTTFMasksRcd.h"
#include "L1Trigger/L1TMuon/interface/GeometryTranslator.h"



//
// class declaration
//

class L1TPhase2MuCorrelatorHitProducer : public edm::stream::EDProducer<> {
   public:
      explicit L1TPhase2MuCorrelatorHitProducer(const edm::ParameterSet&);
      ~L1TPhase2MuCorrelatorHitProducer() override;

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      void beginStream(edm::StreamID) override;
      void produce(edm::Event&, const edm::EventSetup&) override;
      void endStream() override;

  edm::EDGetTokenT<MuonDigiCollection<CSCDetId,CSCCorrelatedLCTDigi> > srcCSC_;
  edm::EDGetTokenT<L1Phase2MuDTPhContainer> srcDT_;
  edm::EDGetTokenT<L1MuDTChambThContainer> srcDTTheta_;
  edm::EDGetTokenT<RPCDigiCollection> srcRPC_;


  L1TMuCorrelatorPhase2DTStubProcessor * procDT_;
  L1TMuCorrelatorRPCBarrelStubProcessor * procRPCBarrel_;
  L1TMuCorrelatorRPCEndcapStubProcessor * procRPCEndcap_;
  L1TMuCorrelatorCSCStubProcessor * procCSC_;
  edm::ESHandle< L1TMuonBarrelParams > bmtfParamsHandle_;
  L1TMuon::GeometryTranslator * translator_;


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
L1TPhase2MuCorrelatorHitProducer::L1TPhase2MuCorrelatorHitProducer(const edm::ParameterSet& iConfig):
  srcCSC_(consumes<MuonDigiCollection<CSCDetId,CSCCorrelatedLCTDigi> > (iConfig.getParameter<edm::InputTag>("srcCSC"))),
  srcDT_(consumes<L1Phase2MuDTPhContainer>(iConfig.getParameter<edm::InputTag>("srcDT"))),
  srcDTTheta_(consumes<L1MuDTChambThContainer>(iConfig.getParameter<edm::InputTag>("srcDTTheta"))),
  srcRPC_(consumes<RPCDigiCollection>(iConfig.getParameter<edm::InputTag>("srcRPC"))),
  procDT_(new L1TMuCorrelatorPhase2DTStubProcessor(iConfig.getParameter<edm::ParameterSet> ("DT"))),
  procRPCBarrel_(new L1TMuCorrelatorRPCBarrelStubProcessor(iConfig.getParameter<edm::ParameterSet> ("RPCBarrel"))),
  procRPCEndcap_(new L1TMuCorrelatorRPCEndcapStubProcessor(iConfig.getParameter<edm::ParameterSet> ("RPCEndcap"))),
  procCSC_(new L1TMuCorrelatorCSCStubProcessor(iConfig.getParameter<edm::ParameterSet> ("CSC"))),
  translator_(new L1TMuon::GeometryTranslator),
  verbose_(iConfig.getParameter<int>("verbose"))
{
  produces <L1MuCorrelatorHitCollection>();
}


L1TPhase2MuCorrelatorHitProducer::~L1TPhase2MuCorrelatorHitProducer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)
  if (procDT_!=nullptr)
    delete procDT_;
  if (procRPCBarrel_!=nullptr)
    delete procRPCBarrel_;
  if (procRPCEndcap_!=nullptr)
    delete procRPCEndcap_;

  if (procCSC_!=nullptr)
    delete procCSC_;
  if (translator_!=nullptr)
    delete translator_;



}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
L1TPhase2MuCorrelatorHitProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   translator_->checkAndUpdateGeometry(iSetup);


   Handle<MuonDigiCollection<CSCDetId,CSCCorrelatedLCTDigi> > cscDigis;
   iEvent.getByToken(srcCSC_,cscDigis);


   Handle<L1Phase2MuDTPhContainer> dtDigis;
   iEvent.getByToken(srcDT_,dtDigis);

   Handle<L1MuDTChambThContainer> dtThetaDigis;
   iEvent.getByToken(srcDTTheta_,dtThetaDigis);


   Handle<RPCDigiCollection> rpcDigis;
   iEvent.getByToken(srcRPC_,rpcDigis);

   RPCtoDTTranslator translator(*rpcDigis);
   translator.run(iSetup);




   //Get parameters

   const L1TMuonBarrelParamsRcd& bmtfParamsRcd = iSetup.get<L1TMuonBarrelParamsRcd>();
   bmtfParamsRcd.get(bmtfParamsHandle_);
   const L1TMuonBarrelParams& bmtfParams = *bmtfParamsHandle_.product();

   L1MuCorrelatorHitCollection stubs= procCSC_->makeStubs(*cscDigis,translator_);
   L1MuCorrelatorHitCollection stubsRPCBarrel = procRPCBarrel_->makeStubs(translator.getDTRPCHitsContainer());
   std::copy (stubsRPCBarrel.begin(), stubsRPCBarrel.end(), std::back_inserter(stubs));

   L1MuCorrelatorHitCollection stubsRPCEndcap = procRPCEndcap_->makeStubs(*rpcDigis,translator_,iSetup);
   std::copy (stubsRPCEndcap.begin(), stubsRPCEndcap.end(), std::back_inserter(stubs));

   L1MuCorrelatorHitCollection stubsDT = procDT_->makeStubs(dtDigis.product(),dtThetaDigis.product(),bmtfParams);
   std::copy (stubsDT.begin(), stubsDT.end(), std::back_inserter(stubs));
   iEvent.put(std::make_unique<L1MuCorrelatorHitCollection>(stubs));
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
L1TPhase2MuCorrelatorHitProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
L1TPhase2MuCorrelatorHitProducer::endStream() {
}

void
L1TPhase2MuCorrelatorHitProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TPhase2MuCorrelatorHitProducer);

// -*- C++ -*-
//
// Package:    HLTrigger/FakeVertexProducer
// Class:      FakeVertexProducer
// 
/**\class FakeVertexProducer FakeVertexProducer.cc HLTrigger/FakeVertexProducer/plugins/FakeVertexProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Michail Bachtis
//         Created:  Wed, 03 Dec 2014 12:31:52 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
//
// class declaration
//

class FakeVertexProducer : public edm::EDProducer {
   public:
      explicit FakeVertexProducer(const edm::ParameterSet&);
      ~FakeVertexProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

  edm::EDGetTokenT<reco::BeamSpot> beamspotToken_;

};

#ifndef EEPedestalOnlineTask_H
#define EEPedestalOnlineTask_H

/*
 * \file EEPedestalOnlineTask.h
 *
 * \author G. Della Ricca
 *
*/

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

class MonitorElement;
class DQMStore;

class EEPedestalOnlineTask: public edm::EDAnalyzer{

public:

/// Constructor
EEPedestalOnlineTask(const edm::ParameterSet& ps);

/// Destructor
virtual ~EEPedestalOnlineTask();

protected:

/// Analyze
void analyze(const edm::Event& e, const edm::EventSetup& c);

/// BeginJob
void beginJob(void);

/// EndJob
void endJob(void);

/// BeginRun
void beginRun(const edm::Run & r, const edm::EventSetup & c);

/// EndRun
void endRun(const edm::Run & r, const edm::EventSetup & c);

/// Reset
void reset(void);

/// Setup
void setup(void);

/// Cleanup
void cleanup(void);

private:

int ievt_;

DQMStore* dqmStore_;

std::string prefixME_;

 std::string subfolder_;

bool enableCleanup_;

bool mergeRuns_;

edm::EDGetTokenT<EEDigiCollection> EEDigiCollection_;

MonitorElement* mePedMapG12_[18];

bool init_;

};

#endif

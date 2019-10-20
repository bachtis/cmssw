#ifndef HcalCommonData_CaloSimParametersFromDD_h
#define HcalCommonData_CaloSimParametersFromDD_h

#include "DetectorDescription/Core/interface/DDsvalues.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include <DetectorDescription/DDCMS/interface/DDCompactView.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <string>
#include <vector>

class DDFilteredView;
class CaloSimulationParameters;

class CaloSimParametersFromDD {
public:
  CaloSimParametersFromDD() {}
  virtual ~CaloSimParametersFromDD() {}

  bool build(const DDCompactView*, CaloSimulationParameters&);
  bool build(const cms::DDCompactView*, CaloSimulationParameters&);

private:
  bool buildParameters(const CaloSimulationParameters&);
  std::vector<std::string> getNames(const std::string&, const DDsvalues_type&, bool);
  std::vector<int> getNumbers(const std::string&, const DDsvalues_type&, bool);

  template <typename T>
  void myPrint(std::string value, const std::vector<T>& vec) {
    edm::LogVerbatim("HCalGeom") << "CaloSimParametersFromDD: " << vec.size() << " entries for " << value << ":";
    for (unsigned int i = 0; i < vec.size(); i++)
      edm::LogVerbatim("HCalGeom") << " (" << i << ") " << vec[i];
  }
};

#endif

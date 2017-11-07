#ifndef CTPPSTimingVDetectorOrganization_h
#define CTPPSTimingVDetectorOrganization_h

#include "G4Step.hh"
#include <boost/cstdint.hpp>

class CTPPSTimingVDetectorOrganization {

public:
  CTPPSTimingVDetectorOrganization(){};
  virtual ~CTPPSTimingVDetectorOrganization(){};   
  virtual uint32_t GetUnitID(const  G4Step* aStep) const =0;
};      


#endif  //CTPPSTimingVDetectorOrganization_h



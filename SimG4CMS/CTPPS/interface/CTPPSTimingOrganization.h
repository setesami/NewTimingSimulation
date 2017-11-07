#ifndef CTPPS_CTPPSTimingOrganization_h
#define CTPPS_CTPPSTimingOrganization_h 

//***************************************************************************************************** Debugging
// Turns on and off the debug (Levels are: None, Error, Routine, Verbose, Trace and Debug).
// Comment the following line to turn off every kind of debug information.
// #define TotemT1Organization_DebugLevel 

//#ifdef TotemT1Organization_DebugLevel
//#include "Geometry/Totem/interface/DebugTools.h"
//#endif /* TotemT1Organization_DebugLevel */

// #define SCRIVI
//****************************************************************************************************** Includes

#include "globals.hh"
#include "SimG4CMS/CTPPS/interface/CTPPSTimingVDetectorOrganization.h"
#include "G4Step.hh"

class CTPPSTimingOrganization : public CTPPSTimingVDetectorOrganization
{
 public:
  inline CTPPSTimingOrganization();
  virtual ~CTPPSTimingOrganization();

  uint32_t GetUnitID(const G4Step* aStep);
  uint32_t GetUnitID(const G4Step* aStep) const;
};


inline CTPPSTimingOrganization :: CTPPSTimingOrganization()
{}

#endif  //CTPPS_CTPPSTimingOrganization_h



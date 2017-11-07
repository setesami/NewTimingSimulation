#ifndef CTPPS_CTPPSTimingNumberingScheme_h
#define CTPPS_CTPPSTimingNumberingScheme_h

#include "SimG4CMS/CTPPS/interface/CTPPSTimingOrganization.h"

class CTPPSTimingNumberingScheme : public CTPPSTimingOrganization {

public:
  CTPPSTimingNumberingScheme(int i);
  ~CTPPSTimingNumberingScheme();
	 
//  virtual unsigned int GetUnitID(const G4Step* aStep) const ;

};

#endif  //CTPPS_CTPPSTimingNumberingScheme_h



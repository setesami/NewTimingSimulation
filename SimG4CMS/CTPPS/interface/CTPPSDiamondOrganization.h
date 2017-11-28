// ///////////////////////
// Author Seyed Mohsen Etesami
// //////////////////////////////

#ifndef Forward_CTPPSDiamondOrganization_h
#define Forward_CTPPSDiamondOrganization_h 


#include "globals.hh"
#include "SimG4CMS/CTPPS/interface/CTPPSVDetectorOrganization.h"
#include "G4Step.hh"

class CTPPSDiamondOrganization : public CTPPSVDetectorOrganization
{
  public:
    CTPPSDiamondOrganization();
    virtual ~CTPPSDiamondOrganization();
 
    uint32_t GetUnitID(const G4Step* aStep);
    uint32_t GetUnitID(const G4Step* aStep) const;

  private:
    unsigned int theArm ;
    unsigned int theStation;
    unsigned int theRoman_pot;
    unsigned int thePlane;
    unsigned int theDetector ;

};



#endif //Forward_CTPPSDiamondOrganization_h

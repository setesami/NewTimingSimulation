//  //////////////////////
// Author Seyed Mohsen Etesami setesami@cern.ch
// ////////////////////////

#ifndef Forward_CTPPSDiamondNumberingScheme_h
#define Forward_CTPPSDiamondNumberingScheme_h

#include "SimG4CMS/CTPPS/interface/CTPPSDiamondOrganization.h"

class CTPPSDiamondNumberingScheme : public CTPPSDiamondOrganization 
{
  public:
    CTPPSDiamondNumberingScheme(int i);
    ~CTPPSDiamondNumberingScheme();
	 
    //  virtual unsigned int GetUnitID(const G4Step* aStep) const ;

};

#endif //Forward_CTPPSDiamondNumberingScheme_h

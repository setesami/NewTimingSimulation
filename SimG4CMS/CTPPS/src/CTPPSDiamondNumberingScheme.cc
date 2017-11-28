// //////////////////////
// Author Seyed Mohsen Etesami setesami@cern.ch
// ////////////////////////////

#include "SimG4CMS/CTPPS/interface/CTPPSDiamondNumberingScheme.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>

CTPPSDiamondNumberingScheme::CTPPSDiamondNumberingScheme(int i)
{
  edm::LogInfo("Diamond") << " Creating CTPPSDiamondNumberingScheme" << std::endl;
}

CTPPSDiamondNumberingScheme::~CTPPSDiamondNumberingScheme()
{
  edm::LogInfo("Diamond") << " Deleting CTPPSDiamondNumberingScheme" << std::endl;
}

#include "SimG4CMS/CTPPS/interface/CTPPSTimingNumberingScheme.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
// #define debug

CTPPSTimingNumberingScheme::CTPPSTimingNumberingScheme(int i)
{
  edm::LogInfo("CTPPS") << " Creating CTPPSTimingNumberingScheme" << std::endl;
}

CTPPSTimingNumberingScheme::~CTPPSTimingNumberingScheme()
{
  edm::LogInfo("CTPPSTiming") << " Deleting CTPPSTimingNumberingScheme" << std::endl;
}



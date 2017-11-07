#include "SimG4CMS/CTPPS/interface/CTPPSSD.h"
#include "SimG4CMS/CTPPS/interface/CTPPS_Timing_SD.h"


#include "SimG4Core/SensitiveDetector/interface/SensitiveDetectorPluginFactory.h"
#include "SimG4Core/Watcher/interface/SimWatcherFactory.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
  

typedef CTPPSSD CTPPSSensitiveDetector;
DEFINE_SENSITIVEDETECTOR(CTPPSSensitiveDetector);

typedef CTPPS_Timing_SD CTPPSTimingSensitiveDetector;
DEFINE_SENSITIVEDETECTOR(CTPPSTimingSensitiveDetector);






#ifndef SimPPS_CTPPSPixelDigiProducer_RPix_LINEAR_INDUCE_CHARGE_H
#define SimPPS_CTPPSPixelDigiProducer_RPix_LINEAR_INDUCE_CHARGE_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <vector>
//#include "SimPPS/PPSTrackerDigiProducer/interface/RPixSimTypes.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelSimTopology.h"
#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixSignalPoint.h"
class TFile;
class TH2D;
class RPixLinearInduceCharge
{
  public:
    RPixLinearInduceCharge(const edm::ParameterSet &params, uint32_t det_id);
//    SimRP::strip_charge_map 
std::map<unsigned short, double, std::less<unsigned short> > Induce(const std::vector<RPixSignalPoint> &charge_map);
  private:
    uint32_t det_id_;
    std::vector<double> signalCoupling_;
    std::map<unsigned short, double, std::less<unsigned short> > thePixelChargeMap;
    CTPPSPixelSimTopology theRPixDetTopology_;
    double sqrt_2;
    int no_of_pixels_;
    int verbosity_;
    std::string ChargeMapFile_;
    TFile * fChargeMap;
    TH2D * hChargeMap;
    //double sigmas_no_;
    //double active_edge_smearing_;
    //double bottom_edge_smearing_;
    //double top_edge_smearing_;
};

#endif

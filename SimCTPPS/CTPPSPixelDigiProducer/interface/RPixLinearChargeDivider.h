#ifndef SimCTPPS_CTPPSPixelDigiProducer_LINEAR_CHARGE_DIVIDER_H
#define SimCTPPS_CTPPSPixelDigiProducer_LINEAR_CHARGE_DIVIDER_H

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimTracker/Common/interface/SiG4UniversalFluctuation.h"
//#include "SimTotem/RPDigiProducer/interface/RPSimTypes.h"
#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixEnergyDepositUnit.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace CLHEP{
	class HepRandomEngine;
}


class RPixLinearChargeDivider
{
  public:
    RPixLinearChargeDivider(const edm::ParameterSet &params, CLHEP::HepRandomEngine& eng, uint32_t det_id);
    ~RPixLinearChargeDivider();
    //   SimRP::energy_path_distribution divide(const PSimHit& hit);
    std::vector<RPixEnergyDepositUnit> divide(const PSimHit& hit);
  private:
    const edm::ParameterSet &params_;
    CLHEP::HepRandomEngine& rndEngine;
    uint32_t _det_id;

    bool fluctuateCharge_;
//    int chargedivisionsPerStrip_;
//    int chargedivisionsPerThickness_;
    int chargedivisions_;
    double deltaCut_;
    double pitch_;
    double thickness_;
//    SimRP::energy_path_distribution the_energy_path_distribution_;
    std::vector<RPixEnergyDepositUnit> the_energy_path_distribution_;
    SiG4UniversalFluctuation * fluctuate; 
    int verbosity_;
    
    void FluctuateEloss(int pid, double particleMomentum, 
        double eloss, double length, int NumberOfSegs, 
       std::vector<RPixEnergyDepositUnit>  &elossVector);
};

#endif  //SimTotem_CTPPSPixelDigiProducer_LINEAR_CHARGE_DIVIDER_H

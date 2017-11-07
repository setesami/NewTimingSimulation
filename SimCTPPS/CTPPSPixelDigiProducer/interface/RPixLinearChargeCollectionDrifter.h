#ifndef SimCTPPS_CTPPSPixelDigiProducer_RPix_LINEAR_CHARGE_COLLECTION_DRIFTER_H
#define SimCTPPS_CTPPSPixelDigiProducer_RPix_LINEAR_CHARGE_COLLECTION_DRIFTER_H

#include <vector>
#include <iostream>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixSignalPoint.h"
#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixEnergyDepositUnit.h"


class RPixLinearChargeCollectionDrifter
{
  public:
    RPixLinearChargeCollectionDrifter(const edm::ParameterSet &params, uint32_t det_id);
//    SimRP::charge_induced_on_surface Drift(const std::vector<RPixEnergyDepositUnit>  &energy_deposition);
    std::vector<RPixSignalPoint> Drift(const std::vector<RPixEnergyDepositUnit>  &energy_deposition);

  private:
    std::vector<RPixSignalPoint>  _temp;
//    double charge_cloud_sigma_;
    std::vector<double> charge_cloud_sigmas_vect_; 
    double GeV_per_electron_;
    int verbosity_;
    double det_thickness_;
    uint32_t _det_id;
    
    inline double GetSigma(double z)  //z - z position
    {
       if(charge_cloud_sigmas_vect_.size()==1)
         return charge_cloud_sigmas_vect_[0];
       
       double factor = (z/det_thickness_)*(charge_cloud_sigmas_vect_.size()-1);
       double lo_i = floor(factor);
       double hi_i = ceil(factor);
       if(lo_i==hi_i)
       {
         return charge_cloud_sigmas_vect_[(int)factor];
       }
       else
       {
         double lo_weight = hi_i-factor;
         double hi_weight = factor - lo_i;
         
         return charge_cloud_sigmas_vect_[(int)lo_i]*lo_weight 
                + charge_cloud_sigmas_vect_[(int)hi_i]*hi_weight;
       }
    }
};

#endif  

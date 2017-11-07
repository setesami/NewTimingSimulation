#ifndef SimCTPPS_CTPPSPixelDigiProducer_RPix_DET_DIGITIZER_H
#define SimCTPPS_CTPPSPixelDigiProducer_RPix_DET_DIGITIZER_H

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include <vector>
#include <string>


//#include "SimGeneral/HepPDT/interface/HepPDTable.h"
#include "SimTracker/Common/interface/SiG4UniversalFluctuation.h"
#include "SimGeneral/NoiseGenerators/interface/GaussianTailNoiseGenerator.h"

//#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPSimTypes.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"


#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixHitChargeConverter.h"
#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixDummyROCSimulator.h"
//#include "DataFormats/TotemRPDataTypes/interface/RPStripDigi.h"
//#include "DataFormats/TotemRPDataTypes/interface/RPDetTrigger.h"
//#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigi.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigiCollection.h"
//#include "SimCTPPS/RPDigiProducer/interface/RPixDisplacementGenerator.h"
//#include "SimCTPPS/RPDigiProducer/interface/RPixGaussianTailNoiseAdder.h"
#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixPileUpSignals.h"


namespace CLHEP{
        class HepRandomEngine;
}


class RPixDetDigitizer
{
  public:
    RPixDetDigitizer(const edm::ParameterSet &params, CLHEP::HepRandomEngine& eng, uint32_t det_id, const edm::EventSetup& iSetup);
    void run(const std::vector<PSimHit> &input, const std::vector<int> &input_links, 
	     std::vector<CTPPSPixelDigi> &output_digi, //std::vector<RPDetTrigger> &output_trig, 
        std::vector<std::vector<std::pair<int, double> > >  &output_digi_links
//,   // for each digi in the output the vector of the number of PSimHit and its weight
//        SimRP::TriggerPrimaryMapType &output_trig_links
);
    ~RPixDetDigitizer();
      
  private:
//    RPGaussianTailNoiseAdder *theRPGaussianTailNoiseAdder;
    RPixPileUpSignals *theRPixPileUpSignals;
    RPixHitChargeConverter *theRPixHitChargeConverter;
    RPixDummyROCSimulator *theRPixDummyROCSimulator;
//    RPDisplacementGenerator *theRPDisplacementGenerator;

  private:
    const edm::ParameterSet &params_;

    int numPixels;
    double theNoiseInElectrons;   // Noise (RMS) in units of electrons.
    double thePixelThresholdInE;  // Pixel noise treshold in electorns.
    bool noNoise;                 //if the nos is included
    uint32_t det_id_;
    bool misalignment_simulation_on_;
    int verbosity_;
    bool  _links_persistence;
};

#endif  //SimCTPPS_RPDigiProducer_RP_DET_DIGITIZER_H

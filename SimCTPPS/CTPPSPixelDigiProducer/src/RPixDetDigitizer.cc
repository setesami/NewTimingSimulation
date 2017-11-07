#include <vector>
#include <iostream>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixDetDigitizer.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelTopology.h"


RPixDetDigitizer::RPixDetDigitizer(const edm::ParameterSet &params, CLHEP::HepRandomEngine& eng, uint32_t det_id, const edm::EventSetup& iSetup)
  : params_(params), det_id_(det_id)
{
  verbosity_ = params.getParameter<int>("RPixVerbosity");
  numPixels = CTPPSPixelTopology().detPixelNo();
  theNoiseInElectrons = params.getParameter<double>("RPixEquivalentNoiseCharge");
  thePixelThresholdInE = params.getParameter<double>("RPixDummyROCThreshold");
  noNoise = params.getParameter<bool>("RPixNoNoise");
//  misalignment_simulation_on_ = params_.getParameter<bool>("RPDisplacementOn");
  _links_persistence = params.getParameter<bool>("CTPPSPixelDigiSimHitRelationsPersistence");

// theRPGaussianTailNoiseAdder = new RPGaussianTailNoiseAdder(numStrips,  theNoiseInElectrons, theStripThresholdInE, verbosity_);
  theRPixPileUpSignals = new RPixPileUpSignals(params_, det_id_);
 theRPixDummyROCSimulator = new RPixDummyROCSimulator(params_, det_id_);
  theRPixHitChargeConverter = new RPixHitChargeConverter(params_, eng, det_id_);
//  theRPDisplacementGenerator = new RPDisplacementGenerator(params_, det_id_, iSetup);
}

RPixDetDigitizer::~RPixDetDigitizer()
{
// delete theRPGaussianTailNoiseAdder;
  delete theRPixPileUpSignals;
  delete theRPixDummyROCSimulator;
  delete theRPixHitChargeConverter;
//  delete theRPDisplacementGenerator;
}

void RPixDetDigitizer::run(const std::vector<PSimHit> &input, const std::vector<int> &input_links, 
			   std::vector<CTPPSPixelDigi> &output_digi, //std::vector<RPDetTrigger> &output_trig, 
    std::vector<std::vector<std::pair<int, double> > >  &output_digi_links
//    ,SimRP::TriggerPrimaryMapType &output_trig_links
)
{
  if(verbosity_)
    std::cout<<"RPixDetDigitizer "<<det_id_<<" received input.size()="<<input.size()<<std::endl;
  theRPixPileUpSignals->reset();
  
  bool links_persistence_checked = _links_persistence && input_links.size()==input.size();
  
  int input_size = input.size();
  for (int i=0; i<input_size; ++i)
  {
    std::map<unsigned short, double, std::less<unsigned short> >  the_pixel_charge_map;
/*    if(misalignment_simulation_on_)
      the_strip_charge_map = theRPHitChargeConverter->processHit(
            theRPDisplacementGenerator->Displace(input[i]));
    else
*/     the_pixel_charge_map = theRPixHitChargeConverter->processHit(input[i]);
      
    if(verbosity_)
      std::cout<<"RPixHitChargeConverter "<<det_id_<<" returned hits="<<the_pixel_charge_map.size()<<std::endl;
    if(links_persistence_checked)
      theRPixPileUpSignals->add(the_pixel_charge_map, input_links[i]);
    else
      theRPixPileUpSignals->add(the_pixel_charge_map, 0);
  }

  const std::map<unsigned short, double, std::less<unsigned short> >  &theSignal = theRPixPileUpSignals->dumpSignal();
  std::map<unsigned short, std::vector< std::pair<int, double> > >  &theSignalProvenance = theRPixPileUpSignals->dumpLinks();
  std::map<unsigned short, double, std::less<unsigned short> >  afterNoise;
//  if(noNoise)
    afterNoise = theSignal;
//  else
//    afterNoise = theRPGaussianTailNoiseAdder->addNoise(theSignal);

  theRPixDummyROCSimulator->ConvertChargeToHits(afterNoise, theSignalProvenance, 
        output_digi,  output_digi_links);
}

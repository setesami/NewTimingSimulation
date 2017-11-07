#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixDummyROCSimulator.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelTopology.h"
#include <vector>
#include "TRandom.h"
#include <iostream>


RPixDummyROCSimulator::RPixDummyROCSimulator(const edm::ParameterSet &params, uint32_t det_id)
 : params_(params), det_id_(det_id)
{
//  trigger_mode_ = params_.getParameter<int>("RPVFATTriggerMode");
  threshold_ = params.getParameter<double>("RPixDummyROCThreshold");
  electron_per_adc_ = params.getParameter<double>("RPixDummyROCElectronPerADC");
  dead_pixel_probability_ = params.getParameter<double>("RPixDeadPixelProbability");
  dead_pixels_simulation_on_ = params.getParameter<bool>("RPixDeadPixelSimulationOn");
  pixels_no_ = CTPPSPixelTopology().detPixelNo();
  verbosity_ = params.getParameter<int>("RPixVerbosity");
  links_persistence_ = params.getParameter<bool>("CTPPSPixelDigiSimHitRelationsPersistence");
  
  if(dead_pixels_simulation_on_)
    SetDeadPixels();

  //trigger_mode: 0=no trigger, 1=one sector per chip, 2=4 sectors, 3=8 sectors, 4=gem mode (not implemented)
/* switch(trigger_mode_)
  {
    case 0: 
      strips_per_section_ = 0;    break;
    case 1:
      strips_per_section_ = 128; //since we have 4 chips
      break;
    case 2:
      strips_per_section_ = 128/4; //since we have 4 chips
      break;
    case 3:
      strips_per_section_ = 128/8; //since we have 4 chips
      break;
    default:
      strips_per_section_ = 0;
  }
*/
}

void RPixDummyROCSimulator::ConvertChargeToHits(const std::map<unsigned short, double, std::less<unsigned short> > &signals, 
    std::map<unsigned short, std::vector< std::pair<int, double> > > &theSignalProvenance, 
   std::vector<CTPPSPixelDigi> &output_digi, 
//std::vector<RPDetTrigger> &output_trig, 
   std::vector<std::vector<std::pair<int, double> > >  &output_digi_links
//  ,  SimRP::TriggerPrimaryMapType &output_trig_links
)
{
//  the_trig_cont_.clear();
//  the_trig_cont_links_.clear();
  for(std::map<unsigned short, double, std::less<unsigned short> >::const_iterator i=signals.begin(); 
        i!=signals.end(); ++i)
  {
    //one threshold per hybrid
    unsigned short pixel_no = i->first;   // questo dovrebbe essere il PixelIndex di CTPPSPixelSimTopology.h (col*160+row)
      if(verbosity_)std::cout << "Dummy ROC adc and threshold : "<< i->second << ", " << threshold_ << std::endl; 
    if(i->second > threshold_ && (!dead_pixels_simulation_on_ 
          || dead_pixels_.find(pixel_no)==dead_pixels_.end() ))
    {

      int col = pixel_no / 160;
      int row = pixel_no % 160;
      int adc = int(i->second / electron_per_adc_);

/// set maximum for 8 bits adc
      if (adc >=255) adc=255;

      output_digi.push_back(CTPPSPixelDigi(row,col,adc) );//(det_id_, pixel_no)); /// ?????????????????????????????????????? devo metterci row, col e conteggi adc
      if(links_persistence_)
      {
        output_digi_links.push_back(theSignalProvenance[pixel_no]);
        if(verbosity_)
        {
          std::cout<<"digi links size="<<theSignalProvenance[pixel_no].size()<<std::endl;
          for(unsigned int u=0; u<theSignalProvenance[pixel_no].size(); ++u)
          {
            std::cout<<"   digi: particle="<<theSignalProvenance[pixel_no][u].first<<" energy [electrons]="<<theSignalProvenance[pixel_no][u].second<<std::endl;
          }
        }
      }
    /*     
      if(strips_per_section_)
      {
        int det_trig_section = strip_no/strips_per_section_;
        the_trig_cont_.insert(det_trig_section);
        
        if(links_persistence_)
        {
          std::vector< std::pair<int, double> >::const_iterator j=theSignalProvenance[strip_no].begin();
          std::vector< std::pair<int, double> >::const_iterator end=theSignalProvenance[strip_no].end();
          for(; j!=end; ++j)
          {
            the_trig_cont_links_[det_trig_section][j->first]+=j->second;
          }
        }
      }
*/
    }
  }
/* 
  for(SimRP::TriggerContainer::const_iterator j=the_trig_cont_.begin();
      j!=the_trig_cont_.end(); ++j)
  {
    output_trig.push_back(RPDetTrigger(det_id_, *j));
    if(links_persistence_)
    {
      std::map<int, double>::const_iterator k=the_trig_cont_links_[*j].begin();
      std::map<int, double>::const_iterator end=the_trig_cont_links_[*j].end();
      std::vector<std::pair<int, double> > links_vector(k, end);
      output_trig_links.push_back(links_vector);
      if(verbosity_)
      {
        std::cout<<"trigger links size="<<links_vector.size()<<std::endl;
        for(unsigned int u=0; u<links_vector.size(); ++u)
        {
          std::cout<<"   trigger: particle="<<links_vector[u].first<<" energy [electrons]="<<links_vector[u].second<<std::endl;
        }
        std::cout<<std::endl;
      }
    }
  }
*/
  if(verbosity_)
  {
    for(unsigned int i=0; i<output_digi.size(); ++i)
    {
      std::cout<<"Dummy ROC Simulator "<<   det_id_ << "     row= " //output_digi[i].GetDetId()<<" "
	       <<output_digi[i].row() << "   col= " << output_digi[i].column() << "   adc= "<<  output_digi[i].adc() <<std::endl;
    }
  }
}

void RPixDummyROCSimulator::SetDeadPixels()
{
  dead_pixels_.clear();
  double dead_pixel_number = gRandom->Binomial(pixels_no_, dead_pixel_probability_);
  
  for(int i=0; i<dead_pixel_number; ++i)
  {
    dead_pixels_.insert(gRandom->Integer(pixels_no_));
  }
}


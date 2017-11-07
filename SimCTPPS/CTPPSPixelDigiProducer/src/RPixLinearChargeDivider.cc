#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixLinearChargeDivider.h"
#include "Geometry/VeryForwardGeometry/interface/RPixHepPDTWrapper.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelTopology.h"

RPixLinearChargeDivider::RPixLinearChargeDivider(const edm::ParameterSet &params,  CLHEP::HepRandomEngine& eng,
    uint32_t det_id) : params_(params), rndEngine(eng) , _det_id(det_id)
{
  verbosity_ = params.getParameter<int>("RPixVerbosity");

  fluctuate = new SiG4UniversalFluctuation();

    // Run APV in peak instead of deconvolution mode, which degrades the 
  // time resolution.
  //SimpleConfigurable<bool> SiLinearChargeDivider::peakMode(false,"SiStripDigitizer:APVpeakmode");

  // Enable interstrip Landau fluctuations within a cluster.
  //SimpleConfigurable<bool> SiLinearChargeDivider::fluctuateCharge(true,"SiStripDigitizer:LandauFluctuations");
  fluctuateCharge_ = params.getParameter<bool>("RPixLandauFluctuations");
  
  // Number of segments per strip into which charge is divided during
  // simulation. If large, precision of simulation improves.
  //SimpleConfigurable<int> SiLinearChargeDivider::chargeDivisionsPerStrip(10,"SiStripDigitizer:chargeDivisionsPerStrip");
//  chargedivisionsPerStrip_ = params.getParameter<int>("RPixChargeDivisionsPerStrip");
//  chargedivisionsPerThickness_ = params.getParameter<int>("RPixChargeDivisionsPerThickness");
  chargedivisions_ = params.getParameter<int>("RPixChargeDivisions");
 
  // delta cutoff in MeV, has to be same as in OSCAR (0.120425 MeV corresponding // to 100um range for electrons)
  //SimpleConfigurable<double>  SiLinearChargeDivider::deltaCut(0.120425,
  deltaCut_ = params.getParameter<double>("RPixDeltaProductionCut");
/*
  RPTopology rp_det_topol;
  pitch_ = rp_det_topol.DetPitch();
  thickness_ = rp_det_topol.DetThickness();
*/
}

RPixLinearChargeDivider::~RPixLinearChargeDivider(){
  delete fluctuate;
}


std::vector<RPixEnergyDepositUnit> RPixLinearChargeDivider::divide(const PSimHit& hit)
{
  LocalVector direction = hit.exitPoint() - hit.entryPoint();
  if(direction.z()>10 || direction.x()>200 || direction.y()>200)
  {
    the_energy_path_distribution_.clear();
    return the_energy_path_distribution_;
  }

//  int NumberOfSegmentation_y = (int)(1+chargedivisionsPerStrip_*fabs(direction.y())/pitch_);
//  int NumberOfSegmentation_z = (int)(1+chargedivisionsPerThickness_*fabs(direction.z())/thickness_);
//  int NumberOfSegmentation = std::max(NumberOfSegmentation_y, NumberOfSegmentation_z);
   int NumberOfSegmentation = chargedivisions_ ;
  
  double eLoss = hit.energyLoss();  // Eloss in GeV
 
  the_energy_path_distribution_.resize(NumberOfSegmentation);
 
  if(fluctuateCharge_)
  {
    int pid = hit.particleType();
    double momentum = hit.pabs();
    double length = direction.mag();  // Track length in Silicon
    FluctuateEloss(pid, momentum, eLoss, length, NumberOfSegmentation, the_energy_path_distribution_);
    for (int i=0; i<NumberOfSegmentation; i++)
    {
      the_energy_path_distribution_[i].Position() 
            = hit.entryPoint()+double((i+0.5)/NumberOfSegmentation)*direction;
    }
  }
  else
  {
    for(int i=0; i<NumberOfSegmentation; i++)
    {
      the_energy_path_distribution_[i].Position() 
            = hit.entryPoint()+double((i+0.5)/NumberOfSegmentation)*direction;
      the_energy_path_distribution_[i].Energy() = eLoss/(double)NumberOfSegmentation;
    }
  }
  
  if(verbosity_)
  {
    std::cout<<_det_id<<" charge along the track:"<<std::endl;
    double sum=0;
    for(unsigned int i=0; i<the_energy_path_distribution_.size(); i++)
    {
      std::cout<<the_energy_path_distribution_[i].X()<<" "
          <<the_energy_path_distribution_[i].Y()<<" "
          <<the_energy_path_distribution_[i].Z()<<" "
          <<the_energy_path_distribution_[i].Energy()<<std::endl;
      sum += the_energy_path_distribution_[i].Energy();
    }
    std::cout<<"energy dep. sum="<<sum<<std::endl;
  }
  
  return the_energy_path_distribution_;
}
    
void RPixLinearChargeDivider::FluctuateEloss(int pid, double particleMomentum, 
      double eloss, double length, int NumberOfSegs, 
      std::vector<RPixEnergyDepositUnit>  &elossVector)
{
  // Get dedx for this track
//  double dedx;
//  if( length > 0.) dedx = eloss/length;
//  else dedx = eloss;

  double particleMass = RPixHepPDTWrapper::GetMass(pid)*1000; //to have the mass in MeV
  if(particleMass==-1)
    particleMass = 139.57; // Mass in MeV, Assume pion

  double segmentLength = length/NumberOfSegs;

  // Generate charge fluctuations.
  double de=0.;
  double sum=0.;
  double segmentEloss = (eloss*1000)/NumberOfSegs; //eloss in MeV
  for (int i=0;i<NumberOfSegs;i++)
  {
    // The G4 routine needs momentum in MeV, mass in Mev, delta-cut in MeV,
    // track segment length in mm, segment eloss in MeV 
    // Returns fluctuated eloss in MeV
    //    double deltaCutoff = deltaCut.value(); // the cutoff is sometimes redefined inside, so fix it.
    double deltaCutoff = deltaCut_;
    de = fluctuate->SampleFluctuations(particleMomentum*1000, particleMass, 
        deltaCutoff, segmentLength, segmentEloss, &(rndEngine))/1000; //convert to GeV
    elossVector[i].Energy()=de;
    sum+=de;
  }

  if(sum>0.)
  {  // If fluctuations give eloss>0.
    // Rescale to the same total eloss
    double ratio = eloss/sum;
    for (int ii=0;ii<NumberOfSegs;ii++) elossVector[ii].Energy()=ratio*elossVector[ii].Energy();
  }
  else 
  {  // If fluctuations gives 0 eloss
    double averageEloss = eloss/NumberOfSegs;
    for (int ii=0;ii<NumberOfSegs;ii++) elossVector[ii].Energy()=averageEloss; 
  }
  return;
}


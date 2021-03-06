///////////////////////////////////////////////////////////////////////////////
//Author: Seyed Mohsen Etesami
// setesami@cern.ch
// 2016 Nov
///////////////////////////////////////////////////////////////////////////////
#include "SimG4CMS/CTPPS/interface/CTPPS_Diamond_SD.h"
//#include "SimG4CMS/Forward/interface/CTPPSDiamondNumberingScheme.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimG4Core/Notification/interface/TrackInformation.h"
#include "SimG4Core/Notification/interface/G4TrackToParticleID.h"
//#include "SimG4Core/Geometry/interface/SDCatalog.h"
#include "SimG4Core/Geometry/interface/SensitiveDetectorCatalog.h"
#include "SimG4Core/Physics/interface/G4ProcessTypeEnumerator.h"

#include "SimG4Core/SensitiveDetector/interface/SensitiveTkDetector.h"

#include "SimDataFormats/TrackingHit/interface/UpdatablePSimHit.h"
#include "SimDataFormats/SimHitMaker/interface/TrackingSlaveSD.h"

#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"


#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"

#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

//pps added
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "SimG4Core/Notification/interface/TrackInformation.h"

#include <iostream>
#include <vector>
#include <string>


CTPPS_Diamond_SD::CTPPS_Diamond_SD(std::string name, const DDCompactView & cpv, const SensitiveDetectorCatalog & clg,
     edm::ParameterSet const & p, const SimTrackManager* manager) : 
  SensitiveTkDetector(name, cpv, clg, p), 
  numberingScheme(0),  
  name(name), hcID(-1), theHC(0), currentHit(0), theTrack(0), currentPV(0), 
  unitID(0),  preStepPoint(0), postStepPoint(0), eventno(0)
{


CTPPS_Diamond_SD::initRun();


  collectionName.insert(name);  
  edm::ParameterSet m_Anal = p.getParameter<edm::ParameterSet>("CTPPS_Diamond_SD");
  verbosity_ = m_Anal.getParameter<int>("Verbosity");
  
  // LogDebug("CTPPS_Diamond")
  std::cout
    << "*******************************************************\n"
    << "*                                                     *\n"
    << "* Constructing a CTPPS_Diamond_SD  with name " << name << "\n"
    << "*                                                     *\n"
    << "*******************************************************" << std::endl;


  slave  = new TrackingSlaveSD(name);

  
  // Now attach the right detectors (LogicalVolumes) to me
  
  
  std::vector<std::string> lvNames = clg.logicalNames(name);
  this->Register();
  for (std::vector<std::string>::iterator it=lvNames.begin();  it !=lvNames.end(); it++) 
  {
  

    this->AssignSD(*it);
    edm::LogInfo("CTPPS_Diamond") << "CTPPS_Diamond_SD : Assigns SD to LV " << (*it);
  }

  //std::cout<<"Register and Assign the right detector "<<std::endl;
  

  if (name == "CTPPSDiamondHits")  
  {

    numberingScheme = dynamic_cast<CTPPSVDetectorOrganization*>(new CTPPSDiamondNumberingScheme(3));
    std::cout<<"find PPSTimingHits as name "<<std::endl;
  }
  else 
  {
    edm::LogWarning("CTPPS_Diamond") << "CTPPS_Diamond_SD: ReadoutName not supported\n";
    std::cout<<"CTPPS_Diamond_SD: ReadoutName not supported\n"<<std::endl;

  }
  
  edm::LogInfo("CTPPS_Diamond") << "CTPPS_Diamond_SD: Instantiation completed";
  std::cout << "CTPPS_Diamond: Instantiation completed"<<std::endl;

}


CTPPS_Diamond_SD::~CTPPS_Diamond_SD()
{ 
  if (slave)
    delete slave; 
  if (numberingScheme)
    delete numberingScheme;
}

void CTPPS_Diamond_SD::Initialize(G4HCofThisEvent * HCE) {
  LogDebug("CTPPS_Diamond") << "CTPPS_Diamond_SD : Initialize called for " << name;

  //std::cout << "CTPPS_Diamond_SD: Initialize called for:   " << name<<std::endl<<std::endl<<std::endl;
  theHC = new CTPPS_Diamond_G4HitCollection(name, collectionName[0]);
  if (hcID<0) 
    hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection(hcID, theHC);
}


void CTPPS_Diamond_SD::Print_Hit_Info()
{
  LogDebug("CTPPS_Diamond")
  << theTrack->GetDefinition()->GetParticleName()
  << " PPS_Timing_SD CreateNewHit for"
  << " PV "     << currentPV->GetName()
  << " PVid = " << currentPV->GetCopyNo()
  //<< " MVid = " << currentPV->GetMother()->GetCopyNo()
  << " Unit "   << unitID;
  LogDebug("CTPPS_Diamond") 
  << " primary "    << primaryID
  << " time slice " << tSliceID 
  << " of energy " << theTrack->GetTotalEnergy()
  << " Eloss " << Eloss
  << " positions ";
  printf("(%10f,%10f,%10f)",preStepPoint->GetPosition().x(),preStepPoint->GetPosition().y(),preStepPoint->GetPosition().z());
  printf("(%10f,%10f,%10f)",postStepPoint->GetPosition().x(),postStepPoint->GetPosition().y(),postStepPoint->GetPosition().z());
  LogDebug("CTPPS_Diamond") 
  << " positions " << "(" <<postStepPoint->GetPosition().x()<<","<<postStepPoint->GetPosition().y()<<","<<postStepPoint->GetPosition().z()<<")"
  << " For Track  " << theTrack->GetTrackID()
  << " which is a " << theTrack->GetDefinition()->GetParticleName()
  << " ParentID is " << theTrack->GetParentID()<<std::endl<<std::endl;
     
  if(theTrack->GetTrackID()==1)
  {
    LogDebug("CTPPS_Diamond_SD") << " primary particle ";
  }
  else
  {
    LogDebug("CTPPS_Diamond_SD") << " daughter of part. " << theTrack->GetParentID();
  }

  LogDebug("CTPPS_Diamond_SD")  << " and created by " ;
  
  if(theTrack->GetCreatorProcess()!=NULL)
    LogDebug("CTPPS_Diamond_SD") << theTrack->GetCreatorProcess()->GetProcessName() ;
  else 
    LogDebug("CTPPS_Diamond_SD") << "NO process";
    
  LogDebug("CTPPS_Diamond_SD") << std::endl;
}


G4bool CTPPS_Diamond_SD::ProcessHits(G4Step * aStep, G4TouchableHistory * )
{
  if (aStep == NULL)
  {	    
    std::cout<<"CTPPS_TIMING : There is no hit to process"<<std::endl<<std::endl;
    return true;
  }
  else
  {
 
    std::cout
     << "*******************************************************\n"
     << "*                                                     *\n"
     << "* PPS Diamond Hit initialized  with name " << name << "\n" 
     << "*                                                     *\n" 
     << "*******************************************************" << std::endl;



    GetStepInfo(aStep);
 
    if(theTrack->GetDefinition()->GetPDGEncoding()==2212)  
      {
	Print_Hit_Info();
	//ImportInfotoHit();    //in addtion to import info to hit it STORE hit as well
	std::cout<<("CTPPS_Diamond")<<"New hit created"<<std::endl;

      }
     Print_Hit_Info();
     ImportInfotoHit();


      return true;
  }
}


void CTPPS_Diamond_SD::GetStepInfo(G4Step* aStep)
{
 

  theTrack = aStep->GetTrack();   
  preStepPoint = aStep->GetPreStepPoint(); 
  postStepPoint = aStep->GetPostStepPoint(); 
  //  theLocalEntryPoint = SensitiveDetector::InitialStepPosition(aStep,LocalCoordinates);  
  //  theLocalExitPoint = SensitiveDetector::FinalStepPosition(aStep,LocalCoordinates);
  hitPoint = preStepPoint->GetPosition();
  exitPoint = postStepPoint->GetPosition();
  currentPV = preStepPoint->GetPhysicalVolume();
  theLocalEntryPoint = SetToLocal(hitPoint);
  theLocalExitPoint = SetToLocal(exitPoint);
  theglobaltimehit=preStepPoint->GetGlobalTime()/nanosecond;
  incidentEnergy=(aStep->GetPreStepPoint()->GetTotalEnergy()/eV);
  tSlice = (postStepPoint->GetGlobalTime() )/nanosecond;
  tSliceID = (int) tSlice;
  unitID = setDetUnitId(aStep);

  if(verbosity_)
    LogDebug("CTPPS_Diamond") << "UNITa " << unitID <<std::endl;

  primaryID = theTrack->GetTrackID();
  Pabs = (aStep->GetPreStepPoint()->GetMomentum().mag())/GeV;
  p_x = (aStep->GetPreStepPoint()->GetMomentum().x())/GeV;
  p_y = (aStep->GetPreStepPoint()->GetMomentum().y())/GeV;
  p_z = (aStep->GetPreStepPoint()->GetMomentum().z())/GeV;
  Tof = aStep->GetPreStepPoint()->GetGlobalTime()/nanosecond;  
  Eloss = (aStep->GetPreStepPoint()->GetTotalEnergy()/eV); //pps added 
  ParticleType = theTrack->GetDefinition()->GetPDGEncoding();

  //corrected phi and theta treatment
  G4ThreeVector gmd  = aStep->GetPreStepPoint()->GetMomentumDirection();
  // convert it to local frame
  G4ThreeVector lmd = ((G4TouchableHistory *)(aStep->GetPreStepPoint()->GetTouchable()))->GetHistory()->GetTopTransform().TransformAxis(gmd);
  Local3DPoint lnmd = ConvertToLocal3DPoint(lmd);
  ThetaAtEntry = lnmd.theta();
  PhiAtEntry = lnmd.phi(); 

 if(IsPrimary(theTrack))
    ParentId = 0;
  else ParentId = theTrack->GetParentID();
  
  Vx = theTrack->GetVertexPosition().x()/mm;
  Vy = theTrack->GetVertexPosition().y()/mm;
  Vz = theTrack->GetVertexPosition().z()/mm;
}


uint32_t CTPPS_Diamond_SD::setDetUnitId(G4Step * aStep)
{ 
  //LogDebug("CTPPS_Diamond") << "numbering " << numberingScheme->GetCurrentPlane()<<std::endl;
  return (numberingScheme == 0 ? 0 : numberingScheme->GetUnitID(aStep));
}


void CTPPS_Diamond_SD::StoreHit(CTPPS_Diamond_G4Hit* hit)
{
  if (hit == 0 )
  {
    if(verbosity_)
      LogDebug("CTPPS_Diamond") << "CTPPS_Diamond: hit to be stored is NULL !!" <<std::endl;
    return;
  }

  theHC->insert( hit );
}


void CTPPS_Diamond_SD::ImportInfotoHit()    // added pps
{
  currentHit = new CTPPS_Diamond_G4Hit;
  currentHit->setTrackID(primaryID);
  currentHit->setTimeSlice(tSlice);
  currentHit->setUnitID(unitID);
  currentHit->setIncidentEnergy(incidentEnergy);
  currentHit->setPabs(Pabs);
  currentHit->setTof(Tof);
  currentHit->setEnergyLoss(Eloss);
  currentHit->setParticleType(ParticleType);
  currentHit->setThetaAtEntry(ThetaAtEntry);
  currentHit->setPhiAtEntry(PhiAtEntry);
  currentHit->setEntry(hitPoint);
  currentHit->setExit(exitPoint);
  currentHit->setLocalEntry(hitPoint);
  currentHit->setLocalExit(exitPoint);
  currentHit->setParentId(ParentId);
  currentHit->setVx(Vx);
  currentHit->setVy(Vy);
  currentHit->setVz(Vz); 
  currentHit->set_p_x(p_x);
  currentHit->set_p_y(p_y);
  currentHit->set_p_z(p_z);
  currentHit->setGlobalTimehit(Globaltimehit);

  StoreHit(currentHit);
  // LogDebug("CTPPS_Diamond") << "STORED HIT IN: " << unitID << std::endl;
}	 


G4ThreeVector CTPPS_Diamond_SD::SetToLocal(G4ThreeVector global)
{
  G4ThreeVector localPoint;
  const G4VTouchable* touch= preStepPoint->GetTouchable();
  localPoint = touch->GetHistory()->GetTopTransform().TransformPoint(global);
  
  return localPoint;
}
     

void CTPPS_Diamond_SD::EndOfEvent(G4HCofThisEvent* )
{
  // here we loop over transient hits and make them persistent
  for (int j=0; j<theHC->entries() && j<15000; j++)
  {
    CTPPS_Diamond_G4Hit* aHit = (*theHC)[j];
    
    Local3DPoint Entrata(aHit->getLocalEntry().x(),
			 aHit->getLocalEntry().y(),
			 aHit->getLocalEntry().z());
    Local3DPoint Uscita(aHit->getLocalExit().x(),
			aHit->getLocalExit().y(),
			aHit->getLocalExit().z());
    slave->processHits(PSimHit(Entrata,Uscita,
			       aHit->getPabs(), aHit->getTof(),
			       aHit->getEnergyLoss(), aHit->getParticleType(),
			       aHit->getUnitID(), aHit->getTrackID(),
			       aHit->getThetaAtEntry(),aHit->getPhiAtEntry()));
  }
  Summarize();
}
     

void CTPPS_Diamond_SD::Summarize()
{
}


void CTPPS_Diamond_SD::clear()
{
} 


void CTPPS_Diamond_SD::DrawAll()
{
} 


void CTPPS_Diamond_SD::PrintAll()
{
  LogDebug("CTPPS_Diamond") << "CTPPS_Diamond: Collection " << theHC->GetName() << std::endl;
  //theHC->PrintAllHits();
} 

void CTPPS_Diamond_SD::fillHits(edm::PSimHitContainer& c, std::string n) 
{
  if (slave->name() == n) c=slave->hits();
}


void CTPPS_Diamond_SD::SetNumberingScheme(CTPPSVDetectorOrganization* scheme)
{
  if (numberingScheme)
    delete numberingScheme;
  numberingScheme = scheme;
}

void CTPPS_Diamond_SD::update(const BeginOfEvent * i)
{
  //  LogDebug("CTPPS_Diamond") <<" Dispatched BeginOfEvent !"<<std::endl;
  clearHits();
  eventno = (*i)()->GetEventID();
  //std::cout<<"#######################Begin of Event number: "<<eventno<<std::endl<<std::endl;

}

void CTPPS_Diamond_SD::update (const ::EndOfEvent*)
{
}


void CTPPS_Diamond_SD::clearTrack( G4Track * track)
{
    track->SetTrackStatus(fStopAndKill);   
}


void CTPPS_Diamond_SD::clearHits()
{
  slave->Initialize();
}

bool CTPPS_Diamond_SD::IsPrimary(const G4Track * track)
{
  TrackInformation* info = dynamic_cast<TrackInformation*>( track->GetUserInformation() );
  return info && info->isPrimary();
}



void CTPPS_Diamond_SD::initRun()
{
  // construct your own material properties for setting refractionindex and so on

}

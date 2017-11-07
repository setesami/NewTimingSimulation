#ifndef CTPPS_Timing_MaterialProperties_h
#define CTPPS_Timing_MaterialProperties_h

 /** \class CTPPS_Timing_MaterialProperties
  *  Class to define custom material properties for CTPPS_timing detector with CTPPS_timing_box, window_box and Lbar sub detectorss.
  *
  *  $Date: 2015/06/24 21:01:00 $
  *  $Revision: 0.0 $
  *  \author Mohsen Naseri
  */
 
 #include "G4MaterialPropertiesTable.hh"
 #include "G4SDManager.hh"
#include "G4OpticalSurface.hh" 
 class CTPPS_Timing_MaterialProperties
 {
  public:
     /// two different constructors to have more flexibility.
   CTPPS_Timing_MaterialProperties(int DebugLevel);
  // CTPPS_Timing_MaterialProperties();
     /// destructor
   ~CTPPS_Timing_MaterialProperties();


   static void DumpSurfaceInfo();
 
  private:
     /// define optical properties of materials in the timing detector of CTPPS project
   void setMaterialProperties();
 
  private:
   const G4MaterialTable * theMaterialTable;
   void  SetSurfaceProperties(G4OpticalSurface* OpSilSurface);
 
  private:
   int theMPDebugLevel;
   G4MaterialPropertiesTable * theMPT;
   G4Material * theAir;
   G4Material * theSil;
   G4Material * theGlass;
 };
 #endif



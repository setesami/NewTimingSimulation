# ctppsFullSim
- cmsrel CMSSW_9_4_0_pre2
- git clone https:://github.com/dilsonjd/ctppsFullSim.git CMSSW_9_4_0_pre2/src
- cd CMSSW_9_4_0_pre2/src
- cmsenv
- scram b -j8

CTPPS standalone (with ParticleGun)
- cmsRun run_only_CTPPS_cfg_DIG_.py
- cmsRun step2_DIGI_DIGI2RAW.py
- cmsRun step25_RAW2DIGI.py

CMS FullSimulation (with ExHuME) 
- cmsRun GluGluTo2Jets_M_300_2000_13TeV_exhume_cff_py_GEN_SIM.py
- cmsRun gluglu_step2_DIGI_DIGI2RAW.py
- cmsRun gluglu_step25_RAW2DIGI.py

gluglu_step3_RAW2DIGI_L1Reco_RECO.py - not running yet

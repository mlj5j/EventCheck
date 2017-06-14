#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"  
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"  
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalCleaningAlgo.h"  

 
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"

#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"

#include "CondFormats/DataRecord/interface/EcalPedestalsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalPedestals.h"

#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"

#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"


#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"




#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTriggerRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTrigger.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFLayer.h"



#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"


#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


#include "JetMETCorrections/MCJet/plugins/ChanstatusTester.h" 
#include "JetMETCorrections/MCJet/plugins/JetUtilMC.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"




#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"




#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"


#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalElectronicsId.h"


#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"

#include "CondFormats/DataRecord/interface/EcalPedestalsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalPedestals.h"


#include "Math/VectorUtil.h"
#include "TVector3.h"

using namespace edm;
using namespace reco;
using namespace std;

DEFINE_FWK_MODULE(ChanstatusTester);

ChanstatusTester::ChanstatusTester(const edm::ParameterSet& cfg)
  : bunchstartbx_(cfg.getParameter<std::vector<int> >("bunchstartbx"))
{
  jets_          = cfg.getParameter<std::string> ("jets");
  histogramFile_ = cfg.getParameter<std::string> ("histogramFile");
  tracks_        = cfg.getParameter<std::string> ("tracks");
  vertex_coll_   = cfg.getParameter<std::string> ("vertex");
  ebhitcoll_     = cfg.getParameter<std::string> ("EBRecHitCollection");
  eehitcoll_     = cfg.getParameter<std::string> ("EERecHitCollection");
  isMC_          = cfg.getParameter<bool>("IsMC");




  edm::ParameterSet cleaningPs = 
    cfg.getParameter<edm::ParameterSet>("cleaningConfig");
  cleaningAlgo_ = new EcalCleaningAlgo(cleaningPs);


}
//////////////////////////////////////////////////////////////////////////////////////////
void ChanstatusTester::beginJob() 
{
  
  file_          = new TFile(histogramFile_.c_str(),"RECREATE");

  histo_event_   = new TH1D("histo_event","",350,0.,3500.);

  eb_rechitenergy_   = new TH1D("eb_rechitenergy","",2020,-2.,200.);
  ee_rechitenergy_   = new TH1D("ee_rechitenergy","",2020,-2.,200.);

  ee_rechitenergy_notypeb_             = new TH1D("ee_rechitenergy_notypeb","",2020,-2.,200.);
  eb_rechitenergy_spiketag             = new TH1D("eb_rechitenergy_spiketag","",2020,-2.,200.);
  eb_rechitenergy_swisscross           = new TH1D("eb_rechitenergy_swisscross","",2020,-2.,200.);
  eb_rechitenergy_kweird               = new TH1D("eb_rechitenergy_kweird","",2020,-2.,200.);
  eb_rechitenergy_knotweird            = new TH1D("eb_rechitenergy_knotweird","",2020,-2.,200.);
  eb_rechitenergy_spiketag_kweird      = new TH1D("eb_rechitenergy_spiketag_kweird","",2020,-2.,200.);
  eb_rechitenergy_spiketag_swisscross  = new TH1D("eb_rechitenergy_spiketag_swisscross","",2020,-2.,200.);
  eb_rechitenergy_spiketag_noisedep    = new TH1D("eb_rechitenergy_spiketag_noisedep","",2020,-2.,200.);

  eb_rechitenergy_sctag                = new TH1D("eb_rechitenergy_sctag","",2020,-2.,200.);
  eb_rechitenergy_sctag_kweird         = new TH1D("eb_rechitenergy_sctag_kweird","",2020,-2.,200.);
  eb_rechitenergy_sctag_swisscross     = new TH1D("eb_rechitenergy_sctag_swisscross","",2020,-2.,200.);
  eb_rechitenergy_sctag_noisedep       = new TH1D("eb_rechitenergy_sctag_noisedep","",2020,-2.,200.);


  Emin_=1000.0;
  side_=5;


  eb_rechitenergy_02    = new TH1D("eb_rechitenergy_02","",1100,-2.,20.);
  eb_rechitenergy_04    = new TH1D("eb_rechitenergy_04","",1100,-2.,20.);
  eb_rechitenergy_06    = new TH1D("eb_rechitenergy_06","",1100,-2.,20.);
  eb_rechitenergy_08    = new TH1D("eb_rechitenergy_08","",1100,-2.,20.);
  eb_rechitenergy_10    = new TH1D("eb_rechitenergy_10","",1100,-2.,20.);
  eb_rechitenergy_12    = new TH1D("eb_rechitenergy_12","",1100,-2.,20.);
  eb_rechitenergy_14    = new TH1D("eb_rechitenergy_14","",1100,-2.,20.);
  eb_rechitenergy_148   = new TH1D("eb_rechitenergy_148","",1100,-2.,20.);
 
  ee_rechitenergy_16    = new TH1D("ee_rechitenergy_16","",1100,-2.,20.);
  ee_rechitenergy_18    = new TH1D("ee_rechitenergy_18","",1100,-2.,20.);
  ee_rechitenergy_20    = new TH1D("ee_rechitenergy_20","",1100,-2.,20.);
  ee_rechitenergy_22    = new TH1D("ee_rechitenergy_22","",1100,-2.,20.);
  ee_rechitenergy_24    = new TH1D("ee_rechitenergy_24","",1100,-2.,20.);
  ee_rechitenergy_26    = new TH1D("ee_rechitenergy_26","",1100,-2.,20.);
  ee_rechitenergy_28    = new TH1D("ee_rechitenergy_28","",1100,-2.,20.);
  ee_rechitenergy_30    = new TH1D("ee_rechitenergy_30","",1100,-2.,20.);
 

  eb_rechitet_02    = new TH1D("eb_rechitet_02","",1100,-2.,20.);
  eb_rechitet_04    = new TH1D("eb_rechitet_04","",1100,-2.,20.);
  eb_rechitet_06    = new TH1D("eb_rechitet_06","",1100,-2.,20.);
  eb_rechitet_08    = new TH1D("eb_rechitet_08","",1100,-2.,20.);
  eb_rechitet_10    = new TH1D("eb_rechitet_10","",1100,-2.,20.);
  eb_rechitet_12    = new TH1D("eb_rechitet_12","",1100,-2.,20.);
  eb_rechitet_14    = new TH1D("eb_rechitet_14","",1100,-2.,20.);
  eb_rechitet_148   = new TH1D("eb_rechitet_148","",1100,-2.,20.);
 
  ee_rechitet_16    = new TH1D("ee_rechitet_16","",1100,-2.,20.);
  ee_rechitet_18    = new TH1D("ee_rechitet_18","",1100,-2.,20.);
  ee_rechitet_20    = new TH1D("ee_rechitet_20","",1100,-2.,20.);
  ee_rechitet_22    = new TH1D("ee_rechitet_22","",1100,-2.,20.);
  ee_rechitet_24    = new TH1D("ee_rechitet_24","",1100,-2.,20.);
  ee_rechitet_26    = new TH1D("ee_rechitet_26","",1100,-2.,20.);
  ee_rechitet_28    = new TH1D("ee_rechitet_28","",1100,-2.,20.);
  ee_rechitet_30    = new TH1D("ee_rechitet_30","",1100,-2.,20.);
 


  eb_rechitetvspu_05 = new TH2D("eb_rechitetvspu_05","",25,0,50,500,-0.5,20);
  eb_rechitetvspu_10 = new TH2D("eb_rechitetvspu_10","",25,0,50,500,-0.5,20);
  eb_rechitetvspu_15 = new TH2D("eb_rechitetvspu_15","",25,0,50,500,-0.5,20);
 

  ee_rechitetvspu_20 = new TH2D("ee_rechitetvspu_20","",25,0,50,500,-0.5,20);
  ee_rechitetvspu_25 = new TH2D("ee_rechitetvspu_25","",25,0,50,500,-0.5,20);
  ee_rechitetvspu_30 = new TH2D("ee_rechitetvspu_30","",25,0,50,500,-0.5,20);
    

 




  eb_rechitet_   = new TH1D("eb_rechitet","",1100,-2.,20.);
  ee_rechitet_   = new TH1D("ee_rechitet","",1100,-2.,20.);

  eb_rechiten_vs_eta = new TH2D("ebrechiten_vs_eta","",30,-1.5,1.5,350,-2,5);
  eb_rechitet_vs_eta = new TH2D("ebrechitet_vs_eta","",30,-1.5,1.5,350,-2,5);


  eep_rechiten_vs_eta = new TH2D("eep_rechiten_vs_eta","",18,1.4,3.2,350,-2,5);
  eep_rechiten_vs_phi = new TH2D("eep_rechiten_vs_phi","",18,-3.1416,3.1416,350,-2,5);
  eem_rechiten_vs_eta = new TH2D("eem_rechiten_vs_eta","",18,1.4,3.2,350,-2,5);
  eem_rechiten_vs_phi = new TH2D("eem_rechiten_vs_phi","",18,-3.1416,3.1416,350,-2,5);


  eep_rechitet_vs_eta = new TH2D("eep_rechitet_vs_eta","",18,1.4,3.2,350,-2,5);
  eep_rechitet_vs_phi = new TH2D("eep_rechitet_vs_phi","",18,-3.1416,3.1416,350,-2,5);
  eem_rechitet_vs_eta = new TH2D("eem_rechitet_vs_eta","",18,1.4,3.2,350,-2,5);
  eem_rechitet_vs_phi = new TH2D("eem_rechitet_vs_phi","",18,-3.1416,3.1416,350,-2,5);

 
 
  ebocc = new TH2F("ebocc","",360,0,360,170,-85,85);
  eboccgt1 = new TH2F("eboccgt1","",360,0,360,170,-85,85);
  eboccgt3 = new TH2F("eboccgt3","",360,0,360,170,-85,85);
  eboccgt5 = new TH2F("eboccgt5","",360,0,360,170,-85,85);
  eboccgt1et = new TH2F("eboccgt1et","",360,0,360,170,-85,85);
  eboccet = new TH2F("eboccet","",360,0,360,170,-85,85);
  eboccetgt1et = new TH2F("eboccetgt1et","",360,0,360,170,-85,85);
  eboccen = new TH2F("eboccen","",360,0,360,170,-85,85);
  eboccengt1 = new TH2F("eboccengt1","",360,0,360,170,-85,85);


  eeocc = new TH2F("eeocc","",200,0,200,100,0,100);
  eeoccgt1 = new TH2F("eeoccgt1","",200,0,200,100,0,100);
  eeoccgt1et = new TH2F("eeoccgt1et","",200,0,200,100,0,100);
  eeoccet = new TH2F("eeoccet","",200,0,200,100,0,100);
  eeoccetgt1et = new TH2F("eeoccetgt1et","",200,0,200,100,0,100);
  eeoccen = new TH2F("eeoccen","",200,0,200,100,0,100);
  eeoccengt1 = new TH2F("eeoccengt1","",200,0,200,100,0,100);


  spikeadc=new TH1F("spikeadc","",100,0,1000);
  spiketime50=new TH1F("spiketime50","",10,0.5,10.5);


  etaphi_ped = new TH2D("etaphi_ped","",360,0,360,170,-85,85);
  etaphi_pedsq = new TH2D("etaphi_pedsq","",360,0,360,170,-85,85);
  etaphi_pedn = new TH2D("etaphi_pedn","",360,0,360,170,-85,85);
  etaphi_pednoise = new TH2D("etaphi_pednoise","",360,0,360,170,-85,85);


  noise2d = new TH2D("noise2d","",360,0,360,170,-85,85);

  eventenergy = new TH2D("eventenergy","",360,0,360,170,-85,85);
  eventet     = new TH2D("eventet","",360,0,360,170,-85,85);
  eventtime   = new TH2D("eventtime","",360,0,360,170,-85,85);
  eventkoot   = new TH2D("eventkoot","",360,0,360,170,-85,85);
  eventkdiweird = new TH2D("eventkdiweird","",360,0,360,170,-85,85);


  spike_timevset_all_08 = new TH2D("spike_timevset_all_08","",120,-60,60,200,-2,18);
  spike_timevset_highest_08 = new TH2D("spike_timevset_highest_08","",120,-60,60,200,-2,18);
  spike_timevset_koot_all_08 = new TH2D("spike_timevset_koot_all_08","",120,-60,60,200,-2,18);
  spike_timevset_koot_highest_08 = new TH2D("spike_timevset_koot_highest_08","",120,-60,60,200,-2,18);
  spike_timevset_kdiweird_all_08 = new TH2D("spike_timevset_kdiweird_all_08","",120,-60,60,200,-2,18);
  spike_timevset_kdiweird_highest_08 = new TH2D("spike_timevset_kdiweird_highest_08","",120,-60,60,200,-2,18);

  spike_timevset_all_07 = new TH2D("spike_timevset_all_07","",120,-60,60,200,-2,18);
  spike_timevset_highest_07 = new TH2D("spike_timevset_highest_07","",120,-60,60,200,-2,18);
  spike_timevset_koot_all_07 = new TH2D("spike_timevset_koot_all_07","",120,-60,60,200,-2,18);
  spike_timevset_koot_highest_07 = new TH2D("spike_timevset_koot_highest_07","",120,-60,60,200,-2,18);
  spike_timevset_kdiweird_all_07 = new TH2D("spike_timevset_kdiweird_all_07","",120,-60,60,200,-2,18);
  spike_timevset_kdiweird_highest_07 = new TH2D("spike_timevset_kdiweird_highest_07","",120,-60,60,200,-2,18);

  spike_timevset_all_06 = new TH2D("spike_timevset_all_06","",120,-60,60,200,-2,18);
  spike_timevset_highest_06 = new TH2D("spike_timevset_highest_06","",120,-60,60,200,-2,18);
  spike_timevset_koot_all_06 = new TH2D("spike_timevset_koot_all_06","",120,-60,60,200,-2,18);
  spike_timevset_koot_highest_06 = new TH2D("spike_timevset_koot_highest_06","",120,-60,60,200,-2,18);
  spike_timevset_kdiweird_all_06 = new TH2D("spike_timevset_kdiweird_all_06","",120,-60,60,200,-2,18);
  spike_timevset_kdiweird_highest_06 = new TH2D("spike_timevset_kdiweird_highest_06","",120,-60,60,200,-2,18);



  scocc_eb_gt50 = new TH2F("scocc_eb_gt50","",100,-3.14,3.14,100,-3,3);
  scocc_ee_gt50 = new TH2F("scocc_ee_gt50","",100,-3.14,3.14,100,-3,3);


  tower_spike_ene_swiss_t10 = new TH2D("tower_spike_ene_swiss_t10","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t07 = new TH2D("tower_spike_ene_swiss_t07","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t05 = new TH2D("tower_spike_ene_swiss_t05","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t04 = new TH2D("tower_spike_ene_swiss_t04","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t03 = new TH2D("tower_spike_ene_swiss_t03","",100,0.205,1.205,2,-0.5,1.5);


  sc_tower_spike_ene_swiss_t10 = new TH2D("sc_tower_spike_ene_swiss_t10","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t07 = new TH2D("sc_tower_spike_ene_swiss_t07","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t05 = new TH2D("sc_tower_spike_ene_swiss_t05","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t04 = new TH2D("sc_tower_spike_ene_swiss_t04","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t03 = new TH2D("sc_tower_spike_ene_swiss_t03","",100,0.205,1.205,2,-0.5,1.5);



  tower_spike_ene_swiss_t10_thresh08 = new TH2D("tower_spike_ene_swiss_t10_thresh08","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t07_thresh08 = new TH2D("tower_spike_ene_swiss_t07_thresh08","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t05_thresh08 = new TH2D("tower_spike_ene_swiss_t05_thresh08","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t04_thresh08 = new TH2D("tower_spike_ene_swiss_t04_thresh08","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t03_thresh08 = new TH2D("tower_spike_ene_swiss_t03_thresh08","",100,0.205,1.205,2,-0.5,1.5);


  tower_spike_ene_swiss_t10_thresh10 = new TH2D("tower_spike_ene_swiss_t10_thresh10","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t07_thresh10 = new TH2D("tower_spike_ene_swiss_t07_thresh10","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t05_thresh10 = new TH2D("tower_spike_ene_swiss_t05_thresh10","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t04_thresh10 = new TH2D("tower_spike_ene_swiss_t04_thresh10","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t03_thresh10 = new TH2D("tower_spike_ene_swiss_t03_thresh10","",100,0.205,1.205,2,-0.5,1.5);

  tower_spike_ene_swiss_t10_thresh12 = new TH2D("tower_spike_ene_swiss_t10_thresh12","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t07_thresh12 = new TH2D("tower_spike_ene_swiss_t07_thresh12","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t05_thresh12 = new TH2D("tower_spike_ene_swiss_t05_thresh12","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t04_thresh12 = new TH2D("tower_spike_ene_swiss_t04_thresh12","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t03_thresh12 = new TH2D("tower_spike_ene_swiss_t03_thresh12","",100,0.205,1.205,2,-0.5,1.5);

  tower_spike_ene_swiss_t10_thresh15 = new TH2D("tower_spike_ene_swiss_t10_thresh15","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t07_thresh15 = new TH2D("tower_spike_ene_swiss_t07_thresh15","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t05_thresh15 = new TH2D("tower_spike_ene_swiss_t05_thresh15","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t04_thresh15 = new TH2D("tower_spike_ene_swiss_t04_thresh15","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t03_thresh15 = new TH2D("tower_spike_ene_swiss_t03_thresh15","",100,0.205,1.205,2,-0.5,1.5);

  tower_spike_ene_swiss_t10_thresh20 = new TH2D("tower_spike_ene_swiss_t10_thresh20","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t07_thresh20 = new TH2D("tower_spike_ene_swiss_t07_thresh20","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t05_thresh20 = new TH2D("tower_spike_ene_swiss_t05_thresh20","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t04_thresh20 = new TH2D("tower_spike_ene_swiss_t04_thresh20","",100,0.205,1.205,2,-0.5,1.5);
  tower_spike_ene_swiss_t03_thresh20 = new TH2D("tower_spike_ene_swiss_t03_thresh20","",100,0.205,1.205,2,-0.5,1.5);



  sc_tower_spike_ene_swiss_t10_thresh08 = new TH2D("sc_tower_spike_ene_swiss_t10_thresh08","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t07_thresh08 = new TH2D("sc_tower_spike_ene_swiss_t07_thresh08","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t05_thresh08 = new TH2D("sc_tower_spike_ene_swiss_t05_thresh08","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t04_thresh08 = new TH2D("sc_tower_spike_ene_swiss_t04_thresh08","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t03_thresh08 = new TH2D("sc_tower_spike_ene_swiss_t03_thresh08","",100,0.205,1.205,2,-0.5,1.5);


  sc_tower_spike_ene_swiss_t10_thresh10 = new TH2D("sc_tower_spike_ene_swiss_t10_thresh10","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t07_thresh10 = new TH2D("sc_tower_spike_ene_swiss_t07_thresh10","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t05_thresh10 = new TH2D("sc_tower_spike_ene_swiss_t05_thresh10","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t04_thresh10 = new TH2D("sc_tower_spike_ene_swiss_t04_thresh10","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t03_thresh10 = new TH2D("sc_tower_spike_ene_swiss_t03_thresh10","",100,0.205,1.205,2,-0.5,1.5);

  sc_tower_spike_ene_swiss_t10_thresh12 = new TH2D("sc_tower_spike_ene_swiss_t10_thresh12","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t07_thresh12 = new TH2D("sc_tower_spike_ene_swiss_t07_thresh12","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t05_thresh12 = new TH2D("sc_tower_spike_ene_swiss_t05_thresh12","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t04_thresh12 = new TH2D("sc_tower_spike_ene_swiss_t04_thresh12","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t03_thresh12 = new TH2D("sc_tower_spike_ene_swiss_t03_thresh12","",100,0.205,1.205,2,-0.5,1.5);

  sc_tower_spike_ene_swiss_t10_thresh15 = new TH2D("sc_tower_spike_ene_swiss_t10_thresh15","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t07_thresh15 = new TH2D("sc_tower_spike_ene_swiss_t07_thresh15","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t05_thresh15 = new TH2D("sc_tower_spike_ene_swiss_t05_thresh15","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t04_thresh15 = new TH2D("sc_tower_spike_ene_swiss_t04_thresh15","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t03_thresh15 = new TH2D("sc_tower_spike_ene_swiss_t03_thresh15","",100,0.205,1.205,2,-0.5,1.5);

  sc_tower_spike_ene_swiss_t10_thresh20 = new TH2D("sc_tower_spike_ene_swiss_t10_thresh20","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t07_thresh20 = new TH2D("sc_tower_spike_ene_swiss_t07_thresh20","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t05_thresh20 = new TH2D("sc_tower_spike_ene_swiss_t05_thresh20","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t04_thresh20 = new TH2D("sc_tower_spike_ene_swiss_t04_thresh20","",100,0.205,1.205,2,-0.5,1.5);
  sc_tower_spike_ene_swiss_t03_thresh20 = new TH2D("sc_tower_spike_ene_swiss_t03_thresh20","",100,0.205,1.205,2,-0.5,1.5);



 tower_spike_et_swisstt = new TH2D("tower_spike_et_swisstt","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swisstt = new TH2D("tower_spike_ene_swisstt","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swisstt_thresh08 = new TH2D("tower_spike_et_swisstt_thresh08","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swisstt_thresh08 = new TH2D("tower_spike_ene_swisstt_thresh08","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swisstt_thresh10 = new TH2D("tower_spike_et_swisstt_thresh10","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swisstt_thresh10 = new TH2D("tower_spike_ene_swisstt_thresh10","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swisstt_thresh12 = new TH2D("tower_spike_et_swisstt_thresh12","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swisstt_thresh12 = new TH2D("tower_spike_ene_swisstt_thresh12","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swisstt_thresh15 = new TH2D("tower_spike_et_swisstt_thresh15","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swisstt_thresh15 = new TH2D("tower_spike_ene_swisstt_thresh15","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swisstt_thresh20 = new TH2D("tower_spike_et_swisstt_thresh20","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swisstt_thresh20 = new TH2D("tower_spike_ene_swisstt_thresh20","",100,0.205,1.205,2,-0.5,1.5);


 sc_tower_spike_et_swisstt = new TH2D("sc_tower_spike_et_swisstt","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swisstt = new TH2D("sc_tower_spike_ene_swisstt","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swisstt_thresh08 = new TH2D("sc_tower_spike_et_swisstt_thresh08","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swisstt_thresh08 = new TH2D("sc_tower_spike_ene_swisstt_thresh08","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swisstt_thresh10 = new TH2D("sc_tower_spike_et_swisstt_thresh10","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swisstt_thresh10 = new TH2D("sc_tower_spike_ene_swisstt_thresh10","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swisstt_thresh12 = new TH2D("sc_tower_spike_et_swisstt_thresh12","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swisstt_thresh12 = new TH2D("sc_tower_spike_ene_swisstt_thresh12","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swisstt_thresh15 = new TH2D("sc_tower_spike_et_swisstt_thresh15","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swisstt_thresh15 = new TH2D("sc_tower_spike_ene_swisstt_thresh15","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swisstt_thresh20 = new TH2D("sc_tower_spike_et_swisstt_thresh20","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swisstt_thresh20 = new TH2D("sc_tower_spike_ene_swisstt_thresh20","",100,0.205,1.205,2,-0.5,1.5);


 tower_spike_et_swiss = new TH2D("tower_spike_et_swiss","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swiss = new TH2D("tower_spike_ene_swiss","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swiss_thresh08 = new TH2D("tower_spike_et_swiss_thresh08","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swiss_thresh08 = new TH2D("tower_spike_ene_swiss_thresh08","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swiss_thresh10 = new TH2D("tower_spike_et_swiss_thresh10","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swiss_thresh10 = new TH2D("tower_spike_ene_swiss_thresh10","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swiss_thresh12 = new TH2D("tower_spike_et_swiss_thresh12","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swiss_thresh12 = new TH2D("tower_spike_ene_swiss_thresh12","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swiss_thresh15 = new TH2D("tower_spike_et_swiss_thresh15","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swiss_thresh15 = new TH2D("tower_spike_ene_swiss_thresh15","",100,0.205,1.205,2,-0.5,1.5);

 tower_spike_et_swiss_thresh20 = new TH2D("tower_spike_et_swiss_thresh20","",100,0.205,1.205,2,-0.5,1.5);
 tower_spike_ene_swiss_thresh20 = new TH2D("tower_spike_ene_swiss_thresh20","",100,0.205,1.205,2,-0.5,1.5);


 sc_tower_spike_et_swiss = new TH2D("sc_tower_spike_et_swiss","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swiss = new TH2D("sc_tower_spike_ene_swiss","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swiss_thresh08 = new TH2D("sc_tower_spike_et_swiss_thresh08","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swiss_thresh08 = new TH2D("sc_tower_spike_ene_swiss_thresh08","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swiss_thresh10 = new TH2D("sc_tower_spike_et_swiss_thresh10","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swiss_thresh10 = new TH2D("sc_tower_spike_ene_swiss_thresh10","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swiss_thresh12 = new TH2D("sc_tower_spike_et_swiss_thresh12","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swiss_thresh12 = new TH2D("sc_tower_spike_ene_swiss_thresh12","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swiss_thresh15 = new TH2D("sc_tower_spike_et_swiss_thresh15","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swiss_thresh15 = new TH2D("sc_tower_spike_ene_swiss_thresh15","",100,0.205,1.205,2,-0.5,1.5);

 sc_tower_spike_et_swiss_thresh20 = new TH2D("sc_tower_spike_et_swiss_thresh20","",100,0.205,1.205,2,-0.5,1.5);
 sc_tower_spike_ene_swiss_thresh20 = new TH2D("sc_tower_spike_ene_swiss_thresh20","",100,0.205,1.205,2,-0.5,1.5);




  strip_prob_et = new TH2D("strip_prob_et","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene = new TH2D("strip_prob_ene","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et = new TH2D("tower_spike_et","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene = new TH2D("tower_spike_ene","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_pu = new TH3D("strip_prob_et_pu","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  strip_prob_ene_pu = new TH3D("strip_prob_ene_pu","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_et_pu = new TH3D("tower_spike_et_pu","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_ene_pu = new TH3D("tower_spike_ene_pu","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);


  strip_prob_et_thresh08   = new TH2D("strip_prob_et_thresh08","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_thresh08  = new TH2D("strip_prob_ene_thresh08","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_thresh08  = new TH2D("tower_spike_et_thresh08","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_thresh08 = new TH2D("tower_spike_ene_thresh08","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_pu_thresh08   = new TH3D("strip_prob_et_pu_thresh08","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  strip_prob_ene_pu_thresh08  = new TH3D("strip_prob_ene_pu_thresh08","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_et_pu_thresh08  = new TH3D("tower_spike_et_pu_thresh08","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_ene_pu_thresh08 = new TH3D("tower_spike_ene_pu_thresh08","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);



  strip_prob_et_thresh10   = new TH2D("strip_prob_et_thresh10","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_thresh10  = new TH2D("strip_prob_ene_thresh10","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_thresh10  = new TH2D("tower_spike_et_thresh10","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_thresh10 = new TH2D("tower_spike_ene_thresh10","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_pu_thresh10   = new TH3D("strip_prob_et_pu_thresh10","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  strip_prob_ene_pu_thresh10  = new TH3D("strip_prob_ene_pu_thresh10","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_et_pu_thresh10  = new TH3D("tower_spike_et_pu_thresh10","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_ene_pu_thresh10 = new TH3D("tower_spike_ene_pu_thresh10","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);



  strip_prob_et_thresh12   = new TH2D("strip_prob_et_thresh12","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_thresh12  = new TH2D("strip_prob_ene_thresh12","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_thresh12  = new TH2D("tower_spike_et_thresh12","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_thresh12 = new TH2D("tower_spike_ene_thresh12","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_pu_thresh12   = new TH3D("strip_prob_et_pu_thresh12","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  strip_prob_ene_pu_thresh12  = new TH3D("strip_prob_ene_pu_thresh12","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_et_pu_thresh12  = new TH3D("tower_spike_et_pu_thresh12","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_ene_pu_thresh12 = new TH3D("tower_spike_ene_pu_thresh12","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);



  strip_prob_et_thresh15   = new TH2D("strip_prob_et_thresh15","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_thresh15  = new TH2D("strip_prob_ene_thresh15","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_thresh15  = new TH2D("tower_spike_et_thresh15","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_thresh15 = new TH2D("tower_spike_ene_thresh15","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_pu_thresh15   = new TH3D("strip_prob_et_pu_thresh15","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  strip_prob_ene_pu_thresh15  = new TH3D("strip_prob_ene_pu_thresh15","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_et_pu_thresh15  = new TH3D("tower_spike_et_pu_thresh15","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_ene_pu_thresh15 = new TH3D("tower_spike_ene_pu_thresh15","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);



  strip_prob_et_thresh20   = new TH2D("strip_prob_et_thresh20","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_thresh20  = new TH2D("strip_prob_ene_thresh20","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_thresh20  = new TH2D("tower_spike_et_thresh20","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_thresh20 = new TH2D("tower_spike_ene_thresh20","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_pu_thresh20   = new TH3D("strip_prob_et_pu_thresh20","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  strip_prob_ene_pu_thresh20  = new TH3D("strip_prob_ene_pu_thresh20","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_et_pu_thresh20  = new TH3D("tower_spike_et_pu_thresh20","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  tower_spike_ene_pu_thresh20 = new TH3D("tower_spike_ene_pu_thresh20","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);




  strip_prob_et_mod1 = new TH2D("strip_prob_et_mod1","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_mod1 = new TH2D("strip_prob_ene_mod1","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_mod1 = new TH2D("tower_spike_et_mod1","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_mod1 = new TH2D("tower_spike_ene_mod1","",100,-0.5,1.5,2,-0.5,1.5);
  
  strip_prob_et_mod2 = new TH2D("strip_prob_et_mod2","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_mod2 = new TH2D("strip_prob_ene_mod2","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_mod2 = new TH2D("tower_spike_et_mod2","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_mod2 = new TH2D("tower_spike_ene_mod2","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_mod3 = new TH2D("strip_prob_et_mod3","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_mod3 = new TH2D("strip_prob_ene_mod3","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_mod3 = new TH2D("tower_spike_et_mod3","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_mod3 = new TH2D("tower_spike_ene_mod3","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_mod4 = new TH2D("strip_prob_et_mod4","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_mod4 = new TH2D("strip_prob_ene_mod4","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_mod4 = new TH2D("tower_spike_et_mod4","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_mod4 = new TH2D("tower_spike_ene_mod4","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_300 = new TH2D("strip_prob_et_300","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_300 = new TH2D("strip_prob_ene_300","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_300 = new TH2D("tower_spike_et_300","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_300 = new TH2D("tower_spike_ene_300","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_pu0 = new TH2D("strip_prob_et_pu0","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_pu0 = new TH2D("strip_prob_ene_pu0","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_pu0 = new TH2D("tower_spike_et_pu0","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_pu0 = new TH2D("tower_spike_ene_pu0","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_pu10 = new TH2D("strip_prob_et_pu10","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_pu10 = new TH2D("strip_prob_ene_pu10","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_pu10 = new TH2D("tower_spike_et_pu10","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_pu10 = new TH2D("tower_spike_ene_pu10","",100,-0.5,1.5,2,-0.5,1.5);

  strip_prob_et_pu20 = new TH2D("strip_prob_et_pu20","",100,-0.5,1.5,2,-0.5,1.5);
  strip_prob_ene_pu20 = new TH2D("strip_prob_ene_pu20","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_et_pu20 = new TH2D("tower_spike_et_pu20","",100,-0.5,1.5,2,-0.5,1.5);
  tower_spike_ene_pu20 = new TH2D("tower_spike_ene_pu20","",100,-0.5,1.5,2,-0.5,1.5);




  sc_strip_prob_et = new TH2D("sc_strip_prob_et","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene = new TH2D("sc_strip_prob_ene","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et = new TH2D("sc_tower_spike_et","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene = new TH2D("sc_tower_spike_ene","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_pu = new TH3D("sc_strip_prob_et_pu","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_strip_prob_ene_pu = new TH3D("sc_strip_prob_ene_pu","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_et_pu = new TH3D("sc_tower_spike_et_pu","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_ene_pu = new TH3D("sc_tower_spike_ene_pu","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);




  sc_strip_prob_et_thresh08   = new TH2D("sc_strip_prob_et_thresh08","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_thresh08  = new TH2D("sc_strip_prob_ene_thresh08","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_thresh08  = new TH2D("sc_tower_spike_et_thresh08","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_thresh08 = new TH2D("sc_tower_spike_ene_thresh08","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_pu_thresh08   = new TH3D("sc_strip_prob_et_pu_thresh08","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_strip_prob_ene_pu_thresh08  = new TH3D("sc_strip_prob_ene_pu_thresh08","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_et_pu_thresh08  = new TH3D("sc_tower_spike_et_pu_thresh08","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_ene_pu_thresh08 = new TH3D("sc_tower_spike_ene_pu_thresh08","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);



  sc_strip_prob_et_thresh10   = new TH2D("sc_strip_prob_et_thresh10","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_thresh10  = new TH2D("sc_strip_prob_ene_thresh10","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_thresh10  = new TH2D("sc_tower_spike_et_thresh10","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_thresh10 = new TH2D("sc_tower_spike_ene_thresh10","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_pu_thresh10   = new TH3D("sc_strip_prob_et_pu_thresh10","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_strip_prob_ene_pu_thresh10  = new TH3D("sc_strip_prob_ene_pu_thresh10","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_et_pu_thresh10  = new TH3D("sc_tower_spike_et_pu_thresh10","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_ene_pu_thresh10 = new TH3D("sc_tower_spike_ene_pu_thresh10","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);



  sc_strip_prob_et_thresh12   = new TH2D("sc_strip_prob_et_thresh12","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_thresh12  = new TH2D("sc_strip_prob_ene_thresh12","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_thresh12  = new TH2D("sc_tower_spike_et_thresh12","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_thresh12 = new TH2D("sc_tower_spike_ene_thresh12","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_pu_thresh12   = new TH3D("sc_strip_prob_et_pu_thresh12","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_strip_prob_ene_pu_thresh12  = new TH3D("sc_strip_prob_ene_pu_thresh12","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_et_pu_thresh12  = new TH3D("sc_tower_spike_et_pu_thresh12","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_ene_pu_thresh12 = new TH3D("sc_tower_spike_ene_pu_thresh12","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);



  sc_strip_prob_et_thresh15   = new TH2D("sc_strip_prob_et_thresh15","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_thresh15  = new TH2D("sc_strip_prob_ene_thresh15","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_thresh15  = new TH2D("sc_tower_spike_et_thresh15","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_thresh15 = new TH2D("sc_tower_spike_ene_thresh15","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_pu_thresh15   = new TH3D("sc_strip_prob_et_pu_thresh15","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_strip_prob_ene_pu_thresh15  = new TH3D("sc_strip_prob_ene_pu_thresh15","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_et_pu_thresh15  = new TH3D("sc_tower_spike_et_pu_thresh15","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_ene_pu_thresh15 = new TH3D("sc_tower_spike_ene_pu_thresh15","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);



  sc_strip_prob_et_thresh20   = new TH2D("sc_strip_prob_et_thresh20","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_thresh20  = new TH2D("sc_strip_prob_ene_thresh20","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_thresh20  = new TH2D("sc_tower_spike_et_thresh20","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_thresh20 = new TH2D("sc_tower_spike_ene_thresh20","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_pu_thresh20   = new TH3D("sc_strip_prob_et_pu_thresh20","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_strip_prob_ene_pu_thresh20  = new TH3D("sc_strip_prob_ene_pu_thresh20","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_et_pu_thresh20  = new TH3D("sc_tower_spike_et_pu_thresh20","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);
  sc_tower_spike_ene_pu_thresh20 = new TH3D("sc_tower_spike_ene_pu_thresh20","",100,-0.5,1.5,2,-0.5,1.5,20,0.,200.);









  sc_strip_prob_et_mod1 = new TH2D("sc_strip_prob_et_mod1","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_mod1 = new TH2D("sc_strip_prob_ene_mod1","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_mod1 = new TH2D("sc_tower_spike_et_mod1","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_mod1 = new TH2D("sc_tower_spike_ene_mod1","",100,-0.5,1.5,2,-0.5,1.5);
  
  sc_strip_prob_et_mod2 = new TH2D("sc_strip_prob_et_mod2","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_mod2 = new TH2D("sc_strip_prob_ene_mod2","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_mod2 = new TH2D("sc_tower_spike_et_mod2","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_mod2 = new TH2D("sc_tower_spike_ene_mod2","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_mod3 = new TH2D("sc_strip_prob_et_mod3","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_mod3 = new TH2D("sc_strip_prob_ene_mod3","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_mod3 = new TH2D("sc_tower_spike_et_mod3","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_mod3 = new TH2D("sc_tower_spike_ene_mod3","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_mod4 = new TH2D("sc_strip_prob_et_mod4","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_mod4 = new TH2D("sc_strip_prob_ene_mod4","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_mod4 = new TH2D("sc_tower_spike_et_mod4","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_mod4 = new TH2D("sc_tower_spike_ene_mod4","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_10 = new TH2D("sc_strip_prob_et_300","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_10 = new TH2D("sc_strip_prob_ene_300","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_10 = new TH2D("sc_tower_spike_et_300","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_10 = new TH2D("sc_tower_spike_ene_300","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_pu0 = new TH2D("sc_strip_prob_et_pu0","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_pu0 = new TH2D("sc_strip_prob_ene_pu0","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_pu0 = new TH2D("sc_tower_spike_et_pu0","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_pu0 = new TH2D("sc_tower_spike_ene_pu0","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_pu10 = new TH2D("sc_strip_prob_et_pu10","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_pu10 = new TH2D("sc_strip_prob_ene_pu10","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_pu10 = new TH2D("sc_tower_spike_et_pu10","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_pu10 = new TH2D("sc_tower_spike_ene_pu10","",100,-0.5,1.5,2,-0.5,1.5);

  sc_strip_prob_et_pu20 = new TH2D("sc_strip_prob_et_pu20","",100,-0.5,1.5,2,-0.5,1.5);
  sc_strip_prob_ene_pu20 = new TH2D("sc_strip_prob_ene_pu20","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_et_pu20 = new TH2D("sc_tower_spike_et_pu20","",100,-0.5,1.5,2,-0.5,1.5);
  sc_tower_spike_ene_pu20 = new TH2D("sc_tower_spike_ene_pu20","",100,-0.5,1.5,2,-0.5,1.5);





  eb_timing_0   = new TH1D("eb_timing_0","",400,-100.,100.);
  eb_timing_200   = new TH1D("eb_timing_200","",400,-100.,100.);
  eb_timing_400   = new TH1D("eb_timing_400","",400,-100.,100.);
  eb_timing_600   = new TH1D("eb_timing_600","",400,-100.,100.);
  eb_timing_800   = new TH1D("eb_timing_800","",400,-100.,100.);
  eb_timing_1000   = new TH1D("eb_timing_1000","",400,-100.,100.);
  eb_timing_2000   = new TH1D("eb_timing_2000","",400,-100.,100.);
  eb_timing_3000   = new TH1D("eb_timing_3000","",400,-100.,100.);
  eb_timing_4000   = new TH1D("eb_timing_4000","",400,-100.,100.);
  eb_timing_5000   = new TH1D("eb_timing_5000","",400,-100.,100.);
  eb_timing_10000   = new TH1D("eb_timing_10000","",400,-100.,100.);


  eb_timing_3000_spiketag            = new TH1D("eb_timing_3000_spiketag","",400,-100.,100.);
  eb_timing_4000_spiketag            = new TH1D("eb_timing_4000_spiketag","",400,-100.,100.);
  eb_timing_4000_kweird              = new TH1D("eb_timing_4000_kweird","",400,-100.,100.);
  eb_timing_4000_swisscross          = new TH1D("eb_timing_4000_swisscross","",400,-100.,100.);
  eb_timing_4000_spiketag_kweird     = new TH1D("eb_timing_4000_spiketag_kweird","",400,-100.,100.);
  eb_timing_4000_spiketag_swisscross = new TH1D("eb_timing_4000_spiketag_swisscross","",400,-100.,100.);
  eb_timing_4000_spiketag_noisedep   = new TH1D("eb_timing_4000_spiketag_noisedep","",400,-100.,100.);
  eb_timing_4000_sctag               = new TH1D("eb_timing_4000_sctag","",400,-100.,100.);
  eb_timing_4000_sctag_kweird        = new TH1D("eb_timing_4000_sctag_kweird","",400,-100.,100.);
  eb_timing_4000_sctag_swisscross    = new TH1D("eb_timing_4000_sctag_swisscross","",400,-100.,100.);
  eb_timing_4000_sctag_noisedep      = new TH1D("eb_timing_4000_sctag_noisedep","",400,-100.,100.);


  e4_over_noise=new TH1D("e4_over_noise","",1000,0,20);
  e4_over_noise_spiketag=new TH1D("e4_over_noise_spiketag","",1000,0,20);
  e4_over_noise_sctag=new TH1D("e4_over_noise_sctag","",1000,0,20);



  eb_timing_10000_spiketag            = new TH1D("eb_timing_10000_spiketag","",400,-100.,100.);
  eb_timing_10000_kweird              = new TH1D("eb_timing_10000_kweird","",400,-100.,100.);
  eb_timing_10000_swisscross          = new TH1D("eb_timing_10000_swisscross","",400,-100.,100.);
  eb_timing_10000_spiketag_kweird     = new TH1D("eb_timing_10000_spiketag_kweird","",400,-100.,100.);
  eb_timing_10000_spiketag_swisscross = new TH1D("eb_timing_10000_spiketag_swisscross","",400,-100.,100.);
  eb_timing_10000_spiketag_noisedep   = new TH1D("eb_timing_10000_spiketag_noisedep","",400,-100.,100.);
  eb_timing_10000_sctag               = new TH1D("eb_timing_10000_sctag","",400,-100.,100.);
  eb_timing_10000_sctag_kweird        = new TH1D("eb_timing_10000_sctag_kweird","",400,-100.,100.);
  eb_timing_10000_sctag_swisscross    = new TH1D("eb_timing_10000_sctag_swisscross","",400,-100.,100.);
  eb_timing_10000_sctag_noisedep      = new TH1D("eb_timing_10000_sctag_noisedep","",400,-100.,100.);


  spikes_vs_ieta_spiketag_4000 =            new TH1D("spikes_vs_ieta_spiketag_4000","",85,0,85);
  spikes_vs_ieta_spiketag_4000_kweird =     new TH1D("spikes_vs_ieta_spiketag_4000_kweird","",85,0,85);
  spikes_vs_ieta_spiketag_4000_swisscross = new TH1D("spikes_vs_ieta_spiketag_4000_swisscross","",85,0,85);
  spikes_vs_ieta_spiketag_4000_noisedep   = new TH1D("spikes_vs_ieta_spiketag_4000_noisedep","",85,0,85);


  sc_vs_ieta_sctag_4000 =            new TH1D("sc_vs_ieta_sctag_4000","",85,0,85);
  sc_vs_ieta_sctag_4000_kweird =     new TH1D("sc_vs_ieta_sctag_4000_kweird","",85,0,85);
  sc_vs_ieta_sctag_4000_swisscross = new TH1D("sc_vs_ieta_sctag_4000_swisscross","",85,0,85);
  sc_vs_ieta_sctag_4000_noisedep   = new TH1D("sc_vs_ieta_sctag_4000_noisedep","",85,0,85);


  spikes_vs_ieta_spiketag_10000 =            new TH1D("spikes_vs_ieta_spiketag_10000","",85,0,85);
  spikes_vs_ieta_spiketag_10000_kweird =     new TH1D("spikes_vs_ieta_spiketag_10000_kweird","",85,0,85);
  spikes_vs_ieta_spiketag_10000_swisscross = new TH1D("spikes_vs_ieta_spiketag_10000_swisscross","",85,0,85);
  spikes_vs_ieta_spiketag_10000_noisedep   = new TH1D("spikes_vs_ieta_spiketag_10000_noisedep","",85,0,85);
 
  sc_vs_ieta_sctag_10000 =            new TH1D("sc_vs_ieta_sctag_10000","",85,0,85);
  sc_vs_ieta_sctag_10000_kweird =     new TH1D("sc_vs_ieta_sctag_10000_kweird","",85,0,85);
  sc_vs_ieta_sctag_10000_swisscross = new TH1D("sc_vs_ieta_sctag_10000_swisscross","",85,0,85);
  sc_vs_ieta_sctag_10000_noisedep   = new TH1D("sc_vs_ieta_sctag_10000_noisedep","",85,0,85);


  swisscross_vs_ieta_spiketag_4000            = new TH2D("swisscross_vs_ieta_spiketag_4000","",80,0.7,1.1,85,0,85);
  swisscross_vs_ieta_spiketag_4000_kweird     = new TH2D("swisscross_vs_ieta_spiketag_4000_kweird","",80,0.7,1.1,85,0,85);

  swisscross_vs_ieta_spiketag_10000           = new TH2D("swisscross_vs_ieta_spiketag_10000","",80,0.7,1.1,85,0,85);
  swisscross_vs_ieta_spiketag_10000_kweird    = new TH2D("swisscross_vs_ieta_spiketag_10000_kweird","",80,0.7,1.1,85,0,85);



  swisscross_vs_ieta_sctag_4000            = new TH2D("swisscross_vs_ieta_sctag_4000","",80,0.7,1.1,85,0,85);
  swisscross_vs_ieta_sctag_4000_kweird     = new TH2D("swisscross_vs_ieta_sctag_4000_kweird","",80,0.7,1.1,85,0,85);

  swisscross_vs_ieta_sctag_10000           = new TH2D("swisscross_vs_ieta_sctag_10000","",80,0.7,1.1,85,0,85);
  swisscross_vs_ieta_sctag_10000_kweird    = new TH2D("swisscross_vs_ieta_sctag_10000_kweird","",80,0.7,1.1,85,0,85);




  eb_r4_0   = new TH1D("eb_r4_0","",200,0.2,1.2);
  eb_r4_200   = new TH1D("eb_r4_200","",200,0.2,1.2);
  eb_r4_400   = new TH1D("eb_r4_400","",200,0.2,1.2);
  eb_r4_600   = new TH1D("eb_r4_600","",200,0.2,1.2);
  eb_r4_800   = new TH1D("eb_r4_800","",200,0.2,1.2);
  eb_r4_1000   = new TH1D("eb_r4_1000","",200,0.2,1.2);
  eb_r4_2000   = new TH1D("eb_r4_2000","",200,0.2,1.2);
  eb_r4_3000   = new TH1D("eb_r4_3000","",200,0.2,1.2);
  eb_r4_4000   = new TH1D("eb_r4_4000","",200,0.2,1.2);
  eb_r4_5000   = new TH1D("eb_r4_5000","",200,0.2,1.2);
  eb_r4_10000   = new TH1D("eb_r4_10000","",200,0.2,1.2);


  eb_r4_3000_spiketag   = new TH1D("eb_r4_3000_spiketag","",200,0.2,1.2);
  eb_r4_4000_spiketag   = new TH1D("eb_r4_4000_spiketag","",200,0.2,1.2);
  eb_r4_4000_sctag      = new TH1D("eb_r4_4000_sctag","",200,0.2,1.2);
  eb_r4_4000_kweird     = new TH1D("eb_r4_4000_kweird","",200,0.2,1.2);
  eb_r4_4000_swisscross = new TH1D("eb_r4_4000_swisscross","",200,0.2,1.2);

  eb_r4_10000_spiketag   = new TH1D("eb_r4_10000_spiketag","",200,0.2,1.2);
  eb_r4_10000_sctag      = new TH1D("eb_r4_10000_sctag","",200,0.2,1.2);
  eb_r4_10000_kweird     = new TH1D("eb_r4_10000_kweird","",200,0.2,1.2);
  eb_r4_10000_swisscross = new TH1D("eb_r4_10000_swisscross","",200,0.2,1.2);




  eb_r6_1000   = new TH1D("eb_r6_1000","",200,0.2,1.2);
  eb_r6_2000   = new TH1D("eb_r6_2000","",200,0.2,1.2);
  eb_r6_3000   = new TH1D("eb_r6_3000","",200,0.2,1.2);
  eb_r6_4000   = new TH1D("eb_r6_4000","",200,0.2,1.2);
  eb_r6_5000   = new TH1D("eb_r6_5000","",200,0.2,1.2);
  eb_r6_10000   = new TH1D("eb_r6_10000","",200,0.2,1.2);


  eb_r4vsr6_3000   = new TH2D("eb_r4vsr6_3000","",100,0.2,1.2,100,0.2,1.2);
  eb_r4vsr6_4000   = new TH2D("eb_r4vsr6_4000","",100,0.2,1.2,100,0.2,1.2);
  eb_r4vsr6_5000   = new TH2D("eb_r4vsr6_5000","",100,0.2,1.2,100,0.2,1.2);
  eb_r4vsr6_10000   = new TH2D("eb_r4vsr6_10000","",100,0.2,1.2,100,0.2,1.2);




  eb_r6_3000_spiketag   = new TH1D("eb_r6_3000_spiketag","",200,0.2,2.2);
  eb_r6_4000_spiketag   = new TH1D("eb_r6_4000_spiketag","",200,0.2,2.2);
  eb_r6_4000_sctag      = new TH1D("eb_r6_4000_spiketag","",200,0.2,2.2);
  eb_r6_4000_kweird     = new TH1D("eb_r6_4000_kweird","",200,0.2,2.2);
  eb_r6_4000_swisscross = new TH1D("eb_r6_4000_swisscross","",200,0.2,2.2);

  eb_r6_10000_spiketag   = new TH1D("eb_r6_10000_spiketag","",200,0.2,2.2);
  eb_r6_10000_sctag      = new TH1D("eb_r6_10000_sctag","",200,0.2,2.2);
  eb_r6_10000_kweird     = new TH1D("eb_r6_10000_kweird","",200,0.2,2.2);
  eb_r6_10000_swisscross = new TH1D("eb_r6_10000_swisscross","",200,0.2,2.2);



 
 time_4gev_spiketime=new TH1D("time_4gev_spiketime","",200,-100,100);
 time_10gev_spiketime=new TH1D("time_10gev_spiketime","",200,-100,100);

 time_4gev_spikekweird=new TH1D("time_4gev_spikekweird","",200,-100,100);
 time_10gev_spikekweird=new TH1D("time_10gev_spikekweird","",200,-100,100);


 time_4gev_swisscross=new TH1D("time_4gev_swisscross","",200,-100,100);
 time_10gev_swisscross=new TH1D("time_10gev_swisscross","",200,-100,100);

 pu_4gev_spiketime=new TH1D("pu_4gev_spiketime","",100,0,100);
 pu_10gev_spiketime=new TH1D("pu_10gev_spiketime","",100,0,100);
 
 pu_4gev_spikekweird=new TH1D("pu_4gev_spikekweird","",100,0,100);
 pu_10gev_spikekweird=new TH1D("pu_10gev_spikekweird","",100,0,100);
 
 pu_4gev_swisscross=new TH1D("pu_4gev_swisscross","",100,0,100);
 pu_10gev_swisscross=new TH1D("pu_10gev_swisscross","",100,0,100);


 ene_4gev_spiketime=new TH1D("ene_4gev_spiketime","",100,0,100);
 ene_10gev_spiketime=new TH1D("ene_10gev_spiketime","",100,0,100);
 
 ene_4gev_spikekweird=new TH1D("ene_4gev_spikekweird","",100,0,100);
 ene_10gev_spikekweird=new TH1D("ene_10gev_spikekweird","",100,0,100);

 ene_4gev_swisscross=new TH1D("ene_4gev_swisscross","",100,0,100);
 ene_10gev_swisscross=new TH1D("ene_10gev_swisscross","",100,0,100);


 pu_vs_ene_spiketime=new TH2D("pu_vs_ene_spiketime","",50,0,100,50,0,100);
 pu_vs_ene_spikekweird=new TH2D("pu_vs_ene_spikekweird","",50,0,100,50,0,100);
 pu_vs_ene_swisscross=new TH2D("pu_vs_ene_swisscross","",50,0,100,50,0,100);





  eb_timing_r4_0   = new TH1D("eb_timing_r4_0","",400,-100.,100.);
  eb_timing_r4_200   = new TH1D("eb_timing_r4_200","",400,-100.,100.);
  eb_timing_r4_400   = new TH1D("eb_timing_r4_400","",400,-100.,100.);
  eb_timing_r4_600   = new TH1D("eb_timing_r4_600","",400,-100.,100.);
  eb_timing_r4_800   = new TH1D("eb_timing_r4_800","",400,-100.,100.);
  eb_timing_r4_1000   = new TH1D("eb_timing_r4_1000","",400,-100.,100.);
  eb_timing_r4_2000   = new TH1D("eb_timing_r4_2000","",400,-100.,100.);
  eb_timing_r4_3000   = new TH1D("eb_timing_r4_3000","",400,-100.,100.);
  eb_timing_r4_5000   = new TH1D("eb_timing_r4_5000","",400,-100.,100.);


  eb_timing_vs_r4_0   = new TH2D("eb_timing_vs_r4_0","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_200   = new TH2D("eb_timing_vs_r4_200","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_400   = new TH2D("eb_timing_vs_r4_400","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_600   = new TH2D("eb_timing_vs_r4_600","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_800   = new TH2D("eb_timing_vs_r4_800","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_1000   = new TH2D("eb_timing_vs_r4_1000","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_2000   = new TH2D("eb_timing_vs_r4_2000","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_3000   = new TH2D("eb_timing_vs_r4_3000","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_5000   = new TH2D("eb_timing_vs_r4_5000","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_10000   = new TH2D("eb_timing_vs_r4_10000","",100,0.2,1.2,200,-100.,100.);


  eb_timing_vs_r4_sctag_3000   = new TH2D("eb_timing_vs_r4_sctag_3000","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_sctag_5000   = new TH2D("eb_timing_vs_r4_sctag_5000","",100,0.2,1.2,200,-100.,100.);
  eb_timing_vs_r4_sctag_10000   = new TH2D("eb_timing_vs_r4_sctag_10000","",100,0.2,1.2,200,-100.,100.);



  rechiteta_vs_bxtrain_01 = new TH2D("rechiteta_vs_bxtrain_01","",40,-2,38,60,-3,3);
  rechiteta_vs_bxtrain_05 = new TH2D("rechiteta_vs_bxtrain_05","",40,-2,38,60,-3,3);
  sceta_vs_bxtrain = new TH2D("sceta_vs_bxtrain","",40,-2,38,60,-3,3);


  ebtime_vs_bxtrain_01 = new TH2D("ebtime_vx_bxtrain_01","",40,-2,38,200,-100,100);
  ebtime_vs_bxtrain_05 = new TH2D("ebtime_vx_bxtrain_05","",40,-2,38,200,-100,100);

  eetime_vs_bxtrain_01 = new TH2D("eetime_vx_bxtrain_01","",40,-2,38,200,-100,100);
  eetime_vs_bxtrain_05 = new TH2D("eetime_vx_bxtrain_05","",40,-2,38,200,-100,100);


  rechiteta_all=new TH1F("rechiteta_all","",100,-3,3);
  rechiteta_gt1et=new TH1F("rechiteta_gt1et","",100,-3,3);
  rechiteta_etweight=new TH1F("rechiteta_etweight","",100,-3,3);
  rechiteta_etweight_gt1et=new TH1F("rechiteta_etweight_gt1et","",100,-3,3);


  rechiteta_gt1et_pu10=new TH1F("rechiteta_gt1et_pu10","",100,-3,3);
  rechiteta_gt1et_pu20=new TH1F("rechiteta_gt1et_pu20","",100,-3,3);
  rechiteta_gt1et_pu30=new TH1F("rechiteta_gt1et_pu30","",100,-3,3);





  calotowereta_all=new TH1F("calotowereta_all","",100,-3,3);
  calotowereta_gt1et=new TH1F("calotowereta_gt1et","",100,-3,3);
  calotowereta_etweight=new TH1F("calotowereta_etweight","",100,-3,3);
  calotowereta_etweight_gt1et=new TH1F("calotowereta_etweight_gt1et","",100,-3,3);


  calotowereta_gt1et_pu10=new TH1F("calotowereta_gt1et_pu10","",100,-3,3);
  calotowereta_gt1et_pu20=new TH1F("calotowereta_gt1et_pu20","",100,-3,3);
  calotowereta_gt1et_pu30=new TH1F("calotowereta_gt1et_pu30","",100,-3,3);



  sceta_all=new TH1F("sceta_all","",100,-3,3);
  sceta_severity0=new TH1F("sceta_severity0","",100,-3,3);
  sceta_koot0=new TH1F("sceta_koot0","",100,-3,3);


  sceta_all_gt2=new TH1F("sceta_all_gt2","",100,-3,3);
  sceta_severity0_gt2=new TH1F("sceta_severity0_gt2","",100,-3,3);
  sceta_koot0_gt2=new TH1F("sceta_koot0_gt2","",100,-3,3);


  sceta_all_gt5=new TH1F("sceta_all_gt5","",100,-3,3);
  sceta_severity0_gt5=new TH1F("sceta_severity0_gt5","",100,-3,3);
  sceta_koot0_gt5=new TH1F("sceta_koot0_gt5","",100,-3,3);

  sceta_all_gt10=new TH1F("sceta_all_gt10","",100,-3,3);
  sceta_severity0_gt10=new TH1F("sceta_severity0_gt10","",100,-3,3);
  sceta_koot0_gt10=new TH1F("sceta_koot0_gt10","",100,-3,3);
  


  sceta_all_pueq01=new TH1F("sceta_all_pueq01","",100,-3,3);
  sceta_severity0_pueq01=new TH1F("sceta_severity0_pueq01","",100,-3,3);

  sceta_all_pueq02=new TH1F("sceta_all_pueq02","",100,-3,3);
  sceta_severity0_pueq02=new TH1F("sceta_severity0_pueq02","",100,-3,3);

  sceta_all_pueq03=new TH1F("sceta_all_pueq03","",100,-3,3);
  sceta_severity0_pueq03=new TH1F("sceta_severity0_pueq03","",100,-3,3);

  sceta_all_pueq04=new TH1F("sceta_all_pueq04","",100,-3,3);
  sceta_severity0_pueq04=new TH1F("sceta_severity0_pueq04","",100,-3,3);

  sceta_all_pueq05=new TH1F("sceta_all_pueq05","",100,-3,3);
  sceta_severity0_pueq05=new TH1F("sceta_severity0_pueq05","",100,-3,3);

  sceta_all_pueq06=new TH1F("sceta_all_pueq06","",100,-3,3);
  sceta_severity0_pueq06=new TH1F("sceta_severity0_pueq06","",100,-3,3);

  sceta_all_pueq07=new TH1F("sceta_all_pueq07","",100,-3,3);
  sceta_severity0_pueq07=new TH1F("sceta_severity0_pueq07","",100,-3,3);

  sceta_all_pueq08=new TH1F("sceta_all_pueq08","",100,-3,3);
  sceta_severity0_pueq08=new TH1F("sceta_severity0_pueq08","",100,-3,3);

  sceta_all_pueq09=new TH1F("sceta_all_pueq09","",100,-3,3);
  sceta_severity0_pueq09=new TH1F("sceta_severity0_pueq09","",100,-3,3);



  scet_eb_all=new TH1F("scet_eb_all","",200,0,100);
  scet_eb_severity0=new TH1F("scet_eb_severity0","",200,0,100);
  scet_eb_koot0=new TH1F("scet_eb_koot0","",200,0,100);

  scet_ee_all=new TH1F("scet_ee_all","",200,0,100);
  scet_ee_severity0=new TH1F("scet_ee_severity0","",200,0,100);
  scet_ee_koot0=new TH1F("scet_ee_koot0","",200,0,100);



  scet_eb_all_eta15=new TH1F("scet_eb_all_eta15","",200,0,100);
  scet_eb_all_eta20=new TH1F("scet_eb_all_eta20","",200,0,100);
  scet_eb_all_eta25=new TH1F("scet_eb_all_eta25","",200,0,100);


  scet_eb_all_eta15_pu10=new TH1F("scet_eb_all_eta15_pu10","",200,0,100);
  scet_eb_all_eta20_pu10=new TH1F("scet_eb_all_eta20_pu10","",200,0,100);
  scet_eb_all_eta25_pu10=new TH1F("scet_eb_all_eta25_pu10","",200,0,100);

  scet_eb_all_eta15_pu20=new TH1F("scet_eb_all_eta15_pu20","",200,0,100);
  scet_eb_all_eta20_pu20=new TH1F("scet_eb_all_eta20_pu20","",200,0,100);
  scet_eb_all_eta25_pu20=new TH1F("scet_eb_all_eta25_pu20","",200,0,100);

  scet_eb_all_eta15_pu30=new TH1F("scet_eb_all_eta15_pu30","",200,0,100);
  scet_eb_all_eta20_pu30=new TH1F("scet_eb_all_eta20_pu30","",200,0,100);
  scet_eb_all_eta25_pu30=new TH1F("scet_eb_all_eta25_pu30","",200,0,100);


  scet_eb_all_eta15_pueq10=new TH1F("scet_eb_all_eta15_pueq10","",200,0,100);
  scet_eb_all_eta20_pueq10=new TH1F("scet_eb_all_eta20_pueq10","",200,0,100);
  scet_eb_all_eta25_pueq10=new TH1F("scet_eb_all_eta25_pueq10","",200,0,100);

  scet_eb_all_eta15_pueq20=new TH1F("scet_eb_all_eta15_pueq20","",200,0,100);
  scet_eb_all_eta20_pueq20=new TH1F("scet_eb_all_eta20_pueq20","",200,0,100);
  scet_eb_all_eta25_pueq20=new TH1F("scet_eb_all_eta25_pueq20","",200,0,100);


  
  scet_ee_all_eta15=new TH1F("scet_ee_all_eta15","",200,0,100);
  scet_ee_all_eta20=new TH1F("scet_ee_all_eta20","",200,0,100);
  scet_ee_all_eta25=new TH1F("scet_ee_all_eta25","",200,0,100);


  scet_ee_all_eta15_pu10=new TH1F("scet_ee_all_eta15_pu10","",200,0,100);
  scet_ee_all_eta20_pu10=new TH1F("scet_ee_all_eta20_pu10","",200,0,100);
  scet_ee_all_eta25_pu10=new TH1F("scet_ee_all_eta25_pu10","",200,0,100);

  scet_ee_all_eta15_pu20=new TH1F("scet_ee_all_eta15_pu20","",200,0,100);
  scet_ee_all_eta20_pu20=new TH1F("scet_ee_all_eta20_pu20","",200,0,100);
  scet_ee_all_eta25_pu20=new TH1F("scet_ee_all_eta25_pu20","",200,0,100);

  scet_ee_all_eta15_pu30=new TH1F("scet_ee_all_eta15_pu30","",200,0,100);
  scet_ee_all_eta20_pu30=new TH1F("scet_ee_all_eta20_pu30","",200,0,100);
  scet_ee_all_eta25_pu30=new TH1F("scet_ee_all_eta25_pu30","",200,0,100);


  scet_ee_all_eta15_pueq10=new TH1F("scet_ee_all_eta15_pueq10","",200,0,100);
  scet_ee_all_eta20_pueq10=new TH1F("scet_ee_all_eta20_pueq10","",200,0,100);
  scet_ee_all_eta25_pueq10=new TH1F("scet_ee_all_eta25_pueq10","",200,0,100);

  scet_ee_all_eta15_pueq20=new TH1F("scet_ee_all_eta15_pueq20","",200,0,100);
  scet_ee_all_eta20_pueq20=new TH1F("scet_ee_all_eta20_pueq20","",200,0,100);
  scet_ee_all_eta25_pueq20=new TH1F("scet_ee_all_eta25_pueq20","",200,0,100);



  scsumet_eb_all_eta15=new TH1F("scsumet_eb_all_eta15","",200,0,200);
  scsumet_eb_all_eta20=new TH1F("scsumet_eb_all_eta20","",200,0,200);
  scsumet_eb_all_eta25=new TH1F("scsumet_eb_all_eta25","",200,0,200);


  scsumet_eb_all_eta15_pu10=new TH1F("scsumet_eb_all_eta15_pu10","",200,0,200);
  scsumet_eb_all_eta20_pu10=new TH1F("scsumet_eb_all_eta20_pu10","",200,0,200);
  scsumet_eb_all_eta25_pu10=new TH1F("scsumet_eb_all_eta25_pu10","",200,0,200);

  scsumet_eb_all_eta15_pu20=new TH1F("scsumet_eb_all_eta15_pu20","",200,0,200);
  scsumet_eb_all_eta20_pu20=new TH1F("scsumet_eb_all_eta20_pu20","",200,0,200);
  scsumet_eb_all_eta25_pu20=new TH1F("scsumet_eb_all_eta25_pu20","",200,0,200);

  scsumet_eb_all_eta15_pu30=new TH1F("scsumet_eb_all_eta15_pu30","",200,0,200);
  scsumet_eb_all_eta20_pu30=new TH1F("scsumet_eb_all_eta20_pu30","",200,0,200);
  scsumet_eb_all_eta25_pu30=new TH1F("scsumet_eb_all_eta25_pu30","",200,0,200);


  scsumet_eb_all_eta15_pueq10=new TH1F("scsumet_eb_all_eta15_pueq10","",200,0,200);
  scsumet_eb_all_eta20_pueq10=new TH1F("scsumet_eb_all_eta20_pueq10","",200,0,200);
  scsumet_eb_all_eta25_pueq10=new TH1F("scsumet_eb_all_eta25_pueq10","",200,0,200);

  scsumet_eb_all_eta15_pueq20=new TH1F("scsumet_eb_all_eta15_pueq20","",200,0,200);
  scsumet_eb_all_eta20_pueq20=new TH1F("scsumet_eb_all_eta20_pueq20","",200,0,200);
  scsumet_eb_all_eta25_pueq20=new TH1F("scsumet_eb_all_eta25_pueq20","",200,0,200);


  
  scsumet_ee_all_eta15=new TH1F("scsumet_ee_all_eta15","",200,0,200);
  scsumet_ee_all_eta20=new TH1F("scsumet_ee_all_eta20","",200,0,200);
  scsumet_ee_all_eta25=new TH1F("scsumet_ee_all_eta25","",200,0,200);


  scsumet_ee_all_eta15_pu10=new TH1F("scsumet_ee_all_eta15_pu10","",200,0,200);
  scsumet_ee_all_eta20_pu10=new TH1F("scsumet_ee_all_eta20_pu10","",200,0,200);
  scsumet_ee_all_eta25_pu10=new TH1F("scsumet_ee_all_eta25_pu10","",200,0,200);

  scsumet_ee_all_eta15_pu20=new TH1F("scsumet_ee_all_eta15_pu20","",200,0,200);
  scsumet_ee_all_eta20_pu20=new TH1F("scsumet_ee_all_eta20_pu20","",200,0,200);
  scsumet_ee_all_eta25_pu20=new TH1F("scsumet_ee_all_eta25_pu20","",200,0,200);

  scsumet_ee_all_eta15_pu30=new TH1F("scsumet_ee_all_eta15_pu30","",200,0,200);
  scsumet_ee_all_eta20_pu30=new TH1F("scsumet_ee_all_eta20_pu30","",200,0,200);
  scsumet_ee_all_eta25_pu30=new TH1F("scsumet_ee_all_eta25_pu30","",200,0,200);


  scsumet_ee_all_eta15_pueq10=new TH1F("scsumet_ee_all_eta15_pueq10","",200,0,200);
  scsumet_ee_all_eta20_pueq10=new TH1F("scsumet_ee_all_eta20_pueq10","",200,0,200);
  scsumet_ee_all_eta25_pueq10=new TH1F("scsumet_ee_all_eta25_pueq10","",200,0,200);

  scsumet_ee_all_eta15_pueq20=new TH1F("scsumet_ee_all_eta15_pueq20","",200,0,200);
  scsumet_ee_all_eta20_pueq20=new TH1F("scsumet_ee_all_eta20_pueq20","",200,0,200);
  scsumet_ee_all_eta25_pueq20=new TH1F("scsumet_ee_all_eta25_pueq20","",200,0,200);




  scet_eb_all_eta15_pueq01=new TH1F("scet_eb_all_eta15_pueq01","",200,0,100);
  scet_eb_all_eta20_pueq01=new TH1F("scet_eb_all_eta20_pueq01","",200,0,100);
  scet_eb_all_eta25_pueq01=new TH1F("scet_eb_all_eta25_pueq01","",200,0,100);

  scet_ee_all_eta15_pueq01=new TH1F("scet_ee_all_eta15_pueq01","",200,0,100);
  scet_ee_all_eta20_pueq01=new TH1F("scet_ee_all_eta20_pueq01","",200,0,100);
  scet_ee_all_eta25_pueq01=new TH1F("scet_ee_all_eta25_pueq01","",200,0,100);

  scsumet_eb_all_eta15_pueq01=new TH1F("scsumet_eb_all_eta15_pueq01","",200,0,200);
  scsumet_eb_all_eta20_pueq01=new TH1F("scsumet_eb_all_eta20_pueq01","",200,0,200);
  scsumet_eb_all_eta25_pueq01=new TH1F("scsumet_eb_all_eta25_pueq01","",200,0,200);

  scsumet_ee_all_eta15_pueq01=new TH1F("scsumet_ee_all_eta15_pueq01","",200,0,200);
  scsumet_ee_all_eta20_pueq01=new TH1F("scsumet_ee_all_eta20_pueq01","",200,0,200);
  scsumet_ee_all_eta25_pueq01=new TH1F("scsumet_ee_all_eta25_pueq01","",200,0,200);


  scet_eb_all_eta15_pueq02=new TH1F("scet_eb_all_eta15_pueq02","",200,0,100);
  scet_eb_all_eta20_pueq02=new TH1F("scet_eb_all_eta20_pueq02","",200,0,100);
  scet_eb_all_eta25_pueq02=new TH1F("scet_eb_all_eta25_pueq02","",200,0,100);

  scet_ee_all_eta15_pueq02=new TH1F("scet_ee_all_eta15_pueq02","",200,0,100);
  scet_ee_all_eta20_pueq02=new TH1F("scet_ee_all_eta20_pueq02","",200,0,100);
  scet_ee_all_eta25_pueq02=new TH1F("scet_ee_all_eta25_pueq02","",200,0,100);

  scsumet_eb_all_eta15_pueq02=new TH1F("scsumet_eb_all_eta15_pueq02","",200,0,200);
  scsumet_eb_all_eta20_pueq02=new TH1F("scsumet_eb_all_eta20_pueq02","",200,0,200);
  scsumet_eb_all_eta25_pueq02=new TH1F("scsumet_eb_all_eta25_pueq02","",200,0,200);

  scsumet_ee_all_eta15_pueq02=new TH1F("scsumet_ee_all_eta15_pueq02","",200,0,200);
  scsumet_ee_all_eta20_pueq02=new TH1F("scsumet_ee_all_eta20_pueq02","",200,0,200);
  scsumet_ee_all_eta25_pueq02=new TH1F("scsumet_ee_all_eta25_pueq02","",200,0,200);


  scet_eb_all_eta15_pueq03=new TH1F("scet_eb_all_eta15_pueq03","",200,0,100);
  scet_eb_all_eta20_pueq03=new TH1F("scet_eb_all_eta20_pueq03","",200,0,100);
  scet_eb_all_eta25_pueq03=new TH1F("scet_eb_all_eta25_pueq03","",200,0,100);

  scet_ee_all_eta15_pueq03=new TH1F("scet_ee_all_eta15_pueq03","",200,0,100);
  scet_ee_all_eta20_pueq03=new TH1F("scet_ee_all_eta20_pueq03","",200,0,100);
  scet_ee_all_eta25_pueq03=new TH1F("scet_ee_all_eta25_pueq03","",200,0,100);

  scsumet_eb_all_eta15_pueq03=new TH1F("scsumet_eb_all_eta15_pueq03","",200,0,200);
  scsumet_eb_all_eta20_pueq03=new TH1F("scsumet_eb_all_eta20_pueq03","",200,0,200);
  scsumet_eb_all_eta25_pueq03=new TH1F("scsumet_eb_all_eta25_pueq03","",200,0,200);

  scsumet_ee_all_eta15_pueq03=new TH1F("scsumet_ee_all_eta15_pueq03","",200,0,200);
  scsumet_ee_all_eta20_pueq03=new TH1F("scsumet_ee_all_eta20_pueq03","",200,0,200);
  scsumet_ee_all_eta25_pueq03=new TH1F("scsumet_ee_all_eta25_pueq03","",200,0,200);


  scet_eb_all_eta15_pueq04=new TH1F("scet_eb_all_eta15_pueq04","",200,0,100);
  scet_eb_all_eta20_pueq04=new TH1F("scet_eb_all_eta20_pueq04","",200,0,100);
  scet_eb_all_eta25_pueq04=new TH1F("scet_eb_all_eta25_pueq04","",200,0,100);

  scet_ee_all_eta15_pueq04=new TH1F("scet_ee_all_eta15_pueq04","",200,0,100);
  scet_ee_all_eta20_pueq04=new TH1F("scet_ee_all_eta20_pueq04","",200,0,100);
  scet_ee_all_eta25_pueq04=new TH1F("scet_ee_all_eta25_pueq04","",200,0,100);

  scsumet_eb_all_eta15_pueq04=new TH1F("scsumet_eb_all_eta15_pueq04","",200,0,200);
  scsumet_eb_all_eta20_pueq04=new TH1F("scsumet_eb_all_eta20_pueq04","",200,0,200);
  scsumet_eb_all_eta25_pueq04=new TH1F("scsumet_eb_all_eta25_pueq04","",200,0,200);

  scsumet_ee_all_eta15_pueq04=new TH1F("scsumet_ee_all_eta15_pueq04","",200,0,200);
  scsumet_ee_all_eta20_pueq04=new TH1F("scsumet_ee_all_eta20_pueq04","",200,0,200);
  scsumet_ee_all_eta25_pueq04=new TH1F("scsumet_ee_all_eta25_pueq04","",200,0,200);


  scet_eb_all_eta15_pueq05=new TH1F("scet_eb_all_eta15_pueq05","",200,0,100);
  scet_eb_all_eta20_pueq05=new TH1F("scet_eb_all_eta20_pueq05","",200,0,100);
  scet_eb_all_eta25_pueq05=new TH1F("scet_eb_all_eta25_pueq05","",200,0,100);

  scet_ee_all_eta15_pueq05=new TH1F("scet_ee_all_eta15_pueq05","",200,0,100);
  scet_ee_all_eta20_pueq05=new TH1F("scet_ee_all_eta20_pueq05","",200,0,100);
  scet_ee_all_eta25_pueq05=new TH1F("scet_ee_all_eta25_pueq05","",200,0,100);

  scsumet_eb_all_eta15_pueq05=new TH1F("scsumet_eb_all_eta15_pueq05","",200,0,200);
  scsumet_eb_all_eta20_pueq05=new TH1F("scsumet_eb_all_eta20_pueq05","",200,0,200);
  scsumet_eb_all_eta25_pueq05=new TH1F("scsumet_eb_all_eta25_pueq05","",200,0,200);

  scsumet_ee_all_eta15_pueq05=new TH1F("scsumet_ee_all_eta15_pueq05","",200,0,200);
  scsumet_ee_all_eta20_pueq05=new TH1F("scsumet_ee_all_eta20_pueq05","",200,0,200);
  scsumet_ee_all_eta25_pueq05=new TH1F("scsumet_ee_all_eta25_pueq05","",200,0,200);


  scet_eb_all_eta15_pueq06=new TH1F("scet_eb_all_eta15_pueq06","",200,0,100);
  scet_eb_all_eta20_pueq06=new TH1F("scet_eb_all_eta20_pueq06","",200,0,100);
  scet_eb_all_eta25_pueq06=new TH1F("scet_eb_all_eta25_pueq06","",200,0,100);

  scet_ee_all_eta15_pueq06=new TH1F("scet_ee_all_eta15_pueq06","",200,0,100);
  scet_ee_all_eta20_pueq06=new TH1F("scet_ee_all_eta20_pueq06","",200,0,100);
  scet_ee_all_eta25_pueq06=new TH1F("scet_ee_all_eta25_pueq06","",200,0,100);

  scsumet_eb_all_eta15_pueq06=new TH1F("scsumet_eb_all_eta15_pueq06","",200,0,200);
  scsumet_eb_all_eta20_pueq06=new TH1F("scsumet_eb_all_eta20_pueq06","",200,0,200);
  scsumet_eb_all_eta25_pueq06=new TH1F("scsumet_eb_all_eta25_pueq06","",200,0,200);

  scsumet_ee_all_eta15_pueq06=new TH1F("scsumet_ee_all_eta15_pueq06","",200,0,200);
  scsumet_ee_all_eta20_pueq06=new TH1F("scsumet_ee_all_eta20_pueq06","",200,0,200);
  scsumet_ee_all_eta25_pueq06=new TH1F("scsumet_ee_all_eta25_pueq06","",200,0,200);


  scet_eb_all_eta15_pueq07=new TH1F("scet_eb_all_eta15_pueq07","",200,0,100);
  scet_eb_all_eta20_pueq07=new TH1F("scet_eb_all_eta20_pueq07","",200,0,100);
  scet_eb_all_eta25_pueq07=new TH1F("scet_eb_all_eta25_pueq07","",200,0,100);

  scet_ee_all_eta15_pueq07=new TH1F("scet_ee_all_eta15_pueq07","",200,0,100);
  scet_ee_all_eta20_pueq07=new TH1F("scet_ee_all_eta20_pueq07","",200,0,100);
  scet_ee_all_eta25_pueq07=new TH1F("scet_ee_all_eta25_pueq07","",200,0,100);

  scsumet_eb_all_eta15_pueq07=new TH1F("scsumet_eb_all_eta15_pueq07","",200,0,200);
  scsumet_eb_all_eta20_pueq07=new TH1F("scsumet_eb_all_eta20_pueq07","",200,0,200);
  scsumet_eb_all_eta25_pueq07=new TH1F("scsumet_eb_all_eta25_pueq07","",200,0,200);

  scsumet_ee_all_eta15_pueq07=new TH1F("scsumet_ee_all_eta15_pueq07","",200,0,200);
  scsumet_ee_all_eta20_pueq07=new TH1F("scsumet_ee_all_eta20_pueq07","",200,0,200);
  scsumet_ee_all_eta25_pueq07=new TH1F("scsumet_ee_all_eta25_pueq07","",200,0,200);


  scet_eb_all_eta15_pueq08=new TH1F("scet_eb_all_eta15_pueq08","",200,0,100);
  scet_eb_all_eta20_pueq08=new TH1F("scet_eb_all_eta20_pueq08","",200,0,100);
  scet_eb_all_eta25_pueq08=new TH1F("scet_eb_all_eta25_pueq08","",200,0,100);

  scet_ee_all_eta15_pueq08=new TH1F("scet_ee_all_eta15_pueq08","",200,0,100);
  scet_ee_all_eta20_pueq08=new TH1F("scet_ee_all_eta20_pueq08","",200,0,100);
  scet_ee_all_eta25_pueq08=new TH1F("scet_ee_all_eta25_pueq08","",200,0,100);

  scsumet_eb_all_eta15_pueq08=new TH1F("scsumet_eb_all_eta15_pueq08","",200,0,200);
  scsumet_eb_all_eta20_pueq08=new TH1F("scsumet_eb_all_eta20_pueq08","",200,0,200);
  scsumet_eb_all_eta25_pueq08=new TH1F("scsumet_eb_all_eta25_pueq08","",200,0,200);

  scsumet_ee_all_eta15_pueq08=new TH1F("scsumet_ee_all_eta15_pueq08","",200,0,200);
  scsumet_ee_all_eta20_pueq08=new TH1F("scsumet_ee_all_eta20_pueq08","",200,0,200);
  scsumet_ee_all_eta25_pueq08=new TH1F("scsumet_ee_all_eta25_pueq08","",200,0,200);


  scet_eb_all_eta15_pueq09=new TH1F("scet_eb_all_eta15_pueq09","",200,0,100);
  scet_eb_all_eta20_pueq09=new TH1F("scet_eb_all_eta20_pueq09","",200,0,100);
  scet_eb_all_eta25_pueq09=new TH1F("scet_eb_all_eta25_pueq09","",200,0,100);

  scet_ee_all_eta15_pueq09=new TH1F("scet_ee_all_eta15_pueq09","",200,0,100);
  scet_ee_all_eta20_pueq09=new TH1F("scet_ee_all_eta20_pueq09","",200,0,100);
  scet_ee_all_eta25_pueq09=new TH1F("scet_ee_all_eta25_pueq09","",200,0,100);

  scsumet_eb_all_eta15_pueq09=new TH1F("scsumet_eb_all_eta15_pueq09","",200,0,200);
  scsumet_eb_all_eta20_pueq09=new TH1F("scsumet_eb_all_eta20_pueq09","",200,0,200);
  scsumet_eb_all_eta25_pueq09=new TH1F("scsumet_eb_all_eta25_pueq09","",200,0,200);

  scsumet_ee_all_eta15_pueq09=new TH1F("scsumet_ee_all_eta15_pueq09","",200,0,200);
  scsumet_ee_all_eta20_pueq09=new TH1F("scsumet_ee_all_eta20_pueq09","",200,0,200);
  scsumet_ee_all_eta25_pueq09=new TH1F("scsumet_ee_all_eta25_pueq09","",200,0,200);



  scsumet_eb_all=new TH1F("scsumet_eb_all","",200,0,200);
  scsumet_eb_severity0=new TH1F("scsumet_eb_severity0","",200,0,200);
  scsumet_eb_koot0=new TH1F("scsumet_eb_koot0","",200,0,200);

  scsumet_ee_all=new TH1F("scsumet_ee_all","",200,0,200);
  scsumet_ee_severity0=new TH1F("scsumet_ee_severity0","",200,0,200);
  scsumet_ee_koot0=new TH1F("scsumet_ee_koot0","",200,0,200);


  scsumet_eb_all_gt2=new TH1F("scsumet_eb_all_gt2","",200,0,200);
  scsumet_eb_severity0_gt2=new TH1F("scsumet_eb_severity0_gt2","",200,0,200);
  scsumet_eb_koot0_gt2=new TH1F("scsumet_eb_koot0_gt2","",200,0,200);

  scsumet_ee_all_gt2=new TH1F("scsumet_ee_all_gt2","",200,0,200);
  scsumet_ee_severity0_gt2=new TH1F("scsumet_ee_severity0_gt2","",200,0,200);
  scsumet_ee_koot0_gt2=new TH1F("scsumet_ee_koot0_gt2","",200,0,200);



  scsumet_eb_all_gt5=new TH1F("scsumet_eb_all_gt5","",200,0,200);
  scsumet_eb_severity0_gt5=new TH1F("scsumet_eb_severity0_gt5","",200,0,200);
  scsumet_eb_koot0_gt5=new TH1F("scsumet_eb_koot0_gt5","",200,0,200);

  scsumet_ee_all_gt5=new TH1F("scsumet_ee_all_gt5","",200,0,200);
  scsumet_ee_severity0_gt5=new TH1F("scsumet_ee_severity0_gt5","",200,0,200);
  scsumet_ee_koot0_gt5=new TH1F("scsumet_ee_koot0_gt5","",200,0,200);



  scsumet_eb_all_gt10=new TH1F("scsumet_eb_all_gt10","",200,0,200);
  scsumet_eb_severity0_gt10=new TH1F("scsumet_eb_severity0_gt10","",200,0,200);
  scsumet_eb_koot0_gt10=new TH1F("scsumet_eb_koot0_gt10","",200,0,200);

  scsumet_ee_all_gt10=new TH1F("scsumet_ee_all_gt10","",200,0,200);
  scsumet_ee_severity0_gt10=new TH1F("scsumet_ee_severity0_gt10","",200,0,200);
  scsumet_ee_koot0_gt10=new TH1F("scsumet_ee_koot0_gt10","",200,0,200);

  swisscross_vs_energy_spiketag=new TH2D("swisscross_vs_energy_spiketag","",80,0.7,1.1,100,0,20);
  swisscross_vs_energy_sctag=new TH2D("swisscross_vs_energy_sctag","",80,0.7,1.1,100,0,20);
  swisscross_vs_energy_spiketag_kweird=new TH2D("swisscross_vs_energy_spiketag_kweird","",80,0.7,1.1,100,0,20);
  swisscross_vs_energy_sctag_kweird=new TH2D("swisscross_vs_energy_sctag_kweird","",80,0.7,1.1,100,0,20);


  time_vs_energy_spiketag=new TH2D("time_vs_energy_spiketag","",120,-60,60,100,0,20);
  time_vs_energy_sctag=new TH2D("time_vs_energy_sctag","",120,-60,60,100,0,20);
  time_vs_energy_spiketag_kweird=new TH2D("time_vs_energy_spiketag_kweird","",120,-60,60,100,0,20);
  time_vs_energy_spiketag_swisscross=new TH2D("time_vs_energy_spiketag_swisscross","",120,-60,60,100,0,20);
  time_vs_energy_sctag_kweird=new TH2D("time_vs_energy_sctag_kweird","",120,-60,60,100,0,20);
  time_vs_energy_sctag_swisscross=new TH2D("time_vs_energy_sctag_swisscross","",120,-60,60,100,0,20);


  eb_digi_01=new TH2D("eb_digi_01","",10,0,10,1000,0,1000);
  ee_digi_01=new TH2D("ee_digi_01","",10,0,10,1000,0,1000);

  eb_digi_05=new TH2D("eb_digi_05","",10,0,10,1000,0,1000);
  ee_digi_05=new TH2D("ee_digi_05","",10,0,10,1000,0,1000);

  eb_digi_30=new TH2D("eb_digi_30","",10,0,10,1000,0,1000);
  ee_digi_30=new TH2D("ee_digi_30","",10,0,10,1000,0,1000);
  

  eb_digi_0105=new TH2D("eb_digi_0105","",10,0,10,100,190,290);
  ee_digi_0105=new TH2D("ee_digi_0105","",10,0,10,100,190,290);

  eb_digi_0530=new TH2D("eb_digi_0530","",10,0,10,200,190,390);
  ee_digi_0530=new TH2D("ee_digi_0530","",10,0,10,200,190,390);


  eb_digi_0105_vs_time=new TH2D("eb_digi_0105_vs_time","",120,-60,60,10,0,10);
  ee_digi_0105_vs_time=new TH2D("ee_digi_0105_vs_time","",120,-60,60,10,0,10);

  eb_digi_0530_vs_time=new TH2D("eb_digi_0530_vs_time","",120,-60,60,10,0,10);
  ee_digi_0530_vs_time=new TH2D("ee_digi_0530_vs_time","",120,-60,60,10,0,10);


  eb_digi_0105_vs_time_norm=new TH2D("eb_digi_0105_vs_time_norm","",120,-60,60,10,0,10);
  ee_digi_0105_vs_time_norm=new TH2D("ee_digi_0105_vs_time_norm","",120,-60,60,10,0,10);

  eb_digi_0530_vs_time_norm=new TH2D("eb_digi_0530_vs_time_norm","",120,-60,60,10,0,10);
  ee_digi_0530_vs_time_norm=new TH2D("ee_digi_0530_vs_time_norm","",120,-60,60,10,0,10);
  

  eb_digi_0105_vs_bxtrain=new TH2D("eb_digi_0105_vs_bxtrain","",40,-2,38,10,0,10);
  ee_digi_0105_vs_bxtrain=new TH2D("ee_digi_0105_vs_bxtrain","",40,-2,38,10,0,10);

  eb_digi_0530_vs_bxtrain=new TH2D("eb_digi_0530_vs_bxtrain","",40,-2,38,10,0,10);
  ee_digi_0530_vs_bxtrain=new TH2D("ee_digi_0530_vs_bxtrain","",40,-2,38,10,0,10);



  eb_digi_0105_vs_bxtrain_norm=new TH2D("eb_digi_0105_vs_bxtrain_norm","",40,-2,38,10,0,10);
  ee_digi_0105_vs_bxtrain_norm=new TH2D("ee_digi_0105_vs_bxtrain_norm","",40,-2,38,10,0,10);

  eb_digi_0530_vs_bxtrain_norm=new TH2D("eb_digi_0530_vs_bxtrain_norm","",40,-2,38,10,0,10);
  ee_digi_0530_vs_bxtrain_norm=new TH2D("ee_digi_0530_vs_bxtrain_norm","",40,-2,38,10,0,10);



  eb_digi_0105_vs_time_eta15=new TH2D("eb_digi_0105_vs_time_eta15","",120,-60,60,10,0,10);
  ee_digi_0105_vs_time_eta15=new TH2D("ee_digi_0105_vs_time_eta15","",120,-60,60,10,0,10);

  eb_digi_0530_vs_time_eta15=new TH2D("eb_digi_0530_vs_time_eta15","",120,-60,60,10,0,10);
  ee_digi_0530_vs_time_eta15=new TH2D("ee_digi_0530_vs_time_eta15","",120,-60,60,10,0,10);


  eb_digi_0105_vs_time_norm_eta15=new TH2D("eb_digi_0105_vs_time_norm_eta15","",120,-60,60,10,0,10);
  ee_digi_0105_vs_time_norm_eta15=new TH2D("ee_digi_0105_vs_time_norm_eta15","",120,-60,60,10,0,10);

  eb_digi_0530_vs_time_norm_eta15=new TH2D("eb_digi_0530_vs_time_norm_eta15","",120,-60,60,10,0,10);
  ee_digi_0530_vs_time_norm_eta15=new TH2D("ee_digi_0530_vs_time_norm_eta15","",120,-60,60,10,0,10);
  

  eb_digi_0105_vs_bxtrain_eta15=new TH2D("eb_digi_0105_vs_bxtrain_eta15","",40,-2,38,10,0,10);
  ee_digi_0105_vs_bxtrain_eta15=new TH2D("ee_digi_0105_vs_bxtrain_eta15","",40,-2,38,10,0,10);

  eb_digi_0530_vs_bxtrain_eta15=new TH2D("eb_digi_0530_vs_bxtrain_eta15","",40,-2,38,10,0,10);
  ee_digi_0530_vs_bxtrain_eta15=new TH2D("ee_digi_0530_vs_bxtrain_eta15","",40,-2,38,10,0,10);



  eb_digi_0105_vs_bxtrain_norm_eta15=new TH2D("eb_digi_0105_vs_bxtrain_norm_eta15","",40,-2,38,10,0,10);
  ee_digi_0105_vs_bxtrain_norm_eta15=new TH2D("ee_digi_0105_vs_bxtrain_norm_eta15","",40,-2,38,10,0,10);

  eb_digi_0530_vs_bxtrain_norm_eta15=new TH2D("eb_digi_0530_vs_bxtrain_norm_eta15","",40,-2,38,10,0,10);
  ee_digi_0530_vs_bxtrain_norm_eta15=new TH2D("ee_digi_0530_vs_bxtrain_norm_eta15","",40,-2,38,10,0,10);



  eb_digi_0105_vs_time_eta20=new TH2D("eb_digi_0105_vs_time_eta20","",120,-60,60,10,0,10);
  ee_digi_0105_vs_time_eta20=new TH2D("ee_digi_0105_vs_time_eta20","",120,-60,60,10,0,10);

  eb_digi_0530_vs_time_eta20=new TH2D("eb_digi_0530_vs_time_eta20","",120,-60,60,10,0,10);
  ee_digi_0530_vs_time_eta20=new TH2D("ee_digi_0530_vs_time_eta20","",120,-60,60,10,0,10);


  eb_digi_0105_vs_time_norm_eta20=new TH2D("eb_digi_0105_vs_time_norm_eta20","",120,-60,60,10,0,10);
  ee_digi_0105_vs_time_norm_eta20=new TH2D("ee_digi_0105_vs_time_norm_eta20","",120,-60,60,10,0,10);

  eb_digi_0530_vs_time_norm_eta20=new TH2D("eb_digi_0530_vs_time_norm_eta20","",120,-60,60,10,0,10);
  ee_digi_0530_vs_time_norm_eta20=new TH2D("ee_digi_0530_vs_time_norm_eta20","",120,-60,60,10,0,10);
  

  eb_digi_0105_vs_bxtrain_eta20=new TH2D("eb_digi_0105_vs_bxtrain_eta20","",40,-2,38,10,0,10);
  ee_digi_0105_vs_bxtrain_eta20=new TH2D("ee_digi_0105_vs_bxtrain_eta20","",40,-2,38,10,0,10);

  eb_digi_0530_vs_bxtrain_eta20=new TH2D("eb_digi_0530_vs_bxtrain_eta20","",40,-2,38,10,0,10);
  ee_digi_0530_vs_bxtrain_eta20=new TH2D("ee_digi_0530_vs_bxtrain_eta20","",40,-2,38,10,0,10);



  eb_digi_0105_vs_bxtrain_norm_eta20=new TH2D("eb_digi_0105_vs_bxtrain_norm_eta20","",40,-2,38,10,0,10);
  ee_digi_0105_vs_bxtrain_norm_eta20=new TH2D("ee_digi_0105_vs_bxtrain_norm_eta20","",40,-2,38,10,0,10);

  eb_digi_0530_vs_bxtrain_norm_eta20=new TH2D("eb_digi_0530_vs_bxtrain_norm_eta20","",40,-2,38,10,0,10);
  ee_digi_0530_vs_bxtrain_norm_eta20=new TH2D("ee_digi_0530_vs_bxtrain_norm_eta20","",40,-2,38,10,0,10);




  eb_digi_0105_vs_time_eta25=new TH2D("eb_digi_0105_vs_time_eta25","",120,-60,60,10,0,10);
  ee_digi_0105_vs_time_eta25=new TH2D("ee_digi_0105_vs_time_eta25","",120,-60,60,10,0,10);

  eb_digi_0530_vs_time_eta25=new TH2D("eb_digi_0530_vs_time_eta25","",120,-60,60,10,0,10);
  ee_digi_0530_vs_time_eta25=new TH2D("ee_digi_0530_vs_time_eta25","",120,-60,60,10,0,10);


  eb_digi_0105_vs_time_norm_eta25=new TH2D("eb_digi_0105_vs_time_norm_eta25","",120,-60,60,10,0,10);
  ee_digi_0105_vs_time_norm_eta25=new TH2D("ee_digi_0105_vs_time_norm_eta25","",120,-60,60,10,0,10);

  eb_digi_0530_vs_time_norm_eta25=new TH2D("eb_digi_0530_vs_time_norm_eta25","",120,-60,60,10,0,10);
  ee_digi_0530_vs_time_norm_eta25=new TH2D("ee_digi_0530_vs_time_norm_eta25","",120,-60,60,10,0,10);
  

  eb_digi_0105_vs_bxtrain_eta25=new TH2D("eb_digi_0105_vs_bxtrain_eta25","",40,-2,38,10,0,10);
  ee_digi_0105_vs_bxtrain_eta25=new TH2D("ee_digi_0105_vs_bxtrain_eta25","",40,-2,38,10,0,10);

  eb_digi_0530_vs_bxtrain_eta25=new TH2D("eb_digi_0530_vs_bxtrain_eta25","",40,-2,38,10,0,10);
  ee_digi_0530_vs_bxtrain_eta25=new TH2D("ee_digi_0530_vs_bxtrain_eta25","",40,-2,38,10,0,10);



  eb_digi_0105_vs_bxtrain_norm_eta25=new TH2D("eb_digi_0105_vs_bxtrain_norm_eta25","",40,-2,38,10,0,10);
  ee_digi_0105_vs_bxtrain_norm_eta25=new TH2D("ee_digi_0105_vs_bxtrain_norm_eta25","",40,-2,38,10,0,10);

  eb_digi_0530_vs_bxtrain_norm_eta25=new TH2D("eb_digi_0530_vs_bxtrain_norm_eta25","",40,-2,38,10,0,10);
  ee_digi_0530_vs_bxtrain_norm_eta25=new TH2D("ee_digi_0530_vs_bxtrain_norm_eta25","",40,-2,38,10,0,10);


  ebchstatus=new TH2D("ebchstatus","",360,0,360,170,-85,85);
  eechstatus=new TH2D("eechstatus","",200,0,200,100,0,100);


  ebpedmean_g12=new TH2D("ebpedmean_g12","",360,0,360,170,-85,85);
  ebpedmean_g6=new TH2D("ebpedmean_g6","",360,0,360,170,-85,85);
  ebpedmean_g1=new TH2D("ebpedmean_g1","",360,0,360,170,-85,85);


  ebpedrms_g12=new TH2D("ebpedrms_g12","",360,0,360,170,-85,85);
  ebpedrms_g6=new TH2D("ebpedrms_g6","",360,0,360,170,-85,85);
  ebpedrms_g1=new TH2D("ebpedrms_g1","",360,0,360,170,-85,85);

  eepedmean_g12=new TH2D("eepedmean_g12","",200,0,200,100,0,100);
  eepedmean_g6=new TH2D("eepedmean_g6","",200,0,200,100,0,100);
  eepedmean_g1=new TH2D("eepedmean_g1","",200,0,200,100,0,100);

  eepedrms_g12=new TH2D("eepedrms_g12","",200,0,200,100,0,100);
  eepedrms_g6=new TH2D("eepedrms_g6","",200,0,200,100,0,100);
  eepedrms_g1=new TH2D("eepedrms_g1","",200,0,200,100,0,100);






  dataTree_      = new TTree("dataTree","dataTree");

  // physics declared and technical trigger bits

  dataTree_->Branch("physdeclared",&physdeclared,"physdeclared/I");  
  dataTree_->Branch("bit36",       &bit36,   	 "bit36/I");
  dataTree_->Branch("bit37",       &bit37,   	 "bit37/I");
  dataTree_->Branch("bit38",       &bit38,   	 "bit38/I");
  dataTree_->Branch("bit39",       &bit39,   	 "bit39/I");
  dataTree_->Branch("bit40",       &bit40,   	 "bit40/I");
  dataTree_->Branch("bit41",       &bit41,   	 "bit41/I");
  dataTree_->Branch("bit3",        &bit3,        "bit3/I");
  dataTree_->Branch("bit4",        &bit4,        "bit4/I");
  dataTree_->Branch("bit9",        &bit9,        "bit9/I");
  
  dataTree_->Branch("bit0",        &bit0,        "bit0/I");


  dataTree_->Branch("eg1",         &eg1,         "eg1/I");
  dataTree_->Branch("eg2",         &eg2,         "eg2/I");
  dataTree_->Branch("eg5",         &eg5,         "eg5/I");
  dataTree_->Branch("algo124",     &algo124,     "algo124/I");


  // muon triggers

  dataTree_->Branch("algo54",     &algo54,     "algo54/I");
  dataTree_->Branch("algo55",     &algo55,     "algo55/I");
  dataTree_->Branch("algo56",     &algo56,     "algo56/I");
  dataTree_->Branch("algo57",     &algo57,     "algo57/I");
  dataTree_->Branch("algo58",     &algo58,     "algo58/I");
  dataTree_->Branch("algo59",     &algo59,     "algo59/I");
  dataTree_->Branch("algo60",     &algo60,     "algo60/I");
  dataTree_->Branch("algo61",     &algo61,     "algo61/I");
  dataTree_->Branch("algo62",     &algo62,     "algo62/I");

  dataTree_->Branch("algo106",    &algo106,    "algo106/I");
  dataTree_->Branch("algo107",    &algo107,    "algo107/I");


  // run/event info
  
  dataTree_->Branch("run",         &run,     	 "run/I");  
  dataTree_->Branch("even",        &even,   	 "even/I");
  dataTree_->Branch("lumi",        &lumi,    	 "lumi/I");
  dataTree_->Branch("bx",          &bx,      	 "bx/I");   
  dataTree_->Branch("orbit",       &orbit,   	 "orbit/I");
  dataTree_->Branch("time",        &time,    	 "time/F");
  
 
  // tracks - for monster event cut

  dataTree_->Branch("ntrk",        &ntrk,	  "ntrk/I");
  dataTree_->Branch("goodtrk",     &goodtrk,	  "goodtrk/I");


  //  primary vertex info

  dataTree_->Branch("numvtx",      &numvtx,	  "numvtx/I");
  dataTree_->Branch("numgoodvtx",  &numgoodvtx,	  "numgoodvtx/I");

  dataTree_->Branch("vtx_x",       &vtx_x,	  "vtx_x/F");
  dataTree_->Branch("vtx_y",       &vtx_y,	  "vtx_y/F");
  dataTree_->Branch("vtx_z",       &vtx_z,	  "vtx_z/F");

  dataTree_->Branch("vtx_x_err",   &vtx_x_err,	  "vtx_x_err/F");
  dataTree_->Branch("vtx_y_err",   &vtx_y_err,	  "vtx_y_err/F");
  dataTree_->Branch("vtx_z_err",   &vtx_z_err,	  "vtx_z_err/F");

  dataTree_->Branch("vtx_chi2",    &vtx_chi2,	  "vtx_chi2/F");
  dataTree_->Branch("vtx_ndof",    &vtx_ndof,	  "vtx_ndof/F");

  dataTree_->Branch("vtx_ntracks", &vtx_ntracks,  "vtx_ntracks/I");
  dataTree_->Branch("vtx_isfake",  &vtx_isfake,   "vtx_isfake/I");

  dataTree_->Branch("vtx_good",    &vtx_good,     "vtx_good/I");




  dataTree_->Branch("scale",        &scale,        "scale/F");

  dataTree_->Branch("ebmax",        &ebmax,        "ebmax/F");
  dataTree_->Branch("ebmaxet",      &ebmaxet,      "ebmaxet/F");
  dataTree_->Branch("ebtime",       &ebtime,       "ebtime/F");
  dataTree_->Branch("ebflags",      &ebflags,      "ebflags/I");
  dataTree_->Branch("eb_ieta",      &eb_ieta,      "eb_ieta/I");
  dataTree_->Branch("eb_iphi",      &eb_iphi,      "eb_iphi/I");  
  dataTree_->Branch("eb_eta",       &eb_eta,       "eb_eta/F");
  dataTree_->Branch("eb_phi",       &eb_phi,       "eb_phi/F");
  dataTree_->Branch("ebhits",       &ebhits,       "ebhits/I");
  dataTree_->Branch("ebhits1GeV",   &ebhits1GeV,   "ebhits1GeV/I");
  dataTree_->Branch("ebhits2GeV",   &ebhits2GeV,   "ebhits2GeV/I");
  dataTree_->Branch("ebhits4GeV",   &ebhits4GeV,   "ebhits4GeV/I");
  dataTree_->Branch("ebhits1GeVet",   &ebhits1GeVet,   "ebhits1GeVet/I");
  dataTree_->Branch("ebhits2GeVet",   &ebhits2GeVet,   "ebhits2GeVet/I");
  dataTree_->Branch("ebhits4GeVet",   &ebhits4GeVet,   "ebhits4GeVet/I");
  dataTree_->Branch("eb_r9",        &eb_r9,        "eb_r9/F");
  dataTree_->Branch("eb_r4",        &eb_r4,        "eb_r4/F");


  dataTree_->Branch("eb_e9",        &eb_e9,        "eb_e9/F");
  dataTree_->Branch("eb_e25",       &eb_e25,       "eb_e25/F");


  dataTree_->Branch("ebmax2",        &ebmax2,        "ebmax2/F");
  dataTree_->Branch("ebmaxet2",      &ebmaxet2,      "ebmaxet2/F");
  dataTree_->Branch("ebtime2",       &ebtime2,       "ebtime2/F");
  dataTree_->Branch("ebflags2",      &ebflags2,      "ebflags2/I");
  dataTree_->Branch("eb_ieta2",      &eb_ieta2,      "eb_ieta2/I");
  dataTree_->Branch("eb_iphi2",      &eb_iphi2,      "eb_iphi2/I");  
  dataTree_->Branch("eb_eta2",       &eb_eta2,       "eb_eta2/F");
  dataTree_->Branch("eb_phi2",       &eb_phi2,       "eb_phi2/F");
  dataTree_->Branch("ebhits1GeV2",   &ebhits1GeV2,   "ebhits1GeV2/I");
  dataTree_->Branch("eb_r92",        &eb_r92,        "eb_r92/F");
  dataTree_->Branch("eb_r42",        &eb_r42,        "eb_r42/F");


  dataTree_->Branch("ebchi2",        &ebchi2,        "ebchi2/F");
  dataTree_->Branch("ebchi2oot",     &ebchi2oot,     "ebchi2oot/F");
  dataTree_->Branch("eb2chi2",       &eb2chi2,       "eb2chi2/F");
  dataTree_->Branch("eb2chi2oot",    &eb2chi2oot,    "eb2chi2oot/F");


  dataTree_->Branch("ebsum_gt1",    &ebsum_gt1,    "ebsum_gt1/F");
  dataTree_->Branch("ebsum_gt2",    &ebsum_gt2,    "ebsum_gt2/F");
  dataTree_->Branch("ebsum_gt4",    &ebsum_gt4,    "ebsum_gt4/F");


  dataTree_->Branch("ebsum_gt1et",    &ebsum_gt1et,    "ebsum_gt1et/F");
  dataTree_->Branch("ebsum_gt2et",    &ebsum_gt2et,    "ebsum_gt2et/F");
  dataTree_->Branch("ebsum_gt4et",    &ebsum_gt4et,    "ebsum_gt4et/F");



  dataTree_->Branch("eesum_gt1",    &eesum_gt1,    "eesum_gt1/F");
  dataTree_->Branch("eesum_gt2",    &eesum_gt2,    "eesum_gt2/F");
  dataTree_->Branch("eesum_gt4",    &eesum_gt4,    "eesum_gt4/F");


  dataTree_->Branch("eesum_gt1et",    &eesum_gt1et,    "eesum_gt1et/F");
  dataTree_->Branch("eesum_gt2et",    &eesum_gt2et,    "eesum_gt2et/F");
  dataTree_->Branch("eesum_gt4et",    &eesum_gt4et,    "eesum_gt4et/F");


  dataTree_->Branch("ebflag_kgood",      &ebflag_kgood,       "ebflag_kgood/I");
  dataTree_->Branch("ebflag_kpoorreco", &ebflag_kpoorreco,  "ebflag_kpoorreco/I");
  dataTree_->Branch("ebflag_koutoftime", &ebflag_koutoftime,  "ebflag_koutoftime/I");
  dataTree_->Branch("ebflag_kfake",      &ebflag_kfake,       "ebflag_kfake/I");


  dataTree_->Branch("eemax",        &eemax,        "eemax/F");
  dataTree_->Branch("eemaxet",      &eemaxet,      "eemaxet/F");
  dataTree_->Branch("eetime",       &eetime,       "eetime/F");
  dataTree_->Branch("eeflags",      &eeflags,      "eeflags/I");
  dataTree_->Branch("eeix",         &eeix,         "eeix/I");
  dataTree_->Branch("eeiy",         &eeiy,         "eeiy/I");
  dataTree_->Branch("eeiz",         &eeiz,         "eeiz/I");
  dataTree_->Branch("ee_eta",       &ee_eta,       "ee_eta/F");
  dataTree_->Branch("ee_phi",       &ee_phi,       "ee_phi/F");
  dataTree_->Branch("eehits",       &eehits,       "eehits/I");
  dataTree_->Branch("eehits1GeV",   &eehits1GeV,   "eehits1GeV/I");
  dataTree_->Branch("eehits2GeV",   &eehits2GeV,   "eehits2GeV/I");
  dataTree_->Branch("eehits4GeV",   &eehits4GeV,   "eehits4GeV/I");
  dataTree_->Branch("eehits1GeVet",   &eehits1GeVet,   "eehits1GeVet/I");
  dataTree_->Branch("eehits2GeVet",   &eehits2GeVet,   "eehits2GeVet/I");
  dataTree_->Branch("eehits4GeVet",   &eehits4GeVet,   "eehits4GeVet/I");
  dataTree_->Branch("ee_r9",        &ee_r9,        "ee_r9/F");
  dataTree_->Branch("eephits",      &eephits,      "eephits/I");
  dataTree_->Branch("eemhits",      &eemhits,      "eemhits/I");


  dataTree_->Branch("eemax2",        &eemax2,        "eemax2/F");
  dataTree_->Branch("eemaxet2",      &eemaxet2,      "eemaxet2/F");
  dataTree_->Branch("eetime2",       &eetime2,       "eetime2/F");
  dataTree_->Branch("eeflags2",      &eeflags2,      "eeflags2/I");
  dataTree_->Branch("eeix2",         &eeix2,         "eeix2/I");
  dataTree_->Branch("eeiy2",         &eeiy2,         "eeiy2/I");
  dataTree_->Branch("eeiz2",         &eeiz2,         "eeiz2/I");
  dataTree_->Branch("ee_eta2",       &ee_eta2,       "ee_eta2/F");
  dataTree_->Branch("ee_phi2",       &ee_phi2,       "ee_phi2/F");
  dataTree_->Branch("eehits1GeV2",   &eehits1GeV2,   "eehits1GeV2/I");
  dataTree_->Branch("ee_r92",        &ee_r92,        "ee_r92/F");


  dataTree_->Branch("tmean_en",      &tmean_en,      "tmean_en/F");
  dataTree_->Branch("terr_en",       &terr_en,       "terr_en/F");
  dataTree_->Branch("tmean_sig",     &tmean_sig,     "tmean_sig/F");
  dataTree_->Branch("terr_sig",      &terr_sig,      "terr_sig/F");


  dataTree_->Branch("r4count",      &r4count,      "r4count/I");

  dataTree_->Branch("e2e9count_thresh0",  &e2e9count_thresh0,  "e2e9count_thresh0/I");
  dataTree_->Branch("e2e25count_thresh0",  &e2e25count_thresh0,  "e2e25count_thresh0/I");
  dataTree_->Branch("e2e9count_thresh1",  &e2e9count_thresh1,  "e2e9count_thresh1/I");
  dataTree_->Branch("e2e25count_thresh1",  &e2e25count_thresh1,  "e2e25count_thresh1/I");

  dataTree_->Branch("e2e9count_thresh0_nor4",  &e2e9count_thresh0_nor4,  "e2e9count_thresh0_nor4/I");
  dataTree_->Branch("e2e25count_thresh0_nor4",  &e2e25count_thresh0_nor4,  "e2e25count_thresh0_nor4/I");
  dataTree_->Branch("e2e9count_thresh1_nor4",  &e2e9count_thresh1_nor4,  "e2e9count_thresh1_nor4/I");
  dataTree_->Branch("e2e25count_thresh1_nor4",  &e2e25count_thresh1_nor4,  "e2e25count_thresh1_nor4/I");

  dataTree_->Branch("r4_algo_count",  &r4_algo_count,  "r4_algo_count/I");

  dataTree_->Branch("e2e9_algo_count",  &e2e9_algo_count,  "e2e9_algo_count/I");
  dataTree_->Branch("e2e9_algo_count_5_1",  &e2e9_algo_count_5_1,  "e2e9_algo_count_5_1/I");
  dataTree_->Branch("e2e9_algo_count_5_0",  &e2e9_algo_count_5_0,  "e2e9_algo_count_5_0/I");
 
  dataTree_->Branch("swisscross_algo",  &swisscross_algo,  "swisscross_algo/F");  dataTree_->Branch("e2e9_algo",  &e2e9_algo,  "e2e9_algo/F");
 


  // calotowers


  dataTree_->Branch("ncalotower", &ncalotower, "ncalotower/I");
 

  dataTree_->Branch("ncalotowereb", &ncalotowereb,"ncalotowereb/I");
  dataTree_->Branch("ncalotoweree", &ncalotoweree,"ncalotoweree/I");
  dataTree_->Branch("ncalotowerhf", &ncalotowerhf,"ncalotowerhf/I");

  dataTree_->Branch("ncalotowerebgt1", &ncalotowerebgt1,"ncalotowerebgt1/I");
  dataTree_->Branch("ncalotowerebgt2", &ncalotowerebgt2,"ncalotowerebgt2/I");
  dataTree_->Branch("ncalotowerebgt5", &ncalotowerebgt5,"ncalotowerebgt5/I");
  dataTree_->Branch("ncalotowerebgt10", &ncalotowerebgt10,"ncalotowerebgt10/I");


  dataTree_->Branch("ncalotowereegt1", &ncalotowereegt1,"ncalotowereegt1/I");
  dataTree_->Branch("ncalotowereegt2", &ncalotowereegt2,"ncalotowereegt2/I");
  dataTree_->Branch("ncalotowereegt5", &ncalotowereegt5,"ncalotowereegt5/I");
  dataTree_->Branch("ncalotowereegt10", &ncalotowereegt10,"ncalotowereegt10/I");


  dataTree_->Branch("ncalotowerhfgt1", &ncalotowerhfgt1,"ncalotowerhfgt1/I");
  dataTree_->Branch("ncalotowerhfgt2", &ncalotowerhfgt2,"ncalotowerhfgt2/I");
  dataTree_->Branch("ncalotowerhfgt5", &ncalotowerhfgt5,"ncalotowerhfgt5/I");
  dataTree_->Branch("ncalotowerhfgt10", &ncalotowerhfgt10,"ncalotowerhfgt10/I");


  dataTree_->Branch("ctsumebgt1", &ctsumebgt1, "ctsumebgt1/F");
  dataTree_->Branch("ctsumebgt2", &ctsumebgt2, "ctsumebgt2/F");
  dataTree_->Branch("ctsumebgt5", &ctsumebgt5, "ctsumebgt5/F");
  dataTree_->Branch("ctsumebgt10", &ctsumebgt10, "ctsumebgt10/F");


  dataTree_->Branch("ctsumeegt1", &ctsumeegt1, "ctsumeegt1/F");
  dataTree_->Branch("ctsumeegt2", &ctsumeegt2, "ctsumeegt2/F");
  dataTree_->Branch("ctsumeegt5", &ctsumeegt5, "ctsumeegt5/F");
  dataTree_->Branch("ctsumeegt10", &ctsumeegt10, "ctsumeegt10/F");


  dataTree_->Branch("ctsumhfgt1", &ctsumhfgt1, "ctsumhfgt1/F");
  dataTree_->Branch("ctsumhfgt2", &ctsumhfgt2, "ctsumhfgt2/F");
  dataTree_->Branch("ctsumhfgt5", &ctsumhfgt5, "ctsumhfgt5/F");
  dataTree_->Branch("ctsumhfgt10", &ctsumhfgt10, "ctsumhfgt10/F");


  dataTree_->Branch("rechitsumet_eb_all", &rechitsumet_eb_all, "rechitsumet_eb_all/F");
  dataTree_->Branch("rechitsumet_eb_01", &rechitsumet_eb_01, "rechitsumet_eb_01/F");
  dataTree_->Branch("rechitsumet_eb_05", &rechitsumet_eb_05, "rechitsumet_eb_05/F");

  dataTree_->Branch("rechitsumet_eb_0105", &rechitsumet_eb_0105, "rechitsumet_eb_0105/F");
  dataTree_->Branch("rechitsumet_eb_0530", &rechitsumet_eb_0530, "rechitsumet_eb_0530/F");

  dataTree_->Branch("rechitsumet_ee_all", &rechitsumet_ee_all, "rechitsumet_ee_all/F");
  dataTree_->Branch("rechitsumet_ee_01", &rechitsumet_ee_01, "rechitsumet_ee_01/F");
  dataTree_->Branch("rechitsumet_ee_05", &rechitsumet_ee_05, "rechitsumet_ee_05/F");

  dataTree_->Branch("rechitsumet_ee_0105", &rechitsumet_ee_0105, "rechitsumet_ee_0105/F");
  dataTree_->Branch("rechitsumet_ee_0530", &rechitsumet_ee_0530, "rechitsumet_ee_0530/F");



  dataTree_->Branch("bunchintrain", &bunchintrain, "bunchintrain/I");


  dataTree_->Branch("ebnumsc_all", &ebnumsc_all, "ebnumsc_all/I");
  dataTree_->Branch("eenumsc_all", &eenumsc_all, "eenumsc_all/I");


  dataTree_->Branch("ebscsumet_all", &ebscsumet_all, "ebscsumet_all/F");
  dataTree_->Branch("eescsumet_all", &eescsumet_all, "eescsumet_all/F");
  dataTree_->Branch("ebscsumet_all_eta15", &ebscsumet_all_eta15, "ebscsumet_all_eta15/F");
  dataTree_->Branch("ebscsumet_all_eta20", &ebscsumet_all_eta20, "ebscsumet_all_eta20/F");
  dataTree_->Branch("ebscsumet_all_eta25", &ebscsumet_all_eta25, "ebscsumet_all_eta25/F");
  dataTree_->Branch("eescsumet_all_eta15", &eescsumet_all_eta15, "eescsumet_all_eta15/F");
  dataTree_->Branch("eescsumet_all_eta20", &eescsumet_all_eta20, "eescsumet_all_eta20/F");
  dataTree_->Branch("eescsumet_all_eta25", &eescsumet_all_eta25, "eescsumet_all_eta25/F");

  dataTree_->Branch("ebnumrechits_01", &ebnumrechits_01, "ebnumrechits_01/I");
  dataTree_->Branch("ebnumrechits_0105", &ebnumrechits_0105, "ebnumrechits_0105/I");
  dataTree_->Branch("ebnumrechits_05", &ebnumrechits_05, "ebnumrechits_05/I");
  dataTree_->Branch("ebnumrechits_0530", &ebnumrechits_0530, "ebnumrechits_0530/I");


  dataTree_->Branch("eenumrechits_01", &eenumrechits_01, "eenumrechits_01/I");
  dataTree_->Branch("eenumrechits_0105", &eenumrechits_0105, "eenumrechits_0105/I");
  dataTree_->Branch("eenumrechits_05", &eenumrechits_05, "eenumrechits_05/I");
  dataTree_->Branch("eenumrechits_0530", &eenumrechits_0530, "eenumrechits_0530/I");




  dataTree_->Branch("numspikes", &numspikes, "numspikes/I");
  dataTree_->Branch("numspikes50", &numspikes50, "numspikes50/I");
  dataTree_->Branch("numspikes100", &numspikes100, "numspikes100/I");
  dataTree_->Branch("numspikes_kweird", &numspikes_kweird, "numspikes_kweird/I");
  dataTree_->Branch("numspikes_swisscross", &numspikes_swisscross, "numspikes_swisscross/I");





}
//////////////////////////////////////////////////////////////////////////////////////////
void ChanstatusTester::endJob() 
{

  cout << "in endjob" << endl;

  if (file_ !=0) 
    {
      file_->cd();



      cout << "BP1" << endl;


      ebchstatus->Write();
      eechstatus->Write();

    tower_spike_ene_swiss_t10->Write();
    tower_spike_ene_swiss_t07->Write();
    tower_spike_ene_swiss_t05->Write();
    tower_spike_ene_swiss_t04->Write();
    tower_spike_ene_swiss_t03->Write();

    tower_spike_ene_swiss_t10_thresh08->Write();
    tower_spike_ene_swiss_t07_thresh08->Write();
    tower_spike_ene_swiss_t05_thresh08->Write();
    tower_spike_ene_swiss_t04_thresh08->Write();
    tower_spike_ene_swiss_t03_thresh08->Write();

    tower_spike_ene_swiss_t10_thresh10->Write();
    tower_spike_ene_swiss_t07_thresh10->Write();
    tower_spike_ene_swiss_t05_thresh10->Write();
    tower_spike_ene_swiss_t04_thresh10->Write();
    tower_spike_ene_swiss_t03_thresh10->Write();


    tower_spike_ene_swiss_t10_thresh12->Write();
    tower_spike_ene_swiss_t07_thresh12->Write();
    tower_spike_ene_swiss_t05_thresh12->Write();
    tower_spike_ene_swiss_t04_thresh12->Write();
    tower_spike_ene_swiss_t03_thresh12->Write();


    tower_spike_ene_swiss_t10_thresh15->Write();
    tower_spike_ene_swiss_t07_thresh15->Write();
    tower_spike_ene_swiss_t05_thresh15->Write();
    tower_spike_ene_swiss_t04_thresh15->Write();
    tower_spike_ene_swiss_t03_thresh15->Write();

    tower_spike_ene_swiss_t10_thresh20->Write();
    tower_spike_ene_swiss_t07_thresh20->Write();
    tower_spike_ene_swiss_t05_thresh20->Write();
    tower_spike_ene_swiss_t04_thresh20->Write();
    tower_spike_ene_swiss_t03_thresh20->Write();

      cout << "BP2" << endl;


    sc_tower_spike_ene_swiss_t10->Write();
    sc_tower_spike_ene_swiss_t07->Write();
    sc_tower_spike_ene_swiss_t05->Write();
    sc_tower_spike_ene_swiss_t04->Write();
    sc_tower_spike_ene_swiss_t03->Write();


    sc_tower_spike_ene_swiss_t10_thresh08->Write();
    sc_tower_spike_ene_swiss_t07_thresh08->Write();
    sc_tower_spike_ene_swiss_t05_thresh08->Write();
    sc_tower_spike_ene_swiss_t04_thresh08->Write();
    sc_tower_spike_ene_swiss_t03_thresh08->Write();

    sc_tower_spike_ene_swiss_t10_thresh10->Write();
    sc_tower_spike_ene_swiss_t07_thresh10->Write();
    sc_tower_spike_ene_swiss_t05_thresh10->Write();
    sc_tower_spike_ene_swiss_t04_thresh10->Write();
    sc_tower_spike_ene_swiss_t03_thresh10->Write();


    sc_tower_spike_ene_swiss_t10_thresh12->Write();
    sc_tower_spike_ene_swiss_t07_thresh12->Write();
    sc_tower_spike_ene_swiss_t05_thresh12->Write();
    sc_tower_spike_ene_swiss_t04_thresh12->Write();
    sc_tower_spike_ene_swiss_t03_thresh12->Write();


    sc_tower_spike_ene_swiss_t10_thresh15->Write();
    sc_tower_spike_ene_swiss_t07_thresh15->Write();
    sc_tower_spike_ene_swiss_t05_thresh15->Write();
    sc_tower_spike_ene_swiss_t04_thresh15->Write();
    sc_tower_spike_ene_swiss_t03_thresh15->Write();

    sc_tower_spike_ene_swiss_t10_thresh20->Write();
    sc_tower_spike_ene_swiss_t07_thresh20->Write();
    sc_tower_spike_ene_swiss_t05_thresh20->Write();
    sc_tower_spike_ene_swiss_t04_thresh20->Write();
    sc_tower_spike_ene_swiss_t03_thresh20->Write();


      cout << "BP3" << endl;





      tower_spike_et_swisstt->Write();
      tower_spike_ene_swisstt->Write();

      tower_spike_et_swisstt_thresh08->Write();
      tower_spike_ene_swisstt_thresh08->Write();

      tower_spike_et_swisstt_thresh10->Write();
      tower_spike_ene_swisstt_thresh10->Write();
      
      tower_spike_et_swisstt_thresh12->Write();
      tower_spike_ene_swisstt_thresh12->Write();

      tower_spike_et_swisstt_thresh15->Write();
      tower_spike_ene_swisstt_thresh15->Write();

      tower_spike_et_swisstt_thresh20->Write();
      tower_spike_ene_swisstt_thresh20->Write();


      sc_tower_spike_et_swisstt->Write();
      sc_tower_spike_ene_swisstt->Write();

      sc_tower_spike_et_swisstt_thresh08->Write();
      sc_tower_spike_ene_swisstt_thresh08->Write();

      sc_tower_spike_et_swisstt_thresh10->Write();
      sc_tower_spike_ene_swisstt_thresh10->Write();
      
      sc_tower_spike_et_swisstt_thresh12->Write();
      sc_tower_spike_ene_swisstt_thresh12->Write();

      sc_tower_spike_et_swisstt_thresh15->Write();
      sc_tower_spike_ene_swisstt_thresh15->Write();

      sc_tower_spike_et_swisstt_thresh20->Write();
      sc_tower_spike_ene_swisstt_thresh20->Write();


      tower_spike_et_swiss->Write();
      tower_spike_ene_swiss->Write();

      tower_spike_et_swiss_thresh08->Write();
      tower_spike_ene_swiss_thresh08->Write();

      tower_spike_et_swiss_thresh10->Write();
      tower_spike_ene_swiss_thresh10->Write();
      
      tower_spike_et_swiss_thresh12->Write();
      tower_spike_ene_swiss_thresh12->Write();

      tower_spike_et_swiss_thresh15->Write();
      tower_spike_ene_swiss_thresh15->Write();

      tower_spike_et_swiss_thresh20->Write();
      tower_spike_ene_swiss_thresh20->Write();


      sc_tower_spike_et_swiss->Write();
      sc_tower_spike_ene_swiss->Write();

      sc_tower_spike_et_swiss_thresh08->Write();
      sc_tower_spike_ene_swiss_thresh08->Write();

      sc_tower_spike_et_swiss_thresh10->Write();
      sc_tower_spike_ene_swiss_thresh10->Write();
      
      sc_tower_spike_et_swiss_thresh12->Write();
      sc_tower_spike_ene_swiss_thresh12->Write();

      sc_tower_spike_et_swiss_thresh15->Write();
      sc_tower_spike_ene_swiss_thresh15->Write();

      sc_tower_spike_et_swiss_thresh20->Write();
      sc_tower_spike_ene_swiss_thresh20->Write();


      noise2d->Write();

      histo_event_->Write();
      eb_rechitenergy_->Write();
      ee_rechitenergy_->Write();


      spike_timevset_all_08->Write();
      spike_timevset_highest_08->Write();
      spike_timevset_koot_all_08->Write();
      spike_timevset_koot_highest_08->Write();
      spike_timevset_kdiweird_all_08->Write();
      spike_timevset_kdiweird_highest_08->Write();

      spike_timevset_all_07->Write();
      spike_timevset_highest_07->Write();
      spike_timevset_koot_all_07->Write();
      spike_timevset_koot_highest_07->Write();
      spike_timevset_kdiweird_all_07->Write();
      spike_timevset_kdiweird_highest_07->Write();

      spike_timevset_all_06->Write();
      spike_timevset_highest_06->Write();
      spike_timevset_koot_all_06->Write();
      spike_timevset_koot_highest_06->Write();
      spike_timevset_kdiweird_all_06->Write();
      spike_timevset_kdiweird_highest_06->Write();



      eb_rechitenergy_spiketag->Write();
      eb_rechitenergy_sctag->Write();
      eb_rechitenergy_kweird->Write();
      eb_rechitenergy_knotweird->Write();
      eb_rechitenergy_swisscross->Write();
      cout << "p1" << endl;


      ee_rechitenergy_notypeb_->Write();

      eb_rechitenergy_spiketag_kweird->Write();
      eb_rechitenergy_spiketag_swisscross->Write();
      eb_rechitenergy_spiketag_noisedep->Write();
    
      eb_rechitenergy_sctag_kweird->Write();
      eb_rechitenergy_sctag_swisscross->Write();
      eb_rechitenergy_sctag_noisedep->Write();

      swisscross_vs_energy_spiketag->Write();
      swisscross_vs_energy_spiketag_kweird->Write();
      swisscross_vs_energy_sctag->Write();
      swisscross_vs_energy_sctag_kweird->Write();
      cout << "p2" << endl;

      e4_over_noise->Write();
      e4_over_noise_spiketag->Write();
      e4_over_noise_sctag->Write();
      time_4gev_spiketime->Write();
      time_10gev_spiketime->Write();

      time_4gev_spikekweird->Write();
      time_10gev_spikekweird->Write();
      
      time_4gev_swisscross->Write();
      time_10gev_swisscross->Write();
      
      pu_4gev_spiketime->Write();
      pu_10gev_spiketime->Write();
      
      pu_4gev_spikekweird->Write();
      pu_10gev_spikekweird->Write();
      
      pu_4gev_swisscross->Write();
      pu_10gev_swisscross->Write();
      
      
      ene_4gev_spiketime->Write();
      ene_10gev_spiketime->Write();
      
      ene_4gev_spikekweird->Write();
      ene_10gev_spikekweird->Write();
      

      ene_4gev_swisscross->Write();
      ene_10gev_swisscross->Write();
      
      pu_vs_ene_spiketime->Write();
      pu_vs_ene_spikekweird->Write();
      pu_vs_ene_swisscross->Write();



      eb_rechitenergy_02->Write();
      eb_rechitenergy_04->Write();
      eb_rechitenergy_06->Write();
      eb_rechitenergy_08->Write();
      eb_rechitenergy_10->Write();
      eb_rechitenergy_12->Write();
      eb_rechitenergy_14->Write();
      eb_rechitenergy_148->Write();
      
      
      
      ee_rechitet_16->Write();
      ee_rechitet_18->Write();
      ee_rechitet_20->Write();
      ee_rechitet_22->Write();
      ee_rechitet_24->Write();
      ee_rechitet_26->Write();
      ee_rechitet_28->Write();
      ee_rechitet_30->Write();
      



      eb_rechitet_02->Write();
      eb_rechitet_04->Write();
      eb_rechitet_06->Write();
      eb_rechitet_08->Write();
      eb_rechitet_10->Write();
      eb_rechitet_12->Write();
      eb_rechitet_14->Write();
      eb_rechitet_148->Write();
      
      
      
      ee_rechitenergy_16->Write();
      ee_rechitenergy_18->Write();
      ee_rechitenergy_20->Write();
      ee_rechitenergy_22->Write();
      ee_rechitenergy_24->Write();
      ee_rechitenergy_26->Write();
      ee_rechitenergy_28->Write();
      ee_rechitenergy_30->Write();
      



      eb_rechitetvspu_05->Write();
      eb_rechitetvspu_10->Write();
      eb_rechitetvspu_15->Write();

      ee_rechitetvspu_20->Write();
      ee_rechitetvspu_25->Write();
      ee_rechitetvspu_30->Write();

      eb_rechitet_->Write();
      ee_rechitet_->Write();

      eb_rechiten_vs_eta->Write();
  
      eb_rechitet_vs_eta->Write();
  



      eep_rechiten_vs_eta->Write();
      eep_rechiten_vs_phi->Write();

      eem_rechiten_vs_eta->Write();
      eem_rechiten_vs_phi->Write();

      eep_rechitet_vs_eta->Write();
      eep_rechitet_vs_phi->Write();

      eem_rechitet_vs_eta->Write();
      eem_rechitet_vs_phi->Write();


      ebocc->Write();
      eboccgt1->Write();
      eboccgt3->Write();
      eboccgt5->Write();
      eboccgt1et->Write();
      eboccet->Write();
      eboccetgt1et->Write();
      eboccen->Write();
      eboccengt1->Write();
 
      eeocc->Write();
      eeoccgt1->Write();
      eeoccgt1et->Write();
      eeoccet->Write();
      eeoccetgt1et->Write();
      eeoccen->Write();
      eeoccengt1->Write();


      eb_timing_0->Write();
      eb_timing_200->Write();
      eb_timing_400->Write();
      eb_timing_600->Write();
      eb_timing_800->Write();
      eb_timing_1000->Write();
      eb_timing_2000->Write();
      eb_timing_3000->Write();
      eb_timing_4000->Write();
      eb_timing_5000->Write();
      eb_timing_10000->Write();

      eb_timing_3000_spiketag->Write();

      eb_timing_4000_spiketag->Write();
      eb_timing_4000_kweird->Write();
      eb_timing_4000_swisscross->Write();
      eb_timing_4000_spiketag_kweird->Write();
      eb_timing_4000_spiketag_swisscross->Write();
      eb_timing_4000_spiketag_noisedep->Write();
      cout << "p3" << endl;

      eb_timing_4000_sctag->Write();
      eb_timing_4000_sctag_kweird->Write();
      eb_timing_4000_sctag_swisscross->Write();
      eb_timing_4000_sctag_noisedep->Write();


      eb_timing_10000_spiketag->Write();
      eb_timing_10000_kweird->Write();
      eb_timing_10000_swisscross->Write();
      eb_timing_10000_spiketag_kweird->Write();
      eb_timing_10000_spiketag_swisscross->Write();
      eb_timing_10000_spiketag_noisedep->Write();

      eb_timing_10000_sctag->Write();
      eb_timing_10000_sctag_kweird->Write();
      eb_timing_10000_sctag_swisscross->Write();
      eb_timing_10000_sctag_noisedep->Write();


      cout << "p4" << endl;

      spikes_vs_ieta_spiketag_4000->Write();
      spikes_vs_ieta_spiketag_4000_kweird->Write();
      spikes_vs_ieta_spiketag_4000_swisscross->Write();
      spikes_vs_ieta_spiketag_4000_noisedep->Write();

      spikes_vs_ieta_spiketag_10000->Write();
      spikes_vs_ieta_spiketag_10000_kweird->Write();
      spikes_vs_ieta_spiketag_10000_swisscross->Write();
      spikes_vs_ieta_spiketag_10000_noisedep->Write();

      sc_vs_ieta_sctag_4000->Write();
      sc_vs_ieta_sctag_4000_kweird->Write();
      sc_vs_ieta_sctag_4000_swisscross->Write();
      sc_vs_ieta_sctag_4000_noisedep->Write();

      sc_vs_ieta_sctag_10000->Write();
      sc_vs_ieta_sctag_10000_kweird->Write();
      sc_vs_ieta_sctag_10000_swisscross->Write();
      sc_vs_ieta_sctag_10000_noisedep->Write();

      cout << "p5" << endl;

      swisscross_vs_ieta_spiketag_4000->Write();
      swisscross_vs_ieta_spiketag_4000_kweird->Write();

      swisscross_vs_ieta_spiketag_10000->Write();
      swisscross_vs_ieta_spiketag_10000_kweird->Write();


      swisscross_vs_ieta_sctag_4000->Write();
      swisscross_vs_ieta_sctag_4000_kweird->Write();

      swisscross_vs_ieta_sctag_10000->Write();
      swisscross_vs_ieta_sctag_10000_kweird->Write();

      cout << "p6" << endl;

      strip_prob_et->Write();
      strip_prob_ene->Write();
      tower_spike_et->Write();
      tower_spike_ene->Write();

      strip_prob_et_pu->Write();
      strip_prob_ene_pu->Write();
      tower_spike_et_pu->Write();
      tower_spike_ene_pu->Write();

 

      strip_prob_et_thresh08->Write();
      strip_prob_ene_thresh08->Write();
      tower_spike_et_thresh08->Write();
      tower_spike_ene_thresh08->Write();

      strip_prob_et_pu_thresh08->Write();
      strip_prob_ene_pu_thresh08->Write();
      tower_spike_et_pu_thresh08->Write();
      tower_spike_ene_pu_thresh08->Write();

 

      strip_prob_et_thresh10->Write();
      strip_prob_ene_thresh10->Write();
      tower_spike_et_thresh10->Write();
      tower_spike_ene_thresh10->Write();

      strip_prob_et_pu_thresh10->Write();
      strip_prob_ene_pu_thresh10->Write();
      tower_spike_et_pu_thresh10->Write();
      tower_spike_ene_pu_thresh10->Write();

 
      strip_prob_et_thresh12->Write();
      strip_prob_ene_thresh12->Write();
      tower_spike_et_thresh12->Write();
      tower_spike_ene_thresh12->Write();

      strip_prob_et_pu_thresh12->Write();
      strip_prob_ene_pu_thresh12->Write();
      tower_spike_et_pu_thresh12->Write();
      tower_spike_ene_pu_thresh12->Write();

 
      strip_prob_et_thresh15->Write();
      strip_prob_ene_thresh15->Write();
      tower_spike_et_thresh15->Write();
      tower_spike_ene_thresh15->Write();

      strip_prob_et_pu_thresh15->Write();
      strip_prob_ene_pu_thresh15->Write();
      tower_spike_et_pu_thresh15->Write();
      tower_spike_ene_pu_thresh15->Write();

 
      strip_prob_et_thresh20->Write();
      strip_prob_ene_thresh20->Write();
      tower_spike_et_thresh20->Write();
      tower_spike_ene_thresh20->Write();

      strip_prob_et_pu_thresh20->Write();
      strip_prob_ene_pu_thresh20->Write();
      tower_spike_et_pu_thresh20->Write();
      tower_spike_ene_pu_thresh20->Write();

 


      strip_prob_et_300->Write();
      strip_prob_ene_300->Write();
      tower_spike_et_300->Write();
      tower_spike_ene_300->Write();

      strip_prob_et_mod1->Write();
      strip_prob_ene_mod1->Write();
      tower_spike_et_mod1->Write();
      tower_spike_ene_mod1->Write();

      strip_prob_et_mod2->Write();
      strip_prob_ene_mod2->Write();
      tower_spike_et_mod2->Write();
      tower_spike_ene_mod2->Write();

      strip_prob_et_mod3->Write();
      strip_prob_ene_mod3->Write();
      tower_spike_et_mod3->Write();
      tower_spike_ene_mod3->Write();

      strip_prob_et_mod4->Write();
      strip_prob_ene_mod4->Write();
      tower_spike_et_mod4->Write();
      tower_spike_ene_mod4->Write();

      strip_prob_et_pu0->Write();
      strip_prob_ene_pu0->Write();
      tower_spike_et_pu0->Write();
      tower_spike_ene_pu0->Write();

      strip_prob_et_pu10->Write();
      strip_prob_ene_pu10->Write();
      tower_spike_et_pu10->Write();
      tower_spike_ene_pu10->Write();

      strip_prob_et_pu20->Write();
      strip_prob_ene_pu20->Write();
      tower_spike_et_pu20->Write();
      tower_spike_ene_pu20->Write();




      sc_strip_prob_et->Write();
      sc_strip_prob_ene->Write();
      sc_tower_spike_et->Write();
      sc_tower_spike_ene->Write();

      sc_strip_prob_et_pu->Write();
      sc_strip_prob_ene_pu->Write();
      sc_tower_spike_et_pu->Write();
      sc_tower_spike_ene_pu->Write();

      sc_strip_prob_et_10->Write();
      sc_strip_prob_ene_10->Write();
      sc_tower_spike_et_10->Write();
      sc_tower_spike_ene_10->Write();

      sc_strip_prob_et_mod1->Write();
      sc_strip_prob_ene_mod1->Write();
      sc_tower_spike_et_mod1->Write();
      sc_tower_spike_ene_mod1->Write();

      sc_strip_prob_et_mod2->Write();
      sc_strip_prob_ene_mod2->Write();
      sc_tower_spike_et_mod2->Write();
      sc_tower_spike_ene_mod2->Write();

      sc_strip_prob_et_mod3->Write();
      sc_strip_prob_ene_mod3->Write();
      sc_tower_spike_et_mod3->Write();
      sc_tower_spike_ene_mod3->Write();

      sc_strip_prob_et_mod4->Write();
      sc_strip_prob_ene_mod4->Write();
      sc_tower_spike_et_mod4->Write();
      sc_tower_spike_ene_mod4->Write();

      sc_strip_prob_et_pu0->Write();
      sc_strip_prob_ene_pu0->Write();
      sc_tower_spike_et_pu0->Write();
      sc_tower_spike_ene_pu0->Write();

      sc_strip_prob_et_pu10->Write();
      sc_strip_prob_ene_pu10->Write();
      sc_tower_spike_et_pu10->Write();
      sc_tower_spike_ene_pu10->Write();

      sc_strip_prob_et_pu20->Write();
      sc_strip_prob_ene_pu20->Write();
      sc_tower_spike_et_pu20->Write();
      sc_tower_spike_ene_pu20->Write();




      sc_strip_prob_et_thresh08->Write();
      sc_strip_prob_ene_thresh08->Write();
      sc_tower_spike_et_thresh08->Write();
      sc_tower_spike_ene_thresh08->Write();

      sc_strip_prob_et_pu_thresh08->Write();
      sc_strip_prob_ene_pu_thresh08->Write();
      sc_tower_spike_et_pu_thresh08->Write();
      sc_tower_spike_ene_pu_thresh08->Write();

 

      sc_strip_prob_et_thresh10->Write();
      sc_strip_prob_ene_thresh10->Write();
      sc_tower_spike_et_thresh10->Write();
      sc_tower_spike_ene_thresh10->Write();

      sc_strip_prob_et_pu_thresh10->Write();
      sc_strip_prob_ene_pu_thresh10->Write();
      sc_tower_spike_et_pu_thresh10->Write();
      sc_tower_spike_ene_pu_thresh10->Write();

 
      sc_strip_prob_et_thresh12->Write();
      sc_strip_prob_ene_thresh12->Write();
      sc_tower_spike_et_thresh12->Write();
      sc_tower_spike_ene_thresh12->Write();

      sc_strip_prob_et_pu_thresh12->Write();
      sc_strip_prob_ene_pu_thresh12->Write();
      sc_tower_spike_et_pu_thresh12->Write();
      sc_tower_spike_ene_pu_thresh12->Write();

 
      sc_strip_prob_et_thresh15->Write();
      sc_strip_prob_ene_thresh15->Write();
      sc_tower_spike_et_thresh15->Write();
      sc_tower_spike_ene_thresh15->Write();

      sc_strip_prob_et_pu_thresh15->Write();
      sc_strip_prob_ene_pu_thresh15->Write();
      sc_tower_spike_et_pu_thresh15->Write();
      sc_tower_spike_ene_pu_thresh15->Write();

 
      sc_strip_prob_et_thresh20->Write();
      sc_strip_prob_ene_thresh20->Write();
      sc_tower_spike_et_thresh20->Write();
      sc_tower_spike_ene_thresh20->Write();

      sc_strip_prob_et_pu_thresh20->Write();
      sc_strip_prob_ene_pu_thresh20->Write();
      sc_tower_spike_et_pu_thresh20->Write();
      sc_tower_spike_ene_pu_thresh20->Write();

 




      eb_r4_0->Write();
      eb_r4_200->Write();
      eb_r4_400->Write();
      eb_r4_600->Write();
      eb_r4_800->Write();
      eb_r4_1000->Write();
      eb_r4_2000->Write();
      eb_r4_3000->Write();
      eb_r4_4000->Write();
      eb_r4_5000->Write();
      eb_r4_10000->Write();

      cout << "p7" << endl;

      eb_r4_3000_spiketag->Write();
      eb_r4_4000_spiketag->Write();
      eb_r4_4000_sctag->Write();
      eb_r4_4000_kweird->Write();
      eb_r4_4000_swisscross->Write();
      eb_r4_10000_spiketag->Write();
      eb_r4_10000_sctag->Write();
      eb_r4_10000_kweird->Write();
      eb_r4_10000_swisscross->Write();


      eb_r6_1000->Write();
      eb_r6_2000->Write();
      eb_r6_3000->Write();
      eb_r6_4000->Write();
      eb_r6_5000->Write();
      eb_r6_10000->Write();


      eb_r6_3000_spiketag->Write();
      eb_r6_4000_spiketag->Write();
      eb_r6_4000_sctag->Write();
      eb_r6_4000_kweird->Write();
      eb_r6_4000_swisscross->Write();
      eb_r6_10000_spiketag->Write();
      eb_r6_10000_sctag->Write();
      eb_r6_10000_kweird->Write();
      eb_r6_10000_swisscross->Write();

      cout << "p8" << endl;


      eb_r4vsr6_3000->Write();
      eb_r4vsr6_4000->Write();
      eb_r4vsr6_5000->Write();
      eb_r4vsr6_10000->Write();


      eb_timing_r4_0->Write();
      eb_timing_r4_200->Write();
      eb_timing_r4_400->Write();
      eb_timing_r4_600->Write();
      eb_timing_r4_800->Write();
      eb_timing_r4_1000->Write();
      eb_timing_r4_2000->Write();
      eb_timing_r4_3000->Write();
      eb_timing_r4_5000->Write();

      eb_timing_vs_r4_0->Write();
      eb_timing_vs_r4_200->Write();
      eb_timing_vs_r4_400->Write();
      eb_timing_vs_r4_600->Write();
      eb_timing_vs_r4_800->Write();
      eb_timing_vs_r4_1000->Write();
      eb_timing_vs_r4_2000->Write();
      eb_timing_vs_r4_3000->Write();
      eb_timing_vs_r4_5000->Write();
      eb_timing_vs_r4_10000->Write();



      eb_timing_vs_r4_sctag_3000->Write();
      eb_timing_vs_r4_sctag_5000->Write();
      eb_timing_vs_r4_sctag_10000->Write();



      spikeadc->Write();
      spiketime50->Write();

      rechiteta_all->Write();
      rechiteta_gt1et->Write();
      rechiteta_etweight->Write();
      rechiteta_gt1et_pu10->Write();
      rechiteta_gt1et_pu20->Write();
      rechiteta_gt1et_pu30->Write();


      calotowereta_all->Write();
      calotowereta_gt1et->Write();
      calotowereta_etweight->Write();
      calotowereta_gt1et_pu10->Write();
      calotowereta_gt1et_pu20->Write();
      calotowereta_gt1et_pu30->Write();

      sceta_all->Write();
      sceta_severity0->Write();
      sceta_koot0->Write();



      sceta_all_pueq01->Write();
      sceta_severity0_pueq01->Write();

      sceta_all_pueq02->Write();
      sceta_severity0_pueq02->Write();

      sceta_all_pueq03->Write();
      sceta_severity0_pueq03->Write();

      sceta_all_pueq04->Write();
      sceta_severity0_pueq04->Write();

      sceta_all_pueq05->Write();
      sceta_severity0_pueq05->Write();

      sceta_all_pueq06->Write();
      sceta_severity0_pueq06->Write();

      sceta_all_pueq07->Write();
      sceta_severity0_pueq07->Write();
      
      sceta_all_pueq08->Write();
      sceta_severity0_pueq08->Write();

      sceta_all_pueq09->Write();
      sceta_severity0_pueq09->Write();


      sceta_all_gt2->Write();
      sceta_severity0_gt2->Write();
      sceta_koot0_gt2->Write();


      sceta_all_gt5->Write();
      sceta_severity0_gt5->Write();
      sceta_koot0_gt5->Write();


      sceta_all_gt10->Write();
      sceta_severity0_gt10->Write();
      sceta_koot0_gt10->Write();


      scet_eb_all->Write();
      scet_eb_severity0->Write();
      scet_eb_koot0->Write();

      scet_ee_all->Write();
      scet_ee_severity0->Write();
      scet_ee_koot0->Write();

      
      scsumet_eb_all->Write();
      scsumet_eb_severity0->Write();
      scsumet_eb_koot0->Write();
   
      scsumet_ee_all->Write();
      scsumet_ee_severity0->Write();
      scsumet_ee_koot0->Write();


      scsumet_eb_all_gt2->Write();
      scsumet_eb_severity0_gt2->Write();
      scsumet_eb_koot0_gt2->Write();
   
      scsumet_ee_all_gt2->Write();
      scsumet_ee_severity0_gt2->Write();
      scsumet_ee_koot0_gt2->Write();

      scsumet_eb_all_gt5->Write();
      scsumet_eb_severity0_gt5->Write();
      scsumet_eb_koot0_gt5->Write();
   
      scsumet_ee_all_gt5->Write();
      scsumet_ee_severity0_gt5->Write();
      scsumet_ee_koot0_gt5->Write();

      scsumet_eb_all_gt10->Write();
      scsumet_eb_severity0_gt10->Write();
      scsumet_eb_koot0_gt10->Write();
   
      scsumet_ee_all_gt10->Write();
      scsumet_ee_severity0_gt10->Write();
      scsumet_ee_koot0_gt10->Write();



      scocc_eb_gt50->Write();
      scocc_ee_gt50->Write();




      scet_eb_all_eta15->Write();
    scet_eb_all_eta20->Write();
    scet_eb_all_eta25->Write();
 
    scet_ee_all_eta15->Write();
    scet_ee_all_eta20->Write();
    scet_ee_all_eta25->Write();



    scet_eb_all_eta15_pu10->Write();
    scet_eb_all_eta20_pu10->Write();
    scet_eb_all_eta25_pu10->Write();
 
    scet_ee_all_eta15_pu10->Write();
    scet_ee_all_eta20_pu10->Write();
    scet_ee_all_eta25_pu10->Write();

    scet_eb_all_eta15_pu20->Write();
    scet_eb_all_eta20_pu20->Write();
    scet_eb_all_eta25_pu20->Write();
 
    scet_ee_all_eta15_pu20->Write();
    scet_ee_all_eta20_pu20->Write();
    scet_ee_all_eta25_pu20->Write();

    scet_eb_all_eta15_pu30->Write();
    scet_eb_all_eta20_pu30->Write();
    scet_eb_all_eta25_pu30->Write();
 
    scet_ee_all_eta15_pu30->Write();
    scet_ee_all_eta20_pu30->Write();
    scet_ee_all_eta25_pu30->Write();


    scet_eb_all_eta15_pueq10->Write();
    scet_eb_all_eta20_pueq10->Write();
    scet_eb_all_eta25_pueq10->Write();
 
    scet_ee_all_eta15_pueq10->Write();
    scet_ee_all_eta20_pueq10->Write();
    scet_ee_all_eta25_pueq10->Write();

    scet_eb_all_eta15_pueq20->Write();
    scet_eb_all_eta20_pueq20->Write();
    scet_eb_all_eta25_pueq20->Write();
 
    scet_ee_all_eta15_pueq20->Write();
    scet_ee_all_eta20_pueq20->Write();
    scet_ee_all_eta25_pueq20->Write();
 



    scsumet_eb_all_eta15->Write();
    scsumet_eb_all_eta20->Write();
    scsumet_eb_all_eta25->Write();
 
    scsumet_ee_all_eta15->Write();
    scsumet_ee_all_eta20->Write();
    scsumet_ee_all_eta25->Write();



    scsumet_eb_all_eta15_pu10->Write();
    scsumet_eb_all_eta20_pu10->Write();
    scsumet_eb_all_eta25_pu10->Write();
 
    scsumet_ee_all_eta15_pu10->Write();
    scsumet_ee_all_eta20_pu10->Write();
    scsumet_ee_all_eta25_pu10->Write();

    scsumet_eb_all_eta15_pu20->Write();
    scsumet_eb_all_eta20_pu20->Write();
    scsumet_eb_all_eta25_pu20->Write();
 
    scsumet_ee_all_eta15_pu20->Write();
    scsumet_ee_all_eta20_pu20->Write();
    scsumet_ee_all_eta25_pu20->Write();

    scsumet_eb_all_eta15_pu30->Write();
    scsumet_eb_all_eta20_pu30->Write();
    scsumet_eb_all_eta25_pu30->Write();
 
    scsumet_ee_all_eta15_pu30->Write();
    scsumet_ee_all_eta20_pu30->Write();
    scsumet_ee_all_eta25_pu30->Write();


    scsumet_eb_all_eta15_pueq10->Write();
    scsumet_eb_all_eta20_pueq10->Write();
    scsumet_eb_all_eta25_pueq10->Write();
 
    scsumet_ee_all_eta15_pueq10->Write();
    scsumet_ee_all_eta20_pueq10->Write();
    scsumet_ee_all_eta25_pueq10->Write();

    scsumet_eb_all_eta15_pueq20->Write();
    scsumet_eb_all_eta20_pueq20->Write();
    scsumet_eb_all_eta25_pueq20->Write();
 
    scsumet_ee_all_eta15_pueq20->Write();
    scsumet_ee_all_eta20_pueq20->Write();
    scsumet_ee_all_eta25_pueq20->Write();
 


    scet_eb_all_eta15_pueq01->Write();
    scet_eb_all_eta20_pueq01->Write();
    scet_eb_all_eta25_pueq01->Write();
 
    scet_ee_all_eta15_pueq01->Write();
    scet_ee_all_eta20_pueq01->Write();
    scet_ee_all_eta25_pueq01->Write();

    scsumet_eb_all_eta15_pueq01->Write();
    scsumet_eb_all_eta20_pueq01->Write();
    scsumet_eb_all_eta25_pueq01->Write();
 
    scsumet_ee_all_eta15_pueq01->Write();
    scsumet_ee_all_eta20_pueq01->Write();
    scsumet_ee_all_eta25_pueq01->Write();



    scet_eb_all_eta15_pueq02->Write();
    scet_eb_all_eta20_pueq02->Write();
    scet_eb_all_eta25_pueq02->Write();
 
    scet_ee_all_eta15_pueq02->Write();
    scet_ee_all_eta20_pueq02->Write();
    scet_ee_all_eta25_pueq02->Write();

    scsumet_eb_all_eta15_pueq02->Write();
    scsumet_eb_all_eta20_pueq02->Write();
    scsumet_eb_all_eta25_pueq02->Write();
 
    scsumet_ee_all_eta15_pueq02->Write();
    scsumet_ee_all_eta20_pueq02->Write();
    scsumet_ee_all_eta25_pueq02->Write();



    scet_eb_all_eta15_pueq03->Write();
    scet_eb_all_eta20_pueq03->Write();
    scet_eb_all_eta25_pueq03->Write();
 
    scet_ee_all_eta15_pueq03->Write();
    scet_ee_all_eta20_pueq03->Write();
    scet_ee_all_eta25_pueq03->Write();

    scsumet_eb_all_eta15_pueq03->Write();
    scsumet_eb_all_eta20_pueq03->Write();
    scsumet_eb_all_eta25_pueq03->Write();
 
    scsumet_ee_all_eta15_pueq03->Write();
    scsumet_ee_all_eta20_pueq03->Write();
    scsumet_ee_all_eta25_pueq03->Write();



    scet_eb_all_eta15_pueq04->Write();
    scet_eb_all_eta20_pueq04->Write();
    scet_eb_all_eta25_pueq04->Write();
 
    scet_ee_all_eta15_pueq04->Write();
    scet_ee_all_eta20_pueq04->Write();
    scet_ee_all_eta25_pueq04->Write();

    scsumet_eb_all_eta15_pueq04->Write();
    scsumet_eb_all_eta20_pueq04->Write();
    scsumet_eb_all_eta25_pueq04->Write();
 
    scsumet_ee_all_eta15_pueq04->Write();
    scsumet_ee_all_eta20_pueq04->Write();
    scsumet_ee_all_eta25_pueq04->Write();



    scet_eb_all_eta15_pueq05->Write();
    scet_eb_all_eta20_pueq05->Write();
    scet_eb_all_eta25_pueq05->Write();
 
    scet_ee_all_eta15_pueq05->Write();
    scet_ee_all_eta20_pueq05->Write();
    scet_ee_all_eta25_pueq05->Write();

    scsumet_eb_all_eta15_pueq05->Write();
    scsumet_eb_all_eta20_pueq05->Write();
    scsumet_eb_all_eta25_pueq05->Write();
 
    scsumet_ee_all_eta15_pueq05->Write();
    scsumet_ee_all_eta20_pueq05->Write();
    scsumet_ee_all_eta25_pueq05->Write();



    scet_eb_all_eta15_pueq06->Write();
    scet_eb_all_eta20_pueq06->Write();
    scet_eb_all_eta25_pueq06->Write();
 
    scet_ee_all_eta15_pueq06->Write();
    scet_ee_all_eta20_pueq06->Write();
    scet_ee_all_eta25_pueq06->Write();

    scsumet_eb_all_eta15_pueq06->Write();
    scsumet_eb_all_eta20_pueq06->Write();
    scsumet_eb_all_eta25_pueq06->Write();
 
    scsumet_ee_all_eta15_pueq06->Write();
    scsumet_ee_all_eta20_pueq06->Write();
    scsumet_ee_all_eta25_pueq06->Write();



    scet_eb_all_eta15_pueq07->Write();
    scet_eb_all_eta20_pueq07->Write();
    scet_eb_all_eta25_pueq07->Write();
 
    scet_ee_all_eta15_pueq07->Write();
    scet_ee_all_eta20_pueq07->Write();
    scet_ee_all_eta25_pueq07->Write();

    scsumet_eb_all_eta15_pueq07->Write();
    scsumet_eb_all_eta20_pueq07->Write();
    scsumet_eb_all_eta25_pueq07->Write();
 
    scsumet_ee_all_eta15_pueq07->Write();
    scsumet_ee_all_eta20_pueq07->Write();
    scsumet_ee_all_eta25_pueq07->Write();



    scet_eb_all_eta15_pueq08->Write();
    scet_eb_all_eta20_pueq08->Write();
    scet_eb_all_eta25_pueq08->Write();
 
    scet_ee_all_eta15_pueq08->Write();
    scet_ee_all_eta20_pueq08->Write();
    scet_ee_all_eta25_pueq08->Write();

    scsumet_eb_all_eta15_pueq08->Write();
    scsumet_eb_all_eta20_pueq08->Write();
    scsumet_eb_all_eta25_pueq08->Write();
 
    scsumet_ee_all_eta15_pueq08->Write();
    scsumet_ee_all_eta20_pueq08->Write();
    scsumet_ee_all_eta25_pueq08->Write();



    scet_eb_all_eta15_pueq09->Write();
    scet_eb_all_eta20_pueq09->Write();
    scet_eb_all_eta25_pueq09->Write();
 
    scet_ee_all_eta15_pueq09->Write();
    scet_ee_all_eta20_pueq09->Write();
    scet_ee_all_eta25_pueq09->Write();

    scsumet_eb_all_eta15_pueq09->Write();
    scsumet_eb_all_eta20_pueq09->Write();
    scsumet_eb_all_eta25_pueq09->Write();
 
    scsumet_ee_all_eta15_pueq09->Write();
    scsumet_ee_all_eta20_pueq09->Write();
    scsumet_ee_all_eta25_pueq09->Write();


    ebtime_vs_bxtrain_01->Write();
    ebtime_vs_bxtrain_05->Write();
    eetime_vs_bxtrain_01->Write();
    eetime_vs_bxtrain_05->Write();



    rechiteta_vs_bxtrain_01->Write();
    rechiteta_vs_bxtrain_05->Write();
    sceta_vs_bxtrain->Write();



    eb_digi_01->Write();
    eb_digi_05->Write();
    eb_digi_0105->Write();
    eb_digi_0530->Write();

    ee_digi_01->Write();
    ee_digi_05->Write();
    ee_digi_0105->Write();
    ee_digi_0530->Write();




    ebpedmean_g12->Write();
    ebpedmean_g6->Write();
    ebpedmean_g1->Write();

    ebpedrms_g12->Write();
    ebpedrms_g6->Write();
    ebpedrms_g1->Write();


    eepedmean_g12->Write();
    eepedmean_g6->Write();
    eepedmean_g1->Write();

    eepedrms_g12->Write();
    eepedrms_g6->Write();
    eepedrms_g1->Write();

    
    for (Int_t i=0;i<360;i++) {

      for (Int_t j=0;j<170;j++) {

	double x=etaphi_ped->GetBinContent(i+1,j+1);
	double y=etaphi_pedsq->GetBinContent(i+1,j+1);
	double n=etaphi_pedn->GetBinContent(i+1,j+1);


	if (n>1) {

	  double variance=(n*y-pow(x,2))/(n*(n-1));
	  double sigma=sqrt(variance);
	  etaphi_pednoise->SetBinContent(i+1,j+1,sigma);

	}

      }


    }

    etaphi_ped->Write();
    etaphi_pedsq->Write();
    etaphi_pedn->Write();
    etaphi_pednoise->Write();


    // normalise 3d digi plots

    for (Int_t i=0;i<120;i++) {
      for (Int_t j=0;j<10;j++) {

	float r=0;

	float a=eb_digi_0105_vs_time->GetBinContent(i+1,j+1);
	float b=eb_digi_0105_vs_time_norm->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0105_vs_time->SetBinContent(i+1,j+1,r);


	r=0;

	a=eb_digi_0530_vs_time->GetBinContent(i+1,j+1);
	b=eb_digi_0530_vs_time_norm->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0530_vs_time->SetBinContent(i+1,j+1,r);



	r=0;

	a=ee_digi_0105_vs_time->GetBinContent(i+1,j+1);
	b=ee_digi_0105_vs_time_norm->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0105_vs_time->SetBinContent(i+1,j+1,r);


	r=0;

	a=ee_digi_0530_vs_time->GetBinContent(i+1,j+1);
	b=ee_digi_0530_vs_time_norm->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0530_vs_time->SetBinContent(i+1,j+1,r);




	r=0;

	a=eb_digi_0105_vs_time_eta15->GetBinContent(i+1,j+1);
	b=eb_digi_0105_vs_time_norm_eta15->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0105_vs_time_eta15->SetBinContent(i+1,j+1,r);


	r=0;

	a=eb_digi_0530_vs_time_eta15->GetBinContent(i+1,j+1);
	b=eb_digi_0530_vs_time_norm_eta15->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0530_vs_time_eta15->SetBinContent(i+1,j+1,r);



	r=0;

	a=ee_digi_0105_vs_time_eta15->GetBinContent(i+1,j+1);
	b=ee_digi_0105_vs_time_norm_eta15->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0105_vs_time_eta15->SetBinContent(i+1,j+1,r);


	r=0;

	a=ee_digi_0530_vs_time_eta15->GetBinContent(i+1,j+1);
	b=ee_digi_0530_vs_time_norm_eta15->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0530_vs_time_eta15->SetBinContent(i+1,j+1,r);







	r=0;

	a=eb_digi_0105_vs_time_eta20->GetBinContent(i+1,j+1);
	b=eb_digi_0105_vs_time_norm_eta20->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0105_vs_time_eta20->SetBinContent(i+1,j+1,r);


	r=0;

	a=eb_digi_0530_vs_time_eta20->GetBinContent(i+1,j+1);
	b=eb_digi_0530_vs_time_norm_eta20->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0530_vs_time_eta20->SetBinContent(i+1,j+1,r);



	r=0;

	a=ee_digi_0105_vs_time_eta20->GetBinContent(i+1,j+1);
	b=ee_digi_0105_vs_time_norm_eta20->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0105_vs_time_eta20->SetBinContent(i+1,j+1,r);


	r=0;

	a=ee_digi_0530_vs_time_eta20->GetBinContent(i+1,j+1);
	b=ee_digi_0530_vs_time_norm_eta20->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0530_vs_time_eta20->SetBinContent(i+1,j+1,r);



	r=0;

	a=eb_digi_0105_vs_time_eta25->GetBinContent(i+1,j+1);
	b=eb_digi_0105_vs_time_norm_eta25->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0105_vs_time_eta25->SetBinContent(i+1,j+1,r);


	r=0;

	a=eb_digi_0530_vs_time_eta25->GetBinContent(i+1,j+1);
	b=eb_digi_0530_vs_time_norm_eta25->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0530_vs_time_eta25->SetBinContent(i+1,j+1,r);



	r=0;

	a=ee_digi_0105_vs_time_eta25->GetBinContent(i+1,j+1);
	b=ee_digi_0105_vs_time_norm_eta25->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0105_vs_time_eta25->SetBinContent(i+1,j+1,r);


	r=0;

	a=ee_digi_0530_vs_time_eta25->GetBinContent(i+1,j+1);
	b=ee_digi_0530_vs_time_norm_eta25->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0530_vs_time_eta25->SetBinContent(i+1,j+1,r);










      }
    }




    for (Int_t i=0;i<40;i++) {
      for (Int_t j=0;j<10;j++) {

	float r=0;

	float a=eb_digi_0105_vs_bxtrain->GetBinContent(i+1,j+1);
	float b=eb_digi_0105_vs_bxtrain_norm->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0105_vs_bxtrain->SetBinContent(i+1,j+1,r);


	r=0;

	a=eb_digi_0530_vs_bxtrain->GetBinContent(i+1,j+1);
	b=eb_digi_0530_vs_bxtrain_norm->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0530_vs_bxtrain->SetBinContent(i+1,j+1,r);



	r=0;

	a=ee_digi_0105_vs_bxtrain->GetBinContent(i+1,j+1);
	b=ee_digi_0105_vs_bxtrain_norm->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0105_vs_bxtrain->SetBinContent(i+1,j+1,r);


	r=0;

	a=ee_digi_0530_vs_bxtrain->GetBinContent(i+1,j+1);
	b=ee_digi_0530_vs_bxtrain_norm->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0530_vs_bxtrain->SetBinContent(i+1,j+1,r);





	r=0;

	a=eb_digi_0105_vs_bxtrain_eta15->GetBinContent(i+1,j+1);
	b=eb_digi_0105_vs_bxtrain_norm_eta15->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0105_vs_bxtrain_eta15->SetBinContent(i+1,j+1,r);


	r=0;

	a=eb_digi_0530_vs_bxtrain_eta15->GetBinContent(i+1,j+1);
	b=eb_digi_0530_vs_bxtrain_norm_eta15->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0530_vs_bxtrain_eta15->SetBinContent(i+1,j+1,r);



	r=0;

	a=ee_digi_0105_vs_bxtrain_eta15->GetBinContent(i+1,j+1);
	b=ee_digi_0105_vs_bxtrain_norm_eta15->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0105_vs_bxtrain_eta15->SetBinContent(i+1,j+1,r);


	r=0;

	a=ee_digi_0530_vs_bxtrain_eta15->GetBinContent(i+1,j+1);
	b=ee_digi_0530_vs_bxtrain_norm_eta15->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0530_vs_bxtrain_eta15->SetBinContent(i+1,j+1,r);







	r=0;

	a=eb_digi_0105_vs_bxtrain_eta20->GetBinContent(i+1,j+1);
	b=eb_digi_0105_vs_bxtrain_norm_eta20->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0105_vs_bxtrain_eta20->SetBinContent(i+1,j+1,r);


	r=0;

	a=eb_digi_0530_vs_bxtrain_eta20->GetBinContent(i+1,j+1);
	b=eb_digi_0530_vs_bxtrain_norm_eta20->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0530_vs_bxtrain_eta20->SetBinContent(i+1,j+1,r);



	r=0;

	a=ee_digi_0105_vs_bxtrain_eta20->GetBinContent(i+1,j+1);
	b=ee_digi_0105_vs_bxtrain_norm_eta20->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0105_vs_bxtrain_eta20->SetBinContent(i+1,j+1,r);


	r=0;

	a=ee_digi_0530_vs_bxtrain_eta20->GetBinContent(i+1,j+1);
	b=ee_digi_0530_vs_bxtrain_norm_eta20->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0530_vs_bxtrain_eta20->SetBinContent(i+1,j+1,r);







	r=0;

	a=eb_digi_0105_vs_bxtrain_eta25->GetBinContent(i+1,j+1);
	b=eb_digi_0105_vs_bxtrain_norm_eta25->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0105_vs_bxtrain_eta25->SetBinContent(i+1,j+1,r);


	r=0;

	a=eb_digi_0530_vs_bxtrain_eta25->GetBinContent(i+1,j+1);
	b=eb_digi_0530_vs_bxtrain_norm_eta25->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	eb_digi_0530_vs_bxtrain_eta25->SetBinContent(i+1,j+1,r);



	r=0;

	a=ee_digi_0105_vs_bxtrain_eta25->GetBinContent(i+1,j+1);
	b=ee_digi_0105_vs_bxtrain_norm_eta25->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0105_vs_bxtrain_eta25->SetBinContent(i+1,j+1,r);


	r=0;

	a=ee_digi_0530_vs_bxtrain_eta25->GetBinContent(i+1,j+1);
	b=ee_digi_0530_vs_bxtrain_norm_eta25->GetBinContent(i+1,j+1);

	if (b>0) r=a/b;

	ee_digi_0530_vs_bxtrain_eta25->SetBinContent(i+1,j+1,r);








      }
    }
    

    eb_digi_0105_vs_time->Write();
    eb_digi_0530_vs_time->Write();


    ee_digi_0105_vs_time->Write();
    ee_digi_0530_vs_time->Write();


    eb_digi_0105_vs_bxtrain->Write();
    eb_digi_0530_vs_bxtrain->Write();


    ee_digi_0105_vs_bxtrain->Write();
    ee_digi_0530_vs_bxtrain->Write();



    eb_digi_0105_vs_time_eta15->Write();
    eb_digi_0530_vs_time_eta15->Write();


    ee_digi_0105_vs_time_eta15->Write();
    ee_digi_0530_vs_time_eta15->Write();


    eb_digi_0105_vs_bxtrain_eta15->Write();
    eb_digi_0530_vs_bxtrain_eta15->Write();


    ee_digi_0105_vs_bxtrain_eta15->Write();
    ee_digi_0530_vs_bxtrain_eta15->Write();






    eb_digi_0105_vs_time_eta20->Write();
    eb_digi_0530_vs_time_eta20->Write();


    ee_digi_0105_vs_time_eta20->Write();
    ee_digi_0530_vs_time_eta20->Write();


    eb_digi_0105_vs_bxtrain_eta20->Write();
    eb_digi_0530_vs_bxtrain_eta20->Write();


    ee_digi_0105_vs_bxtrain_eta20->Write();
    ee_digi_0530_vs_bxtrain_eta20->Write();






    eb_digi_0105_vs_time_eta25->Write();
    eb_digi_0530_vs_time_eta25->Write();


    ee_digi_0105_vs_time_eta25->Write();
    ee_digi_0530_vs_time_eta25->Write();


    eb_digi_0105_vs_bxtrain_eta25->Write();
    eb_digi_0530_vs_bxtrain_eta25->Write();


    ee_digi_0105_vs_bxtrain_eta25->Write();
    ee_digi_0530_vs_bxtrain_eta25->Write();






    dataTree_   ->Write();
    }
  file_ = 0;
}


void ChanstatusTester::scan5x5(const DetId & det, const edm::Handle<EcalRecHitCollection> &hits, const edm::ESHandle<CaloTopology>  &caloTopo, const edm::ESHandle<CaloGeometry>  &geometry, int &nHits, float & totEt)
{
  nHits = 0; 
  totEt = 0;

  CaloNavigator<DetId> cursor = CaloNavigator<DetId>(det,caloTopo->getSubdetectorTopology(det));

  for(int j=side_/2; j>=-side_/2; --j)
    {
      for(int i=-side_/2; i<=side_/2; ++i)
	{
	  cursor.home();
	  cursor.offsetBy(i,j);
	  if(hits->find(*cursor)!=hits->end()) // if hit exists in the rechit collection
	    {
	      EcalRecHit tmpHit = *hits->find(*cursor); // get rechit with detID at cursor

	
	      const GlobalPoint p ( geometry->getPosition(*cursor) ) ; // calculate Et of the rechit
	      TVector3 hitPos(p.x(),p.y(),p.z());
	      hitPos *= 1.0/hitPos.Mag();
	      hitPos *= tmpHit.energy();
	      float rechitEt =  hitPos.Pt();
        
	      //--- return values
	      totEt += rechitEt;  
   

	      if(tmpHit.energy()>Emin_ && !tmpHit.checkFlag(EcalRecHit::kGood))nHits++;

	    }
	}
    }


}


const std::vector<DetId> ChanstatusTester::neighbours(const DetId& id){

  std::vector<DetId> ret;
 
  if ( id.subdetId() == EcalBarrel) {
    
    ret.push_back( EBDetId::offsetBy( id,  1, 0 ));
    ret.push_back( EBDetId::offsetBy( id, -1, 0 ));
    ret.push_back( EBDetId::offsetBy( id,  0, 1 ));
    ret.push_back( EBDetId::offsetBy( id,  0,-1 ));
  }
  return ret;
  
} 


float ChanstatusTester::recHitE( const DetId id, 
                           const EcalRecHitCollection & recHits,
                           int di, int dj )
 {
   // in the barrel:   di = dEta   dj = dPhi
   // in the endcap:   di = dX     dj = dY
   
   DetId nid;
   if( id.subdetId() == EcalBarrel) nid = EBDetId::offsetBy( id, di, dj );
   else if( id.subdetId() == EcalEndcap) nid = EEDetId::offsetBy( id, di, dj );
   
   return ( nid == DetId(0) ? 0 : recHitE( nid, recHits ) );
}


float ChanstatusTester::recHitE( const DetId id, 
				 const EcalRecHitCollection &recHits) {
    
   if ( id.rawId() == 0 ) return 0;

   EcalRecHitCollection::const_iterator it = recHits.find( id );
   if ( it != recHits.end() ){
     float ene= (*it).energy();
     return ene;
   }
   return 0;
}



float ChanstatusTester::e4e1(const DetId& id, 
			       const EcalRecHitCollection& rhs){
 
  
   float s4 = 0;
   float e1 = recHitE( id, rhs );
   

   if ( e1 == 0 ) return 0;
   const std::vector<DetId>& neighs =  neighbours(id);
   for (size_t i=0; i<neighs.size(); ++i){
     s4+=recHitE(neighs[i],rhs);
   }

   return s4 / e1;

}

float ChanstatusTester::e4e1v2(const DetId& id, 
			       const EcalRecHitCollection& rhs){
 
  
   float s4 = 0;
   float e1 = recHitE( id, rhs );
   float temp=0;

   if ( e1 == 0 ) return 0;
   const std::vector<DetId>& neighs =  neighbours(id);
   for (size_t i=0; i<neighs.size(); ++i){
     temp=recHitE(neighs[i],rhs);
     if (temp>0.08) s4+=temp;
   }

   return s4 / e1;

}



float ChanstatusTester::e6e2(const DetId& id, 
			       const EcalRecHitCollection& rhs){

  float s4_1 = 0;
  float s4_2 = 0;
  float e1 = recHitE( id, rhs );
  float maxene=0;
  DetId maxid;
  
  if ( e1 == 0 ) return 0;
  
  const std::vector<DetId>& neighs =  neighbours(id);


  for (size_t i=0; i<neighs.size(); ++i){
    float ene = recHitE(neighs[i],rhs);
    if (ene>maxene) {
      maxene=ene;
      maxid = neighs[i];
    }
  }
  
  float e2=maxene;
     
  s4_1 = e4e1(id,rhs)* e1;
  s4_2 = e4e1(maxid,rhs)* e2;
  
  return (s4_1 + s4_2) / (e1+e2) -1. ;
  
}
 








//////////////////////////////////////////////////////////////////////////////////////////
void ChanstatusTester::analyze(edm::Event const& event, edm::EventSetup const& iSetup) 
{ 



  //  cout << "In analyse" << endl;

  /*
   edm::Handle<L1GlobalTriggerReadoutRecord> gtRecord;
   event.getByLabel("gtDigis", gtRecord);
  */


  //  edm::ESHandle<EcalLaserDbService> laser;
  // iSetup.get<EcalLaserDbRecord>().get(laser);
  


  edm::ESHandle<EcalPedestals> ecalped;
  iSetup.get<EcalPedestalsRcd>().get(ecalped);
  const EcalPedestals* eped=ecalped.product();
   
   
   bit36=0;
   bit37=0;
   bit38=0;
   bit39=0;
   bit40=0;
   bit41=0;
   bit3 =0;
   bit4 =0;
   bit9 =0;
   bit0 =0;

   eg1=0;
   eg2=0;
   eg5=0;
   algo124=0;

   algo54=0;
   algo55=0;
   algo56=0;
   algo57=0;
   algo58=0;
   algo59=0;
   algo60=0;
   algo61=0;
   algo62=0;

   algo106=0;
   algo107=0;



   rank_=0;
   ntrk=0;
   goodtrk=0;

   numvtx=0;
   vtx_x=0;
   vtx_y=0;
   vtx_z=0;
   vtx_x_err=0;
   vtx_y_err=0;
   vtx_z_err=0;

   vtx_chi2=-1;
   vtx_ndof=0;
   vtx_ntracks=0;
   vtx_isfake=0;

   vtx_good=0;
   numgoodvtx=0;

   scale=0;

   energy_ecal=0;
   energy_hcal=0;


  eemax=0;
  eemaxet=0;
  eeix=0;
  eeiy=0;
  eeix=0;
  eetime=0;
  eeflags=0;
  eehits=0;
  eehits1GeV=0;
  ee_eta=0;
  ee_phi=0;
  ee_r9=0;
  eephits=0;
  eemhits=0;

  ebmax=0;
  eb_ieta=0;
  eb_iphi=0;
  eb_eta=0;
  eb_phi=0;
  ebtime=0;
  ebflags=0;
  ebhits=0;
  ebhits1GeV=0;
  ebmaxet=0;
  eb_r9=0;
  eb_r4=0;
  eb_e9=0;
  eb_e25=0;


  ebchi2=0;
  eb2chi2=0;
  ebchi2oot=0;
  eb2chi2oot=0;


  eemax2=0;
  eemaxet2=0;
  eeix2=0;
  eeiy2=0;
  eeix2=0;
  eetime2=0;
  eeflags2=0;
  eehits1GeV2=0;
  ee_eta2=0;
  ee_phi2=0;
  ee_r92=0;

  ebmax2=0;
  eb_ieta2=0;
  eb_iphi2=0;
  eb_eta2=0;
  eb_phi2=0;
  ebtime2=0;
  ebflags2=0;
  ebhits1GeV2=0;
  ebmaxet2=0;
  eb_r92=0;
  eb_r42=0;



  ebnumrechits_01=0;
  ebnumrechits_05=0;
  ebnumrechits_0105=0;
  ebnumrechits_0530=0;

  eenumrechits_01=0;
  eenumrechits_05=0;
  eenumrechits_0105=0;
  eenumrechits_0530=0;



  ebflag_kgood=0;
  ebflag_kpoorreco=0;
  ebflag_koutoftime=0;
  ebflag_kfake=0;


  tmean_en=0;
  terr_en=0;

  tmean_sig=0;
  terr_sig=0;


  r4count=0;
  e2e9count_thresh0=0;
  e2e25count_thresh0=0;
  e2e9count_thresh1=0;
  e2e25count_thresh1=0;

  e2e9count_thresh0_nor4=0;
  e2e25count_thresh0_nor4=0;
  e2e9count_thresh1_nor4=0;
  e2e25count_thresh1_nor4=0;

  r4_algo_count=0;
  e2e9_algo_count=0;
  e2e9_algo_count_5_1=0;
  e2e9_algo_count_5_0=0;

  swisscross_algo=0;
  e2e9_algo=0;

  scale=1.0;
  ncr_ = 0 ;
  
  energy_pf_     = 0;
  energyc_pf_    = 0;
  energyn_pf_    = 0;
  energyg_pf_    = 0;
  
  energy_ecal    = 0;
  energy_hcal    = 0;
  
  ptJet_      = 0;
  etaJet_     = 0;
  phiJet_     = 0;
  chfJet_     = 0;
  nhfJet_     = 0;
  cemfJet_    = 0;
  nemfJet_    = 0;
  cmultiJet_  = 0;
  nmultiJet_  = 0;
  
  
  nrjets_     = 1;

	
  eesum_gt1=0;
  eesum_gt2=0;
  eesum_gt4=0;

  ebsum_gt1=0;
  ebsum_gt2=0;
  ebsum_gt4=0;



  eesum_gt1et=0;
  eesum_gt2et=0;
  eesum_gt4et=0;

  ebsum_gt1et=0;
  ebsum_gt2et=0;
  ebsum_gt4et=0;

   
  ebhits1GeVet=0;
  ebhits2GeV=0;
  ebhits4GeV=0;
  eehits2GeV=0;
  eehits4GeV=0;

  eehits1GeVet=0;
  ebhits2GeVet=0;
  ebhits4GeVet=0;
  eehits2GeVet=0;
  eehits4GeVet=0;


  numspikes_kweird=0;
  numspikes_swisscross=0;



  run         = event.id().run();
  even        = event.id().event();
  lumi        = event.luminosityBlock();
  bx          = event.bunchCrossing();
  //  physdeclared= gtRecord->gtFdlWord().physicsDeclared();
  orbit       = event.orbitNumber(); 
  double sec  = event.time().value() >> 32 ;
  double usec = 0xFFFFFFFF & event.time().value();
  double conv = 1e6;
  time        = (sec+usec/conv);





  
 
 




 

 
 



   edm::ESHandle<EcalChannelStatus> chanstat;
   iSetup.get<EcalChannelStatusRcd>().get(chanstat);
   //   const EcalChannelStatus* cstat=chanstat.product();
 

    
    float db_pedg12=0, db_pedg6=0, db_pedg1=0;
    float db_pedrmsg12=0, db_pedrmsg6=0, db_pedrmsg1=0;

   for (Int_t i=0;i<360;i++) {
     for (Int_t j=0;j<170;j++) {

       int iphitmp=i+1;
       int ietatmp=j-85+1*(j>84);
       // cout << iphitmp << ":" << ietatmp << endl;
       if (EBDetId::validDetId(ietatmp,iphitmp)) {

	 EBDetId deteb(ietatmp,iphitmp,0);

	 int ebhashedid = deteb.hashedIndex();
	 const EcalPedestals::Item *aped=0;
	 aped=&eped->barrel(ebhashedid);
	 db_pedg12=aped->mean_x12;
	 db_pedg6=aped->mean_x6;
	 db_pedg1=aped->mean_x1;
	 
	 db_pedrmsg12=aped->rms_x12;
	 db_pedrmsg6=aped->rms_x6;
	 db_pedrmsg1=aped->rms_x1;

	 ebpedmean_g12->SetBinContent(i+1,j+1,db_pedg12);
	 ebpedmean_g6->SetBinContent(i+1,j+1,db_pedg6);
	 ebpedmean_g1->SetBinContent(i+1,j+1,db_pedg1);

	 ebpedrms_g12->SetBinContent(i+1,j+1,db_pedrmsg12);
	 ebpedrms_g6->SetBinContent(i+1,j+1,db_pedrmsg6);
	 ebpedrms_g1->SetBinContent(i+1,j+1,db_pedrmsg1);


	 EcalChannelStatusCode chstatcode;
	 EcalChannelStatusMap::const_iterator chit=chanstat->find(deteb);
	 if (chit!=chanstat->end()) {
	   chstatcode=*chit;
	   cout << "Detid ieta=" << deteb.ieta() << " Detid iphi=" 
		<< deteb.iphi() << " Channel status=" 
		<< chstatcode.getEncodedStatusCode() << endl;

	   ebchstatus->SetBinContent(i+1,j+1,chstatcode.getEncodedStatusCode());
	 }

       }
     }
   }
   

   for (Int_t i=0;i<100;i++) {
     for (Int_t j=0;j<100;j++) {
       for (Int_t k=0;k<2;k++) {
	 int ixtmp=i+1;
	 int iytmp=j+1;
	 int iztmp=-1+2*(k>0);

	 if (EEDetId::validDetId(ixtmp,iytmp,iztmp)) {
	   
	   EEDetId detee(ixtmp,iytmp,iztmp,0);


	 int eehashedid = detee.hashedIndex();
	 const EcalPedestals::Item *aped=0;
	 aped=&eped->endcap(eehashedid);
	 db_pedg12=aped->mean_x12;
	 db_pedg6=aped->mean_x6;
	 db_pedg1=aped->mean_x1;
	 
	 db_pedrmsg12=aped->rms_x12;
	 db_pedrmsg6=aped->rms_x6;
	 db_pedrmsg1=aped->rms_x1;

	 eepedmean_g12->SetBinContent(i+1+100*k,j+1,db_pedg12);
	 eepedmean_g6->SetBinContent(i+1+100*k,j+1,db_pedg6);
	 eepedmean_g1->SetBinContent(i+1+100*k,j+1,db_pedg1);

	 eepedrms_g12->SetBinContent(i+1+100*k,j+1,db_pedrmsg12);
	 eepedrms_g6->SetBinContent(i+1+100*k,j+1,db_pedrmsg6);
	 eepedrms_g1->SetBinContent(i+1+100*k,j+1,db_pedrmsg1);





	   EcalChannelStatusCode chstatcode;
	   EcalChannelStatusMap::const_iterator chit=chanstat->find(detee);
	   if (chit!=chanstat->end()) {
	     chstatcode=*chit;
	     cout << "Detid ix=" << detee.ix() << " Detid iy=" 
		  << detee.iy() << "Detid iz=" << detee.zside() 
		  <<  " Channel status=" 
		  << chstatcode.getEncodedStatusCode()
		  << " pedmean=" << db_pedg12 << ":" << db_pedg6
		  << ":" << db_pedg1 
		  << " pedrms=" << db_pedrmsg12 
		  << ":" << db_pedrmsg6 << ":" << db_pedrmsg1 << endl;
	     
	     eechstatus->SetBinContent(i+1+100*k,j+1,chstatcode.getEncodedStatusCode());
	   }
	   
	 }
       }
     }
   }




  dataTree_->Fill();

 
  
  //  cout << "end jets" << endl;
      
}
//////////////////////////////////////////////////////////////////////////////////////////
ChanstatusTester::~ChanstatusTester() 
{
  delete file_;
  delete dataTree_;
}
//}

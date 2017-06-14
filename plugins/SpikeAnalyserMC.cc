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


#include "JetMETCorrections/MCJet/plugins/SpikeAnalyserMC.h" 
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

#include "Math/VectorUtil.h"
#include "TVector3.h"

using namespace edm;
using namespace reco;
using namespace std;

DEFINE_FWK_MODULE(SpikeAnalyserMC);

SpikeAnalyserMC::SpikeAnalyserMC(const edm::ParameterSet& cfg)
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
void SpikeAnalyserMC::beginJob() 
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
void SpikeAnalyserMC::endJob() 
{

  cout << "in endjob" << endl;

  if (file_ !=0) 
    {
      file_->cd();



      cout << "BP1" << endl;

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


void SpikeAnalyserMC::scan5x5(const DetId & det, const edm::Handle<EcalRecHitCollection> &hits, const edm::ESHandle<CaloTopology>  &caloTopo, const edm::ESHandle<CaloGeometry>  &geometry, int &nHits, float & totEt)
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


const std::vector<DetId> SpikeAnalyserMC::neighbours(const DetId& id){

  std::vector<DetId> ret;
 
  if ( id.subdetId() == EcalBarrel) {
    
    ret.push_back( EBDetId::offsetBy( id,  1, 0 ));
    ret.push_back( EBDetId::offsetBy( id, -1, 0 ));
    ret.push_back( EBDetId::offsetBy( id,  0, 1 ));
    ret.push_back( EBDetId::offsetBy( id,  0,-1 ));
  }
  return ret;
  
} 


float SpikeAnalyserMC::recHitE( const DetId id, 
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


float SpikeAnalyserMC::recHitE( const DetId id, 
				 const EcalRecHitCollection &recHits) {
    
   if ( id.rawId() == 0 ) return 0;

   EcalRecHitCollection::const_iterator it = recHits.find( id );
   if ( it != recHits.end() ){
     float ene= (*it).energy();
     return ene;
   }
   return 0;
}



float SpikeAnalyserMC::e4e1(const DetId& id, 
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

float SpikeAnalyserMC::e4e1v2(const DetId& id, 
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



float SpikeAnalyserMC::e6e2(const DetId& id, 
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
void SpikeAnalyserMC::analyze(edm::Event const& event, edm::EventSetup const& iSetup) 
{ 



  //  cout << "In analyse" << endl;

  /*
   edm::Handle<L1GlobalTriggerReadoutRecord> gtRecord;
   event.getByLabel("gtDigis", gtRecord);
  */


  //  edm::ESHandle<EcalLaserDbService> laser;
  // iSetup.get<EcalLaserDbRecord>().get(laser);
  

   
   
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




  eventenergy->Reset();
  eventet->Reset();
  eventtime->Reset();
  eventkoot->Reset();
  eventkdiweird->Reset();



  // get position in bunch train

  bunchintrain=-1;

  for (std::vector<int>::const_iterator bxit=bunchstartbx_.begin(); bxit!=bunchstartbx_.end(); ++bxit) {

    Int_t bxpos=bx - *bxit;


    // 50ns
    //    if (bxpos>=0 && bxpos<=70) bunchintrain=bxpos/2;


    // 25ns
    if (bxpos>=0 && bxpos<=50) bunchintrain=bxpos;

  }


  
  // PU info

//   edm::InputTag PileupSrc_("addPileupInfo");
//   Handle<std::vector<PileupSummaryInfo>> PupInfo;
//   event.getByLabel(PileupSrc_, PupInfo);

//   std::vector<PileupSummaryInfo>::const_iterator PVI;

//   for (PVI=PupInfo->begin(); PVI!=PupInfo->end(); ++PVI) {

//       cout << "PU INFO:  bx=" << PVI->getBunchCrossing() << " PU=" << PVI->getPU_NumInteractions() << endl;

//   }

//   // end of PU info

  

  Float_t toffset=0.0;
  
  //    if (run>135000 && run<136000 && !isMC_) toffset=2.08;


  //Int_t numtracks=0;
  //Int_t goodtracks=0;




  // if (even==89083177) {


  //  cout << "begin tracks" << endl;


  /*
  edm::Handle<reco::TrackCollection> tracks;
  event.getByLabel(tracks_,tracks);
  const reco::TrackCollection* itTrack = tracks.product();
  
  numtracks=itTrack->size();

  reco::TrackBase::TrackQuality _trackQuality=reco::TrackBase::qualityByName("highPurity");

  reco::TrackCollection::const_iterator itk=itTrack->begin();
  reco::TrackCollection::const_iterator itk_e=itTrack->end();


  for (;itk!=itk_e;++itk) {
     if (itk->quality(_trackQuality)) goodtracks++;
  }


  ntrk=numtracks;
  goodtrk=goodtracks;

  */
  
  edm::Handle<VertexCollection> vertices;
  event.getByLabel(vertex_coll_,vertices);
  VertexCollection::const_iterator vit;

  numvtx=vertices->size();
  

  if (vertices->size()>0) {

    for (vit=vertices->begin();vit!=vertices->end();++vit) {

      vtx_x=vit->x();
      vtx_y=vit->y();
      vtx_z=vit->z();
      vtx_x_err=vit->xError();
      vtx_y_err=vit->yError();
      vtx_z_err=vit->zError();

      vtx_chi2=vit->chi2();
      vtx_ndof=vit->ndof();

      //      vector<TrackBaseRef>::const_iterator trstart=vit->tracks_begin();
      //      vector<TrackBaseRef>::const_iterator trend=vit->tracks_end();


      vtx_ntracks=vit->tracksSize();


      vtx_isfake=vit->isFake();

      if (vit->isValid() && vtx_isfake==0 && vtx_ndof>4 && vtx_chi2>0 && vtx_chi2<10000) {

        vtx_good=1;
        numgoodvtx++;

      }
     
    }

  } 







 

  //  cout << "begin rechits" << endl;

  // EB rechits

  Handle<EcalRecHitCollection> EBhits;

  // event.getByLabel("ecalRecHit","EcalRecHitsEB",EBhits);

   event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEB","reRECO"),EBhits);



  //alternatives
  //event.getByLabel("reducedEcalRecHitsEB","",EBhits);
  //event.getByLabel("alCaIsolatedElectrons","alCaRecHitsEB",EBhits);

  
  const EcalRecHitCollection *ebRecHits=EBhits.product();



  Handle<EcalRecHitCollection> EEhits;

  //event.getByLabel("ecalRecHit","EcalRecHitsEE",EEhits);
 
  event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEE","reRECO"),EEhits); 

  // alternatives
  // event.getByLabel("reducedEcalRecHitsEE","",EEhits);
  // event.getByLabel("alCaIsolatedElectrons","alCaRecHitsEE",EEhits);

  const EcalRecHitCollection *eeRecHits=EEhits.product();




  // digis
 

 


   Handle<EBDigiCollection>  EBdigis;
 
   event.getByLabel("ecalDigis","ebDigis",EBdigis);
   

   //  event.getByLabel("ecalEBunpacker","ebDigis",EBdigis);
   //  event.getByLabel("selectDigi","selectedEcalEBDigiCollection",EBdigis);
   
 

   //   event.getByLabel("selectDigi","selectedEcalEBDigiCollection",EBdigis);
   

   Handle<EBDigiCollection>  EBdigis2;


   // spike APD digis - MC only

   if (isMC_) {
      event.getByLabel(edm::InputTag("simEcalUnsuppressedDigis","APD","reRECO"),EBdigis2);
  
   }
  
   Handle<EEDigiCollection>  EEdigis;
   if (isMC_) { 
     event.getByLabel("ecalDigis","eeDigis",EEdigis);
   }
   else {
     //   event.getByLabel("ecalEBunpacker","eeDigis",EEdigis);
     event.getByLabel("selectDigi","selectedEcalEEDigiCollection",EEdigis);
   }

   //   event.getByLabel("selectDigi","selectedEcalEEDigiCollection",EEdigis);
   
   
   Int_t ebdigiadc=0;
   // Int_t ebdigigain=0;

   //  cout << "run=" << run << " event=" << even << endl;
  
   //  Int_t apd_ieta=0;
   //  Int_t apd_iphi=0;
   
   numspikes=0;
   numspikes50=0;
   numspikes100=0;



   if (isMC_) {



     for (EBDigiCollection::const_iterator digiItrEB= EBdigis2->begin(); digiItrEB != EBdigis2->end(); ++digiItrEB) {	  
   
            
     
       int adcmax=0;
       int adcmaxsample=0;

     
       EBDataFrame df(*digiItrEB);
       for(int i=0; i<10;++i)
	 {
	   ebdigiadc=df.sample(i).adc();

	   if (ebdigiadc>adcmax) {
	     adcmax=ebdigiadc;
	     adcmaxsample=i+1;
	   }
	   
	   
	 }
       
       numspikes++;
       
       if (adcmax>250) {
	 numspikes50++;
	 spiketime50->Fill(float(adcmaxsample));
       }
       
       if (adcmax>300) numspikes100++;
       
       spikeadc->Fill(adcmax-250);
       
       
       
     }
     
   }
     
     


   for (EBDigiCollection::const_iterator digiItrEB= EBdigis2->begin(); digiItrEB != EBdigis2->end(); ++digiItrEB) {	  
   
            
     
       int adcmax=0;
       int gainmax=0;
       int sampmax=0;
     
       // int dframe[10];
       // int gframe[10];

       EBDataFrame df(*digiItrEB);
       for(int i=0; i<10;++i)
	 {
	   int ebdigiadc=df.sample(i).adc();
	   int ebdigigain=df.sample(i).gainId();

	   // dframe[i]=ebdigiadc;
	   // gframe[i]=ebdigigain;

	   if (ebdigiadc>adcmax) {
	     adcmax=ebdigiadc;
	     gainmax=ebdigigain;
	     sampmax=i+1;
	   }
	   
	 }
       
       
       if ((adcmax>250 || gainmax>1) && (sampmax==4 || sampmax==5 || sampmax==6) ) {
    
	 //	 EBDetId spikedet=digiItrEB->id();
	 // int spike_ieta=spikedet.ieta();      
	 // int spike_iphi=spikedet.iphi();

	 // cout << "Spike ieta,iphi= " << spike_ieta << "," << spike_iphi << endl;

	 for (Int_t i=0;i<10;i++) {

	   // cout << i+1 << " " << gframe[i] << " " <<  dframe[i] << endl;

	 } 

       }

   }


   for (EBDigiCollection::const_iterator digiItrEB= EBdigis->begin(); digiItrEB != EBdigis->end(); ++digiItrEB) {	  
   
            
     
       int adcmax=0;
       int gainmax=0;
       int sampmax=0;
     
       //int dframe[10];
       //int gframe[10];

       EBDataFrame df(*digiItrEB);
       for(int i=0; i<10;++i)
	 {
	   int ebdigiadc=df.sample(i).adc();
	   int ebdigigain=df.sample(i).gainId();

	   //  dframe[i]=ebdigiadc;
	   // gframe[i]=ebdigigain;

	   if (ebdigiadc>adcmax) {
	     adcmax=ebdigiadc;
	     gainmax=ebdigigain;
	     sampmax=i+1;
	   }
	   
	 }
       
       
       if ((adcmax>220 || gainmax>1) && (sampmax==4 || sampmax==5 || sampmax==6) ) {
    
	 //EBDetId spikedet=digiItrEB->id();
	 // int spike_ieta=spikedet.ieta();      
	 // int spike_iphi=spikedet.iphi();

	 //	 cout << "Rechit ieta,iphi= " << spike_ieta << "," << spike_iphi << endl;

	 for (Int_t i=0;i<10;i++) {

	   // cout << i+1 << " " << gframe[i] << " " <<  dframe[i] << endl;

	 } 

       }

   }



  // Calotower section

  edm::Handle<CaloTowerCollection> towers;
  event.getByLabel("towerMaker",towers);


  ncalotower = 0;

  ncalotowereb=0;
  ncalotoweree=0;
  ncalotowerhf=0;

  ncalotowerebgt1=0;
  ncalotowerebgt2=0;
  ncalotowerebgt5=0;
  ncalotowerebgt10=0;

  ncalotowereegt1=0;
  ncalotowereegt2=0;
  ncalotowereegt5=0;
  ncalotowereegt10=0;

  ncalotowerhfgt1=0;
  ncalotowerhfgt2=0;
  ncalotowerhfgt5=0;
  ncalotowerhfgt10=0;

  ctsumebgt1=0;
  ctsumebgt2=0;
  ctsumebgt5=0;
  ctsumebgt10=0;

  ctsumeegt1=0;
  ctsumeegt2=0;
  ctsumeegt5=0;
  ctsumeegt10=0;

  ctsumhfgt1=0;
  ctsumhfgt2=0;
  ctsumhfgt5=0;
  ctsumhfgt10=0;






  if (towers.isValid() && towers->size()>0) {
    for(CaloTowerCollection::const_iterator it = towers->begin();it != towers->end(); it++) {
      
      double ct_eta=it->eta();
      //   double ct_phi = it->phi();
      //   double ct_et = it->et();
      double ct_emf=it->emEt();
      //   double ct_had=it->hadEt();
      //   double ct_hof=it->outerEt();

      ncalotower++;



      if (fabs(ct_eta)<3.0 && ct_emf>0.0) {

	calotowereta_all->Fill(ct_eta);
	calotowereta_etweight->Fill(ct_eta,ct_emf);

	if (ct_emf>1.0) {

	  calotowereta_gt1et->Fill(ct_eta);
	  calotowereta_etweight_gt1et->Fill(ct_eta,ct_emf);


	  if (numgoodvtx<10) 	  calotowereta_gt1et_pu10->Fill(ct_eta);
	  if (numgoodvtx>=10 && numgoodvtx<20) 	  calotowereta_gt1et_pu20->Fill(ct_eta);
	  if (numgoodvtx>20) 	  calotowereta_gt1et_pu30->Fill(ct_eta);


	}	

      }


      if (fabs(ct_eta)<=1.48) {

	ncalotowereb++;

	if (ct_emf>1.0) {
	  ncalotowerebgt1++;
	  ctsumebgt1+=ct_emf;
	}

	if (ct_emf>2.0) {
	  ncalotowerebgt2++;
	  ctsumebgt2+=ct_emf;
	}

	if (ct_emf>5.0) {
	  ncalotowerebgt5++;
	  ctsumebgt5+=ct_emf;
	}

	if (ct_emf>10.0) {
	  ncalotowerebgt10++;
	  ctsumebgt10+=ct_emf;
	}

      }

      if (fabs(ct_eta)>1.48 && fabs(ct_eta)<3.0) {


	ncalotoweree++;

	if (ct_emf>1.0) {
	  ncalotowereegt1++;
	  ctsumeegt1+=ct_emf;
	}

	if (ct_emf>2.0) {
	  ncalotowereegt2++;
	  ctsumeegt2+=ct_emf;
	}

	if (ct_emf>5.0) {
	  ncalotowereegt5++;
	  ctsumeegt5+=ct_emf;
	}

	if (ct_emf>10.0) {
	  ncalotowereegt10++;
	  ctsumeegt10+=ct_emf;
	}

      }


      if (fabs(ct_eta)>3.0) {


	ncalotowerhf++;

	if (ct_emf>1.0) {
	  ncalotowerhfgt1++;
	  ctsumhfgt1+=ct_emf;
	}

	if (ct_emf>2.0) {
	  ncalotowerhfgt2++;
	  ctsumhfgt2+=ct_emf;
	}

	if (ct_emf>5.0) {
	  ncalotowerhfgt5++;
	  ctsumhfgt5+=ct_emf;
	}

	if (ct_emf>10.0) {
	  ncalotowerhfgt10++;
	  ctsumhfgt10+=ct_emf;
	}

      }




    }

  }

  edm::ESHandle<CaloGeometry> pG;
  iSetup.get<CaloGeometryRecord>().get(pG);
  const CaloGeometry* geo=pG.product();

  edm::ESHandle<CaloTopology> pTopology;
  iSetup.get<CaloTopologyRecord>().get(pTopology);


  // edm::ESHandle<EcalChannelStatus> chanstat;
  // iSetup.get<EcalChannelStatusRcd>().get(chanstat);
  // const EcalChannelStatus* cstat=chanstat.product();
 
  edm::ESHandle<EcalPedestals> ecalped;
  iSetup.get<EcalPedestalsRcd>().get(ecalped);
  const EcalPedestals* eped=ecalped.product();
 

  
  edm::ESHandle<EcalADCToGeVConstant> ecaladcgev;
  iSetup.get<EcalADCToGeVConstantRcd>().get(ecaladcgev);
  const EcalADCToGeVConstant* eadcgev=ecaladcgev.product();
 
  float adctogevconst=eadcgev->getEBValue();



  edm::ESHandle<EcalIntercalibConstants> ecalic;
  iSetup.get<EcalIntercalibConstantsRcd>().get(ecalic);
  const EcalIntercalibConstants* eic=ecalic.product();
 







  edm::ESHandle<EcalSeverityLevelAlgo> sevlv;
  iSetup.get<EcalSeverityLevelAlgoRcd>().get(sevlv);
  const EcalSeverityLevelAlgo *severity=sevlv.product();

  edm::Handle<reco::SuperClusterCollection> sc_eb;
  event.getByLabel("correctedHybridSuperClusters","",sc_eb);


  edm::Handle<reco::SuperClusterCollection> sc_ee;
  event.getByLabel("correctedMulti5x5SuperClustersWithPreshower","",sc_ee);



  // sc loop:  EB


  ebscsumet_all=0;
  float ebscsumet_severity0=0;
  float ebscsumet_koot0=0;

  float ebscsumet_all_gt2=0;
  float ebscsumet_severity0_gt2=0;
  float ebscsumet_koot0_gt2=0;

  float ebscsumet_all_gt5=0;
  float ebscsumet_severity0_gt5=0;
  float ebscsumet_koot0_gt5=0;

  float ebscsumet_all_gt10=0;
  float ebscsumet_severity0_gt10=0;
  float ebscsumet_koot0_gt10=0;


  ebscsumet_all_eta15=0;
  ebscsumet_all_eta20=0;
  ebscsumet_all_eta25=0;


  float ebscsumet_all_eta15_pu10=0;
  float ebscsumet_all_eta20_pu10=0;
  float ebscsumet_all_eta25_pu10=0;

  float ebscsumet_all_eta15_pu20=0;
  float ebscsumet_all_eta20_pu20=0;
  float ebscsumet_all_eta25_pu20=0;

  float ebscsumet_all_eta15_pu30=0;
  float ebscsumet_all_eta20_pu30=0;
  float ebscsumet_all_eta25_pu30=0;

  float ebscsumet_all_eta15_pueq10=0;
  float ebscsumet_all_eta20_pueq10=0;
  float ebscsumet_all_eta25_pueq10=0;

  float ebscsumet_all_eta15_pueq20=0;
  float ebscsumet_all_eta20_pueq20=0;
  float ebscsumet_all_eta25_pueq20=0;



  eescsumet_all_eta15=0;
  eescsumet_all_eta20=0;
  eescsumet_all_eta25=0;


  float eescsumet_all_eta15_pu10=0;
  float eescsumet_all_eta20_pu10=0;
  float eescsumet_all_eta25_pu10=0;

  float eescsumet_all_eta15_pu20=0;
  float eescsumet_all_eta20_pu20=0;
  float eescsumet_all_eta25_pu20=0;

  float eescsumet_all_eta15_pu30=0;
  float eescsumet_all_eta20_pu30=0;
  float eescsumet_all_eta25_pu30=0;

  float eescsumet_all_eta15_pueq10=0;
  float eescsumet_all_eta20_pueq10=0;
  float eescsumet_all_eta25_pueq10=0;

  float eescsumet_all_eta15_pueq20=0;
  float eescsumet_all_eta20_pueq20=0;
  float eescsumet_all_eta25_pueq20=0;


  float ebscsumet_all_eta15_pueq01=0;
  float ebscsumet_all_eta20_pueq01=0;
  float ebscsumet_all_eta25_pueq01=0;
  float eescsumet_all_eta15_pueq01=0;
  float eescsumet_all_eta20_pueq01=0;
  float eescsumet_all_eta25_pueq01=0;

  float ebscsumet_all_eta15_pueq02=0;
  float ebscsumet_all_eta20_pueq02=0;
  float ebscsumet_all_eta25_pueq02=0;
  float eescsumet_all_eta15_pueq02=0;
  float eescsumet_all_eta20_pueq02=0;
  float eescsumet_all_eta25_pueq02=0;

  float ebscsumet_all_eta15_pueq03=0;
  float ebscsumet_all_eta20_pueq03=0;
  float ebscsumet_all_eta25_pueq03=0;
  float eescsumet_all_eta15_pueq03=0;
  float eescsumet_all_eta20_pueq03=0;
  float eescsumet_all_eta25_pueq03=0;

  float ebscsumet_all_eta15_pueq04=0;
  float ebscsumet_all_eta20_pueq04=0;
  float ebscsumet_all_eta25_pueq04=0;
  float eescsumet_all_eta15_pueq04=0;
  float eescsumet_all_eta20_pueq04=0;
  float eescsumet_all_eta25_pueq04=0;

  float ebscsumet_all_eta15_pueq05=0;
  float ebscsumet_all_eta20_pueq05=0;
  float ebscsumet_all_eta25_pueq05=0;
  float eescsumet_all_eta15_pueq05=0;
  float eescsumet_all_eta20_pueq05=0;
  float eescsumet_all_eta25_pueq05=0;

  float ebscsumet_all_eta15_pueq06=0;
  float ebscsumet_all_eta20_pueq06=0;
  float ebscsumet_all_eta25_pueq06=0;
  float eescsumet_all_eta15_pueq06=0;
  float eescsumet_all_eta20_pueq06=0;
  float eescsumet_all_eta25_pueq06=0;

  float ebscsumet_all_eta15_pueq07=0;
  float ebscsumet_all_eta20_pueq07=0;
  float ebscsumet_all_eta25_pueq07=0;
  float eescsumet_all_eta15_pueq07=0;
  float eescsumet_all_eta20_pueq07=0;
  float eescsumet_all_eta25_pueq07=0;

  float ebscsumet_all_eta15_pueq08=0;
  float ebscsumet_all_eta20_pueq08=0;
  float ebscsumet_all_eta25_pueq08=0;
  float eescsumet_all_eta15_pueq08=0;
  float eescsumet_all_eta20_pueq08=0;
  float eescsumet_all_eta25_pueq08=0;

  float ebscsumet_all_eta15_pueq09=0;
  float ebscsumet_all_eta20_pueq09=0;
  float ebscsumet_all_eta25_pueq09=0;
  float eescsumet_all_eta15_pueq09=0;
  float eescsumet_all_eta20_pueq09=0;
  float eescsumet_all_eta25_pueq09=0;

  ebnumsc_all=0;
  eenumsc_all=0;

  for (reco::SuperClusterCollection::const_iterator sCEB = sc_eb->begin(); sCEB != sc_eb->end(); ++sCEB) {

    DetId seedid=sCEB->seed()->seed();

    bool goodseed=false;

    int sevlev=0;
    //  float time=0;
    int koot=0;

    for (EcalRecHitCollection::const_iterator hitItr = EBhits->begin(); hitItr != EBhits->end(); ++hitItr) {

      EcalRecHit hit = (*hitItr);
      EBDetId det    = hit.id(); 
    
      if (det!=seedid) continue;

      sevlev=severity->severityLevel(det,*ebRecHits);
      // time     = hit.time();

      koot=0;
      if (hit.checkFlag(EcalRecHit::kOutOfTime)) koot=1;

      goodseed=true;

    }

    if (goodseed) {

      Float_t scenergy=sCEB->energy();
      Float_t sc_eta=sCEB->eta();
      Float_t sc_phi=sCEB->phi();
      Float_t scet=scenergy/cosh(sc_eta);

      ebscsumet_all+=scet;
      if (sevlev==0) ebscsumet_severity0+=scet;
      if (koot==0) ebscsumet_koot0+=scet;
      ebnumsc_all++;
    


      scet_eb_all->Fill(scet);
      if (sevlev==0) scet_eb_severity0->Fill(scet);
      if (koot==0) scet_eb_koot0->Fill(scet);
      


      sceta_all->Fill(sc_eta);
 
      sceta_vs_bxtrain->Fill(bunchintrain,sc_eta);

      if (sevlev==0) sceta_severity0->Fill(sc_eta);
      if (koot==0) sceta_koot0->Fill(sc_eta);

 
      if (numgoodvtx==1) {
	sceta_all_pueq01->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq01->Fill(sc_eta);
      }

 
      if (numgoodvtx==2) {
	sceta_all_pueq02->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq02->Fill(sc_eta);
      }

 
      if (numgoodvtx==3) {
	sceta_all_pueq03->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq03->Fill(sc_eta);
      }

 
      if (numgoodvtx==4) {
	sceta_all_pueq04->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq04->Fill(sc_eta);
      }

 
      if (numgoodvtx==5) {
	sceta_all_pueq05->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq05->Fill(sc_eta);
      }

 
      if (numgoodvtx==6) {
	sceta_all_pueq06->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq06->Fill(sc_eta);
      }

 
      if (numgoodvtx==7) {
	sceta_all_pueq07->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq07->Fill(sc_eta);
      }

 
      if (numgoodvtx==8) {
	sceta_all_pueq08->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq08->Fill(sc_eta);
      }

 
      if (numgoodvtx==9) {
	sceta_all_pueq09->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq09->Fill(sc_eta);
      }




      if (fabs(sc_eta)<0.5) {

	scet_eb_all_eta15->Fill(scet);	
	ebscsumet_all_eta15+=scet;


	if (numgoodvtx<=10) {
	  scet_eb_all_eta15_pu10->Fill(scet);
	  ebscsumet_all_eta15_pu10+=scet;
	}

	if (numgoodvtx>10 && numgoodvtx<=20) {
	  scet_eb_all_eta15_pu20->Fill(scet);
	  ebscsumet_all_eta15_pu20+=scet;
	}

	if (numgoodvtx>20) {
	  scet_eb_all_eta15_pu30->Fill(scet);
	  ebscsumet_all_eta15_pu30+=scet;
	}


	if (numgoodvtx==10) {
	  scet_eb_all_eta15_pueq10->Fill(scet);
	  ebscsumet_all_eta15_pueq10+=scet;
	}


	if (numgoodvtx==20) {
	  scet_eb_all_eta15_pueq20->Fill(scet);
	  ebscsumet_all_eta15_pueq20+=scet;
	}

	if (numgoodvtx==1) {
	  scet_eb_all_eta15_pueq01->Fill(scet);
	  ebscsumet_all_eta15_pueq01+=scet;
	}

	if (numgoodvtx==2) {
	  scet_eb_all_eta15_pueq02->Fill(scet);
	  ebscsumet_all_eta15_pueq02+=scet;
	}

	if (numgoodvtx==3) {
	  scet_eb_all_eta15_pueq03->Fill(scet);
	  ebscsumet_all_eta15_pueq03+=scet;
	}

	if (numgoodvtx==4) {
	  scet_eb_all_eta15_pueq04->Fill(scet);
	  ebscsumet_all_eta15_pueq04+=scet;
	}

	if (numgoodvtx==5) {
	  scet_eb_all_eta15_pueq05->Fill(scet);
	  ebscsumet_all_eta15_pueq05+=scet;
	}

	if (numgoodvtx==6) {
	  scet_eb_all_eta15_pueq06->Fill(scet);
	  ebscsumet_all_eta15_pueq06+=scet;
	}

	if (numgoodvtx==7) {
	  scet_eb_all_eta15_pueq07->Fill(scet);
	  ebscsumet_all_eta15_pueq07+=scet;
	}

	if (numgoodvtx==8) {
	  scet_eb_all_eta15_pueq08->Fill(scet);
	  ebscsumet_all_eta15_pueq08+=scet;
	}

	if (numgoodvtx==9) {
	  scet_eb_all_eta15_pueq09->Fill(scet);
	  ebscsumet_all_eta15_pueq09+=scet;
	}




      }


      if (fabs(sc_eta)>=0.5 && fabs(sc_eta)<1.0) {

	scet_eb_all_eta20->Fill(scet);
	ebscsumet_all_eta20+=scet;



	if (numgoodvtx<=10) {
	  scet_eb_all_eta20_pu10->Fill(scet);
	  ebscsumet_all_eta20_pu10+=scet;
	}

	if (numgoodvtx>10 && numgoodvtx<=20) {
	  scet_eb_all_eta20_pu20->Fill(scet);
	  ebscsumet_all_eta20_pu20+=scet;
	}

	if (numgoodvtx>20) {
	  scet_eb_all_eta20_pu30->Fill(scet);
	  ebscsumet_all_eta20_pu30+=scet;
	}


	if (numgoodvtx==10) {
	  scet_eb_all_eta20_pueq10->Fill(scet);
	  ebscsumet_all_eta20_pueq10+=scet;
	}


	if (numgoodvtx==20) {
	  scet_eb_all_eta20_pueq20->Fill(scet);
	  ebscsumet_all_eta20_pueq20+=scet;
	}



	if (numgoodvtx==1) {
	  scet_eb_all_eta20_pueq01->Fill(scet);
	  ebscsumet_all_eta20_pueq01+=scet;
	}

	if (numgoodvtx==2) {
	  scet_eb_all_eta20_pueq02->Fill(scet);
	  ebscsumet_all_eta20_pueq02+=scet;
	}

	if (numgoodvtx==3) {
	  scet_eb_all_eta20_pueq03->Fill(scet);
	  ebscsumet_all_eta20_pueq03+=scet;
	}

	if (numgoodvtx==4) {
	  scet_eb_all_eta20_pueq04->Fill(scet);
	  ebscsumet_all_eta20_pueq04+=scet;
	}

	if (numgoodvtx==5) {
	  scet_eb_all_eta20_pueq05->Fill(scet);
	  ebscsumet_all_eta20_pueq05+=scet;
	}

	if (numgoodvtx==6) {
	  scet_eb_all_eta20_pueq06->Fill(scet);
	  ebscsumet_all_eta20_pueq06+=scet;
	}

	if (numgoodvtx==7) {
	  scet_eb_all_eta20_pueq07->Fill(scet);
	  ebscsumet_all_eta20_pueq07+=scet;
	}

	if (numgoodvtx==8) {
	  scet_eb_all_eta20_pueq08->Fill(scet);
	  ebscsumet_all_eta20_pueq08+=scet;
	}

	if (numgoodvtx==9) {
	  scet_eb_all_eta20_pueq09->Fill(scet);
	  ebscsumet_all_eta20_pueq09+=scet;
	}



	
      }


      if (fabs(sc_eta)>1.0) {

	scet_eb_all_eta25->Fill(scet);
	ebscsumet_all_eta25+=scet;


	if (numgoodvtx<=10) {
	  scet_eb_all_eta25_pu10->Fill(scet);
	  ebscsumet_all_eta25_pu10+=scet;
	}

	if (numgoodvtx>10 && numgoodvtx<=20) {
	  scet_eb_all_eta25_pu20->Fill(scet);
	  ebscsumet_all_eta25_pu20+=scet;
	}

	if (numgoodvtx>20) {
	  scet_eb_all_eta25_pu30->Fill(scet);
	  ebscsumet_all_eta25_pu30+=scet;
	}


	if (numgoodvtx==10) {
	  scet_eb_all_eta25_pueq10->Fill(scet);
	  ebscsumet_all_eta25_pueq10+=scet;
	}


	if (numgoodvtx==20) {
	  scet_eb_all_eta25_pueq20->Fill(scet);
	  ebscsumet_all_eta25_pueq20+=scet;
	}


 	if (numgoodvtx==1) {
	  scet_eb_all_eta25_pueq01->Fill(scet);
	  ebscsumet_all_eta25_pueq01+=scet;
	}

	if (numgoodvtx==2) {
	  scet_eb_all_eta25_pueq02->Fill(scet);
	  ebscsumet_all_eta25_pueq02+=scet;
	}

	if (numgoodvtx==3) {
	  scet_eb_all_eta25_pueq03->Fill(scet);
	  ebscsumet_all_eta25_pueq03+=scet;
	}

	if (numgoodvtx==4) {
	  scet_eb_all_eta25_pueq04->Fill(scet);
	  ebscsumet_all_eta25_pueq04+=scet;
	}

	if (numgoodvtx==5) {
	  scet_eb_all_eta25_pueq05->Fill(scet);
	  ebscsumet_all_eta25_pueq05+=scet;
	}

	if (numgoodvtx==6) {
	  scet_eb_all_eta25_pueq06->Fill(scet);
	  ebscsumet_all_eta25_pueq06+=scet;
	}

	if (numgoodvtx==7) {
	  scet_eb_all_eta25_pueq07->Fill(scet);
	  ebscsumet_all_eta25_pueq07+=scet;
	}

	if (numgoodvtx==8) {
	  scet_eb_all_eta25_pueq08->Fill(scet);
	  ebscsumet_all_eta25_pueq08+=scet;
	}

	if (numgoodvtx==9) {
	  scet_eb_all_eta25_pueq09->Fill(scet);
	  ebscsumet_all_eta25_pueq09+=scet;
	}




     }





      if (scet>2.0) {

	sceta_all_gt2->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_gt2->Fill(sc_eta);
	if (koot==0) sceta_koot0_gt2->Fill(sc_eta);

	ebscsumet_all_gt2+=scet;
	if (sevlev==0) ebscsumet_severity0_gt2+=scet;
	if (koot==0) ebscsumet_koot0_gt2+=scet;
   

      }

     if (scet>5.0) {

	sceta_all_gt5->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_gt5->Fill(sc_eta);
	if (koot==0) sceta_koot0_gt5->Fill(sc_eta);

 	ebscsumet_all_gt5+=scet;
	if (sevlev==0) ebscsumet_severity0_gt5+=scet;
	if (koot==0) ebscsumet_koot0_gt5+=scet;
 
     }

     if (scet>10.0) {

	sceta_all_gt10->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_gt10->Fill(sc_eta);
	if (koot==0) sceta_koot0_gt10->Fill(sc_eta);

	ebscsumet_all_gt10+=scet;
	if (sevlev==0) ebscsumet_severity0_gt10+=scet;
	if (koot==0) ebscsumet_koot0_gt10+=scet;
 
      }

     if (scet>50.0) {
       scocc_eb_gt50->Fill(sc_phi,sc_eta);
     }
     
    }
  }


  scsumet_eb_all->Fill(ebscsumet_all);
  scsumet_eb_severity0->Fill(ebscsumet_severity0);
  scsumet_eb_koot0->Fill(ebscsumet_koot0);

  scsumet_eb_all_eta15->Fill(ebscsumet_all_eta15);
  scsumet_eb_all_eta20->Fill(ebscsumet_all_eta20);
  scsumet_eb_all_eta25->Fill(ebscsumet_all_eta25);



  scsumet_eb_all_eta15_pu10->Fill(ebscsumet_all_eta15_pu10);
  scsumet_eb_all_eta20_pu10->Fill(ebscsumet_all_eta20_pu10);
  scsumet_eb_all_eta25_pu10->Fill(ebscsumet_all_eta25_pu10);

  scsumet_eb_all_eta15_pu20->Fill(ebscsumet_all_eta15_pu20);
  scsumet_eb_all_eta20_pu20->Fill(ebscsumet_all_eta20_pu20);
  scsumet_eb_all_eta25_pu20->Fill(ebscsumet_all_eta25_pu20);

  scsumet_eb_all_eta15_pu30->Fill(ebscsumet_all_eta15_pu30);
  scsumet_eb_all_eta20_pu30->Fill(ebscsumet_all_eta20_pu30);
  scsumet_eb_all_eta25_pu30->Fill(ebscsumet_all_eta25_pu30);

  scsumet_eb_all_eta15_pueq10->Fill(ebscsumet_all_eta15_pueq10);
  scsumet_eb_all_eta20_pueq10->Fill(ebscsumet_all_eta20_pueq10);
  scsumet_eb_all_eta25_pueq10->Fill(ebscsumet_all_eta25_pueq10);

  scsumet_eb_all_eta15_pueq20->Fill(ebscsumet_all_eta15_pueq20);
  scsumet_eb_all_eta20_pueq20->Fill(ebscsumet_all_eta20_pueq20);
  scsumet_eb_all_eta25_pueq20->Fill(ebscsumet_all_eta25_pueq20);


  scsumet_eb_all_eta15_pueq01->Fill(ebscsumet_all_eta15_pueq01);
  scsumet_eb_all_eta20_pueq01->Fill(ebscsumet_all_eta20_pueq01);
  scsumet_eb_all_eta25_pueq01->Fill(ebscsumet_all_eta25_pueq01);

  scsumet_eb_all_eta15_pueq02->Fill(ebscsumet_all_eta15_pueq02);
  scsumet_eb_all_eta20_pueq02->Fill(ebscsumet_all_eta20_pueq02);
  scsumet_eb_all_eta25_pueq02->Fill(ebscsumet_all_eta25_pueq02);

  scsumet_eb_all_eta15_pueq03->Fill(ebscsumet_all_eta15_pueq03);
  scsumet_eb_all_eta20_pueq03->Fill(ebscsumet_all_eta20_pueq03);
  scsumet_eb_all_eta25_pueq03->Fill(ebscsumet_all_eta25_pueq03);

  scsumet_eb_all_eta15_pueq04->Fill(ebscsumet_all_eta15_pueq04);
  scsumet_eb_all_eta20_pueq04->Fill(ebscsumet_all_eta20_pueq04);
  scsumet_eb_all_eta25_pueq04->Fill(ebscsumet_all_eta25_pueq04);

  scsumet_eb_all_eta15_pueq05->Fill(ebscsumet_all_eta15_pueq05);
  scsumet_eb_all_eta20_pueq05->Fill(ebscsumet_all_eta20_pueq05);
  scsumet_eb_all_eta25_pueq05->Fill(ebscsumet_all_eta25_pueq05);

  scsumet_eb_all_eta15_pueq06->Fill(ebscsumet_all_eta15_pueq06);
  scsumet_eb_all_eta20_pueq06->Fill(ebscsumet_all_eta20_pueq06);
  scsumet_eb_all_eta25_pueq06->Fill(ebscsumet_all_eta25_pueq06);

  scsumet_eb_all_eta15_pueq07->Fill(ebscsumet_all_eta15_pueq07);
  scsumet_eb_all_eta20_pueq07->Fill(ebscsumet_all_eta20_pueq07);
  scsumet_eb_all_eta25_pueq07->Fill(ebscsumet_all_eta25_pueq07);

  scsumet_eb_all_eta15_pueq08->Fill(ebscsumet_all_eta15_pueq08);
  scsumet_eb_all_eta20_pueq08->Fill(ebscsumet_all_eta20_pueq08);
  scsumet_eb_all_eta25_pueq08->Fill(ebscsumet_all_eta25_pueq08);

  scsumet_eb_all_eta15_pueq09->Fill(ebscsumet_all_eta15_pueq09);
  scsumet_eb_all_eta20_pueq09->Fill(ebscsumet_all_eta20_pueq09);
  scsumet_eb_all_eta25_pueq09->Fill(ebscsumet_all_eta25_pueq09);


  scsumet_eb_all_gt2->Fill(ebscsumet_all_gt2);
  scsumet_eb_severity0_gt2->Fill(ebscsumet_severity0_gt2);
  scsumet_eb_koot0_gt2->Fill(ebscsumet_koot0_gt2);

  scsumet_eb_all_gt5->Fill(ebscsumet_all_gt5);
  scsumet_eb_severity0_gt5->Fill(ebscsumet_severity0_gt5);
  scsumet_eb_koot0_gt5->Fill(ebscsumet_koot0_gt5);

  scsumet_eb_all_gt10->Fill(ebscsumet_all_gt10);
  scsumet_eb_severity0_gt10->Fill(ebscsumet_severity0_gt10);
  scsumet_eb_koot0_gt10->Fill(ebscsumet_koot0_gt10);


  // sc loop:  EE


  eescsumet_all=0;
  float eescsumet_severity0=0;
  float eescsumet_koot0=0;

  float eescsumet_all_gt2=0;
  float eescsumet_severity0_gt2=0;
  float eescsumet_koot0_gt2=0;

  float eescsumet_all_gt5=0;
  float eescsumet_severity0_gt5=0;
  float eescsumet_koot0_gt5=0;

  float eescsumet_all_gt10=0;
  float eescsumet_severity0_gt10=0;
  float eescsumet_koot0_gt10=0;




  for (reco::SuperClusterCollection::const_iterator sCEE = sc_ee->begin(); sCEE != sc_ee->end(); ++sCEE) {

    // cout << "IN EE SC" << endl;
    DetId seedid=sCEE->seed()->seed();

    bool goodseed=false;

    int sevlev=0;
    // float time=0;
    int koot=0;

    // float seeden=0;


    for (EcalRecHitCollection::const_iterator hitItr = EEhits->begin(); hitItr != EEhits->end(); ++hitItr) {

      EcalRecHit hit = (*hitItr);
      EEDetId det    = hit.id(); 
  
      if (det!=seedid) continue;

      sevlev=severity->severityLevel(det,*eeRecHits);
      // time     = hit.time();

      //seeden = hit.energy();

      koot=0;

      if (hit.checkFlag(EcalRecHit::kOutOfTime)) koot=1;
      // cout << "sevlev=" << sevlev << endl;

      //      if (sevlev!=0) goodseed=false;
      
      goodseed=true;

    }

    if (goodseed) {

      Float_t scenergy=sCEE->energy();
      Float_t sc_eta=sCEE->eta();
      Float_t sc_phi=sCEE->phi();
      Float_t scet=scenergy/cosh(sc_eta);

      sceta_all->Fill(sc_eta);

      sceta_vs_bxtrain->Fill(bunchintrain,sc_eta);


      if (sevlev==0) sceta_severity0->Fill(sc_eta);
      if (koot==0) sceta_koot0->Fill(sc_eta);

      eenumsc_all++;

      if (numgoodvtx==1) {
	sceta_all_pueq01->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq01->Fill(sc_eta);
      }

 
      if (numgoodvtx==2) {
	sceta_all_pueq02->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq02->Fill(sc_eta);
      }

 
      if (numgoodvtx==3) {
	sceta_all_pueq03->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq03->Fill(sc_eta);
      }

 
      if (numgoodvtx==4) {
	sceta_all_pueq04->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq04->Fill(sc_eta);
      }

 
      if (numgoodvtx==5) {
	sceta_all_pueq05->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq05->Fill(sc_eta);
      }

 
      if (numgoodvtx==6) {
	sceta_all_pueq06->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq06->Fill(sc_eta);
      }

 
      if (numgoodvtx==7) {
	sceta_all_pueq07->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq07->Fill(sc_eta);
      }

 
      if (numgoodvtx==8) {
	sceta_all_pueq08->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq08->Fill(sc_eta);
      }

 
      if (numgoodvtx==9) {
	sceta_all_pueq09->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_pueq09->Fill(sc_eta);
      }





      scet_ee_all->Fill(scet);
      if (sevlev==0) scet_ee_severity0->Fill(scet);
      if (koot==0) scet_ee_koot0->Fill(scet);
      

	
      eescsumet_all+=scet;
      if (sevlev==0) eescsumet_severity0+=scet;
      if (koot==0) eescsumet_koot0+=scet;



      if (fabs(sc_eta)<2.0) {

	scet_ee_all_eta15->Fill(scet);	
	eescsumet_all_eta15+=scet;


	if (numgoodvtx<=10) {
	  scet_ee_all_eta15_pu10->Fill(scet);
	  eescsumet_all_eta15_pu10+=scet;
	}

	if (numgoodvtx>10 && numgoodvtx<=20) {
	  scet_ee_all_eta15_pu20->Fill(scet);
	  eescsumet_all_eta15_pu20+=scet;
	}

	if (numgoodvtx>20) {
	  scet_ee_all_eta15_pu30->Fill(scet);
	  eescsumet_all_eta15_pu30+=scet;
	}


	if (numgoodvtx==10) {
	  scet_ee_all_eta15_pueq10->Fill(scet);
	  eescsumet_all_eta15_pueq10+=scet;
	}


	if (numgoodvtx==20) {
	  scet_ee_all_eta15_pueq20->Fill(scet);
	  eescsumet_all_eta15_pueq20+=scet;
	}



	if (numgoodvtx==1) {
	  scet_ee_all_eta15_pueq01->Fill(scet);
	  eescsumet_all_eta15_pueq01+=scet;
	}

	if (numgoodvtx==2) {
	  scet_ee_all_eta15_pueq02->Fill(scet);
	  eescsumet_all_eta15_pueq02+=scet;
	}

	if (numgoodvtx==3) {
	  scet_ee_all_eta15_pueq03->Fill(scet);
	  eescsumet_all_eta15_pueq03+=scet;
	}

	if (numgoodvtx==4) {
	  scet_ee_all_eta15_pueq04->Fill(scet);
	  eescsumet_all_eta15_pueq04+=scet;
	}

	if (numgoodvtx==5) {
	  scet_ee_all_eta15_pueq05->Fill(scet);
	  eescsumet_all_eta15_pueq05+=scet;
	}

	if (numgoodvtx==6) {
	  scet_ee_all_eta15_pueq06->Fill(scet);
	  eescsumet_all_eta15_pueq06+=scet;
	}

	if (numgoodvtx==7) {
	  scet_ee_all_eta15_pueq07->Fill(scet);
	  eescsumet_all_eta15_pueq07+=scet;
	}

	if (numgoodvtx==8) {
	  scet_ee_all_eta15_pueq08->Fill(scet);
	  eescsumet_all_eta15_pueq08+=scet;
	}

	if (numgoodvtx==9) {
	  scet_ee_all_eta15_pueq09->Fill(scet);
	  eescsumet_all_eta15_pueq09+=scet;
	}



      }


      if (fabs(sc_eta)>=2.0 && fabs(sc_eta)<2.5) {

	scet_ee_all_eta20->Fill(scet);
	eescsumet_all_eta20+=scet;



	if (numgoodvtx<=10) {
	  scet_ee_all_eta20_pu10->Fill(scet);
	  eescsumet_all_eta20_pu10+=scet;
	}

	if (numgoodvtx>10 && numgoodvtx<=20) {
	  scet_ee_all_eta20_pu20->Fill(scet);
	  eescsumet_all_eta20_pu20+=scet;
	}

	if (numgoodvtx>20) {
	  scet_ee_all_eta20_pu30->Fill(scet);
	  eescsumet_all_eta20_pu30+=scet;
	}


	if (numgoodvtx==10) {
	  scet_ee_all_eta20_pueq10->Fill(scet);
	  eescsumet_all_eta20_pueq10+=scet;
	}


	if (numgoodvtx==20) {
	  scet_ee_all_eta20_pueq20->Fill(scet);
	  eescsumet_all_eta20_pueq20+=scet;
	}




	if (numgoodvtx==1) {
	  scet_ee_all_eta20_pueq01->Fill(scet);
	  eescsumet_all_eta20_pueq01+=scet;
	}

	if (numgoodvtx==2) {
	  scet_ee_all_eta20_pueq02->Fill(scet);
	  eescsumet_all_eta20_pueq02+=scet;
	}

	if (numgoodvtx==3) {
	  scet_ee_all_eta20_pueq03->Fill(scet);
	  eescsumet_all_eta20_pueq03+=scet;
	}

	if (numgoodvtx==4) {
	  scet_ee_all_eta20_pueq04->Fill(scet);
	  eescsumet_all_eta20_pueq04+=scet;
	}

	if (numgoodvtx==5) {
	  scet_ee_all_eta20_pueq05->Fill(scet);
	  eescsumet_all_eta20_pueq05+=scet;
	}

	if (numgoodvtx==6) {
	  scet_ee_all_eta20_pueq06->Fill(scet);
	  eescsumet_all_eta20_pueq06+=scet;
	}

	if (numgoodvtx==7) {
	  scet_ee_all_eta20_pueq07->Fill(scet);
	  eescsumet_all_eta20_pueq07+=scet;
	}

	if (numgoodvtx==8) {
	  scet_ee_all_eta20_pueq08->Fill(scet);
	  eescsumet_all_eta20_pueq08+=scet;
	}

	if (numgoodvtx==9) {
	  scet_ee_all_eta20_pueq09->Fill(scet);
	  eescsumet_all_eta20_pueq09+=scet;
	}



	
      }


      if (fabs(sc_eta)>2.5) {

	scet_ee_all_eta25->Fill(scet);
	eescsumet_all_eta25+=scet;


	if (numgoodvtx<=10) {
	  scet_ee_all_eta25_pu10->Fill(scet);
	  eescsumet_all_eta25_pu10+=scet;
	}

	if (numgoodvtx>10 && numgoodvtx<=20) {
	  scet_ee_all_eta25_pu20->Fill(scet);
	  eescsumet_all_eta25_pu20+=scet;
	}

	if (numgoodvtx>20) {
	  scet_ee_all_eta25_pu30->Fill(scet);
	  eescsumet_all_eta25_pu30+=scet;
	}


	if (numgoodvtx==10) {
	  scet_ee_all_eta25_pueq10->Fill(scet);
	  eescsumet_all_eta25_pueq10+=scet;
	}


	if (numgoodvtx==20) {
	  scet_ee_all_eta25_pueq20->Fill(scet);
	  eescsumet_all_eta25_pueq20+=scet;
	}



	if (numgoodvtx==1) {
	  scet_ee_all_eta25_pueq01->Fill(scet);
	  eescsumet_all_eta25_pueq01+=scet;
	}

	if (numgoodvtx==2) {
	  scet_ee_all_eta25_pueq02->Fill(scet);
	  eescsumet_all_eta25_pueq02+=scet;
	}

	if (numgoodvtx==3) {
	  scet_ee_all_eta25_pueq03->Fill(scet);
	  eescsumet_all_eta25_pueq03+=scet;
	}

	if (numgoodvtx==4) {
	  scet_ee_all_eta25_pueq04->Fill(scet);
	  eescsumet_all_eta25_pueq04+=scet;
	}

	if (numgoodvtx==5) {
	  scet_ee_all_eta25_pueq05->Fill(scet);
	  eescsumet_all_eta25_pueq05+=scet;
	}

	if (numgoodvtx==6) {
	  scet_ee_all_eta25_pueq06->Fill(scet);
	  eescsumet_all_eta25_pueq06+=scet;
	}

	if (numgoodvtx==7) {
	  scet_ee_all_eta25_pueq07->Fill(scet);
	  eescsumet_all_eta25_pueq07+=scet;
	}

	if (numgoodvtx==8) {
	  scet_ee_all_eta25_pueq08->Fill(scet);
	  eescsumet_all_eta25_pueq08+=scet;
	}

	if (numgoodvtx==9) {
	  scet_ee_all_eta25_pueq09->Fill(scet);
	  eescsumet_all_eta25_pueq09+=scet;
	}



      }
	

      if (scet>2.0) {

	sceta_all_gt2->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_gt2->Fill(sc_eta);
	if (koot==0) sceta_koot0_gt2->Fill(sc_eta);
	
	eescsumet_all_gt2+=scet;
	if (sevlev==0) eescsumet_severity0_gt2+=scet;
	if (koot==0) eescsumet_koot0_gt2+=scet;
   
      }

      if (scet>5.0) {

	sceta_all_gt5->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_gt5->Fill(sc_eta);
	if (koot==0) sceta_koot0_gt5->Fill(sc_eta);
	
	eescsumet_all_gt5+=scet;
	if (sevlev==0) eescsumet_severity0_gt5+=scet;
	if (koot==0) eescsumet_koot0_gt5+=scet;
 
      }
      
      if (scet>10.0) {

	sceta_all_gt10->Fill(sc_eta);
	if (sevlev==0) sceta_severity0_gt10->Fill(sc_eta);
	if (koot==0) sceta_koot0_gt10->Fill(sc_eta);
	
	eescsumet_all_gt10+=scet;
	if (sevlev==0) eescsumet_severity0_gt10+=scet;
	if (koot==0) eescsumet_koot0_gt10+=scet;
 
      }


     if (scet>50.0) {
       scocc_ee_gt50->Fill(sc_phi,sc_eta);
     }
 


      //      cout << "EE sc ene=" << scenergy << " sc et=" << scet << " sc_seeden=" << seeden << " sc eta=" << sc_eta << " sc_phi=" << sc_phi  << " time=" << time << " koot=" << koot << " sevlev=" << sevlev << endl;
	
    }


  }




  scsumet_ee_all->Fill(eescsumet_all);
  scsumet_ee_severity0->Fill(eescsumet_severity0);
  scsumet_ee_koot0->Fill(eescsumet_koot0);


  scsumet_ee_all_eta15->Fill(eescsumet_all_eta15);
  scsumet_ee_all_eta20->Fill(eescsumet_all_eta20);
  scsumet_ee_all_eta25->Fill(eescsumet_all_eta25);



  scsumet_ee_all_eta15_pu10->Fill(eescsumet_all_eta15_pu10);
  scsumet_ee_all_eta20_pu10->Fill(eescsumet_all_eta20_pu10);
  scsumet_ee_all_eta25_pu10->Fill(eescsumet_all_eta25_pu10);

  scsumet_ee_all_eta15_pu20->Fill(eescsumet_all_eta15_pu20);
  scsumet_ee_all_eta20_pu20->Fill(eescsumet_all_eta20_pu20);
  scsumet_ee_all_eta25_pu20->Fill(eescsumet_all_eta25_pu20);

  scsumet_ee_all_eta15_pu30->Fill(eescsumet_all_eta15_pu30);
  scsumet_ee_all_eta20_pu30->Fill(eescsumet_all_eta20_pu30);
  scsumet_ee_all_eta25_pu30->Fill(eescsumet_all_eta25_pu30);

  scsumet_ee_all_eta15_pueq10->Fill(eescsumet_all_eta15_pueq10);
  scsumet_ee_all_eta20_pueq10->Fill(eescsumet_all_eta20_pueq10);
  scsumet_ee_all_eta25_pueq10->Fill(eescsumet_all_eta25_pueq10);

  scsumet_ee_all_eta15_pueq20->Fill(eescsumet_all_eta15_pueq20);
  scsumet_ee_all_eta20_pueq20->Fill(eescsumet_all_eta20_pueq20);
  scsumet_ee_all_eta25_pueq20->Fill(eescsumet_all_eta25_pueq20);



  scsumet_ee_all_eta15_pueq01->Fill(eescsumet_all_eta15_pueq01);
  scsumet_ee_all_eta20_pueq01->Fill(eescsumet_all_eta20_pueq01);
  scsumet_ee_all_eta25_pueq01->Fill(eescsumet_all_eta25_pueq01);

  scsumet_ee_all_eta15_pueq02->Fill(eescsumet_all_eta15_pueq02);
  scsumet_ee_all_eta20_pueq02->Fill(eescsumet_all_eta20_pueq02);
  scsumet_ee_all_eta25_pueq02->Fill(eescsumet_all_eta25_pueq02);

  scsumet_ee_all_eta15_pueq03->Fill(eescsumet_all_eta15_pueq03);
  scsumet_ee_all_eta20_pueq03->Fill(eescsumet_all_eta20_pueq03);
  scsumet_ee_all_eta25_pueq03->Fill(eescsumet_all_eta25_pueq03);

  scsumet_ee_all_eta15_pueq04->Fill(eescsumet_all_eta15_pueq04);
  scsumet_ee_all_eta20_pueq04->Fill(eescsumet_all_eta20_pueq04);
  scsumet_ee_all_eta25_pueq04->Fill(eescsumet_all_eta25_pueq04);

  scsumet_ee_all_eta15_pueq05->Fill(eescsumet_all_eta15_pueq05);
  scsumet_ee_all_eta20_pueq05->Fill(eescsumet_all_eta20_pueq05);
  scsumet_ee_all_eta25_pueq05->Fill(eescsumet_all_eta25_pueq05);

  scsumet_ee_all_eta15_pueq06->Fill(eescsumet_all_eta15_pueq06);
  scsumet_ee_all_eta20_pueq06->Fill(eescsumet_all_eta20_pueq06);
  scsumet_ee_all_eta25_pueq06->Fill(eescsumet_all_eta25_pueq06);

  scsumet_ee_all_eta15_pueq07->Fill(eescsumet_all_eta15_pueq07);
  scsumet_ee_all_eta20_pueq07->Fill(eescsumet_all_eta20_pueq07);
  scsumet_ee_all_eta25_pueq07->Fill(eescsumet_all_eta25_pueq07);

  scsumet_ee_all_eta15_pueq08->Fill(eescsumet_all_eta15_pueq08);
  scsumet_ee_all_eta20_pueq08->Fill(eescsumet_all_eta20_pueq08);
  scsumet_ee_all_eta25_pueq08->Fill(eescsumet_all_eta25_pueq08);

  scsumet_ee_all_eta15_pueq09->Fill(eescsumet_all_eta15_pueq09);
  scsumet_ee_all_eta20_pueq09->Fill(eescsumet_all_eta20_pueq09);
  scsumet_ee_all_eta25_pueq09->Fill(eescsumet_all_eta25_pueq09);





  scsumet_ee_all_gt2->Fill(eescsumet_all_gt2);
  scsumet_ee_severity0_gt2->Fill(eescsumet_severity0_gt2);
  scsumet_ee_koot0_gt2->Fill(eescsumet_koot0_gt2);

  scsumet_ee_all_gt5->Fill(eescsumet_all_gt5);
  scsumet_ee_severity0_gt5->Fill(eescsumet_severity0_gt5);
  scsumet_ee_koot0_gt5->Fill(eescsumet_koot0_gt5);

  scsumet_ee_all_gt10->Fill(eescsumet_all_gt10);
  scsumet_ee_severity0_gt10->Fill(eescsumet_severity0_gt10);
  scsumet_ee_koot0_gt10->Fill(eescsumet_koot0_gt10);



  EcalRecHit maxebhit;
  EBDetId  maxebid(0);
  EcalRecHit maxeehit;
  EcalRecHit maxebhit2;
  EcalRecHit maxeehit2;


  float ensum1=0;
  float ensum2=0;
  float ensum3=0;
  float ensum4=0;

  float errsum1=0;
  float errsum2=0;

  int numgt1=0;



  rechitsumet_eb_all=0;
  rechitsumet_eb_01=0;
  rechitsumet_eb_05=0;

  rechitsumet_eb_0105=0;
  rechitsumet_eb_0530=0;


  rechitsumet_ee_all=0;
  rechitsumet_ee_01=0;
  rechitsumet_ee_05=0;

  rechitsumet_ee_0105=0;
  rechitsumet_ee_0530=0;



  // EB loop

  //    cout << "starting rechit loop" << endl;

  for (EcalRecHitCollection::const_iterator hitItr = EBhits->begin(); hitItr != EBhits->end(); ++hitItr) {

      EcalRecHit hit = (*hitItr);
      EBDetId det    = hit.id(); 
      //   const DetId det2     = hit.id(); 
      float ampli    = hit.energy();
      float time     = hit.time()-toffset;
      int   ebflag   = hit.recoFlag();

      //      int ebhashedid = det.hashedIndex();
      const EcalIntercalibConstantMap &icalMap=eic->getMap();

      float noiseingev=0;
      float pedg12=0;
      float pedrmsg12=0;
      int i=0;

      float noiseavg=0;
      float noisecount=0;
	
      const std::vector<DetId> neighs =  neighbours(det);
      for (vector<DetId>::const_iterator vec=neighs.begin(); vec!=neighs.end();++vec) {


	EBDetId mydetid=(*vec);
	i++;

      // pedestals

	const EcalPedestals::Item *aped=0;
	int hashedid=mydetid.hashedIndex();
	aped=&eped->barrel(hashedid);
	pedg12=aped->mean_x12;
	pedrmsg12=aped->rms_x12;

	// intercalibration constants

	EcalIntercalibConstantMap::const_iterator icalit=icalMap.find(mydetid);
	float icalconst=0;
	if (icalit!=icalMap.end()) icalconst=(*icalit);


	noiseingev=pedrmsg12*adctogevconst*icalconst;

	//	cout << "hit=" << i << "ieta,iphi=" << mydetid.ieta() 
	//    << " " << mydetid.iphi() << " noise=" << pedrmsg12 
	//   << " adc/gev=" << adctogevconst 
	//   << " ic=" << icalconst << " noise in gev=" << noiseingev << endl;

	if (noiseingev!=0) {
	  noiseavg+=noiseingev;
	  noisecount++;
	}


      }
      noiseavg/=noisecount;

      // cout << "average noise=" << noiseavg << endl;
      float_t chi2   = hit.chi2();
      float_t chi2oot= 0; //hit.outOfTimeChi2();
	

      GlobalPoint poseb=geo->getPosition(hit.detid());


      float eta_eb=poseb.eta();
      float phi_eb=poseb.phi();
      float pf=1.0/cosh(eta_eb);
      float eteb=ampli*pf;
      int ieta=det.ieta();      
      int iphi=det.iphi();


      int iphi_bin=iphi;
      int ieta_bin=ieta+86-int(ieta>0);   

      noise2d->SetBinContent(iphi_bin,ieta_bin,pedrmsg12);

       

      // cout << "ieta=" << ieta << " iphi=" << iphi << " pedmean=" << pedg12 << " pedrms=" << pedrmsg12 << endl;
  

      float eb_r4a=0;
      float s4=0;
      float s4_2=0;

      float s41=0;
      float s42=0;
      float s43=0;
      float s44=0;

      float s45=0;
      float s46=0;
      float s47=0;
      float s48=0;

      float maxen=-1;

      int cursi=0;
      int cursj=0;

      s41 = recHitE( det, *EBhits,  1,  0 );

      if (s41>maxen) {
	maxen=s41;
	cursi=1;
	cursj=0;
      }

      s42 = recHitE( det, *EBhits, -1,  0 );

      if (s42>maxen) {
	maxen=s42;
	cursi=-1;
	cursj=0;
      }

      s43 = recHitE( det, *EBhits,  0,  1 );

      if (s43>maxen) {
	maxen=s43;
	cursi=0;
	cursj=1;
      }

      s44 = recHitE( det, *EBhits,  0, -1 );
      
      if (s44>maxen) {
	maxen=s44;
	cursi=0;
	cursj=-1;
      }



      s45 = recHitE( det, *EBhits,  1+cursi,  0+cursj );
      s46 = recHitE( det, *EBhits, -1+cursi,  0+cursj );
      s47 = recHitE( det, *EBhits,  0+cursi,  1+cursj );
      s48 = recHitE( det, *EBhits,  0+cursi, -1+cursj );



      s4=s41+s42+s43+s44;
      s4_2=s45+s46+s47+s48;


      float s6=s4+s4_2-ampli-maxen;


      if (ampli>0) eb_r4a=1.0-s4/ampli;

      // float eb_r4a_v2=1.0-e4e1(det, *EBhits);
      float eb_r4a_v3=1.0-e4e1v2(det, *EBhits);


      float e4overnoise=s4/noiseavg;


      float e6overe2=0; 

      if (ampli>0) e6overe2=1.0-s6/(ampli+maxen);



      Bool_t is_swiss=false;

      if (ampli>4.0 && eb_r4a>0.95) is_swiss=true;


      Bool_t is_noisedep=false;
      if (ampli>4.0 && eb_r4a>1-(6.0*noiseavg)/ampli) is_noisedep=true;





      //      float eb_r4a=1.0-e4e1(det,*EBhits);


      //    float e6overe2=1.0-e6e2(det,*EBhits);


  
        


      // calculate e2/e9, e2/e25 for all hits with E>5.0

      //	Float_t emax=-999;
      //Int_t n1max=-111;
      //Int_t n2max=-111;

	if (pTopology.isValid() && pG.isValid() && eteb>3.0) { 


	  if (eb_r4a>0.95) r4count++;
	  
	  //  const CaloTopology *topology=pTopology.product();
	  
	  //  const reco::BasicCluster *cluster=0;
	  /*
	    float e3x3tmp=EcalClusterTools::matrixEnergy(*cluster,ebRecHits,topology,hit.id(),-1,1,-1,1);
	    
	    float e5x5tmp=EcalClusterTools::matrixEnergy(*cluster,ebRecHits,topology,hit.id(),-2,2,-2,2);
	  */
	  
	  
	  
	  
	}





	//	Int_t ee_kGood=0;
// 	Int_t ee_kPoorReco=0;
 	Int_t ee_kOutOfTime=0;
// 	Int_t ee_kFaultyHardware=0;
// 	Int_t ee_kNoisy=0;
// 	Int_t ee_kPoorCalib=0;
// 	Int_t ee_kSaturated=0;
// 	Int_t ee_kLeadingEdgeRecovered=0;
// 	Int_t ee_kNeighboursRecovered=0;
// 	Int_t ee_kTowerRecovered=0;
// 	Int_t ee_kDead=0;
// 	Int_t ee_kKilled=0;
// 	Int_t ee_kTPSaturated=0;
// 	Int_t ee_kL1SpikeFlag=0;
	Int_t ee_kWeird=0;
	Int_t ee_kDiWeird=0;
	//	Int_t ee_kUnknown=0;
	
	
	
	//	if (hit.checkFlag(EcalRecHit::kGood)) ee_kGood=1;
// 	if (hit.checkFlag(EcalRecHit::kPoorReco)) ee_kPoorReco=1;
 	if (hit.checkFlag(EcalRecHit::kOutOfTime)) ee_kOutOfTime=1;
// 	if (hit.checkFlag(EcalRecHit::kFaultyHardware)) ee_kFaultyHardware=1;
// 	if (hit.checkFlag(EcalRecHit::kNoisy)) ee_kNoisy=1;
// 	if (hit.checkFlag(EcalRecHit::kPoorCalib)) ee_kPoorCalib=1;
// 	if (hit.checkFlag(EcalRecHit::kSaturated)) ee_kSaturated=1;
// 	if (hit.checkFlag(EcalRecHit::kLeadingEdgeRecovered)) ee_kLeadingEdgeRecovered=1;
// 	if (hit.checkFlag(EcalRecHit::kNeighboursRecovered)) ee_kNeighboursRecovered=1;
// 	if (hit.checkFlag(EcalRecHit::kTowerRecovered)) ee_kTowerRecovered=1;
// 	if (hit.checkFlag(EcalRecHit::kDead)) ee_kDead=1;
// 	if (hit.checkFlag(EcalRecHit::kKilled)) ee_kKilled=1;
// 	if (hit.checkFlag(EcalRecHit::kTPSaturated)) ee_kTPSaturated=1;
// 	if (hit.checkFlag(EcalRecHit::kL1SpikeFlag)) ee_kL1SpikeFlag=1;
	if (hit.checkFlag(EcalRecHit::kWeird)) ee_kWeird=1;
	if (hit.checkFlag(EcalRecHit::kDiWeird)) ee_kDiWeird=1;
	//	if (hit.checkFlag(EcalRecHit::kUnknown)) ee_kUnknown=1;
	



	//	int nearcrack=EBDetId::isNextToBoundary(det);

	//      if (ampli>4.0 && eb_r4a>0.99 && ee_kWeird==0) {
	//	cout << "ieta,iphi= " << ieta << "," << iphi << " ampli=" << ampli << " timing=" << time
	//     << " s4=" << s4 << " s1,2,3,4=" << s41 << "," << s42 << "," << s43 << "," << s44 
	//    << " \nswisscross1=" << eb_r4a << " swisscross2=" << eb_r4a_v2 << " swisscross3=" << eb_r4a_v3 << " kweird=" << ee_kWeird << " crack= " << nearcrack << endl;
	//  }


	// check for hits close to boundaries
	
	//if (abs(ieta)==85) ee_kGood=0;
	//if (EcalTools::isNextToDeadFromNeighbours(det,*cstat,12)) ee_kGood=0;
	

	// mask out noisy channels in run D

	//	 ee_kWeird=1;


	// mask out bad channels


	if ((ieta==11 && iphi==68) || (ieta==68 && iphi==74) || (ieta==-83 && iphi==189) || (ieta==-75 && iphi==199) || (ieta==-81 && iphi==316)) continue;


	  Int_t ebdigiadc=0;
	  Int_t ebdigigain=0;
	  char mytxteb[80];
	  


	Float_t ieta_tmp=float(ieta)+0.5;
	if (ieta>0) ieta_tmp=ieta_tmp-1;
	Float_t iphi_tmp=float(iphi)-0.5;
	

	  EBDigiCollection::const_iterator digiItrEBa= EBdigis->begin();
	  while(digiItrEBa != EBdigis->end() && digiItrEBa->id() != hitItr->id())
	    {
	      digiItrEBa++;
	    }
	  
	  if (digiItrEBa != EBdigis->end() && eteb>2.0 && fabs(time)<5) {


	    //   cout << "digi: ieta,iphi=" << ieta << " " << iphi << endl;	  
	    
	    EBDataFrame df(*digiItrEBa);
	    for(int i=0; i<10;++i)
	      {
		ebdigiadc=df.sample(i).adc();
		//	ebdigiadc+=pedoff;
		ebdigigain=df.sample(i).gainId();
		sprintf(mytxteb,"  %02d    %04d     %d",i+1,ebdigiadc,ebdigigain);
		//	cout << mytxteb << endl;
		

		if (i<3) {
		  etaphi_ped->Fill(iphi_tmp,ieta_tmp,ebdigiadc);
		  etaphi_pedsq->Fill(iphi_tmp,ieta_tmp,ebdigiadc*ebdigiadc);
		  etaphi_pedn->Fill(iphi_tmp,ieta_tmp,1.0);
		}

	      }

	  }

 


	Bool_t isspike=false;


	if (isMC_) {

	  EBDigiCollection::const_iterator digiItrEB= EBdigis2->begin();	  

	  while(digiItrEB != EBdigis2->end() && digiItrEB->id() != hitItr->id())
	    {
	      digiItrEB++;
	    }
	  
	  if (digiItrEB != EBdigis2->end()) {
	    
	    
	    
	    EBDataFrame df(*digiItrEB);
	    for(int i=0; i<10;++i)
	      {
		ebdigiadc=df.sample(i).adc();
		ebdigigain=df.sample(i).gainId();
	
		
		if (ebdigiadc>250 || ebdigigain!=1) {
		  isspike=true;
		}
				
		
	      }
	    
	    
	  }
	  
	}
	


	Bool_t matchsc=false;

	//	float scenergy=0;
	//	float sc_eta=0;
	//	float sc_phi=0;
	//	float scet=0;

	//cout << "before sc loop" << endl;


	for (reco::SuperClusterCollection::const_iterator sCEB = sc_eb->begin(); sCEB != sc_eb->end(); ++sCEB) {

	  DetId seedid=sCEB->seed()->seed();
  
	  if (seedid==det) {
	    matchsc=true;
	    //  scenergy=sCEB->energy();
	    // sc_eta=sCEB->eta();
	    // sc_phi=sCEB->phi();
	    // scet=scenergy/cosh(sc_eta);
	  }


	}


	// supercluster
	
	//cout << "before sc" << endl;


	if (matchsc) {

	  // cout << "in sc" << endl;

	  eb_rechitenergy_sctag->Fill(ampli);
	  swisscross_vs_energy_sctag->Fill(eb_r4a,ampli);
	  time_vs_energy_sctag->Fill(time,ampli);


	  //cout << "BP1" << endl;


	  if (ee_kWeird) {

	    eb_rechitenergy_sctag_kweird->Fill(ampli);
	    swisscross_vs_energy_sctag_kweird->Fill(eb_r4a,ampli);;
	    time_vs_energy_sctag_kweird->Fill(time,ampli);
		
	  }

	  //cout << "BP2" << endl;


	  if (is_swiss) {

	    eb_rechitenergy_sctag_swisscross->Fill(ampli);
	    time_vs_energy_sctag_swisscross->Fill(time,ampli);


	  }

	  //cout << "BP3" << endl;

	  if (is_noisedep) {

	    eb_rechitenergy_sctag_noisedep->Fill(ampli);

	  }


	  //cout << "BP4" << endl;

	  if (ampli>3.0) {
	    eb_timing_vs_r4_sctag_3000->Fill(eb_r4a,time);
	  }

	  if (ampli>5.0) {
	    eb_timing_vs_r4_sctag_5000->Fill(eb_r4a,time);
	  }

	  if (ampli>10.0) {
	    eb_timing_vs_r4_sctag_10000->Fill(eb_r4a,time);
	  }


	  if (ampli>4.0) {

	    // cout << "BP4a" << endl;

	    e4_over_noise_sctag->Fill(e4overnoise);

	    eb_timing_4000_sctag->Fill(time);
	    eb_r4_4000_sctag->Fill(eb_r4a);
	    eb_r6_4000_sctag->Fill(e6overe2);

	    sc_vs_ieta_sctag_4000->Fill(abs(ieta)-0.5);

	    swisscross_vs_ieta_sctag_4000->Fill(eb_r4a,abs(ieta)-0.5);


	    if (ee_kWeird) {

	      // cout << "BP4b" << endl;
	      eb_timing_4000_sctag_kweird->Fill(time);
	      sc_vs_ieta_sctag_4000_kweird->Fill(abs(ieta)-0.5);
	      swisscross_vs_ieta_sctag_4000_kweird->Fill(eb_r4a,abs(ieta)-0.5);
	    }


	    if (is_swiss) {
	      //  cout << "BP4c" << endl;
	      eb_timing_4000_sctag_swisscross->Fill(time);
	      sc_vs_ieta_sctag_4000_swisscross->Fill(abs(ieta)-0.5);
	    }



	    if (is_noisedep) {

	      //  cout << "BP4d" << endl;
	      eb_timing_4000_sctag_noisedep->Fill(time);
	      sc_vs_ieta_sctag_4000_noisedep->Fill(abs(ieta)-0.5);
	    }

	  }

	  //cout << "BP5" << endl;

	  if (ampli>10.0) {
	    // cout << "BP5a" << endl;
	  
	    eb_timing_10000_sctag->Fill(time);
	    eb_r4_10000_sctag->Fill(eb_r4a);
	    eb_r6_10000_sctag->Fill(e6overe2);

	    sc_vs_ieta_sctag_10000->Fill(abs(ieta)-0.5);

	    swisscross_vs_ieta_sctag_10000->Fill(eb_r4a,abs(ieta)-0.5);

	    if (ee_kWeird) {
	      // cout << "BP5b" << endl;

	      eb_timing_10000_sctag_kweird->Fill(time);
	      sc_vs_ieta_sctag_10000_kweird->Fill(abs(ieta)-0.5);
	      swisscross_vs_ieta_sctag_10000_kweird->Fill(eb_r4a,abs(ieta)-0.5);
	    }


	    if (is_swiss) {
	      // cout << "BP5c" << endl;

	      eb_timing_10000_sctag_swisscross->Fill(time);
	      sc_vs_ieta_sctag_10000_swisscross->Fill(abs(ieta)-0.5);
	    }

	    if (is_noisedep) {
	      //  cout << "BP5d" << endl;
	      eb_timing_10000_sctag_noisedep->Fill(time);
	      sc_vs_ieta_sctag_10000_noisedep->Fill(abs(ieta)-0.5);
	    }

	  }


	}

	//	cout << "BP6" << endl;






	// spike truth - from MC

	if (isspike) {

	  eb_rechitenergy_spiketag->Fill(ampli);
	  
	  swisscross_vs_energy_spiketag->Fill(eb_r4a_v3,ampli);

	  time_vs_energy_spiketag->Fill(time,ampli);



	  if (ee_kWeird) {

	    eb_rechitenergy_spiketag_kweird->Fill(ampli);
	    swisscross_vs_energy_spiketag_kweird->Fill(eb_r4a_v3,ampli);;
	    time_vs_energy_spiketag_kweird->Fill(time,ampli);
		
	  }



	  if (is_swiss) {

	    eb_rechitenergy_spiketag_swisscross->Fill(ampli);
	    time_vs_energy_spiketag_swisscross->Fill(time,ampli);


	  }

	  if (is_noisedep) {

	    eb_rechitenergy_spiketag_noisedep->Fill(ampli);

	  }



	  if (ampli>3.0) {

	    eb_timing_3000_spiketag->Fill(time);
	    eb_r4_3000_spiketag->Fill(eb_r4a);
	    eb_r6_3000_spiketag->Fill(e6overe2);

	  }


	  if (ampli>4.0) {

	    e4_over_noise_spiketag->Fill(e4overnoise);

	    eb_timing_4000_spiketag->Fill(time);
	    eb_r4_4000_spiketag->Fill(eb_r4a);
	    eb_r6_4000_spiketag->Fill(e6overe2);

	    spikes_vs_ieta_spiketag_4000->Fill(abs(ieta)-0.5);

	    swisscross_vs_ieta_spiketag_4000->Fill(eb_r4a,abs(ieta)-0.5);


	    if (ee_kWeird) {
	      eb_timing_4000_spiketag_kweird->Fill(time);
	      spikes_vs_ieta_spiketag_4000_kweird->Fill(abs(ieta)-0.5);
	      swisscross_vs_ieta_spiketag_4000_kweird->Fill(eb_r4a,abs(ieta)-0.5);
	    }


	    if (is_swiss) {
	      eb_timing_4000_spiketag_swisscross->Fill(time);
	      spikes_vs_ieta_spiketag_4000_swisscross->Fill(abs(ieta)-0.5);
	    }



	    if (is_noisedep) {
	      eb_timing_4000_spiketag_noisedep->Fill(time);
	      spikes_vs_ieta_spiketag_4000_noisedep->Fill(abs(ieta)-0.5);
	    }

	  }

	  if (ampli>10.0) {

	    eb_timing_10000_spiketag->Fill(time);
	    eb_r4_10000_spiketag->Fill(eb_r4a);
	    eb_r6_10000_spiketag->Fill(e6overe2);

	    spikes_vs_ieta_spiketag_10000->Fill(abs(ieta)-0.5);

	    swisscross_vs_ieta_spiketag_10000->Fill(eb_r4a,abs(ieta)-0.5);

	    if (ee_kWeird) {
	      eb_timing_10000_spiketag_kweird->Fill(time);
	      spikes_vs_ieta_spiketag_10000_kweird->Fill(abs(ieta)-0.5);
	      swisscross_vs_ieta_spiketag_10000_kweird->Fill(eb_r4a,abs(ieta)-0.5);
	    }


	    if (is_swiss) {
	      eb_timing_10000_spiketag_swisscross->Fill(time);
	      spikes_vs_ieta_spiketag_10000_swisscross->Fill(abs(ieta)-0.5);
	    }

	    if (is_noisedep) {
	      eb_timing_10000_spiketag_noisedep->Fill(time);
	      spikes_vs_ieta_spiketag_10000_noisedep->Fill(abs(ieta)-0.5);
	    }

	  }


	}



	if (ee_kWeird==0) eb_rechitenergy_knotweird->Fill(ampli);



	Bool_t spikebytime=false;

	if (ampli>4.0 && abs(time)>3.0) spikebytime=true;

	if (spikebytime) {

	  pu_vs_ene_spiketime->Fill(ampli,numgoodvtx);
	  if (ee_kWeird) pu_vs_ene_spikekweird->Fill(ampli,numgoodvtx);
	  if (is_swiss) pu_vs_ene_swisscross->Fill(ampli,numgoodvtx);


	  if (ampli>4.0) {

	    time_4gev_spiketime->Fill(time);
            pu_4gev_spiketime->Fill(numgoodvtx);
	    ene_4gev_spiketime->Fill(ampli);

	    if (ee_kWeird) {
	      time_4gev_spikekweird->Fill(time);
	      pu_4gev_spikekweird->Fill(numgoodvtx);
	      ene_4gev_spikekweird->Fill(ampli);
	    }

	    if (is_swiss) {
	      time_4gev_swisscross->Fill(time);
	      pu_4gev_swisscross->Fill(numgoodvtx);
	      ene_4gev_swisscross->Fill(ampli);
	    }


	  }


	  if (ampli>10.0) {
	    time_10gev_spiketime->Fill(time);
            pu_10gev_spiketime->Fill(numgoodvtx);
	    ene_10gev_spiketime->Fill(ampli);

	    if (ee_kWeird) {
	      time_10gev_spikekweird->Fill(time);
	      pu_10gev_spikekweird->Fill(numgoodvtx);
	      ene_10gev_spikekweird->Fill(ampli);
	    }

	    if (is_swiss) {
	      time_10gev_swisscross->Fill(time);
	      pu_10gev_swisscross->Fill(numgoodvtx);
	      ene_10gev_swisscross->Fill(ampli);
	    }


	  }




	}



	if (ee_kWeird==1) {


	  numspikes_kweird++;


	  eb_rechitenergy_kweird->Fill(ampli);

	  if (ampli>4.0) {

	    eb_timing_4000_kweird->Fill(time);
	    eb_r4_4000_kweird->Fill(eb_r4a);
	    eb_r6_4000_kweird->Fill(e6overe2);

	  }

	  if (ampli>10.0) {

	    eb_timing_10000_kweird->Fill(time);
	    eb_r4_10000_kweird->Fill(eb_r4a);
	    eb_r6_10000_kweird->Fill(e6overe2);

	  }


	}



	if (is_swiss) {


	  numspikes_swisscross++;


	  eb_rechitenergy_swisscross->Fill(ampli);

	  if (ampli>4.0) {

	    eb_timing_4000_swisscross->Fill(time);
	    eb_r4_4000_swisscross->Fill(eb_r4a);
	    eb_r6_4000_swisscross->Fill(e6overe2);

	  }

	  if (ampli>10.0) {

	    eb_timing_10000_swisscross->Fill(time);
	    eb_r4_10000_swisscross->Fill(eb_r4a);
	    eb_r6_10000_swisscross->Fill(e6overe2);

	  }


	}

	
	
	if (eteb>0.1) ebtime_vs_bxtrain_01->Fill(bunchintrain+0.5,time);
	if (eteb>0.5) ebtime_vs_bxtrain_05->Fill(bunchintrain+0.5,time);

	  
	if (ampli>1.0) { 
	  ebsum_gt1+=ampli;
	  ebhits1GeV++;
	}
	
	if (ampli>2.0) {
	  ebsum_gt2+=ampli;
	  ebhits2GeV++;
	}
	
	if (ampli>4.0) {
	  ebsum_gt4+=ampli;
	  ebhits4GeV++;
	}
	
	
	if (eteb>1.0) {
	  ebsum_gt1et+=eteb;
	  ebhits1GeVet++;
	}
	
	
	if (eteb>2.0) {
	  ebsum_gt2et+=eteb;
	  ebhits2GeVet++;
	}
	
	if (eteb>4.0) {
	  ebsum_gt4et+=eteb;
	  ebhits4GeVet++;
	}
	
	
	// rechit et sums
	
	rechitsumet_eb_all+=eteb;
	if (eteb>0.1) rechitsumet_eb_01+=eteb;
	if (eteb>0.5) rechitsumet_eb_05+=eteb;
	
	
	if (eteb>0.1 && eteb<=0.5) rechitsumet_eb_0105+=eteb;
	if (eteb>0.5 && eteb<=3.0) rechitsumet_eb_0530+=eteb;



	if (eteb>0.1) ebnumrechits_01++;
	if (eteb>0.5) ebnumrechits_05++;


	if (eteb>0.1 && eteb<=0.5) ebnumrechits_0105++;
	if (eteb>0.5 && eteb<=3.0) ebnumrechits_0530++;

 	if (eteb>0.1) rechiteta_vs_bxtrain_01->Fill(bunchintrain,eta_eb);
 	if (eteb>0.5) rechiteta_vs_bxtrain_05->Fill(bunchintrain,eta_eb);


	// do digi step

	//	if (ampli>4.0) {


	  float pedoff=0;

	  if (isMC_) pedoff=0;
	  if (!isMC_) pedoff=200-pedg12;

	
// 	  Int_t ebdigiadc=0;
// 	  Int_t ebdigigain=0;
// 	  char mytxteb[80];
	  




	  
	  EBDigiCollection::const_iterator digiItrEB= EBdigis->begin();
	  while(digiItrEB != EBdigis->end() && digiItrEB->id() != hitItr->id())
	    {
	      digiItrEB++;
	    }
	  
	  if (digiItrEB != EBdigis->end() && eteb>0.5) {


	    //   cout << "digi: ieta,iphi=" << ieta << " " << iphi << endl;	  
	    
	    EBDataFrame df(*digiItrEB);
	    for(int i=0; i<10;++i)
	      {
		ebdigiadc=df.sample(i).adc();
		ebdigiadc+=pedoff;
		ebdigigain=df.sample(i).gainId();
		sprintf(mytxteb,"  %02d    %04d     %d",i+1,ebdigiadc,ebdigigain);
		//		cout << mytxteb << endl;
		

		if (i<3) {
		  //		  etaphi_ped->Fill(iphi_tmp,ieta_tmp,ebdigiadc);
		  //  etaphi_pedsq->Fill(iphi_tmp,ieta_tmp,ebdigiadc*ebdigiadc);
		  // etaphi_pedn->Fill(iphi_tmp,ieta_tmp,1.0);
		}

		eb_digi_01->Fill(i+0.5,ebdigiadc);
		
		if (eteb>0.5) eb_digi_05->Fill(i+0.5,ebdigiadc);


		if (eteb>0.1 && eteb<=0.5) { 
		  eb_digi_0105->Fill(i+0.5,float(ebdigiadc));
		  eb_digi_0105_vs_time->Fill(time,i+0.5,float(ebdigiadc));
		  eb_digi_0105_vs_bxtrain->Fill(bunchintrain+0.5,i+0.5,float(ebdigiadc));

		  eb_digi_0105_vs_time_norm->Fill(time,i+0.5,1.0);
		  eb_digi_0105_vs_bxtrain_norm->Fill(bunchintrain+0.5,i+0.5,1.0);


		  if (abs(eb_eta)<0.5) {

		    eb_digi_0105_vs_time_eta15->Fill(time,i+0.5,float(ebdigiadc));
		    eb_digi_0105_vs_bxtrain_eta15->Fill(bunchintrain+0.5,i+0.5,float(ebdigiadc));
		    
		    eb_digi_0105_vs_time_norm_eta15->Fill(time,i+0.5,1.0);
		    eb_digi_0105_vs_bxtrain_norm_eta15->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }


		  if (abs(eb_eta)>=0.5 && abs(eb_eta)<1.0) {

		    eb_digi_0105_vs_time_eta20->Fill(time,i+0.5,float(ebdigiadc));
		    eb_digi_0105_vs_bxtrain_eta20->Fill(bunchintrain+0.5,i+0.5,float(ebdigiadc));
		    
		    eb_digi_0105_vs_time_norm_eta20->Fill(time,i+0.5,1.0);
		    eb_digi_0105_vs_bxtrain_norm_eta20->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }


		  if (abs(eb_eta)>=1.0) {

		    eb_digi_0105_vs_time_eta25->Fill(time,i+0.5,float(ebdigiadc));
		    eb_digi_0105_vs_bxtrain_eta25->Fill(bunchintrain+0.5,i+0.5,float(ebdigiadc));
		    
		    eb_digi_0105_vs_time_norm_eta25->Fill(time,i+0.5,1.0);
		    eb_digi_0105_vs_bxtrain_norm_eta25->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }



		}


		if (eteb>0.5 && eteb<=3.0) {

		  eb_digi_0530->Fill(i+0.5,ebdigiadc);
		  eb_digi_0530_vs_time->Fill(time,i+0.5,float(ebdigiadc));
		  eb_digi_0530_vs_bxtrain->Fill(bunchintrain+0.5,i+0.5,float(ebdigiadc));

		  eb_digi_0530_vs_time_norm->Fill(time,i+0.5,1.0);
		  eb_digi_0530_vs_bxtrain_norm->Fill(bunchintrain+0.5,i+0.5,1.0);


		  if (abs(eb_eta)<0.5) {

		    eb_digi_0530_vs_time_eta15->Fill(time,i+0.5,float(ebdigiadc));
		    eb_digi_0530_vs_bxtrain_eta15->Fill(bunchintrain+0.5,i+0.5,float(ebdigiadc));
		    
		    eb_digi_0530_vs_time_norm_eta15->Fill(time,i+0.5,1.0);
		    eb_digi_0530_vs_bxtrain_norm_eta15->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }


		  if (abs(eb_eta)>=0.5 && abs(eb_eta)<1.0) {

		    eb_digi_0530_vs_time_eta20->Fill(time,i+0.5,float(ebdigiadc));
		    eb_digi_0530_vs_bxtrain_eta20->Fill(bunchintrain+0.5,i+0.5,float(ebdigiadc));
		    
		    eb_digi_0530_vs_time_norm_eta20->Fill(time,i+0.5,1.0);
		    eb_digi_0530_vs_bxtrain_norm_eta20->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }


		  if (abs(eb_eta)>=1.0) {

		    eb_digi_0530_vs_time_eta25->Fill(time,i+0.5,float(ebdigiadc));
		    eb_digi_0530_vs_bxtrain_eta25->Fill(bunchintrain+0.5,i+0.5,float(ebdigiadc));
		    
		    eb_digi_0530_vs_time_norm_eta25->Fill(time,i+0.5,1.0);
		    eb_digi_0530_vs_bxtrain_norm_eta25->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }




	
		}


	      }
	    
	  }

	  //}
	
      eb_timing_0->Fill(time);
        eb_r4_0->Fill(eb_r4a);
        eb_timing_vs_r4_0->Fill(eb_r4a,time);
        if (eb_r4a<0.95) eb_timing_r4_0->Fill(time);

    

        if (ampli>0.2) {
           eb_timing_200->Fill(time);
           eb_r4_200->Fill(eb_r4a);
           eb_timing_vs_r4_200->Fill(eb_r4a,time);
           if (eb_r4a<0.95) eb_timing_r4_200->Fill(time);

        }


        if (ampli>0.4) {
           eb_timing_400->Fill(time);
           eb_r4_400->Fill(eb_r4a);
           eb_timing_vs_r4_400->Fill(eb_r4a,time);
           if (eb_r4a<0.95) eb_timing_r4_400->Fill(time);

        }


        if (ampli>0.6) {
           eb_timing_600->Fill(time);
           eb_r4_600->Fill(eb_r4a);
           eb_timing_vs_r4_600->Fill(eb_r4a,time);
           if (eb_r4a<0.95) eb_timing_r4_600->Fill(time);
        }


        if (ampli>0.8) {
           eb_timing_800->Fill(time);
           eb_r4_800->Fill(eb_r4a);
           eb_timing_vs_r4_800->Fill(eb_r4a,time);
           if (eb_r4a<0.95) eb_timing_r4_800->Fill(time);
        }

        if (ampli>1.0) {

           eb_timing_1000->Fill(time);
           eb_r4_1000->Fill(eb_r4a);
           eb_r6_1000->Fill(e6overe2);
           eb_timing_vs_r4_1000->Fill(eb_r4a,time);
           if (eb_r4a<0.95) eb_timing_r4_1000->Fill(time);

           numgt1++;
	}  

	if (ampli>2.0) {
	  eb_timing_2000->Fill(time);
	  eb_r4_2000->Fill(eb_r4a);
	  eb_r6_2000->Fill(e6overe2);
	  eb_timing_vs_r4_2000->Fill(eb_r4a,time);
	  if (eb_r4a<0.95) eb_timing_r4_2000->Fill(time);
        }
	
	
	if (ampli>3.0) {
	  eb_timing_3000->Fill(time);
	  eb_r4_3000->Fill(eb_r4a);
 	  eb_r6_3000->Fill(e6overe2);
	  eb_timing_vs_r4_3000->Fill(eb_r4a,time);

	  eb_r4vsr6_3000->Fill(eb_r4a,e6overe2);

	  if (eb_r4a<0.95) eb_timing_r4_3000->Fill(time);
        }

	if (ampli>4.0) {
	  eb_timing_4000->Fill(time);
	  eb_r4_4000->Fill(eb_r4a);
	  eb_r6_4000->Fill(e6overe2);
	  eb_r4vsr6_4000->Fill(eb_r4a,e6overe2);
	  e4_over_noise->Fill(e4overnoise);

	}
	
	
	if (ampli>5.0) {
	  eb_timing_5000->Fill(time);
	  eb_r4_5000->Fill(eb_r4a);
	  eb_r6_5000->Fill(e6overe2);
	  eb_r4vsr6_5000->Fill(eb_r4a,e6overe2);
	  eb_timing_vs_r4_5000->Fill(eb_r4a,time);
	  if (eb_r4a<0.95) eb_timing_r4_5000->Fill(time);
        }


	if (ampli>10.0) {
	  eb_timing_10000->Fill(time);
	  eb_r4_10000->Fill(eb_r4a);
	  eb_r6_10000->Fill(e6overe2);
	  eb_r4vsr6_10000->Fill(eb_r4a,e6overe2);
	  eb_timing_vs_r4_10000->Fill(eb_r4a,time);
	}
	


       eb_rechitenergy_->Fill(ampli);



	rechiteta_all->Fill(eta_eb);
	rechiteta_etweight->Fill(eta_eb,eteb);

	if (eteb>1.0) {
	  rechiteta_gt1et->Fill(eta_eb);
	  rechiteta_etweight_gt1et->Fill(eta_eb,eteb);

	  if (numgoodvtx<10) 	  rechiteta_gt1et_pu10->Fill(eta_eb);
	  if (numgoodvtx>=10 && numgoodvtx<20) 	  rechiteta_gt1et_pu20->Fill(eta_eb);
	  if (numgoodvtx>20) 	  rechiteta_gt1et_pu30->Fill(eta_eb);

	}


        if (fabs(eta_eb)<0.2) eb_rechitenergy_02->Fill(ampli);
	if (fabs(eta_eb)>=0.2 && fabs(eta_eb)<0.4) eb_rechitenergy_04->Fill(ampli);
	if (fabs(eta_eb)>=0.4 && fabs(eta_eb)<0.6) eb_rechitenergy_06->Fill(ampli);
	if (fabs(eta_eb)>=0.6 && fabs(eta_eb)<0.8) eb_rechitenergy_08->Fill(ampli);
	if (fabs(eta_eb)>=0.7 && fabs(eta_eb)<1.0) eb_rechitenergy_10->Fill(ampli);
	if (fabs(eta_eb)>=1.0 && fabs(eta_eb)<1.2) eb_rechitenergy_12->Fill(ampli);
	if (fabs(eta_eb)>=1.2 && fabs(eta_eb)<1.4) eb_rechitenergy_14->Fill(ampli);
	if (fabs(eta_eb)>=1.4) eb_rechitenergy_148->Fill(ampli);

      

      
        eb_rechitet_->Fill(eteb);


        if (fabs(eta_eb)<0.2) eb_rechitet_02->Fill(eteb);
	if (fabs(eta_eb)>=0.2 && fabs(eta_eb)<0.4) eb_rechitet_04->Fill(eteb);
	if (fabs(eta_eb)>=0.4 && fabs(eta_eb)<0.6) eb_rechitet_06->Fill(eteb);
	if (fabs(eta_eb)>=0.6 && fabs(eta_eb)<0.8) eb_rechitet_08->Fill(eteb);
	if (fabs(eta_eb)>=0.7 && fabs(eta_eb)<1.0) eb_rechitet_10->Fill(eteb);
	if (fabs(eta_eb)>=1.0 && fabs(eta_eb)<1.2) eb_rechitet_12->Fill(eteb);
	if (fabs(eta_eb)>=1.2 && fabs(eta_eb)<1.4) eb_rechitet_14->Fill(eteb);
	if (fabs(eta_eb)>=1.4) eb_rechitet_148->Fill(eteb);

 


        if (fabs(eta_eb)<0.5) eb_rechitetvspu_05->Fill(float(numgoodvtx)-0.5,eteb);
	if (fabs(eta_eb)>=0.5 && fabs(eta_eb)<1.0) eb_rechitetvspu_10->Fill(float(numgoodvtx)-0.5,eteb);
	if (fabs(eta_eb)>=1.0 && fabs(eta_eb)<1.5) eb_rechitetvspu_15->Fill(float(numgoodvtx)-0.5,eteb);
       



        ebocc->Fill(iphi_tmp,ieta_tmp,1.);
        eboccen->Fill(iphi_tmp,ieta_tmp,ampli);
        eboccet->Fill(iphi_tmp,ieta_tmp,eteb);

	// fill 2d arrays

	eventenergy->Fill(iphi_tmp,ieta_tmp,ampli);
	eventet->Fill(iphi_tmp,ieta_tmp,eteb);
	eventtime->Fill(iphi_tmp,ieta_tmp,time);

	if (ee_kOutOfTime) eventkoot->Fill(iphi_tmp,ieta_tmp,1.0);
	if (ee_kDiWeird) eventkdiweird->Fill(iphi_tmp,ieta_tmp,1.0);


	
        eb_rechiten_vs_eta->Fill(eta_eb,ampli);
        eb_rechitet_vs_eta->Fill(eta_eb,eteb);
	

        if (eteb>1.0) {

           eboccgt1et->Fill(iphi_tmp,ieta_tmp,1.);
           eboccetgt1et->Fill(iphi_tmp,ieta_tmp,eteb);

        }

	if (ampli>1.0) {

           eboccgt1->Fill(iphi_tmp,ieta_tmp,1.);
           eboccengt1->Fill(iphi_tmp,ieta_tmp,ampli);

        }


	if (ampli>3.0) {

           eboccgt3->Fill(iphi_tmp,ieta_tmp,1.);

        }

	if (ampli>5.0) {

           eboccgt5->Fill(iphi_tmp,ieta_tmp,1.);

        }





	//}
  
	  
	  
	  
	  
      //       if (goodevent) {
   
  

        if (eteb>3.00) {

	  float sigmat=28.51/(ampli/0.04)+sqrt(2)*0.2565;


           ensum1+=time*ampli;
           ensum2+=ampli;
	   ensum3+=pow(ampli*sigmat,2);
	   ensum4+=pow(ampli,2);

           errsum1+=time/pow(sigmat,2);
           errsum2+=1.0/pow(sigmat,2);




	   
        }
	


	
	//}
	
	

	
	//     if (ampli>ebmax && ((ampli<3) || (ampli>3 && abs(time)<4 && eb_r9t<0.90))) {

	if (ampli>ebmax) {
	  ebmax=ampli;
	  ebmaxet=eteb;
	  eb_ieta=ieta;      
	  eb_iphi=iphi;      
	  eb_eta=eta_eb;
	  eb_phi=phi_eb;
	  ebtime=time;
	  ebflags=ebflag;
	  ebchi2=chi2;
	  ebchi2oot=chi2oot;
	  maxebhit=*hitItr;
	  maxebid=det;
	  
	  //       swisscross_algo=swisscross;
	  //  e2e9_algo=e2overe9tmp;
	  
	}
	
	
	
	



	
	//	if (!(ee_kWeird || ee_kDiWeird)) {
	
	

 

	

      ebhits++;
      if (ampli>1.0) ebhits1GeV++;
      //	}
	
  }


  // end of eb loop


  // sfgvb loop - 1: spikes



  if (isMC_) {


    //    int dframe[10];


    for (EBDigiCollection::const_iterator digiItrEB= EBdigis2->begin(); digiItrEB != EBdigis2->end(); ++digiItrEB) {	  
   
            
     
       int adcmax=0;
       int gainmax=0;
       // int sampmax=0;
     
       EBDataFrame df(*digiItrEB);
       for(int i=0; i<10;++i)
	 {
	   int ebdigiadc=df.sample(i).adc();
	   int ebdigigain=df.sample(i).gainId();

	   //	   dframe[i]=ebdigiadc;

	   if (ebdigiadc>adcmax) {
	     adcmax=ebdigiadc;
	     gainmax=ebdigigain;
	     //     sampmax=i+1;
	   }
	   
	 }
       
       
       if (adcmax>250 || gainmax>1 ) {
    
	 EBDetId spikedet=digiItrEB->id();
	 int spike_ieta=spikedet.ieta();      
	 int spike_iphi=spikedet.iphi();

	 int iebin=spike_ieta+86-1*int(spike_ieta>0);

	 double rechittime=eventtime->GetBinContent(spike_iphi,iebin);
	 double rechitenergy=eventenergy->GetBinContent(spike_iphi,iebin);
	 double rechitet=eventet->GetBinContent(spike_iphi,iebin);
	 //	 cout << "Spike ieta,iphi=" << spike_ieta << ":" << spike_iphi
	 //   << " Spike ADC=" << adcmax << " Rechit et=" << rechitet
	 //   << " Rechit time=" << rechittime << endl;



	 if (fabs(rechittime)<15 && rechittime != 0 && rechitenergy>2.0) {

    


	 int iphi_min=(int((spike_iphi-1)/5))*5+1;
	 int ieta_min=(int((abs(spike_ieta)-1)/5))*5;
	 if (spike_ieta<0) ieta_min=-1*ieta_min-5;
	 if (spike_ieta>0) ieta_min++;




	 int goodhit=0;

	 float spiketoweret=0;

	 for (Int_t x=ieta_min;x<ieta_min+5;x++) {

	   for (Int_t y=iphi_min;y<iphi_min+5;y++) {

	     int ietabin=x+86-1*int(x>0);
	     double myene=eventenergy->GetBinContent(y,ietabin);
	     double myet=eventet->GetBinContent(y,ietabin);
	     double mytime=eventtime->GetBinContent(y,ietabin);
	     if (myene !=0 && mytime!=0) goodhit++;
	     spiketoweret+=myet;
	   }
	 }


	 if (goodhit==25 && spiketoweret>5.0) {

	     double e4swiss=0;
	     double et4swiss=0;

	     double e4swisstt=0;
	     double et4swisstt=0;

	     double ene_swiss1=0;
	     double et_swiss1=0;
	     double time_swiss1=0;
	     double koot_swiss1=0;
	     double kdw_swiss1=0;

	     double ene_swiss2=0;
	     double et_swiss2=0;
	     double time_swiss2=0;
	     double koot_swiss2=0;
	     double kdw_swiss2=0;

	     double ene_swiss3=0;
	     double et_swiss3=0;
	     double time_swiss3=0;
	     double koot_swiss3=0;
	     double kdw_swiss3=0;

	     double ene_swiss4=0;
	     double et_swiss4=0;
	     double time_swiss4=0;
	     double koot_swiss4=0;
	     double kdw_swiss4=0;

	     double et_swissmax=0;
	     double time_swissmax=0;
	     double koot_swissmax=0;
	     double kdw_swissmax=0;

	     //	     cout << " SPIKE,  ieta:iphi=" << spike_ieta << ":" << spike_iphi
	     //  << " e=" << rechitenergy << " et=" << rechitet << endl;

	     int xco=0;
	     int yco=0;

	     xco=spike_ieta;
	     yco=spike_iphi-1;
	     if (yco==0) yco=360;


	     int ietabin=xco+86-1*int(xco>0);

	     double myene=eventenergy->GetBinContent(yco,ietabin);
	     double myet=eventet->GetBinContent(yco,ietabin);
	     double mytime=eventtime->GetBinContent(yco,ietabin);
	     double mykoot=eventkoot->GetBinContent(yco,ietabin);
	     double mykdw=eventkdiweird->GetBinContent(yco,ietabin);

	     ene_swiss1=myene;
	     et_swiss1=myet;
	     time_swiss1=mytime;
	     koot_swiss1=mykoot;
	     kdw_swiss1=mykdw;


	     if (myet>et_swissmax) {
	       et_swissmax=myet;
	       time_swissmax=mytime;
	       koot_swissmax=mykoot;
	       kdw_swissmax=mykdw;
	     }

	     e4swiss+=myene;
	     et4swiss+=myet;

	     if (yco>=iphi_min) {
	       e4swisstt+=myene;
	       et4swisstt+=myet;
	     }


	     //     cout << xco << ":" << yco << " e=" << myene << " et=" << myet
	     //  << " e4swiss=" << e4swiss << " et4swiss=" << et4swiss 
	     //  << " e4swisstt=" << e4swisstt << " et4swisstt=" << et4swisstt << endl;

	     xco=spike_ieta;
	     yco=spike_iphi+1;
	     if (yco>360) yco=1;
	     ietabin=xco+86-1*int(xco>0);

	     myene=eventenergy->GetBinContent(yco,ietabin);
	     myet=eventet->GetBinContent(yco,ietabin);
	     mytime=eventtime->GetBinContent(yco,ietabin);
	     mykoot=eventkoot->GetBinContent(yco,ietabin);
	     mykdw=eventkdiweird->GetBinContent(yco,ietabin);

	     ene_swiss2=myene;
	     et_swiss2=myet;
	     time_swiss2=mytime;
	     koot_swiss2=mykoot;
	     kdw_swiss2=mykdw;


	     if (myet>et_swissmax) {
	       et_swissmax=myet;
	       time_swissmax=mytime;
	       koot_swissmax=mykoot;
	       kdw_swissmax=mykdw;
	     }
	     e4swiss+=myene;
	     et4swiss+=myet;

	     if (yco<iphi_min+5) {
	       e4swisstt+=myene;
	       et4swisstt+=myet;
	     }

	     //	     cout << xco << ":" << yco << " e=" << myene << " et=" << myet
	     //  << " e4swiss=" << e4swiss << " et4swiss=" << et4swiss 
	     //  << " e4swisstt=" << e4swisstt << " et4swisstt=" << et4swisstt << endl;



	     xco=spike_ieta+1;
	     yco=spike_iphi;

	     if (xco<86) {

	       ietabin=xco+86-1*int(xco>0);

	       myene=eventenergy->GetBinContent(yco,ietabin);
	       myet=eventet->GetBinContent(yco,ietabin);
	       mytime=eventtime->GetBinContent(yco,ietabin);
	       mykoot=eventkoot->GetBinContent(yco,ietabin);
	       mykdw=eventkdiweird->GetBinContent(yco,ietabin);
	       

	       ene_swiss3=myene;
	       et_swiss3=myet;
	       time_swiss3=mytime;
	       koot_swiss3=mykoot;
	       kdw_swiss3=mykdw;
	       
	       
	       if (myet>et_swissmax) {
		 et_swissmax=myet;
		 time_swissmax=mytime;
		 koot_swissmax=mykoot;
		 kdw_swissmax=mykdw;
	       }

	       e4swiss+=myene;
	       et4swiss+=myet;

	       if (xco<ieta_min+5) {
		 e4swisstt+=myene;
		 et4swisstt+=myet;
	       }

	     }
	     //	     cout << xco << ":" << yco << " e=" << myene << " et=" << myet
	     //  << " e4swiss=" << e4swiss << " et4swiss=" << et4swiss 
	     //  << " e4swisstt=" << e4swisstt << " et4swisstt=" << et4swisstt << endl;


	     xco=spike_ieta-1;
	     yco=spike_iphi;

	     if (xco>-86) {

	       ietabin=xco+86-1*int(xco>0);

	       myene=eventenergy->GetBinContent(yco,ietabin);
	       myet=eventet->GetBinContent(yco,ietabin);
	       mytime=eventtime->GetBinContent(yco,ietabin);
	       mykoot=eventkoot->GetBinContent(yco,ietabin);
	       mykdw=eventkdiweird->GetBinContent(yco,ietabin);
	       
	       
	       ene_swiss4=myene;
	       et_swiss4=myet;
	       time_swiss4=mytime;
	       koot_swiss4=mykoot;
	       kdw_swiss4=mykdw;
	       
	       
	       if (myet>et_swissmax) {
		 et_swissmax=myet;
		 time_swissmax=mytime;
		 koot_swissmax=mykoot;
		 kdw_swissmax=mykdw;
	       }

	       e4swiss+=myene;
	       et4swiss+=myet;

	       if (xco>=ieta_min) {
		 e4swisstt+=myene;
		 et4swisstt+=myet;
	       }

	     }
	     //	     cout << xco << ":" << yco << " e=" << myene << " et=" << myet
	     //  << " e4swiss=" << e4swiss << " et4swiss=" << et4swiss 
	     //  << " e4swisstt=" << e4swisstt << " et4swisstt=" << et4swisstt << endl;




	     if (spiketoweret>=15.0) {


	     // check status of spikes with swisscross<0.8

	     if (1.0-e4swiss/rechitenergy<0.8) {

	       spike_timevset_all_08->Fill(time_swiss1,et_swiss1);
	       spike_timevset_all_08->Fill(time_swiss2,et_swiss2);
	       spike_timevset_all_08->Fill(time_swiss3,et_swiss3);
	       spike_timevset_all_08->Fill(time_swiss4,et_swiss4);
	       spike_timevset_highest_08->Fill(time_swissmax,et_swissmax);



	       if (koot_swiss1) spike_timevset_koot_all_08->Fill(time_swiss1,et_swiss1);
	       if (koot_swiss2) spike_timevset_koot_all_08->Fill(time_swiss2,et_swiss2);
	       if (koot_swiss3) spike_timevset_koot_all_08->Fill(time_swiss3,et_swiss3);
	       if (koot_swiss4) spike_timevset_koot_all_08->Fill(time_swiss4,et_swiss4);
	       if (koot_swissmax) spike_timevset_koot_highest_08->Fill(time_swissmax,et_swissmax);

	       if (kdw_swiss1) spike_timevset_kdiweird_all_08->Fill(time_swiss1,et_swiss1);
	       if (kdw_swiss2) spike_timevset_kdiweird_all_08->Fill(time_swiss2,et_swiss2);
	       if (kdw_swiss3) spike_timevset_kdiweird_all_08->Fill(time_swiss3,et_swiss3);
	       if (kdw_swiss4) spike_timevset_kdiweird_all_08->Fill(time_swiss4,et_swiss4);
	       if (kdw_swissmax) spike_timevset_kdiweird_highest_08->Fill(time_swissmax,et_swissmax);
	      


	     }


	     // check status of spikes with swisscross<0.7

	     if (1.0-e4swiss/rechitenergy<0.7) {

	       spike_timevset_all_07->Fill(time_swiss1,et_swiss1);
	       spike_timevset_all_07->Fill(time_swiss2,et_swiss2);
	       spike_timevset_all_07->Fill(time_swiss3,et_swiss3);
	       spike_timevset_all_07->Fill(time_swiss4,et_swiss4);
	       spike_timevset_highest_07->Fill(time_swissmax,et_swissmax);



	       if (koot_swiss1) spike_timevset_koot_all_07->Fill(time_swiss1,et_swiss1);
	       if (koot_swiss2) spike_timevset_koot_all_07->Fill(time_swiss2,et_swiss2);
	       if (koot_swiss3) spike_timevset_koot_all_07->Fill(time_swiss3,et_swiss3);
	       if (koot_swiss4) spike_timevset_koot_all_07->Fill(time_swiss4,et_swiss4);
	       if (koot_swissmax) spike_timevset_koot_highest_07->Fill(time_swissmax,et_swissmax);

	       if (kdw_swiss1) spike_timevset_kdiweird_all_07->Fill(time_swiss1,et_swiss1);
	       if (kdw_swiss2) spike_timevset_kdiweird_all_07->Fill(time_swiss2,et_swiss2);
	       if (kdw_swiss3) spike_timevset_kdiweird_all_07->Fill(time_swiss3,et_swiss3);
	       if (kdw_swiss4) spike_timevset_kdiweird_all_07->Fill(time_swiss4,et_swiss4);
	       if (kdw_swissmax) spike_timevset_kdiweird_highest_07->Fill(time_swissmax,et_swissmax);
	      


	     }


	     // check status of spikes with swisscross<0.6

	     if (1.0-e4swiss/rechitenergy<0.6) {

	       spike_timevset_all_06->Fill(time_swiss1,et_swiss1);
	       spike_timevset_all_06->Fill(time_swiss2,et_swiss2);
	       spike_timevset_all_06->Fill(time_swiss3,et_swiss3);
	       spike_timevset_all_06->Fill(time_swiss4,et_swiss4);
	       spike_timevset_highest_06->Fill(time_swissmax,et_swissmax);



	       if (koot_swiss1) spike_timevset_koot_all_06->Fill(time_swiss1,et_swiss1);
	       if (koot_swiss2) spike_timevset_koot_all_06->Fill(time_swiss2,et_swiss2);
	       if (koot_swiss3) spike_timevset_koot_all_06->Fill(time_swiss3,et_swiss3);
	       if (koot_swiss4) spike_timevset_koot_all_06->Fill(time_swiss4,et_swiss4);
	       if (koot_swissmax) spike_timevset_koot_highest_06->Fill(time_swissmax,et_swissmax);

	       if (kdw_swiss1) spike_timevset_kdiweird_all_06->Fill(time_swiss1,et_swiss1);
	       if (kdw_swiss2) spike_timevset_kdiweird_all_06->Fill(time_swiss2,et_swiss2);
	       if (kdw_swiss3) spike_timevset_kdiweird_all_06->Fill(time_swiss3,et_swiss3);
	       if (kdw_swiss4) spike_timevset_kdiweird_all_06->Fill(time_swiss4,et_swiss4);
	       if (kdw_swissmax) spike_timevset_kdiweird_highest_06->Fill(time_swissmax,et_swissmax);
	      


	     }


	     }





	     double swisst10=0;
	     double swisst07=0;
	     double swisst05=0;
	     double swisst04=0;
	     double swisst03=0;


	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<10)) swisst10+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<10)) swisst10+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<10)) swisst10+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<10)) swisst10+=ene_swiss4;

	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<7)) swisst07+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<7)) swisst07+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<7)) swisst07+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<7)) swisst07+=ene_swiss4;


	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<5)) swisst05+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<5)) swisst05+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<5)) swisst05+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<5)) swisst05+=ene_swiss4;


	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<4)) swisst04+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<4)) swisst04+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<4)) swisst04+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<4)) swisst04+=ene_swiss4;

	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<3)) swisst03+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<3)) swisst03+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<3)) swisst03+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<3)) swisst03+=ene_swiss4;





	     double swisscross10=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<10)) swisscross10=1.0-swisst10/rechitenergy;

	     double swisscross07=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<7)) swisscross07=1.0-swisst10/rechitenergy;

	     double swisscross05=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<5)) swisscross05=1.0-swisst10/rechitenergy;

	     double swisscross04=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<4)) swisscross04=1.0-swisst10/rechitenergy;

	     double swisscross03=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<3)) swisscross03=1.0-swisst10/rechitenergy;



	   for (Int_t nn=0;nn<100;nn++) {

	     double thresh=nn*0.02+0.01-0.5;

	     double swisscut=0.21+0.01*nn;



	     int probet_swisstt=0;
	     int probet_swiss=0;
	     int probene_swisstt=0;
	     int probene_swiss=0;


	     int probene_swiss_t10=0;
	     int probene_swiss_t07=0;
	     int probene_swiss_t05=0;
	     int probene_swiss_t04=0;
	     int probene_swiss_t03=0;


	     if (1.0-e4swiss/rechitenergy>swisscut) probene_swiss=1;
	     if (1.0-et4swiss/rechitet>swisscut) probet_swiss=1;

	     if (1.0-e4swisstt/rechitenergy>swisscut) probene_swisstt=1;
	     if (1.0-et4swisstt/rechitet>swisscut) probet_swisstt=1;

	     if (swisscross10>swisscut) probene_swiss_t10=1;
	     if (swisscross07>swisscut) probene_swiss_t07=1;
	     if (swisscross05>swisscut) probene_swiss_t05=1;
	     if (swisscross04>swisscut) probene_swiss_t04=1;
	     if (swisscross03>swisscut) probene_swiss_t03=1;





	     //   thresh=0.5;

	     int stripetcount=0;
	     int stripenecount=0;
	     
	     int alletcount=0;
	     int allenecount=0;
	     
	   
	     
	     for (Int_t x=ieta_min;x<ieta_min+5;x++) {
	       
	       stripetcount=0;
	       stripenecount=0;
	       
	       for (Int_t y=iphi_min;y<iphi_min+5;y++) {
		 
		 int ietabin=x+86-1*int(x>0);
		 double myene=eventenergy->GetBinContent(y,ietabin);
		 double myet=eventet->GetBinContent(y,ietabin);
		 // double mytime=eventtime->GetBinContent(y,ietabin);
		 
		 if (myene>thresh) stripenecount++;
		 if (myet>thresh) stripetcount++;
		 
		  
		 // cout << "ieta=" << x << " iphi=" << y
		 //  << " ene=" << myene << " et=" << myet << " time=" << mytime <<endl;
	       }
	       
	       
	       int stripetprob=0;
	       int stripeneprob=0;
	       
	       if (stripenecount>1) stripeneprob=1;
	       if (stripetcount>1) stripetprob=1;
	       
	       strip_prob_ene->Fill(thresh,stripeneprob);
	       strip_prob_et->Fill(thresh,stripetprob);


	       strip_prob_ene_pu->Fill(thresh,stripeneprob,numgoodvtx);
	       strip_prob_et_pu->Fill(thresh,stripetprob,numgoodvtx);


	       if (spiketoweret>=8.0) {

		 strip_prob_ene_thresh08->Fill(thresh,stripeneprob);
		 strip_prob_et_thresh08->Fill(thresh,stripetprob);

		 strip_prob_ene_pu_thresh08->Fill(thresh,stripeneprob,numgoodvtx);
		 strip_prob_et_pu_thresh08->Fill(thresh,stripetprob,numgoodvtx);

	       }


	       if (spiketoweret>=10.0) {

		 strip_prob_ene_thresh10->Fill(thresh,stripeneprob);
		 strip_prob_et_thresh10->Fill(thresh,stripetprob);

		 strip_prob_ene_pu_thresh10->Fill(thresh,stripeneprob,numgoodvtx);
		 strip_prob_et_pu_thresh10->Fill(thresh,stripetprob,numgoodvtx);

	       }


	       if (spiketoweret>=12.0) {

		 strip_prob_ene_thresh12->Fill(thresh,stripeneprob);
		 strip_prob_et_thresh12->Fill(thresh,stripetprob);

		 strip_prob_ene_pu_thresh12->Fill(thresh,stripeneprob,numgoodvtx);
		 strip_prob_et_pu_thresh12->Fill(thresh,stripetprob,numgoodvtx);

	       }


	       if (spiketoweret>=15.0) {

		 strip_prob_ene_thresh15->Fill(thresh,stripeneprob);
		 strip_prob_et_thresh15->Fill(thresh,stripetprob);

		 strip_prob_ene_pu_thresh15->Fill(thresh,stripeneprob,numgoodvtx);
		 strip_prob_et_pu_thresh15->Fill(thresh,stripetprob,numgoodvtx);





	       }


	       if (spiketoweret>=20.0) {

		 strip_prob_ene_thresh20->Fill(thresh,stripeneprob);
		 strip_prob_et_thresh20->Fill(thresh,stripetprob);

		 strip_prob_ene_pu_thresh20->Fill(thresh,stripeneprob,numgoodvtx);
		 strip_prob_et_pu_thresh20->Fill(thresh,stripetprob,numgoodvtx);

	       }




	       if (abs(spike_ieta)<=25) {
		 strip_prob_ene_mod1->Fill(thresh,stripeneprob);
		 strip_prob_et_mod1->Fill(thresh,stripetprob);
	       }

	       if (abs(spike_ieta)>25 && abs(spike_ieta)<=45) {
		 strip_prob_ene_mod2->Fill(thresh,stripeneprob);
		 strip_prob_et_mod2->Fill(thresh,stripetprob);
	       }

	       if (abs(spike_ieta)>45 && abs(spike_ieta)<=65) {
		 strip_prob_ene_mod3->Fill(thresh,stripeneprob);
		 strip_prob_et_mod3->Fill(thresh,stripetprob);
	       }

	       if (abs(spike_ieta)>65) {
		 strip_prob_ene_mod4->Fill(thresh,stripeneprob);
		 strip_prob_et_mod4->Fill(thresh,stripetprob);
	       }

	       if (numgoodvtx<=10) {
		 strip_prob_ene_pu0->Fill(thresh,stripeneprob);
		 strip_prob_et_pu0->Fill(thresh,stripetprob);
	       }

	       if (numgoodvtx>10 && numgoodvtx<=20) {
		 strip_prob_ene_pu10->Fill(thresh,stripeneprob);
		 strip_prob_et_pu10->Fill(thresh,stripetprob);
	       }

	       if (numgoodvtx>20) {
		 strip_prob_ene_pu20->Fill(thresh,stripeneprob);
		 strip_prob_et_pu20->Fill(thresh,stripetprob);
	       }

	       if (adcmax>300 || gainmax>1) {
		 strip_prob_ene_300->Fill(thresh,stripeneprob);
		 strip_prob_et_300->Fill(thresh,stripetprob);
	       }



	       //  cout << " stripenecount=" << stripenecount << " stripetcount=" << stripetcount << endl;

	       allenecount+=stripeneprob;
	       alletcount+=stripetprob;

	     }

	     //   cout << " allenecount=" << allenecount << " alletcount=" << alletcount << endl;

	     int sfgvb_tower_et=0;
	     int sfgvb_tower_ene=0;
	     
	     if (allenecount==0) sfgvb_tower_ene=1;
	     if (alletcount==0) sfgvb_tower_et=1;

	     //   cout << " sfgvb_ene=" << sfgvb_tower_ene << " sfgvb_et=" << sfgvb_tower_et << endl;

	     
	    
	     tower_spike_ene->Fill(thresh,sfgvb_tower_ene);
	     tower_spike_et->Fill(thresh,sfgvb_tower_et);

	     tower_spike_ene_pu->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	     tower_spike_et_pu->Fill(thresh,sfgvb_tower_et,numgoodvtx);



	     tower_spike_ene_swiss->Fill(swisscut,probene_swiss);
	     tower_spike_et_swiss->Fill(swisscut,probet_swiss);
	     tower_spike_ene_swisstt->Fill(swisscut,probene_swisstt);
	     tower_spike_et_swisstt->Fill(swisscut,probet_swisstt);


	     tower_spike_ene_swiss_t10->Fill(swisscut,probene_swiss_t10);
	     tower_spike_ene_swiss_t07->Fill(swisscut,probene_swiss_t07);
	     tower_spike_ene_swiss_t05->Fill(swisscut,probene_swiss_t05);
	     tower_spike_ene_swiss_t04->Fill(swisscut,probene_swiss_t04);
	     tower_spike_ene_swiss_t03->Fill(swisscut,probene_swiss_t03);



	     if (spiketoweret>=8.0) {

	       tower_spike_ene_thresh08->Fill(thresh,sfgvb_tower_ene);
	       tower_spike_et_thresh08->Fill(thresh,sfgvb_tower_et);
	       
	       tower_spike_ene_pu_thresh08->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       tower_spike_et_pu_thresh08->Fill(thresh,sfgvb_tower_et,numgoodvtx);


	       tower_spike_ene_swiss_thresh08->Fill(swisscut,probene_swiss);
	       tower_spike_et_swiss_thresh08->Fill(swisscut,probet_swiss);
	       tower_spike_ene_swisstt_thresh08->Fill(swisscut,probene_swisstt);
	       tower_spike_et_swisstt_thresh08->Fill(swisscut,probet_swisstt);

	       tower_spike_ene_swiss_t10_thresh08->Fill(swisscut,probene_swiss_t10);
	       tower_spike_ene_swiss_t07_thresh08->Fill(swisscut,probene_swiss_t07);
	       tower_spike_ene_swiss_t05_thresh08->Fill(swisscut,probene_swiss_t05);
	       tower_spike_ene_swiss_t04_thresh08->Fill(swisscut,probene_swiss_t04);
	       tower_spike_ene_swiss_t03_thresh08->Fill(swisscut,probene_swiss_t03);


	       
	     }


	     if (spiketoweret>=10.0) {

	       tower_spike_ene_thresh10->Fill(thresh,sfgvb_tower_ene);
	       tower_spike_et_thresh10->Fill(thresh,sfgvb_tower_et);
	       
	       tower_spike_ene_pu_thresh10->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       tower_spike_et_pu_thresh10->Fill(thresh,sfgvb_tower_et,numgoodvtx);

	       tower_spike_ene_swiss_thresh10->Fill(swisscut,probene_swiss);
	       tower_spike_et_swiss_thresh10->Fill(swisscut,probet_swiss);
	       tower_spike_ene_swisstt_thresh10->Fill(swisscut,probene_swisstt);
	       tower_spike_et_swisstt_thresh10->Fill(swisscut,probet_swisstt);

	       tower_spike_ene_swiss_t10_thresh10->Fill(swisscut,probene_swiss_t10);
	       tower_spike_ene_swiss_t07_thresh10->Fill(swisscut,probene_swiss_t07);
	       tower_spike_ene_swiss_t05_thresh10->Fill(swisscut,probene_swiss_t05);
	       tower_spike_ene_swiss_t04_thresh10->Fill(swisscut,probene_swiss_t04);
	       tower_spike_ene_swiss_t03_thresh10->Fill(swisscut,probene_swiss_t03);
	       
	     }


	     if (spiketoweret>=12.0) {

	       tower_spike_ene_thresh12->Fill(thresh,sfgvb_tower_ene);
	       tower_spike_et_thresh12->Fill(thresh,sfgvb_tower_et);
	       
	       tower_spike_ene_pu_thresh12->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       tower_spike_et_pu_thresh12->Fill(thresh,sfgvb_tower_et,numgoodvtx);
	       
	       tower_spike_ene_swiss_thresh12->Fill(swisscut,probene_swiss);
	       tower_spike_et_swiss_thresh12->Fill(swisscut,probet_swiss);
	       tower_spike_ene_swisstt_thresh12->Fill(swisscut,probene_swisstt);
	       tower_spike_et_swisstt_thresh12->Fill(swisscut,probet_swisstt);

	       tower_spike_ene_swiss_t10_thresh12->Fill(swisscut,probene_swiss_t10);
	       tower_spike_ene_swiss_t07_thresh12->Fill(swisscut,probene_swiss_t07);
	       tower_spike_ene_swiss_t05_thresh12->Fill(swisscut,probene_swiss_t05);
	       tower_spike_ene_swiss_t04_thresh12->Fill(swisscut,probene_swiss_t04);
	       tower_spike_ene_swiss_t03_thresh12->Fill(swisscut,probene_swiss_t03);
	       

	     }


	     if (spiketoweret>=15.0) {

	       tower_spike_ene_thresh15->Fill(thresh,sfgvb_tower_ene);
	       tower_spike_et_thresh15->Fill(thresh,sfgvb_tower_et);
	       
	       tower_spike_ene_pu_thresh15->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       tower_spike_et_pu_thresh15->Fill(thresh,sfgvb_tower_et,numgoodvtx);



	       tower_spike_ene_swiss_thresh15->Fill(swisscut,probene_swiss);
	       tower_spike_et_swiss_thresh15->Fill(swisscut,probet_swiss);
	       tower_spike_ene_swisstt_thresh15->Fill(swisscut,probene_swisstt);
	       tower_spike_et_swisstt_thresh15->Fill(swisscut,probet_swisstt);

	       tower_spike_ene_swiss_t10_thresh15->Fill(swisscut,probene_swiss_t10);
	       tower_spike_ene_swiss_t07_thresh15->Fill(swisscut,probene_swiss_t07);
	       tower_spike_ene_swiss_t05_thresh15->Fill(swisscut,probene_swiss_t05);
	       tower_spike_ene_swiss_t04_thresh15->Fill(swisscut,probene_swiss_t04);
	       tower_spike_ene_swiss_t03_thresh15->Fill(swisscut,probene_swiss_t03);
	       



// 		 cout << "Spike:  ADC=" << adcmax-200 << " maxsamp=" << sampmax << " ieta,iphi=" << spike_ieta 
// 		      << "," << spike_iphi << " rechit time=" << rechittime << endl; 
// 		 cout << "iphi_min=" << iphi_min << " ieta_min=" << ieta_min << endl;

// 		 cout << "spike digis:" << endl;
// 		 for (Int_t id=0;id<10;id++) cout << dframe[id] << endl;	     
// 		 cout << "sFGVB=" << sfgvb_tower_et << endl;



// 		 cout << "rechits: " << endl;

// 		 for (Int_t x=ieta_min;x<ieta_min+5;x++) {
	       
	    
// 		   for (Int_t y=iphi_min;y<iphi_min+5;y++) {
		 
// 		     int ietabin=x+86-1*int(x>0);
// 		     double myene=eventenergy->GetBinContent(y,ietabin);
// 		     double myet=eventet->GetBinContent(y,ietabin);
// 		     double mytime=eventtime->GetBinContent(y,ietabin);
		     
	
		  
// 		     cout << "ieta=" << x << " iphi=" << y
// 			  << " ene=" << myene << " et=" << myet << " time=" << mytime <<endl;
// 		   }

		   
// 		 }

	     }

	     if (spiketoweret>=20.0) {

	       tower_spike_ene_thresh20->Fill(thresh,sfgvb_tower_ene);
	       tower_spike_et_thresh20->Fill(thresh,sfgvb_tower_et);
	       
	       tower_spike_ene_pu_thresh20->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       tower_spike_et_pu_thresh20->Fill(thresh,sfgvb_tower_et,numgoodvtx);

	       tower_spike_ene_swiss_thresh20->Fill(swisscut,probene_swiss);
	       tower_spike_et_swiss_thresh20->Fill(swisscut,probet_swiss);
	       tower_spike_ene_swisstt_thresh20->Fill(swisscut,probene_swisstt);
	       tower_spike_et_swisstt_thresh20->Fill(swisscut,probet_swisstt);

	       tower_spike_ene_swiss_t10_thresh20->Fill(swisscut,probene_swiss_t10);
	       tower_spike_ene_swiss_t07_thresh20->Fill(swisscut,probene_swiss_t07);
	       tower_spike_ene_swiss_t05_thresh20->Fill(swisscut,probene_swiss_t05);
	       tower_spike_ene_swiss_t04_thresh20->Fill(swisscut,probene_swiss_t04);
	       tower_spike_ene_swiss_t03_thresh20->Fill(swisscut,probene_swiss_t03);
	       

	       
	     }



	       if (abs(spike_ieta)<=25) {
		 tower_spike_ene_mod1->Fill(thresh,sfgvb_tower_ene);
		 tower_spike_et_mod1->Fill(thresh,sfgvb_tower_et);
	       }

	       if (abs(spike_ieta)>25 && abs(spike_ieta)<=45) {
		 tower_spike_ene_mod2->Fill(thresh,sfgvb_tower_ene);
		 tower_spike_et_mod2->Fill(thresh,sfgvb_tower_et);
	       }

	       if (abs(spike_ieta)>45 && abs(spike_ieta)<=65) {
		 tower_spike_ene_mod3->Fill(thresh,sfgvb_tower_ene);
		 tower_spike_et_mod3->Fill(thresh,sfgvb_tower_et);
	       }

	       if (abs(spike_ieta)>65) {
		 tower_spike_ene_mod4->Fill(thresh,sfgvb_tower_ene);
		 tower_spike_et_mod4->Fill(thresh,sfgvb_tower_et);
	       }

	       if (numgoodvtx<=10) {
		 tower_spike_ene_pu0->Fill(thresh,sfgvb_tower_ene);
		 tower_spike_et_pu0->Fill(thresh,sfgvb_tower_et);
	       }

	       if (numgoodvtx>10 && numgoodvtx<=20) {
		 tower_spike_ene_pu10->Fill(thresh,sfgvb_tower_ene);
		 tower_spike_et_pu10->Fill(thresh,sfgvb_tower_et);
	       }

	       if (numgoodvtx>20) {
		 tower_spike_ene_pu20->Fill(thresh,sfgvb_tower_ene);
		 tower_spike_et_pu20->Fill(thresh,sfgvb_tower_et);
	       }

	       if (adcmax>300 || gainmax>1) {
		 tower_spike_ene_300->Fill(thresh,sfgvb_tower_ene);
		 tower_spike_et_300->Fill(thresh,sfgvb_tower_et);
	       }




	   }
	 }
	 }
       }
       
       
       
       
       
    }
     
  }
     
    

	


 // sfgvb loop - 2: superclusters



  if (!isMC_) {


    for (reco::SuperClusterCollection::const_iterator sCEB = sc_eb->begin(); sCEB != sc_eb->end(); ++sCEB) {

      EBDetId seedid=sCEB->seed()->seed();
  
    
      // float scenergy=sCEB->energy();
      //float sc_eta=sCEB->eta();
      //float sc_phi=sCEB->phi();
      //float scet=scenergy/cosh(sc_eta);
      
      int sc_ieta=seedid.ieta();
      int sc_iphi=seedid.iphi();




      int iebin=sc_ieta+86-1*int(sc_ieta>0);

      double rechitenergy=eventenergy->GetBinContent(sc_iphi,iebin);
      double rechittime=eventtime->GetBinContent(sc_iphi,iebin);
      double rechitet=eventet->GetBinContent(sc_iphi,iebin);

      if (fabs(rechittime)<15 && rechittime != 0 && rechitenergy>4.0) {

    


	 int iphi_min=(int((sc_iphi-1)/5))*5+1;
	 int ieta_min=(int((abs(sc_ieta)-1)/5))*5;
	 if (sc_ieta<0) ieta_min=-1*ieta_min-5;
	 if (sc_ieta>0) ieta_min++;




	 int goodhit=0;
	 float sctoweret=0;

	 for (Int_t x=ieta_min;x<ieta_min+5;x++) {

	   for (Int_t y=iphi_min;y<iphi_min+5;y++) {

	     int ietabin=x+86-1*int(x>0);
	     double myene=eventenergy->GetBinContent(y,ietabin);
	     double myet=eventet->GetBinContent(y,ietabin);
	     double mytime=eventtime->GetBinContent(y,ietabin);
	     if (myene !=0 && mytime!=0) goodhit++;
	     sctoweret+=myet;
	   }
	 }


	 if (goodhit==25 && sctoweret>5.0) {





	     double e4swiss=0;
	     double et4swiss=0;

	     double e4swisstt=0;
	     double et4swisstt=0;


	     double ene_swiss1=0;
	     double et_swiss1=0;
	     double time_swiss1=0;
	     double koot_swiss1=0;
	     double kdw_swiss1=0;


	     double ene_swiss2=0;
	     double et_swiss2=0;
	     double time_swiss2=0;
	     double koot_swiss2=0;
	     double kdw_swiss2=0;


	     double ene_swiss3=0;
	     double et_swiss3=0;
	     double time_swiss3=0;
	     double koot_swiss3=0;
	     double kdw_swiss3=0;


	     double ene_swiss4=0;
	     double et_swiss4=0;
	     double time_swiss4=0;
	     double koot_swiss4=0;
	     double kdw_swiss4=0;

	     double et_swissmax=0.001;
	     double time_swissmax=0;
	     double koot_swissmax=0;
	     double kdw_swissmax=0;



	     int xco=0;
	     int yco=0;

	     xco=sc_ieta;
	     yco=sc_iphi-1;
	     if (yco==0) yco=360;

	     int ietabin=xco+86-1*int(xco>0);

	     double myene=eventenergy->GetBinContent(yco,ietabin);
	     double myet=eventet->GetBinContent(yco,ietabin);
	     double mytime=eventtime->GetBinContent(yco,ietabin);
	     double mykoot=eventkoot->GetBinContent(yco,ietabin);
	     double mykdw=eventkdiweird->GetBinContent(yco,ietabin);

	     ene_swiss1=myene;
	     et_swiss1=myet;
	     time_swiss1=mytime;
	     koot_swiss1=mykoot;
	     kdw_swiss1=mykdw;


	     if (myet>et_swissmax) {
	       et_swissmax=myet;
	       time_swissmax=mytime;
	       koot_swissmax=mykoot;
	       kdw_swissmax=mykdw;
	     }


	     e4swiss+=myene;
	     et4swiss+=myet;

	     if (yco>=iphi_min) {
	       e4swisstt+=myene;
	       et4swisstt+=myet;
	     }


	     xco=sc_ieta;
	     yco=sc_iphi+1;
	     if (yco>360) yco=1;
	     ietabin=xco+86-1*int(xco>0);

	     myene=eventenergy->GetBinContent(yco,ietabin);
	     myet=eventet->GetBinContent(yco,ietabin);
	     mytime=eventtime->GetBinContent(yco,ietabin);
	     mykoot=eventkoot->GetBinContent(yco,ietabin);
	     mykdw=eventkdiweird->GetBinContent(yco,ietabin);

	     ene_swiss2=myene;
	     et_swiss2=myet;
	     time_swiss2=mytime;
	     koot_swiss2=mykoot;
	     kdw_swiss2=mykdw;


	     if (myet>et_swissmax) {
	       et_swissmax=myet;
	       time_swissmax=mytime;
	       koot_swissmax=mykoot;
	       kdw_swissmax=mykdw;
	     }

	     e4swiss+=myene;
	     et4swiss+=myet;

	     if (yco<iphi_min+5) {
	       e4swisstt+=myene;
	       et4swisstt+=myet;
	     }




	     xco=sc_ieta+1;
	     yco=sc_iphi;

	     if (xco<86) {

	       ietabin=xco+86-1*int(xco>0);

	       myene=eventenergy->GetBinContent(yco,ietabin);
	       myet=eventet->GetBinContent(yco,ietabin);

	       mytime=eventtime->GetBinContent(yco,ietabin);
	       mykoot=eventkoot->GetBinContent(yco,ietabin);
	       mykdw=eventkdiweird->GetBinContent(yco,ietabin);
	   
	       ene_swiss3=myene;
	       et_swiss3=myet;
	       time_swiss3=mytime;
	       koot_swiss3=mykoot;
	       kdw_swiss3=mykdw;
	       
	       
	       if (myet>et_swissmax) {
		 et_swissmax=myet;
		 time_swissmax=mytime;
		 koot_swissmax=mykoot;
		 kdw_swissmax=mykdw;
	       }

	       e4swiss+=myene;
	       et4swiss+=myet;

	       if (xco<ieta_min+5) {
		 e4swisstt+=myene;
		 et4swisstt+=myet;
	       }

	     }


	     xco=sc_ieta-1;
	     yco=sc_iphi;

	     if (xco>-86) {

	       ietabin=xco+86-1*int(xco>0);

	       myene=eventenergy->GetBinContent(yco,ietabin);
	       myet=eventet->GetBinContent(yco,ietabin);
	       mytime=eventtime->GetBinContent(yco,ietabin);
	       mykoot=eventkoot->GetBinContent(yco,ietabin);
	       mykdw=eventkdiweird->GetBinContent(yco,ietabin);

	       ene_swiss4=myene;
	       et_swiss4=myet;
	       time_swiss4=mytime;
	       koot_swiss4=mykoot;
	       kdw_swiss4=mykdw;
	       
	       
	       if (myet>et_swissmax) {
		 et_swissmax=myet;
		 time_swissmax=mytime;
		 koot_swissmax=mykoot;
		 kdw_swissmax=mykdw;
	       }

	       e4swiss+=myene;
	       et4swiss+=myet;

	       if (xco>=ieta_min) {
		 e4swisstt+=myene;
		 et4swisstt+=myet;
	       }

	     }


	     


	     // check status of spikes with swisscross<0.8

	     if (1.0-e4swiss/rechitenergy<0.8) {

	       spike_timevset_all_08->Fill(time_swiss1,et_swiss1);
	       spike_timevset_all_08->Fill(time_swiss2,et_swiss2);
	       spike_timevset_all_08->Fill(time_swiss3,et_swiss3);
	       spike_timevset_all_08->Fill(time_swiss4,et_swiss4);
	       spike_timevset_highest_08->Fill(time_swissmax,et_swissmax);



	       if (koot_swiss1) spike_timevset_koot_all_08->Fill(time_swiss1,et_swiss1);
	       if (koot_swiss2) spike_timevset_koot_all_08->Fill(time_swiss2,et_swiss2);
	       if (koot_swiss3) spike_timevset_koot_all_08->Fill(time_swiss3,et_swiss3);
	       if (koot_swiss4) spike_timevset_koot_all_08->Fill(time_swiss4,et_swiss4);
	       if (koot_swissmax) spike_timevset_koot_highest_08->Fill(time_swissmax,et_swissmax);

	       if (kdw_swiss1) spike_timevset_kdiweird_all_08->Fill(time_swiss1,et_swiss1);
	       if (kdw_swiss2) spike_timevset_kdiweird_all_08->Fill(time_swiss2,et_swiss2);
	       if (kdw_swiss3) spike_timevset_kdiweird_all_08->Fill(time_swiss3,et_swiss3);
	       if (kdw_swiss4) spike_timevset_kdiweird_all_08->Fill(time_swiss4,et_swiss4);
	       if (kdw_swissmax) spike_timevset_kdiweird_highest_08->Fill(time_swissmax,et_swissmax);
	      


	     }




	     double swisst10=0;
	     double swisst07=0;
	     double swisst05=0;
	     double swisst04=0;
	     double swisst03=0;


	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<10)) swisst10+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<10)) swisst10+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<10)) swisst10+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<10)) swisst10+=ene_swiss4;

	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<7)) swisst07+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<7)) swisst07+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<7)) swisst07+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<7)) swisst07+=ene_swiss4;


	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<5)) swisst05+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<5)) swisst05+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<5)) swisst05+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<5)) swisst05+=ene_swiss4;


	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<4)) swisst04+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<4)) swisst04+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<4)) swisst04+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<4)) swisst04+=ene_swiss4;

	     if (ene_swiss1<2.0 || (ene_swiss1>=2.0 && abs(time_swiss1)<3)) swisst03+=ene_swiss1;
	     if (ene_swiss2<2.0 || (ene_swiss2>=2.0 && abs(time_swiss2)<3)) swisst03+=ene_swiss2;
	     if (ene_swiss3<2.0 || (ene_swiss3>=2.0 && abs(time_swiss3)<3)) swisst03+=ene_swiss3;
	     if (ene_swiss4<2.0 || (ene_swiss4>=2.0 && abs(time_swiss4)<3)) swisst03+=ene_swiss4;





	     double swisscross10=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<10)) swisscross10=1.0-swisst10/rechitenergy;

	     double swisscross07=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<7)) swisscross07=1.0-swisst10/rechitenergy;

	     double swisscross05=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<5)) swisscross05=1.0-swisst10/rechitenergy;

	     double swisscross04=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<4)) swisscross04=1.0-swisst10/rechitenergy;

	     double swisscross03=1.0;
	     if (rechitenergy<2.0 || (rechitenergy>2.0 && abs(rechittime)<3)) swisscross03=1.0-swisst10/rechitenergy;




	   for (Int_t nn=0;nn<100;nn++) {

	     double thresh=nn*0.02+0.01-0.5;
	     double swisscut=0.21+0.01*nn;

	     int sc_probet_swisstt=0;
	     int sc_probet_swiss=0;
	     int sc_probene_swisstt=0;
	     int sc_probene_swiss=0;



	     int sc_probene_swiss_t10=0;
	     int sc_probene_swiss_t07=0;
	     int sc_probene_swiss_t05=0;
	     int sc_probene_swiss_t04=0;
	     int sc_probene_swiss_t03=0;


	     if (swisscross10>swisscut) sc_probene_swiss_t10=1;
	     if (swisscross07>swisscut) sc_probene_swiss_t07=1;
	     if (swisscross05>swisscut) sc_probene_swiss_t05=1;
	     if (swisscross04>swisscut) sc_probene_swiss_t04=1;
	     if (swisscross03>swisscut) sc_probene_swiss_t03=1;





	     if (1.0-e4swiss/rechitenergy>swisscut) sc_probene_swiss=1;
	     if (1.0-et4swiss/rechitet>swisscut) sc_probet_swiss=1;

	     if (1.0-e4swisstt/rechitenergy>swisscut) sc_probene_swisstt=1;
	     if (1.0-et4swisstt/rechitet>swisscut) sc_probet_swisstt=1;



	     //     cout << "single channel threshold=" << thresh << endl;

	     int stripetcount=0;
	     int stripenecount=0;
	     
	     int alletcount=0;
	     int allenecount=0;
	     
	   
	     //     cout << "SC:  seed energy=" << rechitenergy << " ieta,iphi=" << sc_ieta 
	     //	  << "," << sc_iphi << " rechit time=" << rechittime << endl; 
	     // cout << "iphi_min=" << iphi_min << " ieta_min=" << ieta_min << endl;
	     
	     
	     for (Int_t x=ieta_min;x<ieta_min+5;x++) {
	       
	       stripetcount=0;
	       stripenecount=0;
	       

	       // cout << "STRIP no" << x-ieta_min+1 << endl;

	       for (Int_t y=iphi_min;y<iphi_min+5;y++) {
		 
		 int ietabin=x+86-1*int(x>0);
		 double myene=eventenergy->GetBinContent(y,ietabin);
		 double myet=eventet->GetBinContent(y,ietabin);
		 // double mytime=eventtime->GetBinContent(y,ietabin);
		 
		 if (myene>thresh) stripenecount++;
		 if (myet>thresh) stripetcount++;
		 
// 		 int etthr=0;

// 		 if (myet>thresh) etthr=1;

// 		 cout << "ieta=" << x << " iphi=" << y
// 		      << " ene=" << myene << " et=" << myet << " time=" << mytime << " above thresh=" << etthr << endl;
	       }
	       

	       
	       int stripetprob=0;
	       int stripeneprob=0;
	       
	       if (stripenecount>1) stripeneprob=1;
	       if (stripetcount>1) stripetprob=1;

	       //	       cout << "strips above threshold=" << stripetcount << "  strip LUT value=" << stripetprob << endl;
	       
	       sc_strip_prob_ene->Fill(thresh,stripeneprob);
	       sc_strip_prob_et->Fill(thresh,stripetprob);

	       sc_strip_prob_ene_pu->Fill(thresh,stripeneprob,numgoodvtx);
	       sc_strip_prob_et_pu->Fill(thresh,stripetprob,numgoodvtx);
	     
	       if (sctoweret>8.0) {

		 sc_strip_prob_ene_thresh08->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_thresh08->Fill(thresh,stripetprob);
		 
		 sc_strip_prob_ene_pu_thresh08->Fill(thresh,stripeneprob,numgoodvtx);
		 sc_strip_prob_et_pu_thresh08->Fill(thresh,stripetprob,numgoodvtx);

	       }


	       if (sctoweret>10.0) {

		 sc_strip_prob_ene_thresh10->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_thresh10->Fill(thresh,stripetprob);
		 
		 sc_strip_prob_ene_pu_thresh10->Fill(thresh,stripeneprob,numgoodvtx);
		 sc_strip_prob_et_pu_thresh10->Fill(thresh,stripetprob,numgoodvtx);

	       }

	       if (sctoweret>12.0) {

		 sc_strip_prob_ene_thresh12->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_thresh12->Fill(thresh,stripetprob);
		 
		 sc_strip_prob_ene_pu_thresh12->Fill(thresh,stripeneprob,numgoodvtx);
		 sc_strip_prob_et_pu_thresh12->Fill(thresh,stripetprob,numgoodvtx);

	       }

	       if (sctoweret>15.0) {

		 sc_strip_prob_ene_thresh15->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_thresh15->Fill(thresh,stripetprob);
		 
		 sc_strip_prob_ene_pu_thresh15->Fill(thresh,stripeneprob,numgoodvtx);
		 sc_strip_prob_et_pu_thresh15->Fill(thresh,stripetprob,numgoodvtx);

	       }


	       if (sctoweret>20.0) {

		 sc_strip_prob_ene_thresh20->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_thresh20->Fill(thresh,stripetprob);
		 
		 sc_strip_prob_ene_pu_thresh20->Fill(thresh,stripeneprob,numgoodvtx);
		 sc_strip_prob_et_pu_thresh20->Fill(thresh,stripetprob,numgoodvtx);

	       }


	       if (abs(sc_ieta)<=25) {
		 sc_strip_prob_ene_mod1->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_mod1->Fill(thresh,stripetprob);
	       }

	       if (abs(sc_ieta)>25 && abs(sc_ieta)<=45) {
		 sc_strip_prob_ene_mod2->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_mod2->Fill(thresh,stripetprob);
	       }

	       if (abs(sc_ieta)>45 && abs(sc_ieta)<=65) {
		 sc_strip_prob_ene_mod3->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_mod3->Fill(thresh,stripetprob);
	       }

	       if (abs(sc_ieta)>65) {
		 sc_strip_prob_ene_mod4->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_mod4->Fill(thresh,stripetprob);
	       }

	       if (numgoodvtx<=10) {
		 sc_strip_prob_ene_pu0->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_pu0->Fill(thresh,stripetprob);
	       }

	       if (numgoodvtx>10 && numgoodvtx<=20) {
		 sc_strip_prob_ene_pu10->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_pu10->Fill(thresh,stripetprob);
	       }

 	       if (numgoodvtx>20) {
		 sc_strip_prob_ene_pu20->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_pu20->Fill(thresh,stripetprob);
	       }

	       if (rechitenergy>10) {
		 sc_strip_prob_ene_10->Fill(thresh,stripeneprob);
		 sc_strip_prob_et_10->Fill(thresh,stripetprob);
	       }



	       //  cout << " stripenecount=" << stripenecount << " stripetcount=" << stripetcount << endl;

	       allenecount+=stripeneprob;
	       alletcount+=stripetprob;

	     }


	     int sfgvb_tower_et=0;
	     int sfgvb_tower_ene=0;
	     
	     if (allenecount==0) sfgvb_tower_ene=1;
	     if (alletcount==0) sfgvb_tower_et=1;
	     
	     //	     cout << " **sFGVB value=" << 1-sfgvb_tower_et << "\n" <<  endl;
		       
	     sc_tower_spike_ene->Fill(thresh,sfgvb_tower_ene);
	     sc_tower_spike_et->Fill(thresh,sfgvb_tower_et);
 
	     sc_tower_spike_ene_pu->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	     sc_tower_spike_et_pu->Fill(thresh,sfgvb_tower_et,numgoodvtx);
 

	     sc_tower_spike_ene_swiss->Fill(swisscut,sc_probene_swiss);
	     sc_tower_spike_et_swiss->Fill(swisscut,sc_probet_swiss);
	     sc_tower_spike_ene_swisstt->Fill(swisscut,sc_probene_swisstt);
	     sc_tower_spike_et_swisstt->Fill(swisscut,sc_probet_swisstt);

	       
	       sc_tower_spike_ene_swiss_t10->Fill(swisscut,sc_probene_swiss_t10);
	       sc_tower_spike_ene_swiss_t07->Fill(swisscut,sc_probene_swiss_t07);
	       sc_tower_spike_ene_swiss_t05->Fill(swisscut,sc_probene_swiss_t05);
	       sc_tower_spike_ene_swiss_t04->Fill(swisscut,sc_probene_swiss_t04);
	       sc_tower_spike_ene_swiss_t03->Fill(swisscut,sc_probene_swiss_t03);
	       



	     if (sctoweret>8.0) {

	       sc_tower_spike_ene_thresh08->Fill(thresh,sfgvb_tower_ene);
	       sc_tower_spike_et_thresh08->Fill(thresh,sfgvb_tower_et);
	       
	       sc_tower_spike_ene_pu_thresh08->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       sc_tower_spike_et_pu_thresh08->Fill(thresh,sfgvb_tower_et,numgoodvtx);
 
	       sc_tower_spike_ene_swiss_thresh08->Fill(swisscut,sc_probene_swiss);
	       sc_tower_spike_et_swiss_thresh08->Fill(swisscut,sc_probet_swiss);
	       sc_tower_spike_ene_swisstt_thresh08->Fill(swisscut,sc_probene_swisstt);
	       sc_tower_spike_et_swisstt_thresh08->Fill(swisscut,sc_probet_swisstt);

	       sc_tower_spike_ene_swiss_t10_thresh08->Fill(swisscut,sc_probene_swiss_t10);
	       sc_tower_spike_ene_swiss_t07_thresh08->Fill(swisscut,sc_probene_swiss_t07);
	       sc_tower_spike_ene_swiss_t05_thresh08->Fill(swisscut,sc_probene_swiss_t05);
	       sc_tower_spike_ene_swiss_t04_thresh08->Fill(swisscut,sc_probene_swiss_t04);
	       sc_tower_spike_ene_swiss_t03_thresh08->Fill(swisscut,sc_probene_swiss_t03);
	       

	     }


	     if (sctoweret>10.0) {

	       sc_tower_spike_ene_thresh10->Fill(thresh,sfgvb_tower_ene);
	       sc_tower_spike_et_thresh10->Fill(thresh,sfgvb_tower_et);
	       
	       sc_tower_spike_ene_pu_thresh10->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       sc_tower_spike_et_pu_thresh10->Fill(thresh,sfgvb_tower_et,numgoodvtx);
 
	       sc_tower_spike_ene_swiss_thresh10->Fill(swisscut,sc_probene_swiss);
	       sc_tower_spike_et_swiss_thresh10->Fill(swisscut,sc_probet_swiss);
	       sc_tower_spike_ene_swisstt_thresh10->Fill(swisscut,sc_probene_swisstt);
	       sc_tower_spike_et_swisstt_thresh10->Fill(swisscut,sc_probet_swisstt);

	       sc_tower_spike_ene_swiss_t10_thresh10->Fill(swisscut,sc_probene_swiss_t10);
	       sc_tower_spike_ene_swiss_t07_thresh10->Fill(swisscut,sc_probene_swiss_t07);
	       sc_tower_spike_ene_swiss_t05_thresh10->Fill(swisscut,sc_probene_swiss_t05);
	       sc_tower_spike_ene_swiss_t04_thresh10->Fill(swisscut,sc_probene_swiss_t04);
	       sc_tower_spike_ene_swiss_t03_thresh10->Fill(swisscut,sc_probene_swiss_t03);
	       

	     }


	     if (sctoweret>12.0) {

	       sc_tower_spike_ene_thresh12->Fill(thresh,sfgvb_tower_ene);
	       sc_tower_spike_et_thresh12->Fill(thresh,sfgvb_tower_et);
	       
	       sc_tower_spike_ene_pu_thresh12->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       sc_tower_spike_et_pu_thresh12->Fill(thresh,sfgvb_tower_et,numgoodvtx);
 
	       sc_tower_spike_ene_swiss_thresh12->Fill(swisscut,sc_probene_swiss);
	       sc_tower_spike_et_swiss_thresh12->Fill(swisscut,sc_probet_swiss);
	       sc_tower_spike_ene_swisstt_thresh12->Fill(swisscut,sc_probene_swisstt);
	       sc_tower_spike_et_swisstt_thresh12->Fill(swisscut,sc_probet_swisstt);

	       sc_tower_spike_ene_swiss_t10_thresh12->Fill(swisscut,sc_probene_swiss_t10);
	       sc_tower_spike_ene_swiss_t07_thresh12->Fill(swisscut,sc_probene_swiss_t07);
	       sc_tower_spike_ene_swiss_t05_thresh12->Fill(swisscut,sc_probene_swiss_t05);
	       sc_tower_spike_ene_swiss_t04_thresh12->Fill(swisscut,sc_probene_swiss_t04);
	       sc_tower_spike_ene_swiss_t03_thresh12->Fill(swisscut,sc_probene_swiss_t03);
	       

	     }


	     if (sctoweret>15.0) {

	       sc_tower_spike_ene_thresh15->Fill(thresh,sfgvb_tower_ene);
	       sc_tower_spike_et_thresh15->Fill(thresh,sfgvb_tower_et);
	       
	       sc_tower_spike_ene_pu_thresh15->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       sc_tower_spike_et_pu_thresh15->Fill(thresh,sfgvb_tower_et,numgoodvtx);
 
	       sc_tower_spike_ene_swiss_thresh15->Fill(swisscut,sc_probene_swiss);
	       sc_tower_spike_et_swiss_thresh15->Fill(swisscut,sc_probet_swiss);
	       sc_tower_spike_ene_swisstt_thresh15->Fill(swisscut,sc_probene_swisstt);
	       sc_tower_spike_et_swisstt_thresh15->Fill(swisscut,sc_probet_swisstt);

	       sc_tower_spike_ene_swiss_t10_thresh15->Fill(swisscut,sc_probene_swiss_t10);
	       sc_tower_spike_ene_swiss_t07_thresh15->Fill(swisscut,sc_probene_swiss_t07);
	       sc_tower_spike_ene_swiss_t05_thresh15->Fill(swisscut,sc_probene_swiss_t05);
	       sc_tower_spike_ene_swiss_t04_thresh15->Fill(swisscut,sc_probene_swiss_t04);
	       sc_tower_spike_ene_swiss_t03_thresh15->Fill(swisscut,sc_probene_swiss_t03);
	       

	     }


	     if (sctoweret>20.0) {

	       sc_tower_spike_ene_thresh20->Fill(thresh,sfgvb_tower_ene);
	       sc_tower_spike_et_thresh20->Fill(thresh,sfgvb_tower_et);
	       
	       sc_tower_spike_ene_pu_thresh20->Fill(thresh,sfgvb_tower_ene,numgoodvtx);
	       sc_tower_spike_et_pu_thresh20->Fill(thresh,sfgvb_tower_et,numgoodvtx);
 
	       sc_tower_spike_ene_swiss_thresh20->Fill(swisscut,sc_probene_swiss);
	       sc_tower_spike_et_swiss_thresh20->Fill(swisscut,sc_probet_swiss);
	       sc_tower_spike_ene_swisstt_thresh20->Fill(swisscut,sc_probene_swisstt);
	       sc_tower_spike_et_swisstt_thresh20->Fill(swisscut,sc_probet_swisstt);

	       sc_tower_spike_ene_swiss_t10_thresh20->Fill(swisscut,sc_probene_swiss_t10);
	       sc_tower_spike_ene_swiss_t07_thresh20->Fill(swisscut,sc_probene_swiss_t07);
	       sc_tower_spike_ene_swiss_t05_thresh20->Fill(swisscut,sc_probene_swiss_t05);
	       sc_tower_spike_ene_swiss_t04_thresh20->Fill(swisscut,sc_probene_swiss_t04);
	       sc_tower_spike_ene_swiss_t03_thresh20->Fill(swisscut,sc_probene_swiss_t03);
	       
	     }


	       if (abs(sc_ieta)<=25) {
		 sc_tower_spike_ene_mod1->Fill(thresh,sfgvb_tower_ene);
		 sc_tower_spike_et_mod1->Fill(thresh,sfgvb_tower_et);
	       }

	       if (abs(sc_ieta)>25 && abs(sc_ieta)<=45) {
		 sc_tower_spike_ene_mod2->Fill(thresh,sfgvb_tower_ene);
		 sc_tower_spike_et_mod2->Fill(thresh,sfgvb_tower_et);
	       }

	       if (abs(sc_ieta)>45 && abs(sc_ieta)<=65) {
		 sc_tower_spike_ene_mod3->Fill(thresh,sfgvb_tower_ene);
		 sc_tower_spike_et_mod3->Fill(thresh,sfgvb_tower_et);
	       }

	       if (abs(sc_ieta)>65) {
		 sc_tower_spike_ene_mod4->Fill(thresh,sfgvb_tower_ene);
		 sc_tower_spike_et_mod4->Fill(thresh,sfgvb_tower_et);
	       }

	       if (numgoodvtx<=10) {
		 sc_tower_spike_ene_pu0->Fill(thresh,sfgvb_tower_ene);
		 sc_tower_spike_et_pu0->Fill(thresh,sfgvb_tower_et);
	       }

	       if (numgoodvtx>10 && numgoodvtx<=20) {
		 sc_tower_spike_ene_pu10->Fill(thresh,sfgvb_tower_ene);
		 sc_tower_spike_et_pu10->Fill(thresh,sfgvb_tower_et);
	       }

	       if (numgoodvtx>20) {
		 sc_tower_spike_ene_pu20->Fill(thresh,sfgvb_tower_ene);
		 sc_tower_spike_et_pu20->Fill(thresh,sfgvb_tower_et);
	       }

	       if (rechitenergy>10) {
		 sc_tower_spike_ene_10->Fill(thresh,sfgvb_tower_ene);
		 sc_tower_spike_et_10->Fill(thresh,sfgvb_tower_et);
	       }




	   }
	 }
      }
    }
       
       
       
       
       
  }
     

     
    

	






  if (numgt1>0) {

    tmean_en=ensum1/ensum2;
    terr_en=sqrt(ensum3/ensum4);

    tmean_sig=errsum1/errsum2;
    terr_sig=sqrt(1/errsum2);

  }




  // EE rechits


  float_t badsc1_et=0;
  Int_t badsc1_hits=0;
  
  float_t badsc2_et=0;
  Int_t badsc2_hits=0;
  

  for (EcalRecHitCollection::const_iterator hitItr = EEhits->begin(); hitItr != EEhits->end(); ++hitItr) {
    
    EcalRecHit hit = (*hitItr);
    EEDetId det = hit.id(); 
    
    // int dee=0;
    
    //   float lasercalib_ee=laser->getLaserCorrection(EEDetId(det), event.time());
    
    float ampli = hit.energy();
    float time     = hit.time()-toffset;
    int   ebflag   = hit.recoFlag();
    // float_t chi2   = hit.chi2();
    // float_t chi2oot= hit.outOfTimeChi2();
    
    
    int eehashedid = det.hashedIndex();
    
    const EcalPedestals::Item *aped=0;
    aped=&eped->endcap(eehashedid);
    float pedg12=aped->mean_x12;
    //cout << "EE pedestal g12=" << pedg12 << endl;
    
    
    
    GlobalPoint posee=geo->getPosition(hit.detid());
    float eta_ee=posee.eta();
    float phi_ee=posee.phi();
    float pf=1.0/cosh(eta_ee);
    float etee=ampli*pf;
    int ix=det.ix();
    int iy=det.iy();
    int side=det.zside();
    
    int iz=0;
    if (side==1) iz=1;
    if (side==-1) iz=-1;
    
    
  
    float ee_r4a=0;
    float ee4x1a=0;
    
    if (pTopology.isValid() && pG.isValid()) { 
      
      // const CaloTopology *topology=pTopology.product();
      
      //const reco::BasicCluster *cluster=0;
      /*
        ee4x1a+=EcalClusterTools::matrixEnergy(*cluster,eeRecHits,topology,hit.id(),-1,-1,0,0);
        ee4x1a+=EcalClusterTools::matrixEnergy(*cluster,eeRecHits,topology,hit.id(),1,1,0,0);
        ee4x1a+=EcalClusterTools::matrixEnergy(*cluster,eeRecHits,topology,hit.id(),0,0,-1,-1);
        ee4x1a+=EcalClusterTools::matrixEnergy(*cluster,eeRecHits,topology,hit.id(),0,0,+1,+1);
      */
    }
    
    
    if (ampli!=0) ee_r4a=ee4x1a/ampli;
    ee_r4a=1-ee_r4a;
    
    




    Int_t ee_kGood=0;
    //           Int_t ee_kPoorReco=0;
    //            Int_t ee_kOutOfTime=0;
    //            Int_t ee_kFaultyHardware=0;
    //            Int_t ee_kNoisy=0;
    //            Int_t ee_kPoorCalib=0;
    //            Int_t ee_kSaturated=0;
    //            Int_t ee_kLeadingEdgeRecovered=0;
    //            Int_t ee_kNeighboursRecovered=0;
    //            Int_t ee_kTowerRecovered=0;
    //            Int_t ee_kDead=0;
    //            Int_t ee_kKilled=0;
    //            Int_t ee_kTPSaturated=0;
    //            Int_t ee_kL1SpikeFlag=0;
    Int_t ee_kWeird=0;
    //            Int_t ee_kDiWeird=0;
    //            Int_t ee_kUnknown=0;
    
    
    
    if (hit.checkFlag(EcalRecHit::kGood)) ee_kGood=1;
    // 	   if (hit.checkFlag(EcalRecHit::kPoorReco)) ee_kPoorReco=1;
    // 	   if (hit.checkFlag(EcalRecHit::kOutOfTime)) ee_kOutOfTime=1;
    // 	   if (hit.checkFlag(EcalRecHit::kFaultyHardware)) ee_kFaultyHardware=1;
    // 	   if (hit.checkFlag(EcalRecHit::kNoisy)) ee_kNoisy=1;
    // 	   if (hit.checkFlag(EcalRecHit::kPoorCalib)) ee_kPoorCalib=1;
    // 	   if (hit.checkFlag(EcalRecHit::kSaturated)) ee_kSaturated=1;
    // 	   if (hit.checkFlag(EcalRecHit::kLeadingEdgeRecovered)) ee_kLeadingEdgeRecovered=1;
    // 	   if (hit.checkFlag(EcalRecHit::kNeighboursRecovered)) ee_kNeighboursRecovered=1;
    // 	   if (hit.checkFlag(EcalRecHit::kTowerRecovered)) ee_kTowerRecovered=1;
    // 	   if (hit.checkFlag(EcalRecHit::kDead)) ee_kDead=1;
    // 	   if (hit.checkFlag(EcalRecHit::kKilled)) ee_kKilled=1;
    // 	   if (hit.checkFlag(EcalRecHit::kTPSaturated)) ee_kTPSaturated=1;
    // 	   if (hit.checkFlag(EcalRecHit::kL1SpikeFlag)) ee_kL1SpikeFlag=1;
    if (hit.checkFlag(EcalRecHit::kWeird)) ee_kWeird=1;
    // 	   if (hit.checkFlag(EcalRecHit::kDiWeird)) ee_kDiWeird=1;
    // 	   if (hit.checkFlag(EcalRecHit::kUnknown)) ee_kUnknown=1;
    
    
    
    //   if (EcalTools::isNextToDeadFromNeighbours(det,*cstat,12)) ee_kGood=0;
    // if (EEDetId::isNextToRingBoundary(det)) ee_kGood=0;
    
    
    
    
    // mask out noisy channels in run D
    
    // if (ix==58 && iy==94 && iz==-1)  ee_kWeird=1;
    
    
    if (!(ee_kWeird)) {


      if (etee>0.1) eetime_vs_bxtrain_01->Fill(bunchintrain+0.5,time);
      if (etee>0.5) eetime_vs_bxtrain_05->Fill(bunchintrain+0.5,time);
      
      
      if (ampli>1.0) { 
	eesum_gt1+=ampli;
	eehits1GeV++;
      }
      
      if (ampli>2.0) {
	eesum_gt2+=ampli;
	eehits2GeV++;
      }
      
      if (ampli>4.0) {
	eesum_gt4+=ampli;
	eehits4GeV++;
      }
      
      
      if (etee>1.0) {
	eesum_gt1et+=etee;
	eehits1GeVet++;
      }
      
      
      if (etee>2.0) {
	eesum_gt2et+=etee;
	eehits2GeVet++;
      }
      
      if (etee>4.0) {
	eesum_gt4et+=etee;
	eehits4GeVet++;
      }
      
      
      
      // rechit et sums
      
      rechitsumet_ee_all+=etee;
      if (etee>0.1) rechitsumet_ee_01+=etee;
      if (etee>0.5) rechitsumet_ee_05+=etee;
      
      if (etee>0.1 && etee<=0.5) rechitsumet_ee_0105+=etee;
      if (etee>0.5 && etee<=3.0) rechitsumet_ee_0530+=etee;
      
      
      if (etee>0.1) eenumrechits_01++;
      if (etee>0.5) eenumrechits_05++;
      
      
      if (etee>0.1 && etee<=0.5) eenumrechits_0105++;
      if (etee>0.5 && etee<=3.0) eenumrechits_0530++;
      
      if (etee>0.1) rechiteta_vs_bxtrain_01->Fill(bunchintrain,eta_ee);
      if (etee>0.5) rechiteta_vs_bxtrain_05->Fill(bunchintrain,eta_ee);
      
      
      
      // do digi step
      
      if (etee>0.1) {
	
	float pedoff=0;
	
	if (isMC_) pedoff=0;
	if (!isMC_) pedoff=200-pedg12;
	
	
	//cout << "EE DIGI printout:\n\n";
	
	//  cout << "rechit energy=" << ampli << " rechit et=" << etee << endl; 
	//cout << "SAMPLE   ADC   GAIN\n";
	
	Int_t eedigiadc=0;
	// Int_t eedigigain=0;
	// char mytxtee[80];
	
	
	EEDigiCollection::const_iterator digiItrEE= EEdigis->begin();
	while(digiItrEE != EEdigis->end() && digiItrEE->id() != hitItr->id())
	  {
	    digiItrEE++;
	  }
	
	if (digiItrEE != EEdigis->end()) {
	  
	  EEDataFrame df(*digiItrEE);
	  for(int i=0; i<10;++i)
	    {
	      eedigiadc=df.sample(i).adc();
	      eedigiadc+=pedoff;
	      //eedigigain=df.sample(i).gainId();
	      //		sprintf(mytxtee,"  %02d    %04d     %d",i+1,eedigiadc,eedigigain);
	      //		cout << mytxtee << endl;
	      
	      
	      ee_digi_01->Fill(i+0.5,eedigiadc);
		
		if (etee>0.5) ee_digi_05->Fill(i+0.5,eedigiadc);


		if (etee>0.1 && etee<=0.5) {

		  ee_digi_0105->Fill(i+0.5,eedigiadc);
		  ee_digi_0105_vs_time->Fill(time,i+0.5,float(eedigiadc));
		  ee_digi_0105_vs_bxtrain->Fill(bunchintrain+0.5,i+0.5,float(eedigiadc));

		  ee_digi_0105_vs_time_norm->Fill(time,i+0.5,1.0);
		  ee_digi_0105_vs_bxtrain_norm->Fill(bunchintrain+0.5,i+0.5,1.0);


		  if (abs(ee_eta)<2.0) {

		    ee_digi_0105_vs_time_eta15->Fill(time,i+0.5,float(eedigiadc));
		    ee_digi_0105_vs_bxtrain_eta15->Fill(bunchintrain+0.5,i+0.5,float(eedigiadc));
		    
		    ee_digi_0105_vs_time_norm_eta15->Fill(time,i+0.5,1.0);
		    ee_digi_0105_vs_bxtrain_norm_eta15->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }


		  if (abs(ee_eta)>=2.0 && abs(ee_eta)<2.5) {

		    ee_digi_0105_vs_time_eta20->Fill(time,i+0.5,float(eedigiadc));
		    ee_digi_0105_vs_bxtrain_eta20->Fill(bunchintrain+0.5,i+0.5,float(eedigiadc));
		    
		    ee_digi_0105_vs_time_norm_eta20->Fill(time,i+0.5,1.0);
		    ee_digi_0105_vs_bxtrain_norm_eta20->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }


		  if (abs(ee_eta)>=2.5) {

		    ee_digi_0105_vs_time_eta25->Fill(time,i+0.5,float(eedigiadc));
		    ee_digi_0105_vs_bxtrain_eta25->Fill(bunchintrain+0.5,i+0.5,float(eedigiadc));
		    
		    ee_digi_0105_vs_time_norm_eta25->Fill(time,i+0.5,1.0);
		    ee_digi_0105_vs_bxtrain_norm_eta25->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }






		}

		if (etee>0.5 && etee<=3.0) {

		  ee_digi_0530->Fill(i+0.5,eedigiadc);
		  ee_digi_0530_vs_time->Fill(time,i+0.5,float(eedigiadc));
		  ee_digi_0530_vs_bxtrain->Fill(bunchintrain+0.5,i+0.5,float(eedigiadc));

		  ee_digi_0530_vs_time_norm->Fill(time,i+0.5,1.0);
		  ee_digi_0530_vs_bxtrain_norm->Fill(bunchintrain+0.5,i+0.5,1.0);



		  if (abs(ee_eta)<2.0) {

		    ee_digi_0530_vs_time_eta15->Fill(time,i+0.5,float(eedigiadc));
		    ee_digi_0530_vs_bxtrain_eta15->Fill(bunchintrain+0.5,i+0.5,float(eedigiadc));
		    
		    ee_digi_0530_vs_time_norm_eta15->Fill(time,i+0.5,1.0);
		    ee_digi_0530_vs_bxtrain_norm_eta15->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }


		  if (abs(ee_eta)>=2.0 && abs(ee_eta)<2.5) {

		    ee_digi_0530_vs_time_eta20->Fill(time,i+0.5,float(eedigiadc));
		    ee_digi_0530_vs_bxtrain_eta20->Fill(bunchintrain+0.5,i+0.5,float(eedigiadc));
		    
		    ee_digi_0530_vs_time_norm_eta20->Fill(time,i+0.5,1.0);
		    ee_digi_0530_vs_bxtrain_norm_eta20->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }


		  if (abs(ee_eta)>=2.5) {

		    ee_digi_0530_vs_time_eta25->Fill(time,i+0.5,float(eedigiadc));
		    ee_digi_0530_vs_bxtrain_eta25->Fill(bunchintrain+0.5,i+0.5,float(eedigiadc));
		    
		    ee_digi_0530_vs_time_norm_eta25->Fill(time,i+0.5,1.0);
		    ee_digi_0530_vs_bxtrain_norm_eta25->Fill(bunchintrain+0.5,i+0.5,1.0);

		  }




		}


	      }
	    
	  }

	}
	





	   }

 

	   if (ampli>eemax && ee_kGood) {
	     eemax=ampli;
	     eemaxet=etee;
	     eeix=ix;      
	     eeiy=iy;
	     eeiz=side;
	     ee_eta=eta_ee;
	     ee_phi=phi_ee;
	     eetime=time;
	     eeflags=ebflag;
	     maxeehit=*hitItr;
	     
	   }
	   
	   if (etee>eemaxet2 && ee_kGood) {
	     eemax2=ampli;
	     eemaxet2=etee;
	     eeix2=ix;      
	     eeiy2=iy;
	     eeiz2=side;
	     ee_eta2=eta_ee;
	     ee_phi2=phi_ee;
	     eetime2=time;
	     eeflags2=ebflag;
	     maxeehit2=*hitItr;
	     
	   }



      Float_t ix_tmp=ix-0.5;
      if (side==-1) ix_tmp=ix_tmp+100;
      Float_t iy_tmp=iy-0.5;


      //   goodevent=true;

      if (!(ee_kWeird))  {

	//   cout << "EErechit" << endl;





	

        ee_rechitenergy_->Fill(ampli);



	rechiteta_all->Fill(eta_ee);
	rechiteta_etweight->Fill(eta_ee,etee);

	if (etee>1.0) {
	  rechiteta_gt1et->Fill(eta_ee);
	  rechiteta_etweight_gt1et->Fill(eta_ee,etee);

	  if (numgoodvtx<10) 	  rechiteta_gt1et_pu10->Fill(eta_ee);
	  if (numgoodvtx>=10 && numgoodvtx<20) 	  rechiteta_gt1et_pu20->Fill(eta_ee);
	  if (numgoodvtx>20) 	  rechiteta_gt1et_pu30->Fill(eta_ee);

	}





    
        if (fabs(eta_ee)<1.6)    ee_rechitenergy_16->Fill(ampli);
        if (fabs(eta_ee)>=1.6 && fabs(eta_ee)<1.8)    ee_rechitenergy_18->Fill(ampli);
        if (fabs(eta_ee)>=1.8 && fabs(eta_ee)<2.0)    ee_rechitenergy_20->Fill(ampli);
        if (fabs(eta_ee)>=2.0 && fabs(eta_ee)<2.2)    ee_rechitenergy_22->Fill(ampli);
        if (fabs(eta_ee)>=2.2 && fabs(eta_ee)<2.4)    ee_rechitenergy_24->Fill(ampli);
        if (fabs(eta_ee)>=2.4 && fabs(eta_ee)<2.6)    ee_rechitenergy_26->Fill(ampli);
        if (fabs(eta_ee)>=2.6 && fabs(eta_ee)<2.8)    ee_rechitenergy_28->Fill(ampli);
        if (fabs(eta_ee)>=2.8)    ee_rechitenergy_30->Fill(ampli);

	ee_rechitet_->Fill(etee);

	if (fabs(eta_ee)<1.6)    ee_rechitet_16->Fill(etee);
        if (fabs(eta_ee)>=1.6 && fabs(eta_ee)<1.8)    ee_rechitet_18->Fill(etee);
        if (fabs(eta_ee)>=1.8 && fabs(eta_ee)<2.0)    ee_rechitet_20->Fill(etee);
        if (fabs(eta_ee)>=2.0 && fabs(eta_ee)<2.2)    ee_rechitet_22->Fill(etee);
        if (fabs(eta_ee)>=2.2 && fabs(eta_ee)<2.4)    ee_rechitet_24->Fill(etee);
        if (fabs(eta_ee)>=2.4 && fabs(eta_ee)<2.6)    ee_rechitet_26->Fill(etee);
        if (fabs(eta_ee)>=2.6 && fabs(eta_ee)<2.8)    ee_rechitet_28->Fill(etee);
        if (fabs(eta_ee)>=2.8)    ee_rechitet_30->Fill(etee);


	if (fabs(eta_ee)<2.0) ee_rechitetvspu_20->Fill(float(numgoodvtx)-0.5,etee);
	if (fabs(eta_ee)>=2.0 && fabs(eta_ee)<2.5) ee_rechitetvspu_25->Fill(float(numgoodvtx)-0.5,etee);
	if (fabs(eta_ee)>=2.5) ee_rechitetvspu_30->Fill(float(numgoodvtx)-0.5,etee);
 

	//     ee_rechiten_vs_sm->Fill(dee-0.5,ampli);
	// ee_rechitet_vs_sm->Fill(dee-0.5,etee);

        if (eeRecHits->size()>500)  ee_rechitenergy_notypeb_->Fill(ampli);

	if (side==1) {

           eep_rechiten_vs_eta->Fill(fabs(eta_ee),ampli);
           eep_rechiten_vs_phi->Fill(phi_ee,ampli);

           eep_rechitet_vs_eta->Fill(fabs(eta_ee),etee);
           eep_rechitet_vs_phi->Fill(phi_ee,etee);

        }

        if (side==-1) {

           eem_rechiten_vs_eta->Fill(fabs(eta_ee),ampli);
           eem_rechiten_vs_phi->Fill(phi_ee,ampli);
           eem_rechitet_vs_eta->Fill(fabs(eta_ee),etee);
           eem_rechitet_vs_phi->Fill(phi_ee,etee);
 
        }


         eeocc->Fill(ix_tmp,iy_tmp,1.);
         eeoccen->Fill(ix_tmp,iy_tmp,ampli);
         eeoccet->Fill(ix_tmp,iy_tmp,etee);

         if (etee>1.0) {

            eeoccgt1et->Fill(ix_tmp,iy_tmp,1.);
            eeoccetgt1et->Fill(ix_tmp,iy_tmp,etee);
           
         }

        if (ampli>1.0) {

            eeoccgt1->Fill(ix_tmp,iy_tmp,1.);
            eeoccengt1->Fill(ix_tmp,iy_tmp,ampli);
           
         }


	//int sevlev=severity->severityLevel(det,*eeRecHits);


	   bool badsc1=false;
	   bool badsc2=false;

	   if (ix>=21 && ix<=25 && iy>=21 && iy<=25 && iz==-1) badsc1=true;
	   if (ix>=46 && ix<=50 && iy>=96 && iy<=100 && iz==+1) badsc2=true;





	   if (badsc1) {

	     badsc1_et+=etee;
	     if (ampli>1000.0 && ee_kGood==0) badsc1_hits++;

	   }

	   if (badsc2) {

	     badsc2_et+=etee;
	     if (ampli>1000.0 && ee_kGood==0) badsc2_hits++;

	   }





   

      eehits++;
      if (side==1) eephits++;
      if (side==-1) eemhits++;

      
	   }

  }
  

  // end of ee loop

  
  // Rechit flags:

  if (maxebhit.recoFlag()==EcalRecHit::kGood) ebflag_kgood=1;
  if (maxebhit.recoFlag()==EcalRecHit::kPoorReco) ebflag_kpoorreco=1;
  if (maxebhit.recoFlag()==EcalRecHit::kOutOfTime) ebflag_koutoftime=1;
  // if (maxebhit.recoFlag()==EcalRecHit::kFake) ebflag_kfake=1;



      

 if (pTopology.isValid() && pG.isValid()) { 

   //    const CaloTopology *topology=pTopology.product();




   

   //   const reco::BasicCluster *cluster=0;
    float e3x3=0; //EcalClusterTools::matrixEnergy(*cluster,ebRecHits,topology,maxebhit.id(),-1,1,-1,1);


    // float e5x5=0; //EcalClusterTools::matrixEnergy(*cluster,ebRecHits,topology,maxebhit.id(),-2,2,-2,2);

    eb_e9=0; //e3x3;
    eb_e25=0; //e5x5;

   
    

    float e4x1=0;

    //    e4x1+=EcalClusterTools::matrixEnergy(*cluster,ebRecHits,topology,maxebhit.id(),-1,-1,0,0);

    //    e4x1+=EcalClusterTools::matrixEnergy(*cluster,ebRecHits,topology,maxebhit.id(),1,1,0,0);

    //    e4x1+=EcalClusterTools::matrixEnergy(*cluster,ebRecHits,topology,maxebhit.id(),0,0,-1,-1);

    //    e4x1+=EcalClusterTools::matrixEnergy(*cluster,ebRecHits,topology,maxebhit.id(),0,0,+1,+1);




   
    if (e3x3>0) eb_r9=ebmax/e3x3;

    if (ebmax!=0) eb_r4=e4x1/ebmax;




    //  const reco::BasicCluster *cluster2=0;
     float e3x32=0; //EcalClusterTools::matrixEnergy(*cluster2,ebRecHits,topology,maxebhit2.id(),-1,1,-1,1);

    float e4x12=0;
 
    //    e4x12+=EcalClusterTools::matrixEnergy(*cluster2,ebRecHits,topology,maxebhit2.id(),-1,-1,0,0);

    //e4x12+=EcalClusterTools::matrixEnergy(*cluster2,ebRecHits,topology,maxebhit2.id(),1,1,0,0);

    //e4x12+=EcalClusterTools::matrixEnergy(*cluster2,ebRecHits,topology,maxebhit2.id(),0,0,-1,-1);

    // e4x12+=EcalClusterTools::matrixEnergy(*cluster2,ebRecHits,topology,maxebhit2.id(),0,0,+1,+1);


   
    if (e3x32>0) eb_r92=ebmax2/e3x32;

    if (ebmax2!=0) eb_r42=e4x12/ebmax2;




    //const reco::BasicCluster *clusteree=0;
    float e3x3ee=0; //EcalClusterTools::matrixEnergy(*clusteree,eeRecHits,topology,maxeehit.id(),-1,1,-1,1);


   
    if (e3x3ee>0) ee_r9=eemax/e3x3ee;


    //const reco::BasicCluster *clusteree2=0;
    float e3x3ee2=0; //EcalClusterTools::matrixEnergy(*clusteree2,eeRecHits,topology,maxeehit2.id(),-1,1,-1,1);


   
    if (e3x3ee2>0) ee_r92=eemax2/e3x3ee2;





  
 }




  dataTree_->Fill();

  //   }

  /*


 // cout << "begin jets" << endl;


  edm::Handle<PFJetCollection> jets;
  PFJetCollection::const_iterator i_jet;
  event.getByLabel (jets_,jets);
  std::vector<reco::PFCandidatePtr> PFJetPart;
  
  histo_event_->Fill(event.id().event());





  //  const JetCorrector* corrector = JetCorrector::getJetCorrector(jetcorr_,iSetup);
    

  int njet(0);
  if (jets->size()>0)
    {
      for (i_jet = jets->begin();i_jet != jets->end(); i_jet++)
       {    
              	
	   PFJetPart  = i_jet->getPFConstituents();
	  
	   //       scale=corrector->correction(i_jet->p4());
           scale=1.0;
           ncr_ = PFJetPart.size() ;

	   energy_pf_     = 0;
	   energyc_pf_    = 0;
	   energyn_pf_    = 0;
	   energyg_pf_    = 0;
	    
           energy_ecal    = 0;
           energy_hcal    = 0;

	   	   		  
	  for(Int_t j=0;j<PFJetPart.size();j++){
	    
	    PFCandidate::ParticleType type = PFJetPart[j]->particleId();
            int tpid  = PFJetPart[j]->translateTypeToPdgId(type);
	       
	    bool chargedp   = false;
	    bool neutralp   = false;
	    bool gammap     = false;
	    bool electronsp = false;
	   	      	    
	    chargedp   = TMath::Abs(tpid)==211 || TMath::Abs(tpid) == 321 || TMath::Abs(tpid)  == 2212 || TMath::Abs(tpid) == 3122|| TMath::Abs(tpid) == 3312|| TMath::Abs(tpid) == 3112 || TMath::Abs(tpid) == 3222;
	    neutralp   = TMath::Abs(tpid)==130 || TMath::Abs(tpid) == 2112 || TMath::Abs(tpid) == 310  || TMath::Abs(tpid) == 3322;
	    gammap     = TMath::Abs(tpid)==22;
	    electronsp = TMath::Abs(tpid)==11;
	    		    	   	  		   energy_pf_  = energy_pf_+ PFJetPart[j]->p4().energy(); 	 	  
	    if(chargedp)           energyc_pf_ = energyc_pf_+PFJetPart[j]->p4().energy();
	    if(neutralp)           energyn_pf_ = energyn_pf_+PFJetPart[j]->p4().energy();
	    if(gammap||electronsp) energyg_pf_ = energyg_pf_+PFJetPart[j]->p4().energy();
             //	    	   	    
            energy_ecal = energy_ecal + PFJetPart[j]->ecalEnergy();
            energy_hcal = energy_hcal + PFJetPart[j]->hcalEnergy();


	  }
	  	
          ptJet_      = i_jet->pt()*scale;
          etaJet_     = i_jet->eta();
          phiJet_     = i_jet->phi();
          chfJet_     = i_jet->chargedHadronEnergyFraction();
          nhfJet_     = i_jet->neutralHadronEnergyFraction();
          cemfJet_    = i_jet->chargedEmEnergyFraction();
          nemfJet_    = i_jet->neutralEmEnergyFraction();
          cmultiJet_  = i_jet->chargedMultiplicity();
          nmultiJet_  = i_jet->neutralMultiplicity();
         
	
          nrjets_     = jets->size();
         
	  dataTree_->Fill();
          njet++;
             
          PFJetPart.clear();
         
       }  
  
    }
 
  */
  
  //  cout << "end jets" << endl;
      
}
//////////////////////////////////////////////////////////////////////////////////////////
SpikeAnalyserMC::~SpikeAnalyserMC() 
{
  delete file_;
  delete dataTree_;
}
//}

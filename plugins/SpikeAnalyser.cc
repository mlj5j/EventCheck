#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

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

#include "DataFormats/EcalDigi/interface/EcalTrigPrimCompactColl.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h"
 
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"

#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/DataRecord/interface/EcalPedestalsRcd.h"

#include "CondFormats/EcalObjects/interface/EcalPedestals.h"


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


#include "JetMETCorrections/MCJet/plugins/SpikeAnalyser.h" 
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

DEFINE_FWK_MODULE(SpikeAnalyser);


SpikeAnalyser::SpikeAnalyser(const edm::ParameterSet& cfg)
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
void SpikeAnalyser::beginJob() 
{

  file_          = new TFile(histogramFile_.c_str(),"RECREATE");
  dataTree_      = new TTree("dataTree","dataTree");
  dataTree2_      = new TTree("dataTree2","dataTree2");

 // run/event info
  
  dataTree_->Branch("run",          &run,          "run/I");  
  dataTree_->Branch("ev",           &ev,    	   "ev/I");
  dataTree_->Branch("lumi",         &lumi,     	   "lumi/I");
  dataTree_->Branch("bx",           &bx,       	   "bx/I");   
  dataTree_->Branch("orbit",        &orbit,    	   "orbit/I");
  dataTree_->Branch("bunchintrain", &bunchintrain, "bunchintrain/I");

 //  primary vertex info

  dataTree_->Branch("numvtx",      &numvtx,	  "numvtx/I");
  dataTree_->Branch("numgoodvtx",  &numgoodvtx,	  "numgoodvtx/I");


  // rechit info

  dataTree_->Branch("rechit_ieta",       &rechit_ieta,       "rechit_ieta/I");
  dataTree_->Branch("rechit_iphi",       &rechit_iphi,       "rechit_iphi/I");
  dataTree_->Branch("rechit_eta",        &rechit_eta,        "rechit_eta/F");
  dataTree_->Branch("rechit_phi",        &rechit_phi,        "rechit_phi/F");


  dataTree_->Branch("rechit_ene_weights53",             &rechit_ene_weights53,             "rechit_ene_weights53/F");
  dataTree_->Branch("rechit_et_weights53",              &rechit_et_weights53,              "rechit_et_weights53/F");
  dataTree_->Branch("rechit_time_weights53",            &rechit_time_weights53,            "rechit_time_weights53/F");
  dataTree_->Branch("rechit_swisscross_weights53",      &rechit_swisscross_weights53,      "rechit_swisscross_weights53/F");
  dataTree_->Branch("rechit_chi2_weights53",            &rechit_chi2_weights53,            "rechit_chi2_weights53/F");
  dataTree_->Branch("rechit_flag_kweird_weights53",     &rechit_flag_kweird_weights53,     "rechit_flag_kweird_weights53/I");
  dataTree_->Branch("rechit_flag_kdiweird_weights53",   &rechit_flag_kdiweird_weights53,   "rechit_flag_kdiweird_weights53/I");
  dataTree_->Branch("rechit_flag_koot_weights53",       &rechit_flag_koot_weights53,       "rechit_flag_koot_weights53/I");


  dataTree_->Branch("rechit_ene_weights74",             &rechit_ene_weights74,             "rechit_ene_weights74/F");
  dataTree_->Branch("rechit_et_weights74",              &rechit_et_weights74,              "rechit_et_weights74/F");
  dataTree_->Branch("rechit_time_weights74",            &rechit_time_weights74,            "rechit_time_weights74/F");
  dataTree_->Branch("rechit_swisscross_weights74",      &rechit_swisscross_weights74,      "rechit_swisscross_weights74/F");
  dataTree_->Branch("rechit_chi2_weights74",            &rechit_chi2_weights74,            "rechit_chi2_weights74/F");
  dataTree_->Branch("rechit_flag_kweird_weights74",     &rechit_flag_kweird_weights74,     "rechit_flag_kweird_weights74/I");
  dataTree_->Branch("rechit_flag_kdiweird_weights74",   &rechit_flag_kdiweird_weights74,   "rechit_flag_kdiweird_weights74/I");
  dataTree_->Branch("rechit_flag_koot_weights74",       &rechit_flag_koot_weights74,       "rechit_flag_koot_weights74/I");


  dataTree_->Branch("rechit_ene_multi5",             &rechit_ene_multi5,             "rechit_ene_multi5/F");
  dataTree_->Branch("rechit_et_multi5",              &rechit_et_multi5,              "rechit_et_multi5/F");
  dataTree_->Branch("rechit_time_multi5",            &rechit_time_multi5,            "rechit_time_multi5/F");
  dataTree_->Branch("rechit_swisscross_multi5",      &rechit_swisscross_multi5,      "rechit_swisscross_multi5/F");
  dataTree_->Branch("rechit_chi2_multi5",            &rechit_chi2_multi5,            "rechit_chi2_multi5/F");
  dataTree_->Branch("rechit_flag_kweird_multi5",     &rechit_flag_kweird_multi5,     "rechit_flag_kweird_multi5/I");
  dataTree_->Branch("rechit_flag_kdiweird_multi5",   &rechit_flag_kdiweird_multi5,   "rechit_flag_kdiweird_multi5/I");
  dataTree_->Branch("rechit_flag_koot_multi5",       &rechit_flag_koot_multi5,       "rechit_flag_koot_multi5/I");

  dataTree_->Branch("rechit_ene_multi10",             &rechit_ene_multi10,             "rechit_ene_multi10/F");
  dataTree_->Branch("rechit_et_multi10",              &rechit_et_multi10,              "rechit_et_multi10/F");
  dataTree_->Branch("rechit_time_multi10",            &rechit_time_multi10,            "rechit_time_multi10/F");
  dataTree_->Branch("rechit_swisscross_multi10",      &rechit_swisscross_multi10,      "rechit_swisscross_multi10/F");
  dataTree_->Branch("rechit_chi2_multi10",            &rechit_chi2_multi10,            "rechit_chi2_multi10/F");
  dataTree_->Branch("rechit_flag_kweird_multi10",     &rechit_flag_kweird_multi10,     "rechit_flag_kweird_multi10/I");
  dataTree_->Branch("rechit_flag_kdiweird_multi10",   &rechit_flag_kdiweird_multi10,   "rechit_flag_kdiweird_multi10/I");
  dataTree_->Branch("rechit_flag_koot_multi10",       &rechit_flag_koot_multi10,       "rechit_flag_koot_multi10/I");

  dataTree_->Branch("rechit_ene_multi1",             &rechit_ene_multi1,             "rechit_ene_multi1/F");
  dataTree_->Branch("rechit_et_multi1",              &rechit_et_multi1,              "rechit_et_multi1/F");
  dataTree_->Branch("rechit_time_multi1",            &rechit_time_multi1,            "rechit_time_multi1/F");
  dataTree_->Branch("rechit_swisscross_multi1",      &rechit_swisscross_multi1,      "rechit_swisscross_multi1/F");
  dataTree_->Branch("rechit_chi2_multi1",            &rechit_chi2_multi1,            "rechit_chi2_multi1/F");
  dataTree_->Branch("rechit_flag_kweird_multi1",     &rechit_flag_kweird_multi1,     "rechit_flag_kweird_multi1/I");
  dataTree_->Branch("rechit_flag_kdiweird_multi1",   &rechit_flag_kdiweird_multi1,   "rechit_flag_kdiweird_multi1/I");
  dataTree_->Branch("rechit_flag_koot_multi1",       &rechit_flag_koot_multi1,       "rechit_flag_koot_multi1/I");

  dataTree_->Branch("rechit_flag_kweird_calc",       &rechit_flag_kweird_calc,       "rechit_flag_kweird_calc/I");

  dataTree_->Branch("rechit_swisscross_thresh",      &rechit_swisscross_thresh,      "rechit_swisscross_thresh/F");

   dataTree_->Branch("rechit_swisscross_zs_weights53",      &rechit_swisscross_zs_weights53,      "rechit_swisscross_zs_weights53/F");


   dataTree_->Branch("rechit_swisscross_zs_weights74",      &rechit_swisscross_zs_weights74,      "rechit_swisscross_zs_weights74/F");

   dataTree_->Branch("rechit_swisscross_zs_multi5",      &rechit_swisscross_zs_multi5,      "rechit_swisscross_zs_multi5/F");

   dataTree_->Branch("rechit_swisscross_zs_multi10",      &rechit_swisscross_zs_multi10,      "rechit_swisscross_zs_multi10/F");

   dataTree_->Branch("rechit_swisscross_zs_multi1",      &rechit_swisscross_zs_multi1,      "rechit_swisscross_zs_multi1/F");


   dataTree_->Branch("isnearcrack",       &isnearcrack,       "isnearcrack/I");


   dataTree_->Branch("uncalibrechit_multi10_ampl_eb", &uncalibrechit_multi10_ampl_eb, "uncalibrechit_multi10_ampl_eb/F");
   dataTree_->Branch("uncalibrechit_multi10_amperr_eb", &uncalibrechit_multi10_amperr_eb, "uncalibrechit_multi10_amperr_eb/F");
   dataTree_->Branch("uncalibrechit_multi10_pedestal_eb", &uncalibrechit_multi10_pedestal_eb, "uncalibrechit_multi10_pedestal_eb/F");
   dataTree_->Branch("uncalibrechit_multi10_jitter_eb", &uncalibrechit_multi10_jitter_eb, "uncalibrechit_multi10_jitter_eb/F");
   dataTree_->Branch("uncalibrechit_multi10_chi2_eb", &uncalibrechit_multi10_chi2_eb, "uncalibrechit_multi10_chi2_eb/F");
   dataTree_->Branch("uncalibrechit_multi10_ootampl_eb", &uncalibrechit_multi10_ootampl_eb, "uncalibrechit_multi10_ootampl_eb[10]/F");

   dataTree_->Branch("uncalibrechit_multi5_ampl_eb", &uncalibrechit_multi5_ampl_eb, "uncalibrechit_multi5_ampl_eb/F");
   dataTree_->Branch("uncalibrechit_multi5_amperr_eb", &uncalibrechit_multi5_amperr_eb, "uncalibrechit_multi5_amperr_eb/F");
   dataTree_->Branch("uncalibrechit_multi5_pedestal_eb", &uncalibrechit_multi5_pedestal_eb, "uncalibrechit_multi5_pedestal_eb/F");
   dataTree_->Branch("uncalibrechit_multi5_jitter_eb", &uncalibrechit_multi5_jitter_eb, "uncalibrechit_multi5_jitter_eb/F");
   dataTree_->Branch("uncalibrechit_multi5_chi2_eb", &uncalibrechit_multi5_chi2_eb, "uncalibrechit_multi5_chi2_eb/F");
   dataTree_->Branch("uncalibrechit_multi5_ootampl_eb", &uncalibrechit_multi5_ootampl_eb, "uncalibrechit_multi5_ootampl_eb[10]/F");

   dataTree_->Branch("uncalibrechit_multi1_ampl_eb", &uncalibrechit_multi1_ampl_eb, "uncalibrechit_multi1_ampl_eb/F");
   dataTree_->Branch("uncalibrechit_multi1_amperr_eb", &uncalibrechit_multi1_amperr_eb, "uncalibrechit_multi1_amperr_eb/F");
   dataTree_->Branch("uncalibrechit_multi1_pedestal_eb", &uncalibrechit_multi1_pedestal_eb, "uncalibrechit_multi1_pedestal_eb/F");
   dataTree_->Branch("uncalibrechit_multi1_jitter_eb", &uncalibrechit_multi1_jitter_eb, "uncalibrechit_multi1_jitter_eb/F");
   dataTree_->Branch("uncalibrechit_multi1_chi2_eb", &uncalibrechit_multi1_chi2_eb, "uncalibrechit_multi1_chi2_eb/F");
   dataTree_->Branch("uncalibrechit_multi1_ootampl_eb", &uncalibrechit_multi1_ootampl_eb, "uncalibrechit_multi1_ootampl_eb[10]/F");


  // db info

  dataTree_->Branch("db_pedg12",        &db_pedg12,        "db_pedg12/F");
  dataTree_->Branch("db_pedg6",         &db_pedg6,         "db_pedg6/F");
  dataTree_->Branch("db_pedg1",         &db_pedg1,         "db_pedg1/F");

  dataTree_->Branch("db_pedrmsg12",     &db_pedrmsg12,     "db_pedrmsg12/F");
  dataTree_->Branch("db_pedrmsg6",      &db_pedrmsg6,      "db_pedrmsg6/F");
  dataTree_->Branch("db_pedrmsg1",      &db_pedrmsg1,      "db_pedrmsg1/F");

  dataTree_->Branch("lasercorr",        &lasercorr,        "lasercorr/F");



  // digi info

  dataTree_->Branch("digi_adc", &digi_adc,  "digi_adc[10]/I"); 
  dataTree_->Branch("digi_gainid", &digi_gainid,  "digi_gainid[10]/I");




  // EE data tree

  dataTree2_->Branch("run",          &run,          "run/I");  
  dataTree2_->Branch("ev",           &ev,    	   "ev/I");
  dataTree2_->Branch("lumi",         &lumi,     	   "lumi/I");
  dataTree2_->Branch("bx",           &bx,       	   "bx/I");   
  dataTree2_->Branch("orbit",        &orbit,    	   "orbit/I");
  dataTree2_->Branch("bunchintrain", &bunchintrain, "bunchintrain/I");

 //  primary vertex info

  dataTree2_->Branch("numvtx",      &numvtx,	  "numvtx/I");
  dataTree2_->Branch("numgoodvtx",  &numgoodvtx,	  "numgoodvtx/I");


  // rechit info

  dataTree2_->Branch("rechit_ix",       &rechit_ix,       "rechit_ix/I");
  dataTree2_->Branch("rechit_iy",       &rechit_iy,       "rechit_ix/I");
  dataTree2_->Branch("rechit_iz",       &rechit_iz,       "rechit_iz/I");
  dataTree2_->Branch("rechit_eta",        &rechit_eta_ee,        "rechit_eta_ee/F");
  dataTree2_->Branch("rechit_phi",        &rechit_phi_ee,        "rechit_phi_ee/F");


  dataTree2_->Branch("rechit_ene_weights53",             &rechit_ene_weights53_ee,             "rechit_ene_weights53_ee/F");
  dataTree2_->Branch("rechit_et_weights53",              &rechit_et_weights53_ee,              "rechit_et_weights53_ee/F");
  dataTree2_->Branch("rechit_time_weights53",            &rechit_time_weights53_ee,            "rechit_time_weights53_ee/F");
  dataTree2_->Branch("rechit_chi2_weights53",            &rechit_chi2_weights53_ee,            "rechit_chi2_weights53_ee/F");
  dataTree2_->Branch("rechit_flag_kweird_weights53",     &rechit_flag_kweird_weights53_ee,     "rechit_flag_kweird_weights53_ee/I");
  dataTree2_->Branch("rechit_flag_kdiweird_weights53",   &rechit_flag_kdiweird_weights53_ee,   "rechit_flag_kdiweird_weights53_ee/I");
  dataTree2_->Branch("rechit_flag_koot_weights53",       &rechit_flag_koot_weights53_ee,       "rechit_flag_koot_weights53_ee/I");

  dataTree2_->Branch("rechit_ene_weights74",             &rechit_ene_weights74_ee,             "rechit_ene_weights74_ee/F");
  dataTree2_->Branch("rechit_et_weights74",              &rechit_et_weights74_ee,              "rechit_et_weights74_ee/F");
  dataTree2_->Branch("rechit_time_weights74",            &rechit_time_weights74_ee,            "rechit_time_weights74_ee/F");
  dataTree2_->Branch("rechit_chi2_weights74",            &rechit_chi2_weights74_ee,            "rechit_chi2_weights74_ee/F");
  dataTree2_->Branch("rechit_flag_kweird_weights74",     &rechit_flag_kweird_weights74_ee,     "rechit_flag_kweird_weights74_ee/I");
  dataTree2_->Branch("rechit_flag_kdiweird_weights74",   &rechit_flag_kdiweird_weights74_ee,   "rechit_flag_kdiweird_weights74_ee/I");
  dataTree2_->Branch("rechit_flag_koot_weights74",       &rechit_flag_koot_weights74_ee,       "rechit_flag_koot_weights74_ee/I");


  dataTree2_->Branch("rechit_ene_multi5",             &rechit_ene_multi5_ee,             "rechit_ene_multi5_ee/F");
  dataTree2_->Branch("rechit_et_multi5",              &rechit_et_multi5_ee,              "rechit_et_multi5_ee/F");
  dataTree2_->Branch("rechit_time_multi5",            &rechit_time_multi5_ee,            "rechit_time_multi5_ee/F");
  dataTree2_->Branch("rechit_chi2_multi5",            &rechit_chi2_multi5_ee,            "rechit_chi2_multi5_ee/F");
  dataTree2_->Branch("rechit_flag_kweird_multi5",     &rechit_flag_kweird_multi5_ee,     "rechit_flag_kweird_multi5_ee/I");
  dataTree2_->Branch("rechit_flag_kdiweird_multi5",   &rechit_flag_kdiweird_multi5_ee,   "rechit_flag_kdiweird_multi5_ee/I");
  dataTree2_->Branch("rechit_flag_koot_multi5",       &rechit_flag_koot_multi5_ee,       "rechit_flag_koot_multi5_ee/I");

  dataTree2_->Branch("rechit_ene_multi10",             &rechit_ene_multi10_ee,             "rechit_ene_multi10_ee/F");
  dataTree2_->Branch("rechit_et_multi10",              &rechit_et_multi10_ee,              "rechit_et_multi10_Ee/F");
  dataTree2_->Branch("rechit_time_multi10",            &rechit_time_multi10_ee,            "rechit_time_multi10_ee/F");
  dataTree2_->Branch("rechit_chi2_multi10",            &rechit_chi2_multi10_ee,            "rechit_chi2_multi10_ee/F");
  dataTree2_->Branch("rechit_flag_kweird_multi10",     &rechit_flag_kweird_multi10_ee,     "rechit_flag_kweird_multi10_ee/I");
  dataTree2_->Branch("rechit_flag_kdiweird_multi10",   &rechit_flag_kdiweird_multi10_ee,   "rechit_flag_kdiweird_multi10_ee/I");
  dataTree2_->Branch("rechit_flag_koot_multi10",       &rechit_flag_koot_multi10_ee,       "rechit_flag_koot_multi10_ee/I");

  dataTree2_->Branch("rechit_ene_multi1",             &rechit_ene_multi1_ee,             "rechit_ene_multi1_ee/F");
  dataTree2_->Branch("rechit_et_multi1",              &rechit_et_multi1_ee,              "rechit_et_multi1_ee/F");
  dataTree2_->Branch("rechit_time_multi1",            &rechit_time_multi1_ee,            "rechit_time_multi1_ee/F");
  dataTree2_->Branch("rechit_chi2_multi1",            &rechit_chi2_multi1_ee,            "rechit_chi2_multi1_ee/F");
  dataTree2_->Branch("rechit_flag_kweird_multi1",     &rechit_flag_kweird_multi1_ee,     "rechit_flag_kweird_multi1_ee/I");
  dataTree2_->Branch("rechit_flag_kdiweird_multi1",   &rechit_flag_kdiweird_multi1_ee,   "rechit_flag_kdiweird_multi1_ee/I");
  dataTree2_->Branch("rechit_flag_koot_multi1",       &rechit_flag_koot_multi1_ee,       "rechit_flag_koot_multi1_ee/I");



  // db info

  dataTree2_->Branch("db_pedg12",        &db_pedg12_ee,        "db_pedg12_ee/F");
  dataTree2_->Branch("db_pedg6",         &db_pedg6_ee,         "db_pedg6_ee/F");
  dataTree2_->Branch("db_pedg1",         &db_pedg1_ee,         "db_pedg1_ee/F");

  dataTree2_->Branch("db_pedrmsg12",     &db_pedrmsg12_ee,     "db_pedrmsg12_ee/F");
  dataTree2_->Branch("db_pedrmsg6",      &db_pedrmsg6_ee,      "db_pedrmsg6_ee/F");
  dataTree2_->Branch("db_pedrmsg1",      &db_pedrmsg1_ee,      "db_pedrmsg1_ee/F");

  dataTree2_->Branch("lasercorr",        &lasercorr_ee,        "lasercorr_ee/F");

  dataTree2_->Branch("uncalibrechit_multi10_ampl_ee", &uncalibrechit_multi10_ampl_ee, "uncalibrechit_multi10_ampl_ee/F");
  dataTree2_->Branch("uncalibrechit_multi10_amperr_ee", &uncalibrechit_multi10_amperr_ee, "uncalibrechit_multi10_amperr_ee/F");
  dataTree2_->Branch("uncalibrechit_multi10_pedestal_ee", &uncalibrechit_multi10_pedestal_ee, "uncalibrechit_multi10_pedestal_ee/F");
  dataTree2_->Branch("uncalibrechit_multi10_jitter_ee", &uncalibrechit_multi10_jitter_ee, "uncalibrechit_multi10_jitter_ee/F");
  dataTree2_->Branch("uncalibrechit_multi10_chi2_ee", &uncalibrechit_multi10_chi2_ee, "uncalibrechit_multi10_chi2_ee/F");
  dataTree2_->Branch("uncalibrechit_multi10_ootampl_ee", &uncalibrechit_multi10_ootampl_ee, "uncalibrechit_multi10_ootampl_ee[10]/F");
  
  dataTree2_->Branch("uncalibrechit_multi5_ampl_ee", &uncalibrechit_multi5_ampl_ee, "uncalibrechit_multi5_ampl_ee/F");
  dataTree2_->Branch("uncalibrechit_multi5_amperr_ee", &uncalibrechit_multi5_amperr_ee, "uncalibrechit_multi5_amperr_ee/F");
  dataTree2_->Branch("uncalibrechit_multi5_pedestal_ee", &uncalibrechit_multi5_pedestal_ee, "uncalibrechit_multi5_pedestal_ee/F");
  dataTree2_->Branch("uncalibrechit_multi5_jitter_ee", &uncalibrechit_multi5_jitter_ee, "uncalibrechit_multi5_jitter_ee/F");
  dataTree2_->Branch("uncalibrechit_multi5_chi2_ee", &uncalibrechit_multi5_chi2_ee, "uncalibrechit_multi5_chi2_ee/F");
  dataTree2_->Branch("uncalibrechit_multi5_ootampl_ee", &uncalibrechit_multi5_ootampl_ee, "uncalibrechit_multi5_ootampl_ee[10]/F");
  
  dataTree2_->Branch("uncalibrechit_multi1_ampl_ee", &uncalibrechit_multi1_ampl_ee, "uncalibrechit_multi1_ampl_ee/F");
  dataTree2_->Branch("uncalibrechit_multi1_amperr_ee", &uncalibrechit_multi1_amperr_ee, "uncalibrechit_multi1_amperr_ee/F");
  dataTree2_->Branch("uncalibrechit_multi1_pedestal_ee", &uncalibrechit_multi1_pedestal_ee, "uncalibrechit_multi1_pedestal_ee/F");
  dataTree2_->Branch("uncalibrechit_multi1_jitter_ee", &uncalibrechit_multi1_jitter_ee, "uncalibrechit_multi1_jitter_ee/F");
  dataTree2_->Branch("uncalibrechit_multi1_chi2_ee", &uncalibrechit_multi1_chi2_ee, "uncalibrechit_multi1_chi2_ee/F");
  dataTree2_->Branch("uncalibrechit_multi1_ootampl_ee", &uncalibrechit_multi1_ootampl_ee, "uncalibrechit_multi1_ootampl_ee[10]/F");


  // digi info

  dataTree2_->Branch("digi_adc", &digi_adc_ee,  "digi_adc_ee[10]/I"); 
  dataTree2_->Branch("digi_gainid", &digi_gainid_ee,  "digi_gainid_ee[10]/I");







  ebene = new TH2F("ebene","",360,0,360,170,-85,85);
  ebtime = new TH2F("ebtime","",360,0,360,170,-85,85);


}
//////////////////////////////////////////////////////////////////////////////////////////
void SpikeAnalyser::endJob() 
{
  if (file_ !=0) 
    {
      file_->cd();
 
      dataTree_   ->Write();
      dataTree2_  ->Write();
      ebene->Write();
      ebtime->Write();
   

    }
  file_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////


float SpikeAnalyser::recHitE( const DetId id, 
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

float SpikeAnalyser::recHitE( const DetId id, 
				 const EcalRecHitCollection &recHits) {
    
   if ( id.rawId() == 0 ) return 0;

   EcalRecHitCollection::const_iterator it = recHits.find( id );
   if ( it != recHits.end() ){
     float ene= (*it).energy();
     return ene;
   }
   return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpikeAnalyser::analyze(edm::Event const& event, edm::EventSetup const& iSetup) 
{ 


 for (int i=0;i<10;i++) {

    digi_adc[i]=0;
    digi_gainid[i]=0;
    digi_adc_ee[i]=0;
    digi_gainid_ee[i]=0;
 
  }

 run=0, ev=0, lumi=0, bx=0, orbit=0;


 numvtx=0;
 numgoodvtx=0;


  edm::ESHandle<EcalLaserDbService> laser;
  iSetup.get<EcalLaserDbRecord>().get(laser);
  


  run         = event.id().run();
  ev          = event.id().event();
  lumi        = event.luminosityBlock();
  bx          = event.bunchCrossing();
  orbit       = event.orbitNumber(); 

  /*
  edm::InputTag PileupSrc_("addPileupInfo");
  Handle<std::vector<PileupSummaryInfo>> PupInfo;
  event.getByLabel(PileupSrc_, PupInfo);
 
  std::vector<PileupSummaryInfo>::const_iterator PVI;

  for (PVI=PupInfo->begin(); PVI!=PupInfo->end(); ++PVI) {

    cout << "PU INFO: bx=" << PVI->getBunchCrossing() << " PU=" << PVI->getPU_NumInteractions() << endl;

  }
  */


  //  ebene->Reset();
  //  ebtime->Reset();

  //  if (run==198116 && ev==23523209) {


  // if (run==203708) {

 // get position in bunch train

  bunchintrain=-1;

  for (std::vector<int>::const_iterator bxit=bunchstartbx_.begin(); bxit!=bunchstartbx_.end(); ++bxit) {

    Int_t bxpos=bx - *bxit;


    // 50ns
    if (bxpos>=0 && bxpos<=70) bunchintrain=bxpos/2;
    // 25ns
    // if (bxpos>=0 && bxpos<=50) bunchintrain=bxpos;

  }


  edm::Handle<VertexCollection> vertices;
  event.getByLabel(vertex_coll_,vertices);
  VertexCollection::const_iterator vit;

  numvtx=vertices->size();


  if (vertices->size()>0) {

    for (vit=vertices->begin();vit!=vertices->end();++vit) {
      
      
      int vtx_chi2=vit->chi2();
      int vtx_ndof=vit->ndof();

      int vtx_isfake=vit->isFake();

      if (vit->isValid() && vtx_isfake==0 && vtx_ndof>4 && vtx_chi2>0 && vtx_chi2<10000) {
	
        numgoodvtx++;
	
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


 // Rechit Collection

  Handle<EcalRecHitCollection> EBhits_weights53;
  Handle<EcalRecHitCollection> EBhits_weights74;
  Handle<EcalRecHitCollection> EBhits_multi5;
  Handle<EcalRecHitCollection> EBhits_multi10;
  Handle<EcalRecHitCollection> EBhits_multi1;


  Handle<EcalUncalibratedRecHitCollection> EBhitsU_multi5;
  Handle<EcalUncalibratedRecHitCollection> EBhitsU_multi10;
  Handle<EcalUncalibratedRecHitCollection> EBhitsU_multi1;


  Handle<EcalRecHitCollection> EEhits_weights53;
  Handle<EcalRecHitCollection> EEhits_weights74;
  Handle<EcalRecHitCollection> EEhits_multi5;
  Handle<EcalRecHitCollection> EEhits_multi10;
  Handle<EcalRecHitCollection> EEhits_multi1;

  Handle<EcalUncalibratedRecHitCollection> EEhitsU_multi5;
  Handle<EcalUncalibratedRecHitCollection> EEhitsU_multi10;
  Handle<EcalUncalibratedRecHitCollection> EEhitsU_multi1;




  // 2015 data

  /*  
  event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEB","reRECO"),EBhits_weights74);  
  event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEB","reRECO"),EBhits_weights53);
  event.getByLabel(edm::InputTag("ecalRecHit","EcalRecHitsEB","reRECO"),EBhits_multi10);
  event.getByLabel(edm::InputTag("ecalRecHitMultiFit50ns","EcalRecHitsMultiFit50nsEB","reRECO"),EBhits_multi5);
  event.getByLabel(edm::InputTag("ecalRecHitMultiFitNoOOTPU","EcalRecHitsMultiFitNoOOTPUEB","reRECO"),EBhits_multi1);

  event.getByLabel(edm::InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEB","reRECO"),EBhitsU_multi10);
  event.getByLabel(edm::InputTag("ecalMultiFit50nsUncalibRecHit","EcalUncalibRecHitsEB","reRECO"),EBhitsU_multi5);
  event.getByLabel(edm::InputTag("ecalMultiFitNoOOTPUUncalibRecHit","EcalUncalibRecHitsEB","reRECO"),EBhitsU_multi1);


  event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEE","reRECO"),EEhits_weights74);  
  event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEE","reRECO"),EEhits_weights53);
  event.getByLabel(edm::InputTag("ecalRecHit","EcalRecHitsEE","reRECO"),EEhits_multi10);
  event.getByLabel(edm::InputTag("ecalRecHitMultiFit50ns","EcalRecHitsMultiFit50nsEE","reRECO"),EEhits_multi5);
  event.getByLabel(edm::InputTag("ecalRecHitMultiFitNoOOTPU","EcalRecHitsMultiFitNoOOTPUEE","reRECO"),EEhits_multi1);

  event.getByLabel(edm::InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEE","reRECO"),EEhitsU_multi10);
  event.getByLabel(edm::InputTag("ecalMultiFit50nsUncalibRecHit","EcalUncalibRecHitsEE","reRECO"),EEhitsU_multi5);
  event.getByLabel(edm::InputTag("ecalMultiFitNoOOTPUUncalibRecHit","EcalUncalibRecHitsEE","reRECO"),EEhitsU_multi1);
  */


  // 2012 data

  
  event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEB","reRECO"),EBhits_weights74);  
  event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEB","reRECO"),EBhits_weights53);
  event.getByLabel(edm::InputTag("ecalRecHit","EcalRecHitsEB","reRECO"),EBhits_multi5);
  event.getByLabel(edm::InputTag("ecalRecHitMultiFit25ns","EcalRecHitsMultiFit25nsEB","reRECO"),EBhits_multi10);
  event.getByLabel(edm::InputTag("ecalRecHitMultiFitNoOOTPU","EcalRecHitsMultiFitNoOOTPUEB","reRECO"),EBhits_multi1);

  event.getByLabel(edm::InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEB","reRECO"),EBhitsU_multi5);
  event.getByLabel(edm::InputTag("ecalMultiFit25nsUncalibRecHit","EcalUncalibRecHitsEB","reRECO"),EBhitsU_multi10);
  event.getByLabel(edm::InputTag("ecalMultiFitNoOOTPUUncalibRecHit","EcalUncalibRecHitsEB","reRECO"),EBhitsU_multi1);


  event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEE","reRECO"),EEhits_weights74);  
  event.getByLabel(edm::InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEE","reRECO"),EEhits_weights53);
  event.getByLabel(edm::InputTag("ecalRecHit","EcalRecHitsEE","reRECO"),EEhits_multi5);
  event.getByLabel(edm::InputTag("ecalRecHitMultiFit25ns","EcalRecHitsMultiFit25nsEE","reRECO"),EEhits_multi10);
  event.getByLabel(edm::InputTag("ecalRecHitMultiFitNoOOTPU","EcalRecHitsMultiFitNoOOTPUEE","reRECO"),EEhits_multi1);

  event.getByLabel(edm::InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEE","reRECO"),EEhitsU_multi5);
  event.getByLabel(edm::InputTag("ecalMultiFit25nsUncalibRecHit","EcalUncalibRecHitsEE","reRECO"),EEhitsU_multi10);
  event.getByLabel(edm::InputTag("ecalMultiFitNoOOTPUUncalibRecHit","EcalUncalibRecHitsEE","reRECO"),EEhitsU_multi1);
  


  // Digis, from RAW
 
  Handle<EBDigiCollection>  EBdigis;
  event.getByLabel(edm::InputTag("ecalDigis","ebDigis","reRECO"),EBdigis);

  Handle<EEDigiCollection>  EEdigis;
  event.getByLabel(edm::InputTag("ecalDigis","eeDigis","reRECO"),EEdigis);






  // Rechit loop - EE
  //   cout << "starting EE loop" << endl;

  for (EcalRecHitCollection::const_iterator hitItr = EEhits_weights53->begin(); hitItr != EEhits_weights53->end(); ++hitItr) {

    //    cout << "in EE loop1" << endl;

    rechit_ene_weights53_ee=0, rechit_et_weights53_ee=0, rechit_time_weights53_ee=0;
    rechit_flag_kweird_weights53_ee=0;
    rechit_flag_kdiweird_weights53_ee=0;
    rechit_flag_koot_weights53_ee=0;
    rechit_chi2_weights53_ee=0;
 
  
    rechit_ene_weights74_ee=0, rechit_et_weights74_ee=0, rechit_time_weights74_ee=0;
    rechit_flag_kweird_weights74_ee=0;
    rechit_flag_kdiweird_weights74_ee=0;
    rechit_flag_koot_weights74_ee=0;
    rechit_chi2_weights74_ee=0;
 
  
    rechit_ene_multi5_ee=0, rechit_et_multi5_ee=0, rechit_time_multi5_ee=0;
    rechit_flag_kweird_multi5_ee=0;
    rechit_flag_kdiweird_multi5_ee=0;
    rechit_flag_koot_multi5_ee=0;
    rechit_chi2_multi5_ee=0;
 
    rechit_ene_multi10_ee=0, rechit_et_multi10_ee=0, rechit_time_multi10_ee=0;
    rechit_flag_kweird_multi10_ee=0;
    rechit_flag_kdiweird_multi10_ee=0;
    rechit_flag_koot_multi10_ee=0;
    rechit_chi2_multi10_ee=0;
 
    rechit_ene_multi1_ee=0, rechit_et_multi1_ee=0, rechit_time_multi1_ee=0;
    rechit_flag_kweird_multi1_ee=0;
    rechit_flag_kdiweird_multi1_ee=0;
    rechit_flag_koot_multi1_ee=0;
    rechit_chi2_multi1_ee=0;
 

 
    uncalibrechit_multi10_ampl_ee     = 0;
    uncalibrechit_multi10_amperr_ee   = 0;
    uncalibrechit_multi10_pedestal_ee = 0;
    uncalibrechit_multi10_jitter_ee   = 0;
    uncalibrechit_multi10_chi2_ee     = 0;

    for (int i=0;i<10;i++) uncalibrechit_multi10_ootampl_ee[i]   = 0;


    uncalibrechit_multi5_ampl_ee     = 0;
    uncalibrechit_multi5_amperr_ee   = 0;
    uncalibrechit_multi5_pedestal_ee = 0;
    uncalibrechit_multi5_jitter_ee   = 0;
    uncalibrechit_multi5_chi2_ee     = 0;

    for (int i=0;i<10;i++) uncalibrechit_multi5_ootampl_ee[i]   = 0;


    uncalibrechit_multi1_ampl_ee     = 0;
    uncalibrechit_multi1_amperr_ee   = 0;
    uncalibrechit_multi1_pedestal_ee = 0;
    uncalibrechit_multi1_jitter_ee   = 0;
    uncalibrechit_multi1_chi2_ee     = 0;

    for (int i=0;i<10;i++) uncalibrechit_multi1_ootampl_ee[i]   = 0;





  
    rechit_ix=0, rechit_iy=0, rechit_iz=0, rechit_eta_ee=0, rechit_phi_ee=0;
    
    
    db_pedg12_ee=0, db_pedg6_ee=0, db_pedg1_ee=0;
    db_pedrmsg12_ee=0, db_pedrmsg6_ee=0, db_pedrmsg1_ee=0;
    lasercorr_ee=0;
    
    EcalRecHit hit           = (*hitItr);
    EEDetId det              = hit.id(); 
    rechit_ix                = det.ix();      
    rechit_iy                = det.iy();
    rechit_iz                = det.zside();

    rechit_ene_weights53_ee     = hit.energy();
    rechit_time_weights53_ee    = hit.time();
    rechit_chi2_weights53_ee    = hit.chi2();


    //   int ietabin=rechit_ieta+86-1*(rechit_ieta>0);


    //  ebene->SetBinContent(rechit_iphi,ietabin,rechit_ene_weights53);
    //  ebtime->SetBinContent(rechit_iphi,ietabin,rechit_time_weights53);

 
    
    int eehashedid = det.hashedIndex();
    
    
    GlobalPoint posee=geo->getPosition(hit.detid());
    
    
    rechit_eta_ee=posee.eta();
    rechit_phi_ee=posee.phi();
    float pf=1.0/cosh(rechit_eta_ee);
    
    rechit_et_weights53_ee=rechit_ene_weights53_ee*pf;
  
    if (rechit_et_weights53_ee<1.0) continue;

    //    cout << "rechit_et=" << rechit_et_weights53_ee << endl;

    // read pedestals from DB

    const EcalPedestals::Item *aped=0;
    aped=&eped->barrel(eehashedid);
    db_pedg12_ee=aped->mean_x12;
    db_pedg6_ee=aped->mean_x6;
    db_pedg1_ee=aped->mean_x1;

    db_pedrmsg12_ee=aped->rms_x12;
    db_pedrmsg6_ee=aped->rms_x6;
    db_pedrmsg1_ee=aped->rms_x1;

    //  read lasercalib from db
     lasercorr_ee=laser->getLaserCorrection(EEDetId(det), event.time());

     // lasercorr=1;

    if (hit.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_weights53_ee=1;
    if (hit.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_weights53_ee=1;
    if (hit.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_weights53_ee=1;
  
   
    
    //   if (rechit_et>5.0 && abs(rechit_time)>3) rechit_flag_koot=1;
    
    // mask out noisy channels in data
 

    


    // select high energy hits
    if (rechit_et_weights53_ee>1.0) {
      //  cout << "EEdigistart" << endl;  
    
      // find digi corresponding to rechit
      
      EEDigiCollection::const_iterator digiItrEE= EEdigis->begin();
      while(digiItrEE != EEdigis->end() && digiItrEE->id() != hitItr->id())
	{
	  digiItrEE++;
	}
      
      if (digiItrEE != EEdigis->end()) {
	
	EEDataFrame df(*digiItrEE);
	for(int i=0; i<10;++i)
	  {
	    int eedigiadc=df.sample(i).adc();		
	    int eedigigain=df.sample(i).gainId();
	    digi_adc_ee[i]=eedigiadc;
	    digi_gainid_ee[i]=eedigigain;
	  }
	
	// cout << "EEdigistart" << endl;  
	
      }

      for (EcalRecHitCollection::const_iterator hitItr2 = EEhits_weights74->begin(); hitItr2 != EEhits_weights74->end(); ++hitItr2) {

	//	cout << "in EE loop2" << endl;


	EcalRecHit hit2           = (*hitItr2);
	EEDetId det2              = hit2.id(); 


	if (det2!=det) continue;

	rechit_ene_weights74_ee     = hit2.energy();
	rechit_time_weights74_ee    = hit2.time();
	rechit_chi2_weights74_ee    = hit2.chi2();
	rechit_et_weights74_ee      = rechit_ene_weights74_ee*pf;



	if (hit2.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_weights74_ee=1;
	if (hit2.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_weights74_ee=1;
	if (hit2.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_weights74_ee=1;


      }



      for (EcalRecHitCollection::const_iterator hitItr3 = EEhits_multi5->begin(); hitItr3 != EEhits_multi5->end(); ++hitItr3) {

	//	cout << "in EE loop3" << endl;


	EcalRecHit hit3           = (*hitItr3);
	EEDetId det3              = hit3.id(); 


	if (det3!=det) continue;

	rechit_ene_multi5_ee     = hit3.energy();
	rechit_time_multi5_ee    = hit3.time();
	rechit_chi2_multi5_ee    = hit3.chi2();
	rechit_et_multi5_ee      = rechit_ene_multi5_ee*pf;





	if (hit3.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_multi5_ee=1;
	if (hit3.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_multi5_ee=1;
	if (hit3.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_multi5_ee=1;


      }



      for (EcalRecHitCollection::const_iterator hitItr4 = EEhits_multi10->begin(); hitItr4 != EEhits_multi10->end(); ++hitItr4) {

	//	cout << "in EE loop4" << endl;

	EcalRecHit hit4           = (*hitItr4);
	EEDetId det4              = hit4.id(); 


	if (det4!=det) continue;

	rechit_ene_multi10_ee     = hit4.energy();
	rechit_time_multi10_ee    = hit4.time();
	rechit_chi2_multi10_ee    = hit4.chi2();
	rechit_et_multi10_ee      = rechit_ene_multi10_ee*pf;






	if (hit4.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_multi10_ee=1;
	if (hit4.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_multi10_ee=1;
	if (hit4.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_multi10_ee=1;


      }





      for (EcalRecHitCollection::const_iterator hitItr5 = EEhits_multi1->begin(); hitItr5 != EEhits_multi1->end(); ++hitItr5) {

	//	cout << "in EE loop5" << endl;

	EcalRecHit hit5           = (*hitItr5);
	EEDetId det5              = hit5.id(); 


	if (det5!=det) continue;

	rechit_ene_multi1_ee     = hit5.energy();
	rechit_time_multi1_ee    = hit5.time();
	rechit_chi2_multi1_ee    = hit5.chi2();
	rechit_et_multi1_ee      = rechit_ene_multi1_ee*pf;


	if (hit5.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_multi1_ee=1;
	if (hit5.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_multi1_ee=1;
	if (hit5.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_multi1_ee=1;


      }


    for (EcalUncalibratedRecHitCollection::const_iterator hitItr6 = EEhitsU_multi10->begin(); hitItr6 != EEhitsU_multi10->end(); ++hitItr6) {

	//	cout << "in EE loop4" << endl;

	EcalUncalibratedRecHit hit6           = (*hitItr6);
	EEDetId det6              = hit6.id(); 


	if (det6!=det) continue;

	uncalibrechit_multi10_ampl_ee     = hit6.amplitude();
	uncalibrechit_multi10_amperr_ee   = hit6.amplitudeError();
	uncalibrechit_multi10_pedestal_ee = hit6.pedestal();
	uncalibrechit_multi10_jitter_ee   = hit6.jitter();
	uncalibrechit_multi10_chi2_ee     = hit6.chi2();

	for (int i=0;i<10;i++) uncalibrechit_multi10_ootampl_ee[i]   = hit6.outOfTimeAmplitude(i);

	/*
	cout << "Uncalib rechit: ampl=" << 	uncalibrechit_multi10_ampl_ee
	     << "\n err=" << 	uncalibrechit_multi10_amperr_ee
	     << "\n ped=" << 	uncalibrechit_multi10_pedestal_ee
	     << "\n jitter=" << 	uncalibrechit_multi10_jitter_ee
	     << "\n chi2=" << 	uncalibrechit_multi10_chi2_ee << endl;

	for (int i=0;i<10;i++) {
	  cout << " OOT amp, bin=" << i << " =" << uncalibrechit_multi10_ootampl_ee[i] << endl;
	}
	*/

      }




    for (EcalUncalibratedRecHitCollection::const_iterator hitItr8 = EEhitsU_multi1->begin(); hitItr8 != EEhitsU_multi1->end(); ++hitItr8) {

	//	cout << "in EE loop4" << endl;

	EcalUncalibratedRecHit hit8           = (*hitItr8);
	EEDetId det8              = hit8.id(); 


	if (det8!=det) continue;

	uncalibrechit_multi1_ampl_ee     = hit8.amplitude();
	uncalibrechit_multi1_amperr_ee   = hit8.amplitudeError();
	uncalibrechit_multi1_pedestal_ee = hit8.pedestal();
	uncalibrechit_multi1_jitter_ee   = hit8.jitter();
	uncalibrechit_multi1_chi2_ee     = hit8.chi2();

	for (int i=0;i<10;i++) uncalibrechit_multi1_ootampl_ee[i]   = hit8.outOfTimeAmplitude(i);


      }



   for (EcalUncalibratedRecHitCollection::const_iterator hitItr7 = EEhitsU_multi5->begin(); hitItr7 != EEhitsU_multi5->end(); ++hitItr7) {

	//	cout << "in EE loop4" << endl;

	EcalUncalibratedRecHit hit7           = (*hitItr7);
	EEDetId det7              = hit7.id(); 


	if (det7!=det) continue;

	uncalibrechit_multi5_ampl_ee     = hit7.amplitude();
	uncalibrechit_multi5_amperr_ee   = hit7.amplitudeError();
	uncalibrechit_multi5_pedestal_ee = hit7.pedestal();
	uncalibrechit_multi5_jitter_ee   = hit7.jitter();
	uncalibrechit_multi5_chi2_ee     = hit7.chi2();

	for (int i=0;i<10;i++) uncalibrechit_multi5_ootampl_ee[i]   = hit7.outOfTimeAmplitude(i);


      }






      //      cout << "end of loop" << endl;

      dataTree2_->Fill();


      //      cout << "end of loop - filled tree" << endl;

    }
  }






  // Rechit loop - EB

  for (EcalRecHitCollection::const_iterator hitItr = EBhits_weights53->begin(); hitItr != EBhits_weights53->end(); ++hitItr) {

    rechit_swisscross_weights53=0;
    rechit_ene_weights53=0, rechit_et_weights53=0, rechit_time_weights53=0;
    rechit_flag_kweird_weights53=0;
    rechit_flag_kdiweird_weights53=0;
    rechit_flag_koot_weights53=0;
    rechit_chi2_weights53=0;
 
  
    rechit_swisscross_weights74=0;
    rechit_ene_weights74=0, rechit_et_weights74=0, rechit_time_weights74=0;
    rechit_flag_kweird_weights74=0;
    rechit_flag_kdiweird_weights74=0;
    rechit_flag_koot_weights74=0;
    rechit_chi2_weights74=0;
 
  
    rechit_swisscross_multi5=0;
    rechit_ene_multi5=0, rechit_et_multi5=0, rechit_time_multi5=0;
    rechit_flag_kweird_multi5=0;
    rechit_flag_kdiweird_multi5=0;
    rechit_flag_koot_multi5=0;
    rechit_chi2_multi5=0;
 
    rechit_swisscross_multi10=0;
    rechit_ene_multi10=0, rechit_et_multi10=0, rechit_time_multi10=0;
    rechit_flag_kweird_multi10=0;
    rechit_flag_kdiweird_multi10=0;
    rechit_flag_koot_multi10=0;
    rechit_chi2_multi10=0;
 
    rechit_swisscross_multi1=0;
    rechit_ene_multi1=0, rechit_et_multi1=0, rechit_time_multi1=0;
    rechit_flag_kweird_multi1=0;
    rechit_flag_kdiweird_multi1=0;
    rechit_flag_koot_multi1=0;
    rechit_chi2_multi1=0;
 

    rechit_swisscross_zs_weights53=0;
    rechit_swisscross_zs_weights74=0;
    rechit_swisscross_zs_multi5=0;
    rechit_swisscross_zs_multi10=0;
    rechit_swisscross_zs_multi1=0;

    rechit_swisscross_thresh=0;
    rechit_flag_kweird_calc=0;
  
    isnearcrack=0;

    uncalibrechit_multi10_ampl_eb     = 0;
    uncalibrechit_multi10_amperr_eb   = 0;
    uncalibrechit_multi10_pedestal_eb = 0;
    uncalibrechit_multi10_jitter_eb   = 0;
    uncalibrechit_multi10_chi2_eb     = 0;

    for (int i=0;i<10;i++) uncalibrechit_multi10_ootampl_eb[i]   = 0;


    uncalibrechit_multi5_ampl_eb     = 0;
    uncalibrechit_multi5_amperr_eb   = 0;
    uncalibrechit_multi5_pedestal_eb = 0;
    uncalibrechit_multi5_jitter_eb   = 0;
    uncalibrechit_multi5_chi2_eb     = 0;

    for (int i=0;i<10;i++) uncalibrechit_multi5_ootampl_eb[i]   = 0;


    uncalibrechit_multi1_ampl_eb     = 0;
    uncalibrechit_multi1_amperr_eb   = 0;
    uncalibrechit_multi1_pedestal_eb = 0;
    uncalibrechit_multi1_jitter_eb   = 0;
    uncalibrechit_multi1_chi2_eb     = 0;

    for (int i=0;i<10;i++) uncalibrechit_multi1_ootampl_eb[i]   = 0;
  
    rechit_ieta=0, rechit_iphi=0, rechit_eta=0, rechit_phi=0;
    
    
    db_pedg12=0, db_pedg6=0, db_pedg1=0;
    db_pedrmsg12=0, db_pedrmsg6=0, db_pedrmsg1=0;
    lasercorr=0;
    
    EcalRecHit hit           = (*hitItr);
    EBDetId det              = hit.id(); 
    rechit_ieta              = det.ieta();      
    rechit_iphi              = det.iphi();
    rechit_ene_weights53     = hit.energy();
    rechit_time_weights53    = hit.time();
    rechit_chi2_weights53    = hit.chi2();


    //   int ietabin=rechit_ieta+86-1*(rechit_ieta>0);


    //  ebene->SetBinContent(rechit_iphi,ietabin,rechit_ene_weights53);
    //  ebtime->SetBinContent(rechit_iphi,ietabin,rechit_time_weights53);

 
    
    int ebhashedid = det.hashedIndex();
    
    
    GlobalPoint poseb=geo->getPosition(hit.detid());
    
    
    rechit_eta=poseb.eta();
    rechit_phi=poseb.phi();
    float pf=1.0/cosh(rechit_eta);
    
    rechit_et_weights53=rechit_ene_weights53*pf;
  
    if (rechit_et_weights53<3.0) continue;

    // swiss-cross calculation

    float s41=0;
    float s42=0;
    float s43=0;
    float s44=0;
    
    s41 = recHitE( det, *EBhits_weights53,  1,  0 );
    s42 = recHitE( det, *EBhits_weights53,  -1,  0 );
    s43 = recHitE( det, *EBhits_weights53,  0,  1 );
    s44 = recHitE( det, *EBhits_weights53,  0,  -1 );
    
    float s4=s41+s42+s43+s44;


    float s4zs=0;
    if (s41>0.08) s4zs+=s41;
    if (s42>0.08) s4zs+=s42;
    if (s43>0.08) s4zs+=s43;
    if (s44>0.08) s4zs+=s44;

    isnearcrack=0;
    isnearcrack=EBDetId::isNextToBoundary(det);
  
    if (rechit_ene_weights53>0) {

      rechit_swisscross_weights53=1.0-s4/rechit_ene_weights53;
      rechit_swisscross_zs_weights53=1.0-s4zs/rechit_ene_weights53;
    
    }

    float e4e1thresh=0.04*log10(rechit_ene_weights53)-0.024;
    float e4e1ene=4.0;

    if (isnearcrack) {

      e4e1thresh=e4e1thresh/3.0;
      e4e1ene=e4e1ene*2.0;

    }

    if (rechit_swisscross_zs_weights53>(1.0-e4e1thresh) 
	&& rechit_ene_weights53>e4e1ene) rechit_flag_kweird_calc=1;

    rechit_swisscross_thresh= 1.0 - e4e1thresh;


    // read pedestals from DB

    const EcalPedestals::Item *aped=0;
    aped=&eped->barrel(ebhashedid);
    db_pedg12=aped->mean_x12;
    db_pedg6=aped->mean_x6;
    db_pedg1=aped->mean_x1;

    db_pedrmsg12=aped->rms_x12;
    db_pedrmsg6=aped->rms_x6;
    db_pedrmsg1=aped->rms_x1;

    // read lasercalib from db
     lasercorr=laser->getLaserCorrection(EBDetId(det), event.time());

     //    lasercorr=1;

    if (hit.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_weights53=1;
    if (hit.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_weights53=1;
    if (hit.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_weights53=1;
  
   
    
    //   if (rechit_et>5.0 && abs(rechit_time)>3) rechit_flag_koot=1;
    
    // mask out noisy channels in data
    
    if ((rechit_ieta==11 && rechit_iphi==68) || (rechit_ieta==68 && rechit_iphi==74) || (rechit_ieta==-83 && rechit_iphi==189) || (rechit_ieta==-75 && rechit_iphi==199)) continue;



    


    // select high energy hits
    if (rechit_et_weights53>1.0) {
      
      // find digi corresponding to rechit
      
      EBDigiCollection::const_iterator digiItrEB= EBdigis->begin();
      while(digiItrEB != EBdigis->end() && digiItrEB->id() != hitItr->id())
	{
	  digiItrEB++;
	}
      
      if (digiItrEB != EBdigis->end()) {
	
	EBDataFrame df(*digiItrEB);
	for(int i=0; i<10;++i)
	  {
	    int ebdigiadc=df.sample(i).adc();		
	    int ebdigigain=df.sample(i).gainId();
	    digi_adc[i]=ebdigiadc;
	    digi_gainid[i]=ebdigigain;
	  }
	
	
      }

      for (EcalRecHitCollection::const_iterator hitItr2 = EBhits_weights74->begin(); hitItr2 != EBhits_weights74->end(); ++hitItr2) {
	EcalRecHit hit2           = (*hitItr2);
	EBDetId det2              = hit2.id(); 


	if (det2!=det) continue;

	rechit_ene_weights74     = hit2.energy();
	rechit_time_weights74    = hit2.time();
	rechit_chi2_weights74    = hit2.chi2();
	rechit_et_weights74      = rechit_ene_weights74*pf;


	float s41_2=0;
	float s42_2=0;
	float s43_2=0;
	float s44_2=0;
    
	s41_2 = recHitE( det2, *EBhits_weights74,  1,  0 );
	s42_2 = recHitE( det2, *EBhits_weights74,  -1,  0 );
	s43_2 = recHitE( det2, *EBhits_weights74,  0,  1 );
	s44_2 = recHitE( det2, *EBhits_weights74,  0,  -1 );
    
	float s4_2=s41_2+s42_2+s43_2+s44_2;

	float s4zs_2=0;
	if (s41_2>0.08) s4zs_2+=s41_2;
	if (s42_2>0.08) s4zs_2+=s42_2;
	if (s43_2>0.08) s4zs_2+=s43_2;
	if (s44_2>0.08) s4zs_2+=s44_2;


	if (rechit_ene_weights74>0) {
	  rechit_swisscross_weights74=1.0-s4_2/rechit_ene_weights74;
	  rechit_swisscross_zs_weights74=1.0-s4zs_2/rechit_ene_weights74;
	}

	if (hit2.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_weights74=1;
	if (hit2.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_weights74=1;
	if (hit2.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_weights74=1;


      }



      for (EcalRecHitCollection::const_iterator hitItr3 = EBhits_multi5->begin(); hitItr3 != EBhits_multi5->end(); ++hitItr3) {
	EcalRecHit hit3           = (*hitItr3);
	EBDetId det3              = hit3.id(); 


	if (det3!=det) continue;

	rechit_ene_multi5     = hit3.energy();
	rechit_time_multi5    = hit3.time();
	rechit_chi2_multi5    = hit3.chi2();
	rechit_et_multi5      = rechit_ene_multi5*pf;


	float s41_3=0;
	float s42_3=0;
	float s43_3=0;
	float s44_3=0;
    
	s41_3 = recHitE( det3, *EBhits_multi5,  1,  0 );
	s42_3 = recHitE( det3, *EBhits_multi5,  -1,  0 );
	s43_3 = recHitE( det3, *EBhits_multi5,  0,  1 );
	s44_3 = recHitE( det3, *EBhits_multi5,  0,  -1 );
    
	float s4_3=s41_3+s42_3+s43_3+s44_3;


	float s4zs_3=0;
	if (s41_3>0.08) s4zs_3+=s41_3;
	if (s42_3>0.08) s4zs_3+=s42_3;
	if (s43_3>0.08) s4zs_3+=s43_3;
	if (s44_3>0.08) s4zs_3+=s44_3;




	if (rechit_ene_multi5>0) {
	  rechit_swisscross_multi5=1.0-s4_3/rechit_ene_multi5;
	  rechit_swisscross_zs_multi5=1.0-s4zs_3/rechit_ene_multi5;
	}

	if (hit3.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_multi5=1;
	if (hit3.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_multi5=1;
	if (hit3.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_multi5=1;


      }



      for (EcalRecHitCollection::const_iterator hitItr4 = EBhits_multi10->begin(); hitItr4 != EBhits_multi10->end(); ++hitItr4) {
	EcalRecHit hit4           = (*hitItr4);
	EBDetId det4              = hit4.id(); 


	if (det4!=det) continue;

	rechit_ene_multi10     = hit4.energy();
	rechit_time_multi10    = hit4.time();
	rechit_chi2_multi10    = hit4.chi2();
	rechit_et_multi10      = rechit_ene_multi10*pf;


	float s41_4=0;
	float s42_4=0;
	float s43_4=0;
	float s44_4=0;
    
	s41_4 = recHitE( det4, *EBhits_multi10,  1,  0 );
	s42_4 = recHitE( det4, *EBhits_multi10,  -1,  0 );
	s43_4 = recHitE( det4, *EBhits_multi10,  0,  1 );
	s44_4 = recHitE( det4, *EBhits_multi10,  0,  -1 );
    
	float s4_4=s41_4+s42_4+s43_4+s44_4;

	float s4zs_4=0;
	if (s41_4>0.08) s4zs_4+=s41_4;
	if (s42_4>0.08) s4zs_4+=s42_4;
	if (s43_4>0.08) s4zs_4+=s43_4;
	if (s44_4>0.08) s4zs_4+=s44_4;



	if (rechit_ene_multi10>0) {

	  rechit_swisscross_multi10=1.0-s4_4/rechit_ene_multi10;
	  rechit_swisscross_zs_multi10=1.0-s4zs_4/rechit_ene_multi10;

	}


	if (hit4.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_multi10=1;
	if (hit4.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_multi10=1;
	if (hit4.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_multi10=1;


      }





      for (EcalRecHitCollection::const_iterator hitItr5 = EBhits_multi1->begin(); hitItr5 != EBhits_multi1->end(); ++hitItr5) {
	EcalRecHit hit5           = (*hitItr5);
	EBDetId det5              = hit5.id(); 


	if (det5!=det) continue;

	rechit_ene_multi1     = hit5.energy();
	rechit_time_multi1    = hit5.time();
	rechit_chi2_multi1    = hit5.chi2();
	rechit_et_multi1      = rechit_ene_multi1*pf;


	float s41_5=0;
	float s42_5=0;
	float s43_5=0;
	float s44_5=0;
    
	s41_5 = recHitE( det5, *EBhits_multi1,  1,  0 );
	s42_5 = recHitE( det5, *EBhits_multi1,  -1,  0 );
	s43_5 = recHitE( det5, *EBhits_multi1,  0,  1 );
	s44_5 = recHitE( det5, *EBhits_multi1,  0,  -1 );
    
	float s4_5=s41_5+s42_5+s43_5+s44_5;

	float s4zs_5=0;
	if (s41_5>0.08) s4zs_5+=s41_5;
	if (s42_5>0.08) s4zs_5+=s42_5;
	if (s43_5>0.08) s4zs_5+=s43_5;
	if (s44_5>0.08) s4zs_5+=s44_5;



	if (rechit_ene_multi1>0) {
	  rechit_swisscross_multi1=1.0-s4_5/rechit_ene_multi1;
	  rechit_swisscross_zs_multi1=1.0-s4zs_5/rechit_ene_multi1;

	}

	if (hit5.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird_multi1=1;
	if (hit5.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird_multi1=1;
	if (hit5.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot_multi1=1;


      }





    for (EcalUncalibratedRecHitCollection::const_iterator hitItr6 = EBhitsU_multi10->begin(); hitItr6 != EBhitsU_multi10->end(); ++hitItr6) {



	EcalUncalibratedRecHit hit6           = (*hitItr6);
	EBDetId det6              = hit6.id(); 


	if (det6!=det) continue;

	uncalibrechit_multi10_ampl_eb     = hit6.amplitude();
	uncalibrechit_multi10_amperr_eb   = hit6.amplitudeError();
	uncalibrechit_multi10_pedestal_eb = hit6.pedestal();
	uncalibrechit_multi10_jitter_eb   = hit6.jitter();
	uncalibrechit_multi10_chi2_eb     = hit6.chi2();

	for (int i=0;i<10;i++) uncalibrechit_multi10_ootampl_eb[i]   = hit6.outOfTimeAmplitude(i);

      }

   for (EcalUncalibratedRecHitCollection::const_iterator hitItr7 = EBhitsU_multi5->begin(); hitItr7 != EBhitsU_multi5->end(); ++hitItr7) {

	//	cout << "in EE loop4" << endl;

	EcalUncalibratedRecHit hit7           = (*hitItr7);
	EBDetId det7              = hit7.id(); 


	if (det7!=det) continue;

	uncalibrechit_multi5_ampl_eb     = hit7.amplitude();
	uncalibrechit_multi5_amperr_eb   = hit7.amplitudeError();
	uncalibrechit_multi5_pedestal_eb = hit7.pedestal();
	uncalibrechit_multi5_jitter_eb   = hit7.jitter();
	uncalibrechit_multi5_chi2_eb     = hit7.chi2();

	for (int i=0;i<10;i++) uncalibrechit_multi5_ootampl_eb[i]   = hit7.outOfTimeAmplitude(i);


      }





    for (EcalUncalibratedRecHitCollection::const_iterator hitItr8 = EBhitsU_multi1->begin(); hitItr8 != EBhitsU_multi1->end(); ++hitItr8) {



	EcalUncalibratedRecHit hit8           = (*hitItr8);
	EBDetId det8              = hit8.id(); 


	if (det8!=det) continue;

	uncalibrechit_multi1_ampl_eb     = hit8.amplitude();
	uncalibrechit_multi1_amperr_eb   = hit8.amplitudeError();
	uncalibrechit_multi1_pedestal_eb = hit8.pedestal();
	uncalibrechit_multi1_jitter_eb   = hit8.jitter();
	uncalibrechit_multi1_chi2_eb     = hit8.chi2();

	for (int i=0;i<10;i++) uncalibrechit_multi1_ootampl_eb[i]   = hit8.outOfTimeAmplitude(i);


      }





      dataTree_->Fill();

    }
  }









  //  }  
  // }
  
  
}
//////////////////////////////////////////////////////////////////////////////////////////
SpikeAnalyser::~SpikeAnalyser() 
{
  delete file_;
  delete dataTree_;
  delete dataTree2_;
}
//}

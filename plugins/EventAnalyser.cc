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

#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"

#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"

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


#include "JetMETCorrections/MCJet/plugins/EventAnalyser.h" 
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
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EcalDetId/interface/EcalElectronicsId.h"


#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

//#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"

#include "Math/VectorUtil.h"
#include "TVector3.h"

using namespace edm;
using namespace reco;
using namespace std;

DEFINE_FWK_MODULE(EventAnalyser);


EventAnalyser::EventAnalyser(const edm::ParameterSet& cfg)
  : bunchstartbx_(cfg.getParameter<std::vector<int> >("bunchstartbx"))
{
  jets_          = cfg.getParameter<std::string> ("jets");
  histogramFile_ = cfg.getParameter<std::string> ("histogramFile");
  tracks_        = cfg.getParameter<std::string> ("tracks");
  vertex_coll_   = cfg.getParameter<std::string> ("vertex");
  ebhitcoll_     = cfg.getParameter<std::string> ("EBRecHitCollection");
  eehitcoll_     = cfg.getParameter<std::string> ("EERecHitCollection");
  isMC_          = cfg.getParameter<bool>("IsMC");




  tok_EB=consumes<EcalRecHitCollection>(edm::InputTag("reducedEcalRecHitsEB"));
  tok_EE=consumes<EcalRecHitCollection>(edm::InputTag("reducedEcalRecHitsEE"));
  tok_ES=consumes<EcalRecHitCollection>(edm::InputTag("reducedEcalRecHitsES"));

  tok_EB_digi=consumes<EBDigiCollection>(edm::InputTag("selectDigi","selectedEcalEBDigiCollection")); 
  tok_EE_digi=consumes<EEDigiCollection>(edm::InputTag("selectDigi","selectedEcalEEDigiCollection"));




  edm::ParameterSet cleaningPs = 
    cfg.getParameter<edm::ParameterSet>("cleaningConfig");
  cleaningAlgo_ = new EcalCleaningAlgo(cleaningPs);


}



//////////////////////////////////////////////////////////////////////////////////////////
void EventAnalyser::beginJob() 
{
//Creating the ouput file and the TTree
  file_          = new TFile(histogramFile_.c_str(),"RECREATE");
  dataTree_      = new TTree("dataTree","dataTree");

  evcount=0;

//Making arrays for the calibrated and uncalibrated rechits(?) histograms for barrel and endcaps
  barrel_uncalibrh=new TH2F*[100];
  endcap_uncalibrh=new TH2F*[100];
  barrel_rh=new TH2F*[100];
  endcap_rh=new TH2F*[100];


  char txt[80];

  for (Int_t i=0;i<100;i++) {
//Initializing the histograms for each array element
    sprintf(txt,"barrel_uncalibrh_ev%03d",i+1);
    barrel_uncalibrh[i] = new TH2F(txt,txt,360,0,360,170,-85,85);

    sprintf(txt,"barrel_rh_ev%03d",i+1);
    barrel_rh[i] = new TH2F(txt,txt,360,0,360,170,-85,85);

    sprintf(txt,"endcap_uncalibrh_ev%03d",i+1);
    endcap_uncalibrh[i] = new TH2F(txt,txt,200,0,200,100,0,100);
 
    sprintf(txt,"endcap_rh_ev%03d",i+1);
    endcap_rh[i] = new TH2F(txt,txt,200,0,200,100,0,100);
    
  }

//Initializing all of the other histograms
  eboccet = new TH2F("eboccet","",360,0,360,170,-85,85);
  ebtime = new TH2F("ebtime","",360,0,360,170,-85,85);

  eeoccet = new TH2F("eeoccet","",200,0,200,100,0,100);
  eetime = new TH2F("eetime","",200,0,200,100,0,100);

  eboccet_kgood = new TH2F("eboccet_kgood","",360,0,360,170,-85,85);
  eeoccet_kgood = new TH2F("eeoccet_kgood","",200,0,200,100,0,100);

  ebflag_kweird = new TH2F("ebflag_kweird","",360,0,360,170,-85,85);
  eeflag_kweird = new TH2F("eeflag_kweird","",200,0,200,100,0,100);

  ebflag_kdiweird = new TH2F("ebflag_kdiweird","",360,0,360,170,-85,85);
  eeflag_kdiweird = new TH2F("eeflag_kdiweird","",200,0,200,100,0,100);

  ebflag_koot = new TH2F("ebflag_koot","",360,0,360,170,-85,85);
  eeflag_koot = new TH2F("eeflag_koot","",200,0,200,100,0,100);

  esoccet_esp1 = new TH2F("esoccet_esp1","",40,0.5,40.5,40,0.5,40.5);
  esoccet_esp2 = new TH2F("esoccet_esp2","",40,0.5,40.5,40,0.5,40.5);
  esoccet_esm1 = new TH2F("esoccet_esm1","",40,0.5,40.5,40,0.5,40.5);
  esoccet_esm2 = new TH2F("esoccet_esm2","",40,0.5,40.5,40,0.5,40.5);


  eepocc_etaphi = new TH2F("eepocc_etaphi","",50,1.5,3.0,50,-3.14,3.14);
  eemocc_etaphi = new TH2F("eemocc_etaphi","",50,-3.0,-1.5,50,-3.14,3.14);

  esp1occ_etaphi = new TH2F("esp1occ_etaphi","",50,1.5,3.0,50,-3.14,3.14);
  esm1occ_etaphi = new TH2F("esm1occ_etaphi","",50,-3.0,-1.5,50,-3.14,3.14);

  esp2occ_etaphi = new TH2F("esp2occ_etaphi","",50,1.5,3.0,50,-3.14,3.14);
  esm2occ_etaphi = new TH2F("esm2occ_etaphi","",50,-3.0,-1.5,50,-3.14,3.14);

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


  ebicval=new TH2D("ebicval","",360,0,360,170,-85,85);
  eeicval=new TH2D("eeicval","",200,0,200,100,0,100);

  eblascorr=new TH2D("eblascorr","",360,0,360,170,-85,85);
  eelascorr=new TH2D("eelascorr","",200,0,200,100,0,100);




 // run/event info
 // Setting the branch addresses
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

  dataTree_->Branch("rechit_ene",        &rechit_ene,        "rechit_ene/F");
  dataTree_->Branch("rechit_et",         &rechit_et,         "rechit_et/F");
  dataTree_->Branch("rechit_time",       &rechit_time,       "rechit_time/F");
  dataTree_->Branch("rechit_ieta",       &rechit_ieta,       "rechit_ieta/I");
  dataTree_->Branch("rechit_iphi",       &rechit_iphi,       "rechit_iphi/I");
  dataTree_->Branch("rechit_eta",        &rechit_eta,        "rechit_eta/F");
  dataTree_->Branch("rechit_phi",        &rechit_phi,        "rechit_phi/F");
  dataTree_->Branch("rechit_swisscross", &rechit_swisscross, "rechit_swisscross/F");

  dataTree_->Branch("rechit_chi2",       &rechit_chi2,       "rechit_chi2/F");
  dataTree_->Branch("rechit_chi2oot",    &rechit_chi2oot,    "rechit_chi2oot/F");


  dataTree_->Branch("rechit_flag_kweird",        &rechit_flag_kweird,        "rechit_flag_kweird/I");
  dataTree_->Branch("rechit_flag_kdiweird",      &rechit_flag_kdiweird,      "rechit_flag_kdiweird/I");
  dataTree_->Branch("rechit_flag_koot",          &rechit_flag_koot,          "rechit_flag_koot/I");


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


  // sumet

  dataTree_->Branch("ebsumet",        &ebsumet,        "ebsumet/F");
  dataTree_->Branch("eesumet",        &eesumet,        "eesumet/F");

  dataTree_->Branch("ebsumet_kgood",  &ebsumet_kgood,  "ebsumet_kgood/F");
  dataTree_->Branch("eesumet_kgood",  &eesumet_kgood,  "eesumet_kgood/F");

 

}
//////////////////////////////////////////////////////////////////////////////////////////
void EventAnalyser::endJob() 
{
  if (file_ !=0) 
    {
      file_->cd();
 
      dataTree_   ->Write();


      ebchstatus->Write();
      eechstatus->Write();

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


      ebicval->Write();
      eeicval->Write();

      eblascorr->Write();
      eelascorr->Write();



      eboccet->Write();
      eeoccet->Write();

      ebtime->Write();
      eetime->Write();

      eboccet_kgood->Write();
      eeoccet_kgood->Write();

      ebflag_kweird->Write();
      eeflag_kweird->Write();

      ebflag_kdiweird->Write();
      eeflag_kdiweird->Write();

      ebflag_koot->Write();
      eeflag_koot->Write();

      esoccet_esp1->Write();
      esoccet_esp2->Write();
      esoccet_esm1->Write();
      esoccet_esm2->Write();

      eepocc_etaphi->Write();
      eemocc_etaphi->Write();

      esp1occ_etaphi->Write();
      esp2occ_etaphi->Write();

      esm1occ_etaphi->Write();
      esm2occ_etaphi->Write();


      for (Int_t i=0;i<100;i++) {

	barrel_uncalibrh[i]->Write();
	endcap_uncalibrh[i]->Write();

	barrel_rh[i]->Write();
	endcap_rh[i]->Write();


      }


    }
  file_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////


float EventAnalyser::recHitE( const DetId id, 
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

float EventAnalyser::recHitE( const DetId id, 
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
void EventAnalyser::analyze(edm::Event const& event, edm::EventSetup const& iSetup) 
{ 


 for (int i=0;i<10;i++) {

    digi_adc[i]=0;
    digi_gainid[i]=0;
 
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



  cout << "RUN=" << run << " EVENT=" << ev << " lumi=" << lumi << endl;

  //   if (run==258440 && ev==108011587) {
 




 // get position in bunch train

  bunchintrain=-1;

  for (std::vector<int>::const_iterator bxit=bunchstartbx_.begin(); bxit!=bunchstartbx_.end(); ++bxit) {

    Int_t bxpos=bx - *bxit;


    // 50ns
    if (bxpos>=0 && bxpos<=70) bunchintrain=bxpos/2;
    // 25ns
    // if (bxpos>=0 && bxpos<=50) bunchintrain=bxpos;

  }

  /*

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
  */  

  edm::ESHandle<CaloGeometry> pG;
  iSetup.get<CaloGeometryRecord>().get(pG);
  const CaloGeometry* geo=pG.product();

  edm::ESHandle<CaloTopology> pTopology;
  iSetup.get<CaloTopologyRecord>().get(pTopology);


  edm::ESHandle<EcalChannelStatus> chanstat;
  iSetup.get<EcalChannelStatusRcd>().get(chanstat);
 
  edm::ESHandle<EcalPedestals> ecalped;
  iSetup.get<EcalPedestalsRcd>().get(ecalped);
  const EcalPedestals* eped=ecalped.product();


  edm::ESHandle<EcalADCToGeVConstant> ecaladcgev;
  iSetup.get<EcalADCToGeVConstantRcd>().get(ecaladcgev);
  const EcalADCToGeVConstant* eadcgev=ecaladcgev.product();
 
  float adctogevconst=eadcgev->getEBValue();
  float adctogevconstee=eadcgev->getEEValue();


  edm::ESHandle<EcalIntercalibConstants> ecalic;
  iSetup.get<EcalIntercalibConstantsRcd>().get(ecalic);
  const EcalIntercalibConstants* eic=ecalic.product();


 // Rechit Collection

  edm::Handle<EcalRecHitCollection> EBhits;
  edm::Handle<EcalRecHitCollection> EEhits;
  edm::Handle<ESRecHitCollection> EShits;

  // AOD
   event.getByToken(tok_EB,EBhits);
   event.getByToken(tok_EE,EEhits);
   event.getByToken(tok_ES,EShits);
 
  // RECO
  // event.getByLabel("ecalRecHit","EcalRecHitsEB",EBhits);
  // event.getByLabel("ecalRecHit","EcalRecHitsEE",EEhits);
 

   edm:: Handle<EBDigiCollection>  EBdigis;
   event.getByToken(tok_EB_digi,EBdigis);

     // event.getByLabel("ecalDigis","ebDigis",EBdigis);
  
   edm::Handle<EEDigiCollection>  EEdigis;
   event.getByToken(tok_EE_digi,EEdigis);
      //event.getByLabel("ecalDigis","eeDigis",EEdigis);
  

 
      //  Handle<EcalUncalibratedRecHitCollection> EBhitsU_multi10;
      //  Handle<EcalUncalibratedRecHitCollection> EEhitsU_multi10;

      //  event.getByLabel(edm::InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEB","reRECO"),EBhitsU_multi10);
      // event.getByLabel(edm::InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEE","reRECO"),EEhitsU_multi10);

      /*

    for (EcalUncalibratedRecHitCollection::const_iterator hitItr6 = EBhitsU_multi10->begin(); hitItr6 != EBhitsU_multi10->end(); ++hitItr6) {



	EcalUncalibratedRecHit hit6           = (*hitItr6);
	EBDetId det6              = hit6.id(); 

	Float_t ucrechit_ampl_eb     = hit6.amplitude();
   
	int rechit_ieta    = det6.ieta();      
	int rechit_iphi    = det6.iphi();


	cout << "UCRECHIT, eta,phi,ampl=" << rechit_ieta << ":" 
	     << rechit_iphi << ":" << ucrechit_ampl_eb << endl;

	if (evcount<100) {

	  barrel_uncalibrh[evcount]->Fill(rechit_iphi-0.5,rechit_ieta+0.5-1*(rechit_ieta>0),ucrechit_ampl_eb);
 
	}

    }




    for (EcalUncalibratedRecHitCollection::const_iterator hitItr6 = EEhitsU_multi10->begin(); hitItr6 != EEhitsU_multi10->end(); ++hitItr6) {

	//	cout << "in EE loop4" << endl;

	EcalUncalibratedRecHit hit6           = (*hitItr6);
	EEDetId det6              = hit6.id(); 


	Float_t ucrechit_ampl_ee     = hit6.amplitude();
	int rechit_ix      = det6.ix();      
	int rechit_iy      = det6.iy();
	int rechit_iz      = det6.zside();

 
	if (evcount<100) {
	  endcap_uncalibrh[evcount]->Fill(rechit_ix-0.5+100*(rechit_iz>0),rechit_iy-0.5,ucrechit_ampl_ee);
	  
	}   

    }

      */


   
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


	 const EcalIntercalibConstantMap &icalMap=eic->getMap();

	 EcalIntercalibConstantMap::const_iterator icalit=icalMap.find(deteb);
	 float icalconst=0;
	 if (icalit!=icalMap.end()) icalconst=(*icalit);
	 ebicval->SetBinContent(i+1,j+1,icalconst);

	 lasercorr=0;
	 lasercorr=laser->getLaserCorrection(EBDetId(deteb), event.time());
	 eblascorr->SetBinContent(i+1,j+1,lasercorr);



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


	 const EcalIntercalibConstantMap &icalMap=eic->getMap();

	 EcalIntercalibConstantMap::const_iterator icalit=icalMap.find(detee);
	 float icalconst=0;
	 if (icalit!=icalMap.end()) icalconst=(*icalit);
	 eeicval->SetBinContent(i+1+100*k,j+1,icalconst);

	 lasercorr=0;
	 lasercorr=laser->getLaserCorrection(EEDetId(detee), event.time());
	 eelascorr->SetBinContent(i+1+100*k,j+1,lasercorr);



	   EcalChannelStatusCode chstatcode;
	   EcalChannelStatusMap::const_iterator chit=chanstat->find(detee);
	   if (chit!=chanstat->end()) {
	     chstatcode=*chit;
	     eechstatus->SetBinContent(i+1+100*k,j+1,chstatcode.getEncodedStatusCode());
	   }
	   
	 }
       }
     }
   }






  // Rechit loop

    cout << "\n\n****EB HITS****" << endl;

  ebsumet=0, ebsumet_kgood=0;

  int rechit_flag_kgood=0;


  for (EcalRecHitCollection::const_iterator hitItr = EBhits->begin(); hitItr != EBhits->end(); ++hitItr) {


    rechit_flag_kweird=0;
    rechit_flag_kdiweird=0;
    rechit_flag_koot=0;
    rechit_flag_kgood=0;


	Int_t ee_kGood=0;
	Int_t ee_kPoorReco=0;
	Int_t ee_kOutOfTime=0;
	Int_t ee_kFaultyHardware=0;
	Int_t ee_kNoisy=0;
	Int_t ee_kPoorCalib=0;
	Int_t ee_kSaturated=0;
	Int_t ee_kLeadingEdgeRecovered=0;
	Int_t ee_kNeighboursRecovered=0;
	Int_t ee_kTowerRecovered=0;
	Int_t ee_kDead=0;
	Int_t ee_kKilled=0;
	Int_t ee_kTPSaturated=0;
	Int_t ee_kL1SpikeFlag=0;
	Int_t ee_kWeird=0;
	Int_t ee_kDiWeird=0;
	Int_t ee_kUnknown=0;
    
    rechit_swisscross=0;
    rechit_ene=0, rechit_et=0, rechit_time=0;
    
    rechit_ieta=0, rechit_iphi=0, rechit_eta=0, rechit_phi=0;
    
    rechit_chi2=0, rechit_chi2oot=0;
    
    db_pedg12=0, db_pedg6=0, db_pedg1=0;
    db_pedrmsg12=0, db_pedrmsg6=0, db_pedrmsg1=0;
    lasercorr=0;
    


    EcalRecHit hit = (*hitItr);
    EBDetId det    = hit.id(); 
    rechit_ieta    = det.ieta();      
    rechit_iphi    = det.iphi();
    rechit_ene     = hit.energy();
    rechit_time    = hit.time();
    rechit_chi2    = hit.chi2();
    //    rechit_chi2oot = hit.outOfTimeChi2();  // not used in Run 2
    



    int ebhashedid = det.hashedIndex();
    
    
    GlobalPoint poseb=geo->getPosition(hit.detid());
    
    
    rechit_eta=poseb.eta();
    rechit_phi=poseb.phi();
    float pf=1.0/cosh(rechit_eta);
    
    rechit_et=rechit_ene*pf;
  


    if (evcount<100) {

      barrel_rh[evcount]->Fill(rechit_iphi-0.5,rechit_ieta+0.5-1*(rechit_ieta>0),rechit_et);
 
    }


    //  if (rechit_et<3.0) continue;

    // swiss-cross calculation

    float s41=0;
    float s42=0;
    float s43=0;
    float s44=0;
    
    s41 = recHitE( det, *EBhits,  1,  0 );
    s42 = recHitE( det, *EBhits,  -1,  0 );
    s43 = recHitE( det, *EBhits,  0,  1 );
    s44 = recHitE( det, *EBhits,  0,  -1 );
    
    float s4=s41+s42+s43+s44;


    if (rechit_ene>0) rechit_swisscross=1.0-s4/rechit_ene;
    

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


    const EcalIntercalibConstantMap &icalMap=eic->getMap();

    EcalIntercalibConstantMap::const_iterator icalit=icalMap.find(det);
    float icalconst=0;
    if (icalit!=icalMap.end()) icalconst=(*icalit);




    if (hit.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird=1;
    if (hit.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird=1;
    if (hit.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot=1;
    

    if (rechit_flag_kweird+rechit_flag_kdiweird+rechit_flag_koot==0) rechit_flag_kgood=1;



    if (rechit_et>0.5) {
      ebsumet+=rechit_et;
      if (rechit_flag_kgood) ebsumet_kgood+=rechit_et;
    }

    
    eboccet->Fill(rechit_iphi-0.5,rechit_ieta+0.5-1*(rechit_ieta>0),rechit_et);
 
    if (rechit_flag_kgood) {
 
       eboccet_kgood->Fill(rechit_iphi-0.5,rechit_ieta+0.5-1*(rechit_ieta>0),rechit_et);

     }


     if (rechit_flag_kweird)  ebflag_kweird->Fill(rechit_iphi-0.5,rechit_ieta+0.5-1*(rechit_ieta>0),1);
     if (rechit_flag_kdiweird)  ebflag_kdiweird->Fill(rechit_iphi-0.5,rechit_ieta+0.5-1*(rechit_ieta>0),1);
     if (rechit_flag_koot)  ebflag_koot->Fill(rechit_iphi-0.5,rechit_ieta+0.5-1*(rechit_ieta>0),1);

    


    // select high energy hits
    if (rechit_et>1.0) {
  
      ebtime->Fill(rechit_iphi-0.5,rechit_ieta+0.5-1*(rechit_ieta>0),rechit_time);

      

      cout << "\nieta,iphi=" << rechit_ieta << " " << rechit_iphi << " energy=" << rechit_ene  << " eta,phi=" << rechit_eta << " " << rechit_phi
	   << " et=" << rechit_et << " time=" << rechit_time << endl;
      cout << "swisscross=" << rechit_swisscross << " lasercorr=" << lasercorr <<  " chi2=" << rechit_chi2 << endl; 
      cout << "kWeird=" << rechit_flag_kweird << " kDiWeird=" << rechit_flag_kdiweird << " kOOT=" << rechit_flag_koot << endl; 


      cout << "calibration values:  lasercalib: " << lasercorr << " ic: " << icalconst << " adc/gev: " << adctogevconst << endl;

      cout << "pedestal values: " << 
	" G12: " << db_pedg12 << "," << db_pedrmsg12 <<
	"  G6: " << db_pedg6 << "," << db_pedrmsg6 <<
	"  G1: " << db_pedg1 << "," << db_pedrmsg1 << endl;


     cout<< "Rechit flags:" << endl;
	if (hit.checkFlag(EcalRecHit::kGood)) ee_kGood=1;
	if (hit.checkFlag(EcalRecHit::kPoorReco)) ee_kPoorReco=1;
	if (hit.checkFlag(EcalRecHit::kOutOfTime)) ee_kOutOfTime=1;
	if (hit.checkFlag(EcalRecHit::kFaultyHardware)) ee_kFaultyHardware=1;
	if (hit.checkFlag(EcalRecHit::kNoisy)) ee_kNoisy=1;
	if (hit.checkFlag(EcalRecHit::kPoorCalib)) ee_kPoorCalib=1;
	if (hit.checkFlag(EcalRecHit::kSaturated)) ee_kSaturated=1;
	if (hit.checkFlag(EcalRecHit::kLeadingEdgeRecovered)) ee_kLeadingEdgeRecovered=1;
	if (hit.checkFlag(EcalRecHit::kNeighboursRecovered)) ee_kNeighboursRecovered=1;
	if (hit.checkFlag(EcalRecHit::kTowerRecovered)) ee_kTowerRecovered=1;
	if (hit.checkFlag(EcalRecHit::kDead)) ee_kDead=1;
	if (hit.checkFlag(EcalRecHit::kKilled)) ee_kKilled=1;
	if (hit.checkFlag(EcalRecHit::kTPSaturated)) ee_kTPSaturated=1;
	if (hit.checkFlag(EcalRecHit::kL1SpikeFlag)) ee_kL1SpikeFlag=1;
	if (hit.checkFlag(EcalRecHit::kWeird)) ee_kWeird=1;
	if (hit.checkFlag(EcalRecHit::kDiWeird)) ee_kDiWeird=1;
	if (hit.checkFlag(EcalRecHit::kUnknown)) ee_kUnknown=1;
	

     

      cout << " kGood=" << ee_kGood << endl;
      cout << " kPoorReco=" << ee_kPoorReco << endl;;
      cout << " kOutOfTime=" << ee_kOutOfTime << endl;;
      cout << " kFaultyHardware=" << ee_kFaultyHardware << endl;
      cout << " kNoisy=" << ee_kNoisy << endl;
      cout << " kPoorCalib=" << ee_kPoorCalib << endl;;
      cout << " kSaturated=" << ee_kSaturated << endl;
      cout << " kLeadingEdgeRecovered=" << ee_kLeadingEdgeRecovered << endl;
      cout << " kNeighboursRecovered=" << ee_kNeighboursRecovered << endl;
      cout << " kTowerRecovered=" << ee_kTowerRecovered << endl;
      cout << " kDead=" << ee_kDead << endl;
      cout << " kKilled=" << ee_kKilled << endl;
      cout << " kTPSaturated=" << ee_kTPSaturated << endl;
      cout << " kL1SpikeFlag=" << ee_kL1SpikeFlag << endl;
      cout << " kWeird=" << ee_kWeird << endl;
      cout << " kDiWeird=" << ee_kDiWeird << endl;
      cout << " kUnknown=" << ee_kUnknown << endl;
    
      
    
      // find digi corresponding to rechit
    
        
      EBDigiCollection::const_iterator digiItrEB= EBdigis->begin();
      while(digiItrEB != EBdigis->end() && digiItrEB->id() != hitItr->id())
	{
	  digiItrEB++;
	}
      
      if (digiItrEB != EBdigis->end()) {
		cout << "DIGI found: " << endl;
	
	EBDataFrame df(*digiItrEB);
	for(int i=0; i<10;++i)
	  {
	    int ebdigiadc=df.sample(i).adc();		
	    int ebdigigain=df.sample(i).gainId();
	    digi_adc[i]=ebdigiadc;
	    digi_gainid[i]=ebdigigain;
	    cout << "sample=" << i+1 << " adc=" << digi_adc[i] << " gainid=" << digi_gainid[i] << endl;
	  }
	
	
      }

      
    }
  }
  


  // EE Rechit loop

  eesumet=0, eesumet_kgood=0;
  Float_t eepsumet=0;
  Float_t eemsumet=0;

    cout << "\n\n****EE HITS****" << endl;

  for (EcalRecHitCollection::const_iterator hitItr = EEhits->begin(); hitItr != EEhits->end(); ++hitItr) {


    rechit_flag_kweird=0;
    rechit_flag_kdiweird=0;
    rechit_flag_koot=0;
    rechit_flag_kgood=0;


	Int_t ee_kGood=0;
	Int_t ee_kPoorReco=0;
	Int_t ee_kOutOfTime=0;
	Int_t ee_kFaultyHardware=0;
	Int_t ee_kNoisy=0;
	Int_t ee_kPoorCalib=0;
	Int_t ee_kSaturated=0;
	Int_t ee_kLeadingEdgeRecovered=0;
	Int_t ee_kNeighboursRecovered=0;
	Int_t ee_kTowerRecovered=0;
	Int_t ee_kDead=0;
	Int_t ee_kKilled=0;
	Int_t ee_kTPSaturated=0;
	Int_t ee_kL1SpikeFlag=0;
	Int_t ee_kWeird=0;
	Int_t ee_kDiWeird=0;
	Int_t ee_kUnknown=0;

    
    rechit_swisscross=0;
    rechit_ene=0, rechit_et=0, rechit_time=0;
    
    rechit_ieta=0, rechit_iphi=0, rechit_eta=0, rechit_phi=0;
    
    rechit_chi2=0, rechit_chi2oot=0;

    lasercorr=0;
    
    int rechit_ix=0;
    int rechit_iy=0;
    int rechit_iz=0;


    EcalRecHit hit = (*hitItr);
    EEDetId det    = hit.id(); 
    rechit_ix      = det.ix();      
    rechit_iy      = det.iy();
    rechit_iz      = det.zside();
    rechit_ene     = hit.energy();
    rechit_time    = hit.time();
    rechit_chi2    = hit.chi2();
    //   rechit_chi2oot = hit.outOfTimeChi2();  // not used in Run 2
    int eehashedid = det.hashedIndex();


    const EcalPedestals::Item *aped=0;
    aped=&eped->endcap(eehashedid);
    float eedb_pedg12=aped->mean_x12;
    float eedb_pedg6=aped->mean_x6;
    float eedb_pedg1=aped->mean_x1;

    float eedb_pedrmsg12=aped->rms_x12;
    float eedb_pedrmsg6=aped->rms_x6;
    float eedb_pedrmsg1=aped->rms_x1;

     const EcalIntercalibConstantMap &icalMap=eic->getMap();

    EcalIntercalibConstantMap::const_iterator icalit=icalMap.find(det);
    float eeicalconst=0;
    if (icalit!=icalMap.end()) eeicalconst=(*icalit);



	if (hit.checkFlag(EcalRecHit::kGood)) ee_kGood=1;
	if (hit.checkFlag(EcalRecHit::kPoorReco)) ee_kPoorReco=1;
	if (hit.checkFlag(EcalRecHit::kOutOfTime)) ee_kOutOfTime=1;
	if (hit.checkFlag(EcalRecHit::kFaultyHardware)) ee_kFaultyHardware=1;
	if (hit.checkFlag(EcalRecHit::kNoisy)) ee_kNoisy=1;
	if (hit.checkFlag(EcalRecHit::kPoorCalib)) ee_kPoorCalib=1;
	if (hit.checkFlag(EcalRecHit::kSaturated)) ee_kSaturated=1;
	if (hit.checkFlag(EcalRecHit::kLeadingEdgeRecovered)) ee_kLeadingEdgeRecovered=1;
	if (hit.checkFlag(EcalRecHit::kNeighboursRecovered)) ee_kNeighboursRecovered=1;
	if (hit.checkFlag(EcalRecHit::kTowerRecovered)) ee_kTowerRecovered=1;
	if (hit.checkFlag(EcalRecHit::kDead)) ee_kDead=1;
	if (hit.checkFlag(EcalRecHit::kKilled)) ee_kKilled=1;
	if (hit.checkFlag(EcalRecHit::kTPSaturated)) ee_kTPSaturated=1;
	if (hit.checkFlag(EcalRecHit::kL1SpikeFlag)) ee_kL1SpikeFlag=1;
	if (hit.checkFlag(EcalRecHit::kWeird)) ee_kWeird=1;
	if (hit.checkFlag(EcalRecHit::kDiWeird)) ee_kDiWeird=1;
	if (hit.checkFlag(EcalRecHit::kUnknown)) ee_kUnknown=1;
	


    
    GlobalPoint poseb=geo->getPosition(hit.detid());
    
    
    rechit_eta=poseb.eta();
    rechit_phi=poseb.phi();
    float pf=1.0/cosh(rechit_eta);
    
    rechit_et=rechit_ene*pf;
 
   if (evcount<100) {
     endcap_rh[evcount]->Fill(rechit_ix-0.5+100*(rechit_iz>0),rechit_iy-0.5,rechit_et);
 
    }   
    
 
    //  if (rechit_et<3.0) continue;

    // swiss-cross calculation

    float s41=0;
    float s42=0;
    float s43=0;
    float s44=0;
    
    s41 = recHitE( det, *EEhits,  1,  0 );
    s42 = recHitE( det, *EEhits,  -1,  0 );
    s43 = recHitE( det, *EEhits,  0,  1 );
    s44 = recHitE( det, *EEhits,  0,  -1 );
    
    float s4=s41+s42+s43+s44;


    if (rechit_ene>0) rechit_swisscross=1.0-s4/rechit_ene;
    

  
    // read lasercalib from db
    lasercorr=laser->getLaserCorrection(EEDetId(det), event.time());


    if (hit.checkFlag(EcalRecHit::kWeird)) rechit_flag_kweird=1;
    if (hit.checkFlag(EcalRecHit::kDiWeird)) rechit_flag_kdiweird=1;
    if (hit.checkFlag(EcalRecHit::kOutOfTime)) rechit_flag_koot=1;
    
  if (rechit_flag_kweird+rechit_flag_kdiweird+rechit_flag_koot==0) rechit_flag_kgood=1;
  


    if (rechit_et>0.5) {
      eesumet+=rechit_et;
      if (rechit_flag_kgood) eesumet_kgood+=rechit_et;

      if (rechit_flag_kgood && rechit_iz==1) eepsumet+=rechit_et;
      if (rechit_flag_kgood && rechit_iz==-1) eemsumet+=rechit_et;

    }

 
     eeoccet->Fill(rechit_ix-0.5+100*(rechit_iz>0),rechit_iy-0.5,rechit_et);

     if (rechit_flag_kgood) {
       eeoccet_kgood->Fill(rechit_ix-0.5+100*(rechit_iz>0),rechit_iy-0.5,rechit_et);

       if (rechit_eta>0) eepocc_etaphi->Fill(rechit_eta,rechit_phi,rechit_et);
       if (rechit_eta<0) eemocc_etaphi->Fill(rechit_eta,rechit_phi,rechit_et);


     }


     if (rechit_flag_kweird) eeflag_kweird->Fill(rechit_ix-0.5+100*(rechit_iz>0),rechit_iy-0.5,1.);
     if (rechit_flag_kdiweird) eeflag_kdiweird->Fill(rechit_ix-0.5+100*(rechit_iz>0),rechit_iy-0.5,1.);
     if (rechit_flag_koot) eeflag_koot->Fill(rechit_ix-0.5+100*(rechit_iz>0),rechit_iy-0.5,1.);



    // select high energy hits
    if (rechit_et>1.0) {
  
      eetime->Fill(rechit_ix-0.5+100*(rechit_iz>0),rechit_iy-0.5,rechit_time);
      

      cout << "\nix,iy,iz=" << rechit_ix << " " << rechit_iy << " " << rechit_iz << " eta,phi=" << rechit_eta << " " << rechit_phi 
	   << " energy=" << rechit_ene 
	   << " et=" << rechit_et << " time=" << rechit_time << endl;
      cout << "swisscross=" << rechit_swisscross << " lasercorr=" << lasercorr << " chi2=" << rechit_chi2 << endl; 
      //    cout << "kWeird=" << rechit_flag_kweird << " kDiWeird=" << rechit_flag_kdiweird << " kOOT=" << rechit_flag_koot << endl;

      cout << "calibration values:  lasercalib: " << lasercorr << " ic: " << eeicalconst << " adc/gev: " << adctogevconstee << endl;

      cout << "pedestal values: " << 
	" G12: " << eedb_pedg12 << "," << eedb_pedrmsg12 <<
	"  G6: " << eedb_pedg6 << "," << eedb_pedrmsg6 <<
	"  G1: " << eedb_pedg1 << "," << eedb_pedrmsg1 << endl;


      cout<< "Rechit flags:" << endl;

     

      cout << " kGood=" << ee_kGood << endl;
      cout << " kPoorReco=" << ee_kPoorReco << endl;;
      cout << " kOutOfTime=" << ee_kOutOfTime << endl;;
      cout << " kFaultyHardware=" << ee_kFaultyHardware << endl;
      cout << " kNoisy=" << ee_kNoisy << endl;
      cout << " kPoorCalib=" << ee_kPoorCalib << endl;;
      cout << " kSaturated=" << ee_kSaturated << endl;
      cout << " kLeadingEdgeRecovered=" << ee_kLeadingEdgeRecovered << endl;
      cout << " kNeighboursRecovered=" << ee_kNeighboursRecovered << endl;
      cout << " kTowerRecovered=" << ee_kTowerRecovered << endl;
      cout << " kDead=" << ee_kDead << endl;
      cout << " kKilled=" << ee_kKilled << endl;
      cout << " kTPSaturated=" << ee_kTPSaturated << endl;
      cout << " kL1SpikeFlag=" << ee_kL1SpikeFlag << endl;
      cout << " kWeird=" << ee_kWeird << endl;
      cout << " kDiWeird=" << ee_kDiWeird << endl;
      cout << " kUnknown=" << ee_kUnknown << endl;
    
      

      // find digi corresponding to rechit
      
      EEDigiCollection::const_iterator digiItrEE= EEdigis->begin();
      while(digiItrEE != EEdigis->end() && digiItrEE->id() != hitItr->id())
	{
	  digiItrEE++;
	}
      
      if (digiItrEE != EEdigis->end()) {
		cout << "DIGI found: " << endl;
	
	EEDataFrame df(*digiItrEE);
	for(int i=0; i<10;++i)
	  {
	    int ebdigiadc=df.sample(i).adc();		
	    int ebdigigain=df.sample(i).gainId();
	    digi_adc[i]=ebdigiadc;
	    digi_gainid[i]=ebdigigain;
	    cout << "sample=" << i+1 << " adc=" << digi_adc[i] << " gainid=" << digi_gainid[i] << endl;
	  }
	
	
      }

      
    }
  }
  





  // ES Rechit loop

  Float_t essumet=0; 
  Float_t essumet_kgood=0;

  Float_t espsumet1=0;
  Float_t espsumet2=0;
  Float_t esmsumet1=0;
  Float_t esmsumet2=0;

    cout << "\n\n****ES HITS****" << endl;

  for (ESRecHitCollection::const_iterator hitItr = EShits->begin(); hitItr != EShits->end(); ++hitItr) {

    
    int rechit_ix=0;
    int rechit_iy=0;
    int rechit_iz=0;
    int rechit_plane=0;

    EcalRecHit hit = (*hitItr);
    ESDetId det    = hit.id(); 
    rechit_ix      = det.six();      
    rechit_iy      = det.siy();
    rechit_iz      = det.zside();
    rechit_plane   = det.plane();

    rechit_ene     = hit.energy();
    rechit_time     = hit.time();

    
    GlobalPoint poseb=geo->getPosition(hit.detid());
    
    
    rechit_eta=poseb.eta();
    rechit_phi=poseb.phi();
    float pf=1.0/cosh(rechit_eta);
    
    rechit_et=rechit_ene*pf;
  

    Int_t isgood=1;
 
    if (hit.recoFlag()==14 || hit.recoFlag()==1 || (hit.recoFlag()<=10 && hit.recoFlag()>=5)) isgood=0;




    if (rechit_et>0.) {
      essumet+=rechit_et;
      if (isgood) essumet_kgood+=rechit_et;
    }

 
     eeoccet->Fill(rechit_ix-0.5+100*(rechit_iz>0),rechit_iy-0.5,rechit_et);

     if (rechit_flag_kgood) {

       if (rechit_iz==1) {

         if (rechit_plane==1) {
	   esoccet_esp1->Fill(rechit_ix,rechit_iy,rechit_et);
	   esp1occ_etaphi->Fill(rechit_eta,rechit_phi,rechit_et);
	   espsumet1+=rechit_et;
	 }

         if (rechit_plane==2) {
	   esoccet_esp2->Fill(rechit_ix,rechit_iy,rechit_et);
	   esp2occ_etaphi->Fill(rechit_eta,rechit_phi,rechit_et);
	   espsumet2+=rechit_et;
	 }

       }
       
   
       if (rechit_iz==-1) {

         if (rechit_plane==1) {
	   esoccet_esm1->Fill(rechit_ix,rechit_iy,rechit_et);
	   esm1occ_etaphi->Fill(rechit_eta,rechit_phi,rechit_et);
	   esmsumet1+=rechit_et;

	 }
         if (rechit_plane==2) {
	   esoccet_esm2->Fill(rechit_ix,rechit_iy,rechit_et);
	   esm2occ_etaphi->Fill(rechit_eta,rechit_phi,rechit_et);
	   esmsumet2+=rechit_et;

	 }

	
       }


     }





    // select high energy hits
    if (rechit_ene>0.1) {
  
      
      cout << "\nix,iy,iz, plane=" << rechit_ix << " " << rechit_iy << " " << rechit_iz << " " << rechit_plane << " eta,phi=" << rechit_eta << " " << rechit_phi 
	   << " energy=" << rechit_ene 
	   << " et=" << rechit_et << " time=" << rechit_time << endl;
  
      cout << "good rechit" << isgood << endl;

      

      
    }
  }
  

  cout << "EB sumet=" << ebsumet << " EB sumet (kGood)=" << ebsumet_kgood << endl;


  cout << "EE sumet=" << eesumet << " EE sumet (kGood)=" << eesumet_kgood << endl;
  cout<< " EE+ sumet=" << eepsumet << " EE- sumet=" << eemsumet << endl;


  cout << "ES sumet=" << essumet << " ES sumet (kGood)=" << essumet_kgood << endl;

  cout << " ES+ sumet, plane 1=" << espsumet1 << " ES+ sumet, plane 2=" << espsumet2 << endl;
  cout << " ES- sumet, plane 1=" << esmsumet1 << " ES- sumet, plane 2=" << esmsumet2 << endl;

  cout << "\n\n";

  //  }
  


  //   }
  
  evcount++;  

}
//////////////////////////////////////////////////////////////////////////////////////////
EventAnalyser::~EventAnalyser() 
{
  delete file_;
  delete dataTree_;
}
//}

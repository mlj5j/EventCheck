#ifndef SPIKE_ANALYSER_H
#define SPIKE_ANALYSER_H

#include "TTree.h"
#include "TH1D.h"
#include "TH3D.h"
#include "TH2F.h"
#include "TFile.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

class EcalCleaningAlgo;

class EventAnalyser : public edm::EDAnalyzer 
{
 public:
  explicit EventAnalyser(edm::ParameterSet const& cfg);
  virtual void beginJob();
  virtual void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
  virtual void endJob();
  ~EventAnalyser();
  
 private:	

  float recHitE( const DetId id, 
		 const EcalRecHitCollection &recHits);
  
  float recHitE( const DetId id, 
		 const EcalRecHitCollection & recHits,
		 int di, int dj );
  

  std::string histogramFile_;
  std::string jets_;
  std::string tracks_;
  std::string vertex_coll_;
  std::string ebhitcoll_;
  std::string eehitcoll_;
  const std::vector<int> bunchstartbx_;
  
  bool isMC_;

  EcalCleaningAlgo * cleaningAlgo_;  

  
  TFile* file_;
  TTree* dataTree_;
  
  
  int run, ev, lumi, bx, orbit, bunchintrain;
  int numvtx, numgoodvtx;

  float rechit_ene, rechit_et, rechit_time;

  int rechit_ieta, rechit_iphi;

  float rechit_eta, rechit_phi;

  float rechit_swisscross;

  float rechit_chi2, rechit_chi2oot;

  int rechit_flag_koot, rechit_flag_kweird, rechit_flag_kdiweird;

  float db_pedg12, db_pedg6, db_pedg1;
  float db_pedrmsg12, db_pedrmsg6, db_pedrmsg1;
  float lasercorr;

  float ebsumet, ebsumet_kgood;
  float eesumet, eesumet_kgood;



  int digi_adc[10];
  int digi_gainid[10];

  TH2F **barrel_uncalibrh;

  TH2F **endcap_uncalibrh;


  TH2F **barrel_rh;

  TH2F **endcap_rh;


  TH2D *ebchstatus;
  TH2D *eechstatus;


   TH2D *ebpedmean_g12;
    TH2D *ebpedmean_g6;
    TH2D *ebpedmean_g1;

    TH2D *ebpedrms_g12;
    TH2D *ebpedrms_g6;
    TH2D *ebpedrms_g1;


    TH2D *eepedmean_g12;
    TH2D *eepedmean_g6;
    TH2D *eepedmean_g1;

    TH2D *eepedrms_g12;
    TH2D *eepedrms_g6;
    TH2D *eepedrms_g1;

    TH2D *ebicval;
    TH2D *eeicval;

    TH2D *eblascorr;
    TH2D *eelascorr;



  int evcount;

  TH2F* eboccet;
  TH2F* eeoccet;
  TH2F* ebtime;
  TH2F* eetime;


  TH2F *eboccet_kgood;
  TH2F *eeoccet_kgood;

  TH2F *ebflag_kweird;
  TH2F *eeflag_kweird;

  TH2F *ebflag_kdiweird;
  TH2F *eeflag_kdiweird;

  TH2F *ebflag_koot;
  TH2F *eeflag_koot;

  TH2F *esoccet_esp1;
  TH2F *esoccet_esp2;

  TH2F *esoccet_esm1;
  TH2F *esoccet_esm2;


  TH2F *eepocc_etaphi;
  TH2F *eemocc_etaphi;

  TH2F *esp1occ_etaphi;
  TH2F *esp2occ_etaphi;
  TH2F *esm1occ_etaphi;
  TH2F *esm2occ_etaphi;


  edm::EDGetTokenT<EBRecHitCollection> tok_EB;
  edm::EDGetTokenT<EERecHitCollection> tok_EE;
  edm::EDGetTokenT<ESRecHitCollection> tok_ES;



  edm::EDGetTokenT<EBDigiCollection> tok_EB_digi;
  edm::EDGetTokenT<EEDigiCollection> tok_EE_digi;

  edm::EDGetTokenT<reco::GsfElectronCollection> tok_elec;



};
#endif  

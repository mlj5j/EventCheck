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
#include "DataFormats/DetId/interface/DetId.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

class EcalCleaningAlgo;

class SpikeAnalyser : public edm::EDAnalyzer 
{
 public:
  explicit SpikeAnalyser(edm::ParameterSet const& cfg);
  virtual void beginJob();
  virtual void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
  virtual void endJob();
  ~SpikeAnalyser();
  
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
  TTree* dataTree2_;
  
  
  int run, ev, lumi, bx, orbit, bunchintrain;
  int numvtx, numgoodvtx;
  int rechit_ieta, rechit_iphi;
  float rechit_eta, rechit_phi;

  int rechit_ix, rechit_iy, rechit_iz;
  float rechit_eta_ee, rechit_phi_ee;


  float rechit_ene_weights53, rechit_et_weights53, rechit_time_weights53;
  float rechit_swisscross_weights53;  
  float rechit_swisscross_zs_weights53;
  float rechit_swisscross_zs_weights74;
  float rechit_swisscross_zs_multi5;
  float rechit_swisscross_zs_multi10;
  float rechit_swisscross_zs_multi1;

  float rechit_swisscross_thresh;
  int rechit_flag_kweird_calc;

  float rechit_chi2_weights53;
  int rechit_flag_koot_weights53, rechit_flag_kweird_weights53, rechit_flag_kdiweird_weights53;



  float rechit_ene_weights74, rechit_et_weights74, rechit_time_weights74;
  float rechit_swisscross_weights74;
  float rechit_chi2_weights74;
  int rechit_flag_koot_weights74, rechit_flag_kweird_weights74, rechit_flag_kdiweird_weights74;

  float rechit_ene_multi5, rechit_et_multi5, rechit_time_multi5;
  float rechit_swisscross_multi5;
  float rechit_chi2_multi5;
  int rechit_flag_koot_multi5, rechit_flag_kweird_multi5, rechit_flag_kdiweird_multi5;

  float rechit_ene_multi10, rechit_et_multi10, rechit_time_multi10;
  float rechit_swisscross_multi10;
  float rechit_chi2_multi10;
  int rechit_flag_koot_multi10, rechit_flag_kweird_multi10, rechit_flag_kdiweird_multi10;
 
  float rechit_ene_multi1, rechit_et_multi1, rechit_time_multi1;
  float rechit_swisscross_multi1;
  float rechit_chi2_multi1;
  int rechit_flag_koot_multi1, rechit_flag_kweird_multi1, rechit_flag_kdiweird_multi1;


  float db_pedg12, db_pedg6, db_pedg1;
  float db_pedrmsg12, db_pedrmsg6, db_pedrmsg1;
  float lasercorr;

  int digi_adc[10];
  int digi_gainid[10];

  TH2F *ebene;
  TH2F *ebtime;

  int isnearcrack;




  float rechit_ene_weights53_ee, rechit_et_weights53_ee, rechit_time_weights53_ee;
 
  float rechit_chi2_weights53_ee;
  int rechit_flag_koot_weights53_ee, rechit_flag_kweird_weights53_ee, rechit_flag_kdiweird_weights53_ee;



  float rechit_ene_weights74_ee, rechit_et_weights74_ee, rechit_time_weights74_ee;
 
  float rechit_chi2_weights74_ee;
  int rechit_flag_koot_weights74_ee, rechit_flag_kweird_weights74_ee, rechit_flag_kdiweird_weights74_ee;

  float rechit_ene_multi5_ee, rechit_et_multi5_ee, rechit_time_multi5_ee;
 
  float rechit_chi2_multi5_ee;
  int rechit_flag_koot_multi5_ee, rechit_flag_kweird_multi5_ee, rechit_flag_kdiweird_multi5_ee;

  float rechit_ene_multi10_ee, rechit_et_multi10_ee, rechit_time_multi10_ee;
 
  float rechit_chi2_multi10_ee;
  int rechit_flag_koot_multi10_ee, rechit_flag_kweird_multi10_ee, rechit_flag_kdiweird_multi10_ee;
 
  float rechit_ene_multi1_ee, rechit_et_multi1_ee, rechit_time_multi1_ee;
 
  float rechit_chi2_multi1_ee;
  int rechit_flag_koot_multi1_ee, rechit_flag_kweird_multi1_ee, rechit_flag_kdiweird_multi1_ee;


  float db_pedg12_ee, db_pedg6_ee, db_pedg1_ee;
  float db_pedrmsg12_ee, db_pedrmsg6_ee, db_pedrmsg1_ee;
  float lasercorr_ee;

  int digi_adc_ee[10];
  int digi_gainid_ee[10];

  float uncalibrechit_multi10_ampl_ee;
  float uncalibrechit_multi10_amperr_ee;
  float uncalibrechit_multi10_pedestal_ee;
  float uncalibrechit_multi10_jitter_ee;
  float uncalibrechit_multi10_chi2_ee;

  float uncalibrechit_multi10_ootampl_ee[10]; 


  float uncalibrechit_multi5_ampl_ee;
  float uncalibrechit_multi5_amperr_ee;
  float uncalibrechit_multi5_pedestal_ee;
  float uncalibrechit_multi5_jitter_ee;
  float uncalibrechit_multi5_chi2_ee;

  float uncalibrechit_multi5_ootampl_ee[10]; 


  float uncalibrechit_multi1_ampl_ee;
  float uncalibrechit_multi1_amperr_ee;
  float uncalibrechit_multi1_pedestal_ee;
  float uncalibrechit_multi1_jitter_ee;
  float uncalibrechit_multi1_chi2_ee;

  float uncalibrechit_multi1_ootampl_ee[10]; 

  float uncalibrechit_multi10_ampl_eb;
  float uncalibrechit_multi10_amperr_eb;
  float uncalibrechit_multi10_pedestal_eb;
  float uncalibrechit_multi10_jitter_eb;
  float uncalibrechit_multi10_chi2_eb;

  float uncalibrechit_multi10_ootampl_eb[10]; 


  float uncalibrechit_multi5_ampl_eb;
  float uncalibrechit_multi5_amperr_eb;
  float uncalibrechit_multi5_pedestal_eb;
  float uncalibrechit_multi5_jitter_eb;
  float uncalibrechit_multi5_chi2_eb;

  float uncalibrechit_multi5_ootampl_eb[10]; 


  float uncalibrechit_multi1_ampl_eb;
  float uncalibrechit_multi1_amperr_eb;
  float uncalibrechit_multi1_pedestal_eb;
  float uncalibrechit_multi1_jitter_eb;
  float uncalibrechit_multi1_chi2_eb;

  float uncalibrechit_multi1_ootampl_eb[10]; 





};
#endif  

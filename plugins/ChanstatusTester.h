#ifndef CHANSTATUSTESTER_H
#define CHANSTATUSTESTER_H

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

class ChanstatusTester : public edm::EDAnalyzer 
{
  public:
    explicit ChanstatusTester(edm::ParameterSet const& cfg);
    virtual void beginJob();
    virtual void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
    virtual void endJob();
    ~ChanstatusTester();
    void scan5x5(const DetId & det, const edm::Handle<EcalRecHitCollection> &hits, const edm::ESHandle<CaloTopology>  &caloTopo,const edm::ESHandle<CaloGeometry>  &geometry, int &nHits, float & totEt);


  private:

    float e4e1(const DetId& id, const EcalRecHitCollection& rhs);
    float e4e1v2(const DetId& id, const EcalRecHitCollection& rhs);
    float e6e2 (const DetId& id, const EcalRecHitCollection& rhs);


    float recHitE( const DetId id, 
		   const EcalRecHitCollection &recHits);
	
    float recHitE( const DetId id, 
		   const EcalRecHitCollection & recHits,
		   int di, int dj );

	   
    const std::vector<DetId> neighbours(const DetId& id);



    edm::InputTag L1GtReadoutRecordTag;
    std::string histogramFile_;
    std::string jets_;
    std::string tracks_;
    std::string vertex_coll_;
    std::string jetcorr_;
    std::string ebhitcoll_;
    std::string eehitcoll_;
    std::string pfhitcoll1_;
    std::string pfhitcoll2_;
    bool isMC_;
    std::string photonProducer_;
    std::string photonCollection_;



    TFile* file_;
    TTree* dataTree_;
    TH1D *histo_event_;

    TH1D *eb_rechitenergy_;
    TH1D *ee_rechitenergy_;


    TH1D *eb_rechitenergy_spiketag;
    TH1D *eb_rechitenergy_spiketag_kweird;
    TH1D *eb_rechitenergy_spiketag_swisscross;
    TH1D *eb_rechitenergy_spiketag_noisedep;


    TH1D *eb_rechitenergy_sctag;
    TH1D *eb_rechitenergy_sctag_kweird;
    TH1D *eb_rechitenergy_sctag_swisscross;
    TH1D *eb_rechitenergy_sctag_noisedep;


    TH1D *eb_rechitenergy_kweird;
    TH1D *eb_rechitenergy_swisscross;
    TH1D *eb_rechitenergy_knotweird;

    TH2D *ebchstatus;
    TH2D *eechstatus;


    TH1D *ee_rechitenergy_notypeb_;
 
    TH1D *eb_rechitenergy_02;
    TH1D *eb_rechitenergy_04;
    TH1D *eb_rechitenergy_06;
    TH1D *eb_rechitenergy_08;
    TH1D *eb_rechitenergy_10;
    TH1D *eb_rechitenergy_12;
    TH1D *eb_rechitenergy_14;
    TH1D *eb_rechitenergy_148;


    TH1D *ee_rechitenergy_16;
    TH1D *ee_rechitenergy_18;
    TH1D *ee_rechitenergy_20;
    TH1D *ee_rechitenergy_22;
    TH1D *ee_rechitenergy_24;
    TH1D *ee_rechitenergy_26;
    TH1D *ee_rechitenergy_28;
    TH1D *ee_rechitenergy_30;


    TH1D *e4_over_noise;
    TH1D *e4_over_noise_spiketag;
    TH1D *e4_over_noise_sctag;

    TH1D *ee_rechitet_16;
    TH1D *ee_rechitet_18;
    TH1D *ee_rechitet_20;
    TH1D *ee_rechitet_22;
    TH1D *ee_rechitet_24;
    TH1D *ee_rechitet_26;
    TH1D *ee_rechitet_28;
    TH1D *ee_rechitet_30;



    TH1D *eb_rechitet_02;
    TH1D *eb_rechitet_04;
    TH1D *eb_rechitet_06;
    TH1D *eb_rechitet_08;
    TH1D *eb_rechitet_10;
    TH1D *eb_rechitet_12;
    TH1D *eb_rechitet_14;
    TH1D *eb_rechitet_148;



    TH2D *etaphi_ped;
    TH2D *etaphi_pedsq;
    TH2D *etaphi_pedn;
    TH2D *etaphi_pednoise;



    TH1D *time_4gev_spiketime;
    TH1D *time_10gev_spiketime;



    TH1D *time_4gev_spikekweird;
    TH1D *time_10gev_spikekweird;

    TH1D *time_4gev_swisscross;
    TH1D *time_10gev_swisscross;

    TH1D *pu_4gev_spiketime;
    TH1D *pu_10gev_spiketime;

    TH1D *pu_4gev_spikekweird;
    TH1D *pu_10gev_spikekweird;

    TH1D *pu_4gev_swisscross;
    TH1D *pu_10gev_swisscross;


    TH1D *ene_4gev_spiketime;
    TH1D *ene_10gev_spiketime;

    TH1D *ene_4gev_spikekweird;
    TH1D *ene_10gev_spikekweird;


    TH1D *ene_4gev_swisscross;
    TH1D *ene_10gev_swisscross;

    TH2D *pu_vs_ene_spiketime;
    TH2D *pu_vs_ene_spikekweird;
    TH2D *pu_vs_ene_swisscross;



    TH2D *eb_rechitetvspu_05;
    TH2D *eb_rechitetvspu_10;
    TH2D *eb_rechitetvspu_15;

    TH2D *ee_rechitetvspu_20;
    TH2D *ee_rechitetvspu_25;
    TH2D *ee_rechitetvspu_30;

    TH2D *eventenergy;
    TH2D *eventet;
    TH2D *eventtime;
    TH2D *eventkoot;
    TH2D *eventkdiweird;


    TH2D *spike_timevset_all_08;
    TH2D *spike_timevset_highest_08;
    TH2D *spike_timevset_koot_all_08;
    TH2D *spike_timevset_koot_highest_08;
    TH2D *spike_timevset_kdiweird_all_08;
    TH2D *spike_timevset_kdiweird_highest_08;

    TH2D *spike_timevset_all_07;
    TH2D *spike_timevset_highest_07;
    TH2D *spike_timevset_koot_all_07;
    TH2D *spike_timevset_koot_highest_07;
    TH2D *spike_timevset_kdiweird_all_07;
    TH2D *spike_timevset_kdiweird_highest_07;

    TH2D *spike_timevset_all_06;
    TH2D *spike_timevset_highest_06;
    TH2D *spike_timevset_koot_all_06;
    TH2D *spike_timevset_koot_highest_06;
    TH2D *spike_timevset_kdiweird_all_06;
    TH2D *spike_timevset_kdiweird_highest_06;


 
    TH1D *eb_rechitet_;
    TH1D *ee_rechitet_;


    TH2D *eb_rechiten_vs_eta;

    TH2D *eb_rechitet_vs_eta;



    TH2D *eep_rechitet_vs_eta;
    TH2D *eep_rechitet_vs_phi;

    TH2D *eem_rechitet_vs_eta;
    TH2D *eem_rechitet_vs_phi;



    TH2D *eep_rechiten_vs_eta;
    TH2D *eep_rechiten_vs_phi;

    TH2D *eem_rechiten_vs_eta;
    TH2D *eem_rechiten_vs_phi;

    TH2D *swisscross_vs_energy_spiketag;
    TH2D *swisscross_vs_energy_spiketag_kweird;


    TH2D *swisscross_vs_energy_sctag;
    TH2D *swisscross_vs_energy_sctag_kweird;

    TH2D *time_vs_energy_spiketag;
    TH2D *time_vs_energy_spiketag_kweird;
    TH2D *time_vs_energy_spiketag_swisscross;

    TH2D *time_vs_energy_sctag;
    TH2D *time_vs_energy_sctag_kweird;
    TH2D *time_vs_energy_sctag_swisscross;


    TH2D *noise2d;
    

    TH2D *tower_spike_et_swisstt;
    TH2D *tower_spike_ene_swisstt;

    TH2D *tower_spike_et_swisstt_thresh08;
    TH2D *tower_spike_ene_swisstt_thresh08;

    TH2D *tower_spike_et_swisstt_thresh10;
    TH2D *tower_spike_ene_swisstt_thresh10;

    TH2D *tower_spike_et_swisstt_thresh12;
    TH2D *tower_spike_ene_swisstt_thresh12;

    TH2D *tower_spike_et_swisstt_thresh15;
    TH2D *tower_spike_ene_swisstt_thresh15;

    TH2D *tower_spike_et_swisstt_thresh20;
    TH2D *tower_spike_ene_swisstt_thresh20;




    TH2D *sc_tower_spike_et_swisstt;
    TH2D *sc_tower_spike_ene_swisstt;

    TH2D *sc_tower_spike_et_swisstt_thresh08;
    TH2D *sc_tower_spike_ene_swisstt_thresh08;

    TH2D *sc_tower_spike_et_swisstt_thresh10;
    TH2D *sc_tower_spike_ene_swisstt_thresh10;

    TH2D *sc_tower_spike_et_swisstt_thresh12;
    TH2D *sc_tower_spike_ene_swisstt_thresh12;

    TH2D *sc_tower_spike_et_swisstt_thresh15;
    TH2D *sc_tower_spike_ene_swisstt_thresh15;

    TH2D *sc_tower_spike_et_swisstt_thresh20;
    TH2D *sc_tower_spike_ene_swisstt_thresh20;



    TH2D *tower_spike_et_swiss;
    TH2D *tower_spike_ene_swiss;

    TH2D *tower_spike_et_swiss_thresh08;
    TH2D *tower_spike_ene_swiss_thresh08;

    TH2D *tower_spike_et_swiss_thresh10;
    TH2D *tower_spike_ene_swiss_thresh10;

    TH2D *tower_spike_et_swiss_thresh12;
    TH2D *tower_spike_ene_swiss_thresh12;

    TH2D *tower_spike_et_swiss_thresh15;
    TH2D *tower_spike_ene_swiss_thresh15;

    TH2D *tower_spike_et_swiss_thresh20;
    TH2D *tower_spike_ene_swiss_thresh20;




    TH2D *sc_tower_spike_et_swiss;
    TH2D *sc_tower_spike_ene_swiss;

    TH2D *sc_tower_spike_et_swiss_thresh08;
    TH2D *sc_tower_spike_ene_swiss_thresh08;

    TH2D *sc_tower_spike_et_swiss_thresh10;
    TH2D *sc_tower_spike_ene_swiss_thresh10;

    TH2D *sc_tower_spike_et_swiss_thresh12;
    TH2D *sc_tower_spike_ene_swiss_thresh12;

    TH2D *sc_tower_spike_et_swiss_thresh15;
    TH2D *sc_tower_spike_ene_swiss_thresh15;

    TH2D *sc_tower_spike_et_swiss_thresh20;
    TH2D *sc_tower_spike_ene_swiss_thresh20;


    TH2D *tower_spike_ene_swiss_t10;
    TH2D *tower_spike_ene_swiss_t07;
    TH2D *tower_spike_ene_swiss_t05;
    TH2D *tower_spike_ene_swiss_t04;
    TH2D *tower_spike_ene_swiss_t03;

    TH2D *tower_spike_ene_swiss_t10_thresh08;
    TH2D *tower_spike_ene_swiss_t07_thresh08;
    TH2D *tower_spike_ene_swiss_t05_thresh08;
    TH2D *tower_spike_ene_swiss_t04_thresh08;
    TH2D *tower_spike_ene_swiss_t03_thresh08;

    TH2D *tower_spike_ene_swiss_t10_thresh10;
    TH2D *tower_spike_ene_swiss_t07_thresh10;
    TH2D *tower_spike_ene_swiss_t05_thresh10;
    TH2D *tower_spike_ene_swiss_t04_thresh10;
    TH2D *tower_spike_ene_swiss_t03_thresh10;


    TH2D *tower_spike_ene_swiss_t10_thresh12;
    TH2D *tower_spike_ene_swiss_t07_thresh12;
    TH2D *tower_spike_ene_swiss_t05_thresh12;
    TH2D *tower_spike_ene_swiss_t04_thresh12;
    TH2D *tower_spike_ene_swiss_t03_thresh12;


    TH2D *tower_spike_ene_swiss_t10_thresh15;
    TH2D *tower_spike_ene_swiss_t07_thresh15;
    TH2D *tower_spike_ene_swiss_t05_thresh15;
    TH2D *tower_spike_ene_swiss_t04_thresh15;
    TH2D *tower_spike_ene_swiss_t03_thresh15;

    TH2D *tower_spike_ene_swiss_t10_thresh20;
    TH2D *tower_spike_ene_swiss_t07_thresh20;
    TH2D *tower_spike_ene_swiss_t05_thresh20;
    TH2D *tower_spike_ene_swiss_t04_thresh20;
    TH2D *tower_spike_ene_swiss_t03_thresh20;



    TH2D *sc_tower_spike_ene_swiss_t10;
    TH2D *sc_tower_spike_ene_swiss_t07;
    TH2D *sc_tower_spike_ene_swiss_t05;
    TH2D *sc_tower_spike_ene_swiss_t04;
    TH2D *sc_tower_spike_ene_swiss_t03;


    TH2D *sc_tower_spike_ene_swiss_t10_thresh08;
    TH2D *sc_tower_spike_ene_swiss_t07_thresh08;
    TH2D *sc_tower_spike_ene_swiss_t05_thresh08;
    TH2D *sc_tower_spike_ene_swiss_t04_thresh08;
    TH2D *sc_tower_spike_ene_swiss_t03_thresh08;

    TH2D *sc_tower_spike_ene_swiss_t10_thresh10;
    TH2D *sc_tower_spike_ene_swiss_t07_thresh10;
    TH2D *sc_tower_spike_ene_swiss_t05_thresh10;
    TH2D *sc_tower_spike_ene_swiss_t04_thresh10;
    TH2D *sc_tower_spike_ene_swiss_t03_thresh10;


    TH2D *sc_tower_spike_ene_swiss_t10_thresh12;
    TH2D *sc_tower_spike_ene_swiss_t07_thresh12;
    TH2D *sc_tower_spike_ene_swiss_t05_thresh12;
    TH2D *sc_tower_spike_ene_swiss_t04_thresh12;
    TH2D *sc_tower_spike_ene_swiss_t03_thresh12;


    TH2D *sc_tower_spike_ene_swiss_t10_thresh15;
    TH2D *sc_tower_spike_ene_swiss_t07_thresh15;
    TH2D *sc_tower_spike_ene_swiss_t05_thresh15;
    TH2D *sc_tower_spike_ene_swiss_t04_thresh15;
    TH2D *sc_tower_spike_ene_swiss_t03_thresh15;

    TH2D *sc_tower_spike_ene_swiss_t10_thresh20;
    TH2D *sc_tower_spike_ene_swiss_t07_thresh20;
    TH2D *sc_tower_spike_ene_swiss_t05_thresh20;
    TH2D *sc_tower_spike_ene_swiss_t04_thresh20;
    TH2D *sc_tower_spike_ene_swiss_t03_thresh20;





    TH2D *strip_prob_et;
    TH2D *strip_prob_ene;
    TH2D *tower_spike_et;
    TH2D *tower_spike_ene;

    TH3D *strip_prob_et_pu;
    TH3D *strip_prob_ene_pu;
    TH3D *tower_spike_et_pu;
    TH3D *tower_spike_ene_pu;



    TH2D *strip_prob_et_thresh08;
    TH2D *strip_prob_ene_thresh08;
    TH2D *tower_spike_et_thresh08;
    TH2D *tower_spike_ene_thresh08;

    TH3D *strip_prob_et_pu_thresh08;
    TH3D *strip_prob_ene_pu_thresh08;
    TH3D *tower_spike_et_pu_thresh08;
    TH3D *tower_spike_ene_pu_thresh08;



    TH2D *strip_prob_et_thresh10;
    TH2D *strip_prob_ene_thresh10;
    TH2D *tower_spike_et_thresh10;
    TH2D *tower_spike_ene_thresh10;

    TH3D *strip_prob_et_pu_thresh10;
    TH3D *strip_prob_ene_pu_thresh10;
    TH3D *tower_spike_et_pu_thresh10;
    TH3D *tower_spike_ene_pu_thresh10;



    TH2D *strip_prob_et_thresh12;
    TH2D *strip_prob_ene_thresh12;
    TH2D *tower_spike_et_thresh12;
    TH2D *tower_spike_ene_thresh12;

    TH3D *strip_prob_et_pu_thresh12;
    TH3D *strip_prob_ene_pu_thresh12;
    TH3D *tower_spike_et_pu_thresh12;
    TH3D *tower_spike_ene_pu_thresh12;



    TH2D *strip_prob_et_thresh15;
    TH2D *strip_prob_ene_thresh15;
    TH2D *tower_spike_et_thresh15;
    TH2D *tower_spike_ene_thresh15;

    TH3D *strip_prob_et_pu_thresh15;
    TH3D *strip_prob_ene_pu_thresh15;
    TH3D *tower_spike_et_pu_thresh15;
    TH3D *tower_spike_ene_pu_thresh15;



    TH2D *strip_prob_et_thresh20;
    TH2D *strip_prob_ene_thresh20;
    TH2D *tower_spike_et_thresh20;
    TH2D *tower_spike_ene_thresh20;

    TH3D *strip_prob_et_pu_thresh20;
    TH3D *strip_prob_ene_pu_thresh20;
    TH3D *tower_spike_et_pu_thresh20;
    TH3D *tower_spike_ene_pu_thresh20;




    TH2D *strip_prob_et_300;
    TH2D *strip_prob_ene_300;
    TH2D *tower_spike_et_300;
    TH2D *tower_spike_ene_300;

    TH2D *strip_prob_et_pu0;
    TH2D *strip_prob_ene_pu0;
    TH2D *tower_spike_et_pu0;
    TH2D *tower_spike_ene_pu0;

    TH2D *strip_prob_et_pu10;
    TH2D *strip_prob_ene_pu10;
    TH2D *tower_spike_et_pu10;
    TH2D *tower_spike_ene_pu10;

    TH2D *strip_prob_et_pu20;
    TH2D *strip_prob_ene_pu20;
    TH2D *tower_spike_et_pu20;
    TH2D *tower_spike_ene_pu20;


    TH2D *strip_prob_et_mod1;
    TH2D *strip_prob_ene_mod1;
    TH2D *tower_spike_et_mod1;
    TH2D *tower_spike_ene_mod1;

    TH2D *strip_prob_et_mod2;
    TH2D *strip_prob_ene_mod2;
    TH2D *tower_spike_et_mod2;
    TH2D *tower_spike_ene_mod2;

    TH2D *strip_prob_et_mod3;
    TH2D *strip_prob_ene_mod3;
    TH2D *tower_spike_et_mod3;
    TH2D *tower_spike_ene_mod3;

    TH2D *strip_prob_et_mod4;
    TH2D *strip_prob_ene_mod4;
    TH2D *tower_spike_et_mod4;
    TH2D *tower_spike_ene_mod4;




    TH2D *sc_strip_prob_et;
    TH2D *sc_strip_prob_ene;
    TH2D *sc_tower_spike_et;
    TH2D *sc_tower_spike_ene;

    TH3D *sc_strip_prob_et_pu;
    TH3D *sc_strip_prob_ene_pu;
    TH3D *sc_tower_spike_et_pu;
    TH3D *sc_tower_spike_ene_pu;


    TH2D *sc_strip_prob_et_thresh08;
    TH2D *sc_strip_prob_ene_thresh08;
    TH2D *sc_tower_spike_et_thresh08;
    TH2D *sc_tower_spike_ene_thresh08;

    TH3D *sc_strip_prob_et_pu_thresh08;
    TH3D *sc_strip_prob_ene_pu_thresh08;
    TH3D *sc_tower_spike_et_pu_thresh08;
    TH3D *sc_tower_spike_ene_pu_thresh08;


    TH2D *sc_strip_prob_et_thresh10;
    TH2D *sc_strip_prob_ene_thresh10;
    TH2D *sc_tower_spike_et_thresh10;
    TH2D *sc_tower_spike_ene_thresh10;

    TH3D *sc_strip_prob_et_pu_thresh10;
    TH3D *sc_strip_prob_ene_pu_thresh10;
    TH3D *sc_tower_spike_et_pu_thresh10;
    TH3D *sc_tower_spike_ene_pu_thresh10;


    TH2D *sc_strip_prob_et_thresh12;
    TH2D *sc_strip_prob_ene_thresh12;
    TH2D *sc_tower_spike_et_thresh12;
    TH2D *sc_tower_spike_ene_thresh12;

    TH3D *sc_strip_prob_et_pu_thresh12;
    TH3D *sc_strip_prob_ene_pu_thresh12;
    TH3D *sc_tower_spike_et_pu_thresh12;
    TH3D *sc_tower_spike_ene_pu_thresh12;


    TH2D *sc_strip_prob_et_thresh15;
    TH2D *sc_strip_prob_ene_thresh15;
    TH2D *sc_tower_spike_et_thresh15;
    TH2D *sc_tower_spike_ene_thresh15;

    TH3D *sc_strip_prob_et_pu_thresh15;
    TH3D *sc_strip_prob_ene_pu_thresh15;
    TH3D *sc_tower_spike_et_pu_thresh15;
    TH3D *sc_tower_spike_ene_pu_thresh15;


    TH2D *sc_strip_prob_et_thresh20;
    TH2D *sc_strip_prob_ene_thresh20;
    TH2D *sc_tower_spike_et_thresh20;
    TH2D *sc_tower_spike_ene_thresh20;

    TH3D *sc_strip_prob_et_pu_thresh20;
    TH3D *sc_strip_prob_ene_pu_thresh20;
    TH3D *sc_tower_spike_et_pu_thresh20;
    TH3D *sc_tower_spike_ene_pu_thresh20;





    TH2D *sc_strip_prob_et_10;
    TH2D *sc_strip_prob_ene_10;
    TH2D *sc_tower_spike_et_10;
    TH2D *sc_tower_spike_ene_10;

    TH2D *sc_strip_prob_et_pu0;
    TH2D *sc_strip_prob_ene_pu0;
    TH2D *sc_tower_spike_et_pu0;
    TH2D *sc_tower_spike_ene_pu0;

    TH2D *sc_strip_prob_et_pu10;
    TH2D *sc_strip_prob_ene_pu10;
    TH2D *sc_tower_spike_et_pu10;
    TH2D *sc_tower_spike_ene_pu10;

    TH2D *sc_strip_prob_et_pu20;
    TH2D *sc_strip_prob_ene_pu20;
    TH2D *sc_tower_spike_et_pu20;
    TH2D *sc_tower_spike_ene_pu20;


    TH2D *sc_strip_prob_et_mod1;
    TH2D *sc_strip_prob_ene_mod1;
    TH2D *sc_tower_spike_et_mod1;
    TH2D *sc_tower_spike_ene_mod1;

    TH2D *sc_strip_prob_et_mod2;
    TH2D *sc_strip_prob_ene_mod2;
    TH2D *sc_tower_spike_et_mod2;
    TH2D *sc_tower_spike_ene_mod2;

    TH2D *sc_strip_prob_et_mod3;
    TH2D *sc_strip_prob_ene_mod3;
    TH2D *sc_tower_spike_et_mod3;
    TH2D *sc_tower_spike_ene_mod3;

    TH2D *sc_strip_prob_et_mod4;
    TH2D *sc_strip_prob_ene_mod4;
    TH2D *sc_tower_spike_et_mod4;
    TH2D *sc_tower_spike_ene_mod4;



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




    TH2F *ebocc; 
    TH2F *eboccgt1;
    TH2F *eboccgt3;
    TH2F *eboccgt5;
    TH2F *eboccgt1et;
    TH2F *eboccet;
    TH2F *eboccetgt1et;
    TH2F *eboccen;
    TH2F *eboccengt1;

    TH2F *eeocc;
    TH2F *eeoccgt1;
    TH2F *eeoccgt1et;
    TH2F *eeoccet;
    TH2F *eeoccetgt1et;
    TH2F *eeoccen;
    TH2F *eeoccengt1;


    TH1F *rechiteta_all;
    TH1F *rechiteta_gt1et;
    TH1F *rechiteta_etweight;
    TH1F *rechiteta_etweight_gt1et;

    TH1F *rechiteta_gt1et_pu10;
    TH1F *rechiteta_gt1et_pu20;
    TH1F *rechiteta_gt1et_pu30;



    TH1F *spikeadc;
    TH1F *spiketime50;



    TH1F *calotowereta_all;
    TH1F *calotowereta_gt1et;
    TH1F *calotowereta_etweight;
    TH1F *calotowereta_etweight_gt1et;

    TH1F *calotowereta_gt1et_pu10;
    TH1F *calotowereta_gt1et_pu20;
    TH1F *calotowereta_gt1et_pu30;



    TH1F *sceta_all_pueq01;
    TH1F *sceta_severity0_pueq01;

    TH1F *sceta_all_pueq02;
    TH1F *sceta_severity0_pueq02;

    TH1F *sceta_all_pueq03;
    TH1F *sceta_severity0_pueq03;

    TH1F *sceta_all_pueq04;
    TH1F *sceta_severity0_pueq04;

    TH1F *sceta_all_pueq05;
    TH1F *sceta_severity0_pueq05;

    TH1F *sceta_all_pueq06;
    TH1F *sceta_severity0_pueq06;

    TH1F *sceta_all_pueq07;
    TH1F *sceta_severity0_pueq07;

    TH1F *sceta_all_pueq08;
    TH1F *sceta_severity0_pueq08;

    TH1F *sceta_all_pueq09;
    TH1F *sceta_severity0_pueq09;




    TH1F *sceta_all;
    TH1F *sceta_severity0;
    TH1F *sceta_koot0;


    TH1F *sceta_all_gt2;
    TH1F *sceta_severity0_gt2;
    TH1F *sceta_koot0_gt2;


    TH1F *sceta_all_gt5;
    TH1F *sceta_severity0_gt5;
    TH1F *sceta_koot0_gt5;


    TH1F *sceta_all_gt10;
    TH1F *sceta_severity0_gt10;
    TH1F *sceta_koot0_gt10;


    TH1F *scet_eb_all;
    TH1F *scet_eb_severity0;
    TH1F *scet_eb_koot0;

    TH1F *scet_ee_all;
    TH1F *scet_ee_severity0;
    TH1F *scet_ee_koot0;

    TH1F *scsumet_eb_all;
    TH1F *scsumet_eb_severity0;
    TH1F *scsumet_eb_koot0;

    TH1F *scsumet_ee_all;
    TH1F *scsumet_ee_severity0;
    TH1F *scsumet_ee_koot0;


    TH1F *scsumet_eb_all_gt2;
    TH1F *scsumet_eb_severity0_gt2;
    TH1F *scsumet_eb_koot0_gt2;

    TH1F *scsumet_ee_all_gt2;
    TH1F *scsumet_ee_severity0_gt2;
    TH1F *scsumet_ee_koot0_gt2;

    TH1F *scsumet_eb_all_gt5;
    TH1F *scsumet_eb_severity0_gt5;
    TH1F *scsumet_eb_koot0_gt5;

    TH1F *scsumet_ee_all_gt5;
    TH1F *scsumet_ee_severity0_gt5;
    TH1F *scsumet_ee_koot0_gt5;

    TH1F *scsumet_eb_all_gt10;
    TH1F *scsumet_eb_severity0_gt10;
    TH1F *scsumet_eb_koot0_gt10;

    TH1F *scsumet_ee_all_gt10;
    TH1F *scsumet_ee_severity0_gt10;
    TH1F *scsumet_ee_koot0_gt10;


    TH2F *scocc_eb_gt50;
    TH2F *scocc_ee_gt50;


    TH1F *scet_eb_all_eta15;
    TH1F *scet_eb_all_eta20;
    TH1F *scet_eb_all_eta25;
 
    TH1F *scet_ee_all_eta15;
    TH1F *scet_ee_all_eta20;
    TH1F *scet_ee_all_eta25;



    TH1F *scet_eb_all_eta15_pu10;
    TH1F *scet_eb_all_eta20_pu10;
    TH1F *scet_eb_all_eta25_pu10;
 
    TH1F *scet_ee_all_eta15_pu10;
    TH1F *scet_ee_all_eta20_pu10;
    TH1F *scet_ee_all_eta25_pu10;

    TH1F *scet_eb_all_eta15_pu20;
    TH1F *scet_eb_all_eta20_pu20;
    TH1F *scet_eb_all_eta25_pu20;
 
    TH1F *scet_ee_all_eta15_pu20;
    TH1F *scet_ee_all_eta20_pu20;
    TH1F *scet_ee_all_eta25_pu20;

    TH1F *scet_eb_all_eta15_pu30;
    TH1F *scet_eb_all_eta20_pu30;
    TH1F *scet_eb_all_eta25_pu30;
 
    TH1F *scet_ee_all_eta15_pu30;
    TH1F *scet_ee_all_eta20_pu30;
    TH1F *scet_ee_all_eta25_pu30;


    TH1F *scet_eb_all_eta15_pueq10;
    TH1F *scet_eb_all_eta20_pueq10;
    TH1F *scet_eb_all_eta25_pueq10;
 
    TH1F *scet_ee_all_eta15_pueq10;
    TH1F *scet_ee_all_eta20_pueq10;
    TH1F *scet_ee_all_eta25_pueq10;

    TH1F *scet_eb_all_eta15_pueq20;
    TH1F *scet_eb_all_eta20_pueq20;
    TH1F *scet_eb_all_eta25_pueq20;
 
    TH1F *scet_ee_all_eta15_pueq20;
    TH1F *scet_ee_all_eta20_pueq20;
    TH1F *scet_ee_all_eta25_pueq20;
 


    TH1F *scet_eb_all_eta15_pueq01;
    TH1F *scet_eb_all_eta20_pueq01;
    TH1F *scet_eb_all_eta25_pueq01;
 
    TH1F *scet_ee_all_eta15_pueq01;
    TH1F *scet_ee_all_eta20_pueq01;
    TH1F *scet_ee_all_eta25_pueq01;


    TH1F *scsumet_eb_all_eta15_pueq01;
    TH1F *scsumet_eb_all_eta20_pueq01;
    TH1F *scsumet_eb_all_eta25_pueq01;
 
    TH1F *scsumet_ee_all_eta15_pueq01;
    TH1F *scsumet_ee_all_eta20_pueq01;
    TH1F *scsumet_ee_all_eta25_pueq01;



    TH1F *scet_eb_all_eta15_pueq02;
    TH1F *scet_eb_all_eta20_pueq02;
    TH1F *scet_eb_all_eta25_pueq02;
 
    TH1F *scet_ee_all_eta15_pueq02;
    TH1F *scet_ee_all_eta20_pueq02;
    TH1F *scet_ee_all_eta25_pueq02;


    TH1F *scsumet_eb_all_eta15_pueq02;
    TH1F *scsumet_eb_all_eta20_pueq02;
    TH1F *scsumet_eb_all_eta25_pueq02;
 
    TH1F *scsumet_ee_all_eta15_pueq02;
    TH1F *scsumet_ee_all_eta20_pueq02;
    TH1F *scsumet_ee_all_eta25_pueq02;



    TH1F *scet_eb_all_eta15_pueq03;
    TH1F *scet_eb_all_eta20_pueq03;
    TH1F *scet_eb_all_eta25_pueq03;
 
    TH1F *scet_ee_all_eta15_pueq03;
    TH1F *scet_ee_all_eta20_pueq03;
    TH1F *scet_ee_all_eta25_pueq03;


    TH1F *scsumet_eb_all_eta15_pueq03;
    TH1F *scsumet_eb_all_eta20_pueq03;
    TH1F *scsumet_eb_all_eta25_pueq03;
 
    TH1F *scsumet_ee_all_eta15_pueq03;
    TH1F *scsumet_ee_all_eta20_pueq03;
    TH1F *scsumet_ee_all_eta25_pueq03;



    TH1F *scet_eb_all_eta15_pueq04;
    TH1F *scet_eb_all_eta20_pueq04;
    TH1F *scet_eb_all_eta25_pueq04;
 
    TH1F *scet_ee_all_eta15_pueq04;
    TH1F *scet_ee_all_eta20_pueq04;
    TH1F *scet_ee_all_eta25_pueq04;


    TH1F *scsumet_eb_all_eta15_pueq04;
    TH1F *scsumet_eb_all_eta20_pueq04;
    TH1F *scsumet_eb_all_eta25_pueq04;
 
    TH1F *scsumet_ee_all_eta15_pueq04;
    TH1F *scsumet_ee_all_eta20_pueq04;
    TH1F *scsumet_ee_all_eta25_pueq04;



    TH1F *scet_eb_all_eta15_pueq05;
    TH1F *scet_eb_all_eta20_pueq05;
    TH1F *scet_eb_all_eta25_pueq05;
 
    TH1F *scet_ee_all_eta15_pueq05;
    TH1F *scet_ee_all_eta20_pueq05;
    TH1F *scet_ee_all_eta25_pueq05;


    TH1F *scsumet_eb_all_eta15_pueq05;
    TH1F *scsumet_eb_all_eta20_pueq05;
    TH1F *scsumet_eb_all_eta25_pueq05;
 
    TH1F *scsumet_ee_all_eta15_pueq05;
    TH1F *scsumet_ee_all_eta20_pueq05;
    TH1F *scsumet_ee_all_eta25_pueq05;



    TH1F *scet_eb_all_eta15_pueq06;
    TH1F *scet_eb_all_eta20_pueq06;
    TH1F *scet_eb_all_eta25_pueq06;
 
    TH1F *scet_ee_all_eta15_pueq06;
    TH1F *scet_ee_all_eta20_pueq06;
    TH1F *scet_ee_all_eta25_pueq06;


    TH1F *scsumet_eb_all_eta15_pueq06;
    TH1F *scsumet_eb_all_eta20_pueq06;
    TH1F *scsumet_eb_all_eta25_pueq06;
 
    TH1F *scsumet_ee_all_eta15_pueq06;
    TH1F *scsumet_ee_all_eta20_pueq06;
    TH1F *scsumet_ee_all_eta25_pueq06;



    TH1F *scet_eb_all_eta15_pueq07;
    TH1F *scet_eb_all_eta20_pueq07;
    TH1F *scet_eb_all_eta25_pueq07;
 
    TH1F *scet_ee_all_eta15_pueq07;
    TH1F *scet_ee_all_eta20_pueq07;
    TH1F *scet_ee_all_eta25_pueq07;


    TH1F *scsumet_eb_all_eta15_pueq07;
    TH1F *scsumet_eb_all_eta20_pueq07;
    TH1F *scsumet_eb_all_eta25_pueq07;
 
    TH1F *scsumet_ee_all_eta15_pueq07;
    TH1F *scsumet_ee_all_eta20_pueq07;
    TH1F *scsumet_ee_all_eta25_pueq07;



    TH1F *scet_eb_all_eta15_pueq08;
    TH1F *scet_eb_all_eta20_pueq08;
    TH1F *scet_eb_all_eta25_pueq08;
 
    TH1F *scet_ee_all_eta15_pueq08;
    TH1F *scet_ee_all_eta20_pueq08;
    TH1F *scet_ee_all_eta25_pueq08;


    TH1F *scsumet_eb_all_eta15_pueq08;
    TH1F *scsumet_eb_all_eta20_pueq08;
    TH1F *scsumet_eb_all_eta25_pueq08;
 
    TH1F *scsumet_ee_all_eta15_pueq08;
    TH1F *scsumet_ee_all_eta20_pueq08;
    TH1F *scsumet_ee_all_eta25_pueq08;



    TH1F *scet_eb_all_eta15_pueq09;
    TH1F *scet_eb_all_eta20_pueq09;
    TH1F *scet_eb_all_eta25_pueq09;
 
    TH1F *scet_ee_all_eta15_pueq09;
    TH1F *scet_ee_all_eta20_pueq09;
    TH1F *scet_ee_all_eta25_pueq09;


    TH1F *scsumet_eb_all_eta15_pueq09;
    TH1F *scsumet_eb_all_eta20_pueq09;
    TH1F *scsumet_eb_all_eta25_pueq09;
 
    TH1F *scsumet_ee_all_eta15_pueq09;
    TH1F *scsumet_ee_all_eta20_pueq09;
    TH1F *scsumet_ee_all_eta25_pueq09;







    TH1F *scsumet_eb_all_eta15;
    TH1F *scsumet_eb_all_eta20;
    TH1F *scsumet_eb_all_eta25;
 
    TH1F *scsumet_ee_all_eta15;
    TH1F *scsumet_ee_all_eta20;
    TH1F *scsumet_ee_all_eta25;



    TH1F *scsumet_eb_all_eta15_pu10;
    TH1F *scsumet_eb_all_eta20_pu10;
    TH1F *scsumet_eb_all_eta25_pu10;
 
    TH1F *scsumet_ee_all_eta15_pu10;
    TH1F *scsumet_ee_all_eta20_pu10;
    TH1F *scsumet_ee_all_eta25_pu10;

    TH1F *scsumet_eb_all_eta15_pu20;
    TH1F *scsumet_eb_all_eta20_pu20;
    TH1F *scsumet_eb_all_eta25_pu20;
 
    TH1F *scsumet_ee_all_eta15_pu20;
    TH1F *scsumet_ee_all_eta20_pu20;
    TH1F *scsumet_ee_all_eta25_pu20;

    TH1F *scsumet_eb_all_eta15_pu30;
    TH1F *scsumet_eb_all_eta20_pu30;
    TH1F *scsumet_eb_all_eta25_pu30;
 
    TH1F *scsumet_ee_all_eta15_pu30;
    TH1F *scsumet_ee_all_eta20_pu30;
    TH1F *scsumet_ee_all_eta25_pu30;


    TH1F *scsumet_eb_all_eta15_pueq10;
    TH1F *scsumet_eb_all_eta20_pueq10;
    TH1F *scsumet_eb_all_eta25_pueq10;
 
    TH1F *scsumet_ee_all_eta15_pueq10;
    TH1F *scsumet_ee_all_eta20_pueq10;
    TH1F *scsumet_ee_all_eta25_pueq10;

    TH1F *scsumet_eb_all_eta15_pueq20;
    TH1F *scsumet_eb_all_eta20_pueq20;
    TH1F *scsumet_eb_all_eta25_pueq20;
 
    TH1F *scsumet_ee_all_eta15_pueq20;
    TH1F *scsumet_ee_all_eta20_pueq20;
    TH1F *scsumet_ee_all_eta25_pueq20;
 






    TH1D *eb_timing_0;
    TH1D *eb_timing_200;
    TH1D *eb_timing_400;
    TH1D *eb_timing_600;
    TH1D *eb_timing_800;
    TH1D *eb_timing_1000;
    TH1D *eb_timing_2000;
    TH1D *eb_timing_3000;
    TH1D *eb_timing_4000;
    TH1D *eb_timing_5000;
    TH1D *eb_timing_10000;


    TH1D *eb_timing_3000_spiketag;

    TH1D *eb_timing_4000_spiketag;
    TH1D *eb_timing_4000_kweird;
    TH1D *eb_timing_4000_swisscross;
    TH1D *eb_timing_4000_spiketag_kweird;
    TH1D *eb_timing_4000_spiketag_swisscross;
    TH1D *eb_timing_4000_spiketag_noisedep;
    TH1D *eb_timing_4000_sctag;
    TH1D *eb_timing_4000_sctag_kweird;
    TH1D *eb_timing_4000_sctag_swisscross;
    TH1D *eb_timing_4000_sctag_noisedep;



    TH1D *eb_timing_10000_spiketag;
    TH1D *eb_timing_10000_kweird;
    TH1D *eb_timing_10000_swisscross;
    TH1D *eb_timing_10000_spiketag_kweird;
    TH1D *eb_timing_10000_spiketag_swisscross;
    TH1D *eb_timing_10000_spiketag_noisedep;
    TH1D *eb_timing_10000_sctag;
    TH1D *eb_timing_10000_sctag_kweird;
    TH1D *eb_timing_10000_sctag_swisscross;
    TH1D *eb_timing_10000_sctag_noisedep;


    TH1D *spikes_vs_ieta_spiketag_4000;
    TH1D *spikes_vs_ieta_spiketag_4000_kweird;
    TH1D *spikes_vs_ieta_spiketag_4000_swisscross;
    TH1D *spikes_vs_ieta_spiketag_4000_noisedep;

    TH1D *spikes_vs_ieta_spiketag_10000;
    TH1D *spikes_vs_ieta_spiketag_10000_kweird;
    TH1D *spikes_vs_ieta_spiketag_10000_swisscross;
    TH1D *spikes_vs_ieta_spiketag_10000_noisedep;


    TH1D *sc_vs_ieta_sctag_4000;
    TH1D *sc_vs_ieta_sctag_4000_kweird;
    TH1D *sc_vs_ieta_sctag_4000_swisscross;
    TH1D *sc_vs_ieta_sctag_4000_noisedep;

    TH1D *sc_vs_ieta_sctag_10000;
    TH1D *sc_vs_ieta_sctag_10000_kweird;
    TH1D *sc_vs_ieta_sctag_10000_swisscross;
    TH1D *sc_vs_ieta_sctag_10000_noisedep;

    TH2D *swisscross_vs_ieta_spiketag_4000;
    TH2D *swisscross_vs_ieta_spiketag_4000_kweird;
    TH2D *swisscross_vs_ieta_sctag_4000;
    TH2D *swisscross_vs_ieta_sctag_4000_kweird;


    TH2D *swisscross_vs_ieta_spiketag_10000;
    TH2D *swisscross_vs_ieta_spiketag_10000_kweird;
    TH2D *swisscross_vs_ieta_sctag_10000;
    TH2D *swisscross_vs_ieta_sctag_10000_kweird;


    TH1D *eb_r4_0;
    TH1D *eb_r4_200;
    TH1D *eb_r4_400;
    TH1D *eb_r4_600;
    TH1D *eb_r4_800;
    TH1D *eb_r4_1000;
    TH1D *eb_r4_2000;
    TH1D *eb_r4_3000;
    TH1D *eb_r4_4000;
    TH1D *eb_r4_5000;
    TH1D *eb_r4_10000;
 



 
    TH1D *eb_r4_3000_spiketag;

    TH1D *eb_r4_4000_spiketag;
    TH1D *eb_r4_4000_kweird;
    TH1D *eb_r4_4000_swisscross;
    TH1D *eb_r4_4000_sctag;

    TH1D *eb_r4_10000_spiketag;
    TH1D *eb_r4_10000_sctag;
    TH1D *eb_r4_10000_kweird;
    TH1D *eb_r4_10000_swisscross;



    TH1D *eb_r6_1000;
    TH1D *eb_r6_2000;
    TH1D *eb_r6_3000;
    TH1D *eb_r6_4000;
    TH1D *eb_r6_5000;
    TH1D *eb_r6_10000;
 
    TH1D *eb_r6_3000_spiketag;

    TH1D *eb_r6_4000_spiketag;
    TH1D *eb_r6_4000_sctag;
    TH1D *eb_r6_4000_kweird;
    TH1D *eb_r6_4000_swisscross;
    TH1D *eb_r6_10000_spiketag;
    TH1D *eb_r6_10000_sctag;
    TH1D *eb_r6_10000_kweird;
    TH1D *eb_r6_10000_swisscross;



    TH2D *eb_r4vsr6_3000;
    TH2D *eb_r4vsr6_4000;
    TH2D *eb_r4vsr6_5000;
    TH2D *eb_r4vsr6_10000;
 

    
    TH1D *eb_timing_r4_0;
    TH1D *eb_timing_r4_200;
    TH1D *eb_timing_r4_400;
    TH1D *eb_timing_r4_600;
    TH1D *eb_timing_r4_800;
    TH1D *eb_timing_r4_1000;
    TH1D *eb_timing_r4_2000;
    TH1D *eb_timing_r4_3000;
    TH1D *eb_timing_r4_5000;

    TH2D *eb_timing_vs_r4_0;
    TH2D *eb_timing_vs_r4_200;
    TH2D *eb_timing_vs_r4_400;
    TH2D *eb_timing_vs_r4_600;
    TH2D *eb_timing_vs_r4_800;
    TH2D *eb_timing_vs_r4_1000;
    TH2D *eb_timing_vs_r4_2000;
    TH2D *eb_timing_vs_r4_3000;
    TH2D *eb_timing_vs_r4_5000;
    TH2D *eb_timing_vs_r4_10000;

 


    TH2D *eb_timing_vs_r4_sctag_3000;
    TH2D *eb_timing_vs_r4_sctag_5000;
    TH2D *eb_timing_vs_r4_sctag_10000;

 
    TH2D *ebtime_vs_bxtrain_01;
    TH2D *ebtime_vs_bxtrain_05;
    TH2D *eetime_vs_bxtrain_01;
    TH2D *eetime_vs_bxtrain_05;


    TH2D *sceta_vs_bxtrain;
    TH2D *rechiteta_vs_bxtrain_01;
    TH2D *rechiteta_vs_bxtrain_05;

    TH2D *eb_digi_01;
    TH2D *ee_digi_01;

    TH2D *eb_digi_05;
    TH2D *ee_digi_05;

    TH2D *eb_digi_30;
    TH2D *ee_digi_30;

    TH2D *eb_digi_0105;
    TH2D *ee_digi_0105;
    
    TH2D *eb_digi_0530;
    TH2D *ee_digi_0530;


    TH2D *eb_digi_0105_vs_time;
    TH2D *ee_digi_0105_vs_time;
    
    TH2D *eb_digi_0530_vs_time;
    TH2D *ee_digi_0530_vs_time;


    TH2D *eb_digi_0105_vs_bxtrain;
    TH2D *ee_digi_0105_vs_bxtrain;
    
    TH2D *eb_digi_0530_vs_bxtrain;
    TH2D *ee_digi_0530_vs_bxtrain;


    TH2D *eb_digi_0105_vs_time_norm;
    TH2D *ee_digi_0105_vs_time_norm;
    
    TH2D *eb_digi_0530_vs_time_norm;
    TH2D *ee_digi_0530_vs_time_norm;


    TH2D *eb_digi_0105_vs_bxtrain_norm;
    TH2D *ee_digi_0105_vs_bxtrain_norm;
    
    TH2D *eb_digi_0530_vs_bxtrain_norm;
    TH2D *ee_digi_0530_vs_bxtrain_norm;






    TH2D *eb_digi_0105_vs_time_eta15;
    TH2D *ee_digi_0105_vs_time_eta15;
    
    TH2D *eb_digi_0530_vs_time_eta15;
    TH2D *ee_digi_0530_vs_time_eta15;


    TH2D *eb_digi_0105_vs_bxtrain_eta15;
    TH2D *ee_digi_0105_vs_bxtrain_eta15;
    
    TH2D *eb_digi_0530_vs_bxtrain_eta15;
    TH2D *ee_digi_0530_vs_bxtrain_eta15;


    TH2D *eb_digi_0105_vs_time_norm_eta15;
    TH2D *ee_digi_0105_vs_time_norm_eta15;
    
    TH2D *eb_digi_0530_vs_time_norm_eta15;
    TH2D *ee_digi_0530_vs_time_norm_eta15;


    TH2D *eb_digi_0105_vs_bxtrain_norm_eta15;
    TH2D *ee_digi_0105_vs_bxtrain_norm_eta15;
    
    TH2D *eb_digi_0530_vs_bxtrain_norm_eta15;
    TH2D *ee_digi_0530_vs_bxtrain_norm_eta15;




    TH2D *eb_digi_0105_vs_time_eta20;
    TH2D *ee_digi_0105_vs_time_eta20;
    
    TH2D *eb_digi_0530_vs_time_eta20;
    TH2D *ee_digi_0530_vs_time_eta20;


    TH2D *eb_digi_0105_vs_bxtrain_eta20;
    TH2D *ee_digi_0105_vs_bxtrain_eta20;
    
    TH2D *eb_digi_0530_vs_bxtrain_eta20;
    TH2D *ee_digi_0530_vs_bxtrain_eta20;


    TH2D *eb_digi_0105_vs_time_norm_eta20;
    TH2D *ee_digi_0105_vs_time_norm_eta20;
    
    TH2D *eb_digi_0530_vs_time_norm_eta20;
    TH2D *ee_digi_0530_vs_time_norm_eta20;


    TH2D *eb_digi_0105_vs_bxtrain_norm_eta20;
    TH2D *ee_digi_0105_vs_bxtrain_norm_eta20;
    
    TH2D *eb_digi_0530_vs_bxtrain_norm_eta20;
    TH2D *ee_digi_0530_vs_bxtrain_norm_eta20;



    TH2D *eb_digi_0105_vs_time_eta25;
    TH2D *ee_digi_0105_vs_time_eta25;
    
    TH2D *eb_digi_0530_vs_time_eta25;
    TH2D *ee_digi_0530_vs_time_eta25;


    TH2D *eb_digi_0105_vs_bxtrain_eta25;
    TH2D *ee_digi_0105_vs_bxtrain_eta25;
    
    TH2D *eb_digi_0530_vs_bxtrain_eta25;
    TH2D *ee_digi_0530_vs_bxtrain_eta25;


    TH2D *eb_digi_0105_vs_time_norm_eta25;
    TH2D *ee_digi_0105_vs_time_norm_eta25;
    
    TH2D *eb_digi_0530_vs_time_norm_eta25;
    TH2D *ee_digi_0530_vs_time_norm_eta25;


    TH2D *eb_digi_0105_vs_bxtrain_norm_eta25;
    TH2D *ee_digi_0105_vs_bxtrain_norm_eta25;
    
    TH2D *eb_digi_0530_vs_bxtrain_norm_eta25;
    TH2D *ee_digi_0530_vs_bxtrain_norm_eta25;







    int   rank_;
    int   ntrk;
    int   goodtrk;

    float vtx_x,vtx_y,vtx_z,vtx_x_err,vtx_y_err,vtx_z_err;
    float vtx_chi2,vtx_ndof;
    int   numvtx,vtx_ntracks,vtx_isfake,numgoodvtx;
    int   vtx_good;
    
    float scale;

    int   bit3,bit4,bit9,bit40,bit41,bit36,bit37,bit38,bit39,run,even,lumi,bx,orbit,bit0;
    int   eg1,eg2,eg5, algo124;


    int algo54;
    int algo55;
    int algo56;
    int algo57;
    int algo58;
    int algo59;
    int algo60;
    int algo61;
    int algo62;

    int algo106;
    int algo107;



    int   physdeclared;
    float time;
    float ptJet_,phiJet_,etaJet_,chfJet_,nhfJet_,cemfJet_,nemfJet_; 
    float energy_pf_,energyc_pf_,energyn_pf_,energyg_pf_;
    int   cmultiJet_,nmultiJet_,nrjets_,ncr_;   
    float energy_ecal, energy_hcal; 

    float ebmax, eemax, ebtime, eetime;
    int   eb_ieta,eb_iphi,ebhits,ebhits1GeV, ebflags, recoflag, sevlev;
    int   eeix,eeiy,eeiz,eehits,eehits1GeV, eeflags;
    float eb_eta,eb_phi,ebmaxet, eb_r9, eb_r4;
    float ee_eta,ee_phi,eemaxet, ee_r9;
    float ebchi2, ebchi2oot;

    float eb2chi2, eb2chi2oot;


      Int_t ee_kGood;
      Int_t ee_kPoorReco;
      Int_t ee_kOutOfTime;
      Int_t ee_kFaultyHardware;
      Int_t ee_kNoisy;
      Int_t ee_kPoorCalib;
      Int_t ee_kSaturated;
      Int_t ee_kLeadingEdgeRecovered;
      Int_t ee_kNeighboursRecovered;
      Int_t ee_kTowerRecovered;
      Int_t ee_kDead;
      Int_t ee_kKilled;
      Int_t ee_kTPSaturated;
      Int_t ee_kL1SpikeFlag;
      Int_t ee_kWeird;
      Int_t ee_kDiWeird;
      Int_t ee_kUnknown;


      int numspikes, numspikes50, numspikes100;

      
      int ebhits1GeVet, eehits1GeVet;
      int ebhits2GeV, ebhits4GeV, eehits2GeV, eehits4GeV;
      int ebhits2GeVet, ebhits4GeVet, eehits2GeVet, eehits4GeVet;


    float ebmax2, eemax2, ebtime2, eetime2;
    int   eb_ieta2,eb_iphi2,ebhits1GeV2, ebflags2;
    int   eeix2,eeiy2,eeiz2,eehits1GeV2, eeflags2;
    float eb_eta2,eb_phi2,ebmaxet2, eb_r92, eb_r42;
    float ee_eta2,ee_phi2,eemaxet2, ee_r92;

    int adc01,adc02,adc03,adc04,adc05,adc06,adc07,adc08,adc09,adc10;
    int gain01,gain02,gain03,gain04,gain05,gain06,gain07,gain08,gain09,gain10;

    float eb_e9, eb_e25;

    int eephits, eemhits;

    int ebflag_kgood, ebflag_kpoorreco, ebflag_koutoftime, ebflag_kfake;


    float tmean_en,  terr_en;
    float tmean_sig, terr_sig;


    int r4count;

    int e2e9count_thresh0, e2e25count_thresh0;
    int e2e9count_thresh1, e2e25count_thresh1;

    int e2e9count_thresh0_nor4, e2e25count_thresh0_nor4;
    int e2e9count_thresh1_nor4, e2e25count_thresh1_nor4;


    int r4_algo_count;
    int e2e9_algo_count;
    int e2e9_algo_count_5_1;
    int e2e9_algo_count_5_0;

    float swisscross_algo, e2e9_algo;
    EcalCleaningAlgo * cleaningAlgo_;  

    float Emin_;
    int side_;

    const std::vector<int> badsc_;


    const std::vector<int> bunchstartbx_;


   // photon variables

    float phoEta, phoPhi, phoEt;
    float pho_maxen_xtal, pho_e3x3, pho_e5x5, pho_r9;
    float etaPhoSc;

    float pho_ntrksolidconedr4;
    float pho_ntrksumptsolidconedr4;
    float pho_ntrkhollowconedr4;
    float pho_ntrksumpthollowconedr4;

    float pho_ecaletconedr4;
    float pho_hcaletconedr4;
    float pho_sigmaietaieta;

    float pho_hovere;
    int phoBarrel;



    float eesum_gt1, eesum_gt2, eesum_gt4;
    float ebsum_gt1, ebsum_gt2, ebsum_gt4;



    float eesum_gt1et, eesum_gt2et, eesum_gt4et;
    float ebsum_gt1et, ebsum_gt2et, ebsum_gt4et;


    int ncalotower, ncalotowereb, ncalotoweree, ncalotowerhf;

    int ncalotowerebgt1, ncalotowerebgt2, ncalotowerebgt5, ncalotowerebgt10;

    int ncalotowereegt1,  ncalotowereegt2,    ncalotowereegt5,    ncalotowereegt10;

    int ncalotowerhfgt1,    ncalotowerhfgt2,    ncalotowerhfgt5,    ncalotowerhfgt10;

    float ctsumebgt1,    ctsumebgt2,    ctsumebgt5,    ctsumebgt10;

    float ctsumeegt1,    ctsumeegt2,    ctsumeegt5,    ctsumeegt10;

    float ctsumhfgt1,    ctsumhfgt2,    ctsumhfgt5,    ctsumhfgt10;




    float rechitsumet_eb_all, rechitsumet_eb_01, rechitsumet_eb_05;
    float rechitsumet_ee_all, rechitsumet_ee_01, rechitsumet_ee_05;

    float rechitsumet_eb_0105, rechitsumet_eb_0530;
    float rechitsumet_ee_0105, rechitsumet_ee_0530;


    int bunchintrain;


    int numspikes_kweird;
    int numspikes_swisscross;

    float ebscsumet_all, eescsumet_all;
    float ebscsumet_all_eta15, ebscsumet_all_eta20, ebscsumet_all_eta25;
    float eescsumet_all_eta15, eescsumet_all_eta20, eescsumet_all_eta25;

    int ebnumsc_all, eenumsc_all;

    int ebnumrechits_01, ebnumrechits_0105, ebnumrechits_05, ebnumrechits_0530;
    int eenumrechits_01, eenumrechits_0105, eenumrechits_05, eenumrechits_0530;



};
#endif

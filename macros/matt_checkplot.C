{


  TFile f1("exomgg_0t_runC.root");

  //Specify the number of events to plot
  int N = 2;

  gStyle->SetOptStat(0);
  //Setting coordinates for the cluster seeds
  int etamax[4] = {-43, 80, 48, 12};
  int phimax[4] = {50, 273, 340, 64};


  // read histograms from file

  // transverse energy maps
  
  TH2F *eboccet=(TH2F*)f1.Get("eboccet");
  TH2F *eeoccet=(TH2F*)f1.Get("eeoccet");

  // rechit time maps
  
  TH2F *ebtime=(TH2F*)f1.Get("ebtime");
  TH2F *eetime=(TH2F*)f1.Get("eetime");

  // eb laser correction and intercalibration constants
  
  TH2F *eblascorr=(TH2F*)f1.Get("eblascorr");
  TH2F *ebicval=(TH2F*)f1.Get("ebicval");

  // dead/problematic channel maps
  
  TH2F *eb=(TH2F*)f1.Get("ebchstatus");
  TH2F *ee=(TH2F*)f1.Get("eechstatus");



  TH2F *ebmask=new TH2F("ebmask","",360,0,360,170,-85,85);
  TH2F *ebmask2=new TH2F("ebmask2","",360,0,360,170,-85,85);
  TH2F *eemask=new TH2F("eemask","",200,0,200,100,0,100);
  TH2F *eemask2=new TH2F("eemask2","",200,0,200,100,0,100);


  TH2F *eblascorrall=new TH2F("eblascorrall","",360,0,360,170,-85,85);
  TH2F *eblascorrgood=new TH2F("eblascorrgood","",360,0,360,170,-85,85);
  TH1F *eblascorrall_1d=new TH1F("eblascorrall_1d","",300,0,6);
  TH1F *eblascorrgood_1d=new TH1F("eblascorrgood_1d","",300,0,6);
  
  
  ebmask->SetLineColor(921);
  ebmask->SetFillStyle(0);
  ebmask->SetLineWidth(2);
  eemask->SetLineColor(921);
  eemask->SetFillStyle(0);
  eemask->SetLineWidth(4);


  ebmask2->SetLineColor(1);
  ebmask2->SetFillStyle(0);
  ebmask2->SetLineWidth(2);
  eemask2->SetLineColor(1);
  eemask2->SetFillStyle(0);
  eemask2->SetLineWidth(4);


  // modify maps to plot laser correction, intercalib and timing only for hits with Et>1.0 GeV
  
  for (Int_t j=0;j<170;j++) {

    for (Int_t i=0;i<180;i++) {  //changed 360 to 180

      Float_t tmp=eb->GetBinContent(i+1,j+1);
      Float_t tmp2=eboccet->GetBinContent(i+1,j+1);
      Float_t tmp3=eblascorr->GetBinContent(i+1,j+1);


      if (tmp2<1.0) {
	eblascorr->SetBinContent(i+1,j+1,-999);
	ebicval->SetBinContent(i+1,j+1,-999);
	ebtime->SetBinContent(i+1,j+1,-999);
      }

      
      eblascorrall->SetBinContent(i+1,j+1,tmp3);
      eblascorrall_1d->Fill(tmp3);

      // make a map of laser correction values for good channels only (channel status < 3)
      
      if (tmp<3) {
	eblascorrgood->SetBinContent(i+1,j+1,tmp3);
	eblascorrgood_1d->Fill(tmp3);
      }
      

      // make a map of dead and problematic towers

      // problematic (channel status=13)
      
      if (tmp==13) {
	
	ebmask->SetBinContent(i+1,j+1,0.1);

      }

      // dead (channel status > 13)
      
      if (tmp>13) {
	
	ebmask2->SetBinContent(i+1,j+1,0.1);

      }

    }

  }

  // similar exercise for EE
  
  for (Int_t j=0;j<100;j++) {

    for (Int_t i=0;i<200;i++) {

      Float_t tmp1=ee->GetBinContent(i+1,j+1);
          
      if (tmp1==13) {
	
	eemask->SetBinContent(i+1,j+1,0.1);

      }
      if (tmp1>13) {
	
	eemask2->SetBinContent(i+1,j+1,0.1);

      }

    }

  }


  // DRAW PLOTS

  TCanvas c1("c1","",10,10,1200,700);
  c1.SetLogz(1);
  c1.SetGridx();
  c1.SetGridy();

  // EB transverse energy map
  
  eboccet->GetXaxis()->SetLabelSize(0.03);
  eboccet->GetYaxis()->SetLabelSize(0.03);
  eboccet->SetXTitle("i#phi");
  eboccet->SetYTitle("i#eta");
  TLatex t1;
  t1.SetNDC();

  eboccet->GetXaxis()->SetNdivisions(18);
  eboccet->GetYaxis()->SetNdivisions(2);
  eboccet->Draw("colz");
  ebmask->Draw("box,same");
  ebmask2->Draw("box,same");
  eboccet->Draw("colz,same");

  t1.SetTextSize(0.05);
  t1.DrawLatex(0.90,0.86,"E_{T} [GeV]");

  c1.SaveAs("exomgg_0t_rereco_ev3_ebet_photon2.png");


  TCanvas c2("c2","",10,10,1200,600);
  c2.cd();
  c2.SetLogz(1);
  c2.SetGridx();
  c2.SetGridy();


  
  // EE transverse energy map
  
  if (eeoccet->GetMaximum()<1) c2.SetLogz(0);
  eeoccet->GetXaxis()->SetLabelSize(0);
  eeoccet->GetYaxis()->SetLabelSize(0);
  eeoccet->SetXTitle("ix");
  eeoccet->SetYTitle("iy");
  TLine l1(100,0,100,100);
  eeoccet->GetXaxis()->SetNdivisions(40);
  eeoccet->GetYaxis()->SetNdivisions(20);
  eeoccet->Draw("colz");
  eemask->Draw("box,same");
  eemask2->Draw("box,same");
  eeoccet->Draw("colz,same");

  l1.Draw();
  t1.SetTextSize(0.05);
  t1.DrawLatex(0.90,0.86,"E_{T} [GeV]");
  t1.DrawLatex(0.17,0.8,"EE-");
  t1.DrawLatex(0.8,0.8,"EE+");

  c2.SaveAs("exomgg_0t_rereco_ev3_eeet_photon2.png");



  // ZOOM of EB transverse energy map
  

  TCanvas c3("c3","",10,10,900,700);

  c3.SetRightMargin(0.16);
  c3.cd();
  c3.SetLogz(1);
  gStyle->SetPaintTextFormat("2.1f");

//  char txt[30];
//  for(int k=0; k<4; k++)  {

 
  eboccet->SetMarkerSize(1.2);
  eboccet->GetXaxis()->SetNdivisions(11);
  eboccet->GetYaxis()->SetNdivisions(11);
 
  char txt[30];

  for (int k=0; k<N; k++) {
      eboccet->GetXaxis()->SetRangeUser(phimax[k]-8,phimax[k]+8);
      eboccet->GetYaxis()->SetRangeUser(etamax[k]-8,etamax[k]+8);
      eboccet->Draw("colz,text");
      t1.DrawLatex(0.83,0.875,"E_{T} [GeV]");
      sprintf(txt,"transverse_energy_zoom_%d.png",k+1);
      c3.SaveAs(txt);
  }



  // EB rechit time map
  
  TCanvas c4("c4","",10,10,900,700);

  c4.SetRightMargin(0.16);
  c4.cd();
  c4.SetLogz(0);
  

  for (Int_t i=0;i<360;i++) {
    for (Int_t j=0;j<170;j++) {
      if (ebtime->GetBinContent(i+1,j+1)==0) ebtime->SetBinContent(i+1,j+1,-999);
    }
  }
  
  ebtime->SetMarkerSize(1.2);
  ebtime->GetXaxis()->SetNdivisions(11);
  ebtime->GetYaxis()->SetNdivisions(11);
  ebtime->GetXaxis()->SetLabelSize(0.03);
  ebtime->GetYaxis()->SetLabelSize(0.03);
  ebtime->SetXTitle("i#phi");
  ebtime->SetYTitle("i#eta");
  ebtime->SetMinimum(-3);
  ebtime->SetMaximum(3);

  for (int k=0; k<N; k++)  {
      ebtime->GetXaxis()->SetRangeUser(phimax[k]-8,phimax[k]+8);
      ebtime->GetYaxis()->SetRangeUser(etamax[k]-8,etamax[k]+8);
      ebtime->Draw("colz,text");
      t1.DrawLatex(0.83,0.875,"Time [ns]");
      sprintf(txt,"EB_rechit_time_zoomed_%d.png",k+1);
      c4.SaveAs(txt);
  }


  // EB intercalibration map (zoomed)

  ebicval->SetMarkerSize(1.2);
  ebicval->GetXaxis()->SetNdivisions(11);
  ebicval->GetYaxis()->SetNdivisions(11);
  ebicval->GetXaxis()->SetLabelSize(0.03);
  ebicval->GetYaxis()->SetLabelSize(0.03);
  ebicval->SetXTitle("i#phi");
  ebicval->SetYTitle("i#eta");
  ebicval->SetMinimum(0);
  ebicval->SetMaximum(3);

  for (int k=0; k<N; k++) {
      ebicval->GetXaxis()->SetRangeUser(phimax[k]-8,phimax[k]+8);
      ebicval->GetYaxis()->SetRangeUser(etamax[k]-8,etamax[k]+8);
      ebicval->Draw("colz,text");
      t1.DrawLatex(0.83,0.875,"IC");
      sprintf(txt,"EB_intercalib_map_zoomed_%d.png",k+1);
      c4.SaveAs(txt);
  }



  // EB laser correction map (zoomed)
  
  eblascorr->SetMarkerSize(1.2);
  eblascorr->GetXaxis()->SetNdivisions(11);
  eblascorr->GetYaxis()->SetNdivisions(11);
  eblascorr->GetXaxis()->SetLabelSize(0.03);
  eblascorr->GetYaxis()->SetLabelSize(0.03);
  eblascorr->SetXTitle("i#phi");
  eblascorr->SetYTitle("i#eta");
  eblascorr->SetMinimum(0);
  eblascorr->SetMaximum(3);

  for (int k=0; k<N; k++) {
      eblascorr->GetXaxis()->SetRangeUser(phimax[k]-8,phimax[k]+8);
      eblascorr->GetYaxis()->SetRangeUser(etamax[k]-8,etamax[k]+8);
      eblascorr->Draw("colz,text");
      t1.DrawLatex(0.83,0.875,"Las. corr.");
      sprintf(txt,"Las_corr_zoomed_%d.png",k+1);
      c4.SaveAs(txt);
  }
  /*
  eblascorr->GetXaxis()->SetRangeUser(phimax-5,phimax+6);
  eblascorr->GetYaxis()->SetRangeUser(etamax-5,etamax+6);

  eblascorr->Draw("colz,text");
  t1.DrawLatex(0.83,0.875,"Las. corr.");


  c4.SaveAs("exomgg_0t_rereco_ev3_eblascorr_zoom_photon2.png");
*/

  // EB laser correction map (all channels)
  
  c1.cd();
  c1.SetLogz(0);
  eblascorrall->SetMaximum(2);
  eblascorrall->SetMinimum(0.1);
  eblascorrall->GetXaxis()->SetLabelSize(0.03);
  eblascorrall->GetYaxis()->SetLabelSize(0.03);
  eblascorrall->SetXTitle("i#phi");
  eblascorrall->SetYTitle("i#eta");
 
  eblascorrall->GetXaxis()->SetNdivisions(18);
  eblascorrall->GetYaxis()->SetNdivisions(2);
  eblascorrall->Draw("colz");
  t1.SetTextSize(0.05);
  t1.DrawLatex(0.88,0.91,"Las. Corr.");

  c1.SaveAs("exomgg_0t_rereco_ev3_eblascorr_all_photon2.png");



  // EB laser correction map (all good channels)


  eblascorrgood->SetMaximum(2);
  eblascorrgood->SetMinimum(0.1);
  eblascorrgood->GetXaxis()->SetLabelSize(0.03);
  eblascorrgood->GetYaxis()->SetLabelSize(0.03);
  eblascorrgood->SetXTitle("i#phi");
  eblascorrgood->SetYTitle("i#eta");
 
  eblascorrgood->GetXaxis()->SetNdivisions(18);
  eblascorrgood->GetYaxis()->SetNdivisions(2);
  eblascorrgood->Draw("colz");
  t1.SetTextSize(0.05);
  t1.DrawLatex(0.88,0.91,"Las. Corr.");

  c1.SaveAs("exomgg_0t_rereco_ev3_eblascorr_good_photon2.png");


  
  // EB laser correction 1d distributions 

  
  gStyle->SetOptStat(1111110);
  c4.cd();
  c4.SetLogy(1);
  eblascorrall_1d->SetLineColor(1);
  eblascorrall_1d->SetXTitle("Laser correction");
  eblascorrall_1d->Draw();

  c4.SaveAs("exomgg_0t_rereco_ev3_eblascorr1d_all_photon2.png");

  eblascorrgood_1d->SetLineColor(1);
  eblascorrgood_1d->SetXTitle("Laser correction");
  eblascorrgood_1d->Draw();

  c4.SaveAs("exomgg_0t_rereco_ev3_eblascorr1d_good_photon2.png");
  
}

#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>

void EtaMerger_byTriggers(){


   TString triggers[] = {
        "40",
        "80",
        "140",
        "200",
        "260",
        "320",
        "400",
        "450"
    };
    
    TString eta[] = {
        "0.0",
        "0.5",
        "1.0",
        "1.5",
        "2.0",
        "2.5",
        "3.0",
        "3.2",
        "4.7"
    };
    
    
    
    int bin_counter = (sizeof(eta)/sizeof(*eta));
    int trig_counter = (sizeof(triggers)/sizeof(*triggers));

    
    Double_t w = 1200;
    Double_t h = 800;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    
    
    
    
    TFile *DATA_file = new TFile("output-DATA-1.root");
    
    
    
    ///// Histos booking //////
    
    
    
    TH2D *DATA_low_merged = new TH2D("DATA_low_merged","",100,-4.799,4.799,144,-TMath::Pi(),TMath::Pi());
    TH2D *DATA_high_merged = new TH2D("DATA_high_merged","",100,-4.799,4.799,144,-TMath::Pi(),TMath::Pi());
    
    TH2D* DATA_high_chf[8];
    TH2D* DATA_low_chf[8];
    
    
    

    for (int j=0; j<trig_counter; j++) { //
		
		for (int i=0; i<bin_counter-1; i++) { //
    
		//// DATA MERGER ///
		DATA_low_chf[i] = (TH2D*) DATA_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ triggers[j]+  "/h_low_chf_etaphi"); 
		DATA_high_chf[i] = (TH2D*) DATA_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ triggers[j]+ "/h_high_chf_etaphi");
		
		DATA_high_merged->Add(DATA_high_chf[i]);
		DATA_low_merged->Add(DATA_low_chf[i]);
   	
		}//i
	
    //DATA_high_merged->Scale(1/DATA_high_merged->Integral());
    //DATA_low_merged->Scale(1/DATA_low_merged->Integral());
	
    c1->cd();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    DATA_high_merged->Draw("COLZ2");
    c1->SaveAs("jet"+triggers[j]+"_HIGH.png");
    
    
    c2->cd();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    DATA_low_merged->Draw("COLZ2");
    c2->SaveAs("jet"+triggers[j]+"_LOW.png");
    
    }//j   

}//main
    

#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>

void EtaMerger(){
    
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

    
    Double_t w = 1200;
    Double_t h = 800;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    TCanvas * c3 = new TCanvas("c3", "c3", w, h);
    TCanvas * c4 = new TCanvas("c4", "c4", w, h);
    TCanvas * c5 = new TCanvas("c5", "c5", w, h);
    TCanvas * c6 = new TCanvas("c6", "c6", w, h);
    
    //TFile *DATA_file = new TFile("output-DATA-2b.root");
    //TFile *MC_file = new TFile("output-MC-2b.root");
    
    TFile *DATA_file = new TFile("output-DATA-2b.root");
    TFile *MC_file = new TFile("output-MC-2b.root");
    
    
    ///// Histos booking //////
    
    TH2D *ratio = new TH2D("ratio","",100,-4.799,4.799,144,-TMath::Pi(),TMath::Pi());
    
    TH2D *DATA_low_merged = new TH2D("DATA_low_merged","",100,-4.799,4.799,144,-TMath::Pi(),TMath::Pi());
    TH2D *DATA_high_merged = new TH2D("DATA_high_merged","",100,-4.799,4.799,144,-TMath::Pi(),TMath::Pi());
    
    TH2D *MC_low_merged = new TH2D("MC_low_merged","",100,-4.799,4.799,144,-TMath::Pi(),TMath::Pi());
    TH2D *MC_high_merged = new TH2D("MC_high_merged","",100,-4.799,4.799,144,-TMath::Pi(),TMath::Pi());
    
    TH2D* DATA_high_chf[8];
    TH2D* DATA_low_chf[8];
    
    TH2D* MC_high_chf[8];
    TH2D* MC_low_chf[8];
    
    

    
    for (int i=0; i<bin_counter-1; i++) { //
    
		//// DATA MERGER ///
		DATA_low_chf[i] = (TH2D*) DATA_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/h_low_chf_etaphi"); 
		DATA_high_chf[i] = (TH2D*) DATA_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/h_high_chf_etaphi");
		
		DATA_high_merged->Add(DATA_high_chf[i]);
		DATA_low_merged->Add(DATA_low_chf[i]);
		
		//// MC MERGER ///
		MC_low_chf[i] = (TH2D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/h_low_chf_etaphi"); 
		MC_high_chf[i] = (TH2D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/h_high_chf_etaphi");
		
		MC_high_merged->Add(MC_high_chf[i]);
		MC_low_merged->Add(MC_low_chf[i]);

   	
    }//i
       
    DATA_high_merged->Scale(1/DATA_high_merged->Integral());
    DATA_low_merged->Scale(1/DATA_low_merged->Integral());
    
    MC_high_merged->Scale(1/MC_high_merged->Integral());
    MC_low_merged->Scale(1/MC_low_merged->Integral());
    
    
    c1->cd();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    DATA_high_merged->Draw("COLZ2");
    c1->SaveAs("FtoH_HIGH.png");
    
    
    c2->cd();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    DATA_low_merged->Draw("COLZ2");
    c2->SaveAs("FtoH_LOW.png");
    
    
    c3->cd();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    MC_high_merged->Draw("COLZ2");
    c3->SaveAs("MC_High.png");
    
    
    c4->cd();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    MC_low_merged->Draw("COLZ2");
    c4->SaveAs("MC_LOW.png");
    
    
    
    c5->cd();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    ratio = (TH2D*)DATA_high_merged->Clone("ratio");
    ratio->Divide(MC_high_merged);
    ratio->SetMaximum(1);
    ratio->Draw("COLZ2");
    c5->SaveAs("FtoHratio_HIGH.png");
    
    
    c6->cd();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    ratio = (TH2D*)DATA_low_merged->Clone("ratio");
    ratio->Divide(MC_low_merged);
    ratio->SetMaximum(1);
    ratio->Draw("COLZ2");
    c6->SaveAs("FtoHratio_LOW.png");
    
    
    
    
}//main
    

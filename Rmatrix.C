#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>

void Rmatrix(){
    
    TString eta[] = {
        "0.0",
        "0.5",
        "1.0",
        "1.5",
        "2.0",
        "2.5"
    };
    
    
    
    int bin_counter = (sizeof(eta)/sizeof(*eta));

    
    Double_t w = 1200;
    Double_t h = 800;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    c1->SetLogx();
    c1->SetLogy();
    //c1->SetFillColor(1);
    //c1->SetFrameFillColor(50);
    
    gPad->SetLogz();
    gStyle->SetPalette(1);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPaintTextFormat("1.2f");
    
    TFile *MC_file = new TFile("output-MC-PtHat-1.root");
    
    
    ///// Histos booking //////

    
    TH2D* R_Matrix[5];
    
    double sum_columns[5][100];
    

    
    for (int i=0; i<bin_counter-1; i++) { //
		
		R_Matrix[i] = (TH2D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/matrix_gen_reco");
		
		int first_bin = R_Matrix[i]->FindFirstBinAbove();
		int last_bin = R_Matrix[i]->FindLastBinAbove();
		
		
		 
		
		
		for (int j=first_bin; j<=last_bin; j++) {
		//for (int j=first_bin; j<=first_bin+2; j++) {
			for (int k=1; k<=last_bin; k++) {
				
				double binvalue = R_Matrix[i]->GetBinContent(j,k);
				if (binvalue != 0 ) sum_columns[i][j] += binvalue ;
				//if (binvalue != 0 ) cout << j << "," << k << "Ex value ---->" << binvalue << endl;
				//cout << "SUM---->" << binvalue << endl;
				
			}
			//cout << "SUM---->" << sum_columns[i][j] << endl;
		}
		
		for (int j=first_bin; j<=last_bin; j++) {
		//for (int j=first_bin; j<=first_bin+2; j++) {
			for (int k=1; k<=last_bin; k++) {
				
				double binvalue = R_Matrix[i]->GetBinContent(j,k);
				if (binvalue != 0 ) R_Matrix[i]->SetBinContent(j,k,binvalue/sum_columns[i][j]);
				//if (binvalue != 0 ) cout << j << "," << k << "New value ---->" << R_Matrix[i]->GetBinContent(j,k) << endl;
				
				
			}
			
		}
		
		
		c1->cd();
		
		
		
                R_Matrix[i]->GetYaxis()->SetRangeUser(70.,10000.);
   		R_Matrix[i]->GetXaxis()->SetRangeUser(70.,10000.);
   		R_Matrix[i]->GetZaxis()->SetRangeUser(0.0001,1.);
   		
   		R_Matrix[i]->GetYaxis()->SetTitle("Mjj_{ True}(GeV)");
   		R_Matrix[i]->GetXaxis()->SetTitle("Mjj_{ Reco}(GeV)");
   		R_Matrix[i]->GetYaxis()->SetTitleOffset(0.9);
                R_Matrix[i]->GetYaxis()->SetTitleSize(0.03);
                R_Matrix[i]->GetYaxis()->SetTitleFont(42);
                R_Matrix[i]->GetXaxis()->SetTitleOffset(1.2);
                R_Matrix[i]->GetXaxis()->SetTitleSize(0.03);
                R_Matrix[i]->GetXaxis()->SetTitleFont(42);
                R_Matrix[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                R_Matrix[i]->GetXaxis()->SetLabelSize(20);
                R_Matrix[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                R_Matrix[i]->GetYaxis()->SetLabelSize(20);
                //R_Matrix[i]->SetMarkerSize(0.6);
   		
                //Double_t norm = 1/R_Matrix[i]->Integral();
                //Double_t norm = 1/(R_Matrix[i]->GetMaximum());
                //R_Matrix[i]->Scale(norm);
                R_Matrix[i]->Draw("COLZ");
                
                TLatex lt;
                lt.SetNDC();
            	lt.SetTextSize(0.03);
                lt.DrawLatex(0.77,0.22,eta[i]+" < #eta < "+eta[i+1]);
                lt.SetTextAlign(12);
                
                c1->SaveAs("ResponseMatrix_"+ eta[i] + "-" + eta[i+1] +".png");
                R_Matrix[i]->Delete();
		
		

   	
    }//i
       
    
    
    
    
    
    
    
    
}//main
    
		

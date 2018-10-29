#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>


void Resolution(){
    
    TString eta[] = {
        "0.0",
        "0.5",
        "1.0",
        "1.5",
        "2.0",
        "2.5"
    };
    
    double y[] = {0., 0.5, 1.0, 1.5, 2.0, 2.5};
    
    char HistoName [100];
    
    double nbins = 104;
    double massBoundaries[104] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7060, 7320, 7589, 7866, 8152, 8447, 8752, 9067, 9391, 9726, 10072, 10430, 10798, 11179, 11571, 11977, 12395, 12827, 13272, 13732, 14000};
    
    int bin_counter = (sizeof(eta)/sizeof(*eta));

    
    Double_t w = 1200;
    Double_t h = 800;
    
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    //c2->SetLogy();
    c2->SetLogx();
	
	TLegend *leg = new TLegend(0.7028381,0.8165375,0.8597663,0.8875969,NULL,"brNDC");
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);
	leg->SetTextSize(0.03);
    
    TFile *MC_file = new TFile("binBybin_resolutions.root");
    
    ///// Histos booking //////
    TH1D *projh2X[5][104];
    TGraph *gr_rms[5],*gr_gaus[5];
    double rms[104], gaus[104];
    
    c2->cd(); 
    
    for (int i=0; i<bin_counter-1; i++) { //
 	for (int j=0; j<nbins; j++) { //
		sprintf(HistoName,"%s%d","/Projections_bin_",j+1);
		
		
                projh2X[i][j] = (TH1D*) MC_file->Get("Etas_/Eta_" + eta[i] + "-" + eta[i+1] + HistoName);
                
                rms[j] = projh2X[i][j]->GetRMS();
				
				projh2X[i][j]->Fit("gaus");
				if (projh2X[i][j]->GetMean() != 0) gaus[j] = projh2X[i][j]->GetFunction("gaus")->GetParameter(2); //taking sigma value of the fit
				else gaus[j] = 0;
                   
        }//j
        
        
        
        gr_rms[i] = new TGraph(nbins, massBoundaries, rms);
        gr_rms[i]->SetLineColor(2);
        gr_rms[i]->SetLineWidth(1);
        gr_rms[i]->SetMarkerColor(4);
        gr_rms[i]->SetMarkerSize(0.9);
		gr_rms[i]->SetMarkerStyle(26);
		gr_rms[i]->GetXaxis()->SetTitle("M_{jj}");
		gr_rms[i]->GetYaxis()->SetTitle("#sigmaM_{jj}");
		gr_rms[i]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
		gr_rms[i]->GetXaxis()->SetNoExponent();
		gr_rms[i]->GetXaxis()->SetLimits(40,10500);
		gr_rms[i]->GetHistogram()->SetMaximum(1.2);   // along          
        gr_rms[i]->GetHistogram()->SetMinimum(0.);
		gr_rms[i]->Draw("ALP");
		
		gr_gaus[i] = new TGraph(nbins, massBoundaries, gaus);
		gr_gaus[i]->SetLineColor(1);
        gr_gaus[i]->SetLineWidth(1);
        gr_gaus[i]->SetMarkerColor(1);
        gr_gaus[i]->SetMarkerSize(0.9);
		gr_gaus[i]->SetMarkerStyle(32);
		gr_gaus[i]->Draw("SAME LP");
		
		leg->AddEntry(gr_rms[i],"RMS","l");
		leg->AddEntry(gr_gaus[i],"FitGauss RMS","l");
		leg->Draw();
		
		c2->SaveAs("DeltaM_RMS_"+ eta[i] + "-" + eta[i+1] +".png");
		gr_rms[i]->Delete();
		gr_gaus[i]->Delete();
		leg->Clear();
		c2->Update();
		
   	
    }//i
    
}//main

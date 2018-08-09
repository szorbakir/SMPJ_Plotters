#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>

void Mjj_res(){
    
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
    //c1->SetLogy();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    
    
   
    
    TFile *MC_file = new TFile("output-MC-PtHat-1.root");
    
    
    
    ///// Histos booking //////
    TProfile *Mjj_res[5];
    
    
    
    

    
    for (int i=0; i<bin_counter-1; i++) { //
 	
		Mjj_res[i] = (TProfile*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/pdjmass_res");

                
                
                c1->cd(); 
                
                
            	            
                Mjj_res[i]->SetYTitle("Resolution");
                Mjj_res[i]->SetLineColor(kBlue);
                Mjj_res[i]->SetLineWidth(1);
                Mjj_res[i]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
                Mjj_res[i]->GetXaxis()->SetNoExponent();
                Mjj_res[i]->GetXaxis()->SetRangeUser(40,10000.);
                Mjj_res[i]->GetYaxis()->SetRangeUser(-1.,1.);
                Mjj_res[i]->GetYaxis()->SetLabelFont(43);
                Mjj_res[i]->GetYaxis()->SetLabelSize(12);
                Mjj_res[i]->SetXTitle("Mjj_{Gen}(GeV)");
                
                //Mjj_res[i]->GetYaxis()->SetRangeUser(1e-3,1e+9);
                Mjj_res[i]->Draw("e1");
                
                
           	TLatex lt;
                lt.SetNDC();
            	lt.SetTextSize(0.03);
                lt.DrawLatex(0.77,0.72,eta[i]+" < #eta < "+eta[i+1]);
                lt.SetTextAlign(12);
		
		c1->SaveAs("Mjj_Resolution_"+ eta[i] + "-" + eta[i+1] +".png");
                c1->Update();
                
		
		

   	
    }//i
       
    
    
    
    
    
    
    
    
}//main
    
		

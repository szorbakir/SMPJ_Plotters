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
    
<<<<<<< HEAD
    double y[] = {0., 0.5, 1.0, 1.5, 2.0, 2.5};
    
    char HistoName [100];
    double nbins = 103;

=======
   
    
    
    
>>>>>>> 22029c1badc38a2f9db793cc02446dc83a5af63f
    int bin_counter = (sizeof(eta)/sizeof(*eta));

    
    Double_t w = 1200;
    Double_t h = 800;
<<<<<<< HEAD
    /*
=======
    
>>>>>>> 22029c1badc38a2f9db793cc02446dc83a5af63f
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    c1->SetLogx();
    //c1->SetLogy();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
<<<<<<< HEAD
    */
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    //gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    //c2->SetLogy();
    //c2->SetLogx();

    
    TFile *MC_file = new TFile("output-MC-PtHat-1.root");
    
    TFile *output = new TFile("binBybin_resolutions.root", "RECREATE");
    output->mkdir("Etas_");
    TDirectory *topdir = output->GetDirectory("Etas_"); 
    topdir->cd();
    

    ///// Histos booking //////
    //TProfile *Mjj_res[5];
    TH2D *res2d[5];
    TH1D *projh2X[5][103];
    
 
    for (int i=0; i<bin_counter-1; i++) { //bin_counter-1
 	
		//Mjj_res[i] = (TProfile*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/pdjmass_res");
                res2d[i] = (TH2D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/h2jetres");
                
                // subdirectory for rapidity bin for output file	
     		const char *yname = Form("Eta_%1.1f-%1.1f", y[i], y[i+1]);
      		topdir->mkdir(yname);
      		TDirectory *ydir = topdir->GetDirectory(yname);
      		ydir->cd();
                
                /*
=======
    
    
    
   
    
    TFile *MC_file = new TFile("output-MC-PtHat-1.root");
    
    
    
    ///// Histos booking //////
    TProfile *Mjj_res[5];
    
    
    
    

    
    for (int i=0; i<bin_counter-1; i++) { //
 	
		Mjj_res[i] = (TProfile*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/pdjmass_res");

                
                
>>>>>>> 22029c1badc38a2f9db793cc02446dc83a5af63f
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
<<<<<<< HEAD
                */
                
                c2->cd();
                
                for (int j=0; j<nbins+1; j++) { 
                projh2X[i][j] = res2d[i]->ProjectionY(Form("bin_%d",j+1),j+1,j+2);
                projh2X[i][j]->Draw();
                
                sprintf(HistoName,"%s%d","Projections_bin_",j+1);
                projh2X[i][j]->Write(HistoName);
                
                projh2X[i][j]->Delete(); 
                c2->Update();
                
=======
>>>>>>> 22029c1badc38a2f9db793cc02446dc83a5af63f
                
		
		

<<<<<<< HEAD
     }//j	
=======
   	
>>>>>>> 22029c1badc38a2f9db793cc02446dc83a5af63f
    }//i
       
    
    
<<<<<<< HEAD
    output->Close();
=======
    
>>>>>>> 22029c1badc38a2f9db793cc02446dc83a5af63f
    
    
    
    
    
}//main
    
		

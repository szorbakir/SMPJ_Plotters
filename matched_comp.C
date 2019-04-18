#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>

void matched_comp(){
    
    TString eta[] = {
        "0.0",
        "0.5",
        "1.0",
        "1.5",
        "2.0",
        "2.5"
    };
    
    
    TString x[] ={"0","1","2","3","4"};
    int bin_counter = (sizeof(eta)/sizeof(*eta));

    
    Double_t w = 1200;
    Double_t h = 800;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    c1->SetLogx();
    c1->SetLogy();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    TLegend *leg = new TLegend(0.7646077,0.74677,0.8973289,0.8875969,NULL,"brNDC");
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);
    
   
    
   
    TFile *Gen_file = new TFile("output-MC-PtHat-1.root");
    
    
    
    ///// Histos booking //////

    
    TH1D* Matched_Spectra[5];
    TH1D* Spectra[5];
    TH1D* Ratio;


    
    for (int i=0; i<bin_counter-1; i++) { //
    
    		
 	
		
		Spectra[i] = (TH1D*) Gen_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/hdjmass");
		Matched_Spectra[i] = (TH1D*) Gen_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/hdjmass_matched");
                
                c1->cd(); 
                
                TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
            	pad1->SetBottomMargin(0.012);
            	pad1->Draw();
            	pad1-> SetLogy() ;
            	pad1-> SetLogx();
            	pad1->cd(); 
            	          
                Spectra[i]->SetYTitle("A.U");
                Spectra[i]->SetLineColor(kBlue);
                Spectra[i]->SetLineWidth(1);
                //Spectra[i]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
                //Spectra[i]->GetXaxis()->SetNoExponent();
                //Spectra[i]->GetXaxis()->SetRangeUser(x,8000.);
                Spectra[i]->GetYaxis()->SetLabelFont(43);
                Spectra[i]->GetYaxis()->SetLabelSize(12);
                
                //Spectra[i]->GetYaxis()->SetRangeUser(1e-3,1e+9);
                Spectra[i]->Draw("e1");
                
                Matched_Spectra[i]->SetLineColor(2);
	        Matched_Spectra[i]->SetLineWidth(1);
           	Matched_Spectra[i]->Draw("e1 same");
           	
           	
           	
           	leg->AddEntry(Spectra[i],"Spectra","l");
           	leg->AddEntry(Matched_Spectra[i],"Matched","l");
           	leg->Draw();
           	
           	TLatex lt;
                lt.SetNDC();
            	lt.SetTextSize(0.03);
                lt.DrawLatex(0.77,0.72,eta[i]+" < #eta < "+eta[i+1]);
                lt.SetTextAlign(12);
           	
           	Ratio = (TH1D*)Matched_Spectra[i]->Clone("Ratio");
           	
           	c1->cd(); 
           	
           	
           	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
                pad2->SetTopMargin(0.012);
                pad2->SetBottomMargin(0.28);
                pad2-> SetLogx();
                pad2->SetGridy();
                pad2->Draw();
                pad2->cd();       // pad2 becomes the current pad
            
                Ratio->Divide(Spectra[i]);
                
                //Cosmetics
                Ratio->SetLineWidth(1);     
                Ratio->SetXTitle("M_{jj} (GeV)");
                Ratio->GetYaxis()->SetTitleOffset(0.8);
                Ratio->SetYTitle("Matched/Spectra");
                Ratio->GetYaxis()->CenterTitle();
                Ratio->SetMarkerColor(kBlue);
                Ratio->SetLineColor(kBlue);
                Ratio->GetYaxis()->SetNdivisions(505);
                Ratio->GetXaxis()->SetNdivisions(511);
                Ratio->GetYaxis()->SetTitleSize(0.11);
                Ratio->GetYaxis()->SetTitleFont(42);
                Ratio->GetYaxis()->SetTitleOffset(0.35);
                Ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                Ratio->GetYaxis()->SetLabelSize(12);
                Ratio->GetYaxis()->SetRangeUser(0.8,1.2);
                Ratio->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    	        Ratio->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
                Ratio->GetXaxis()->SetTitleSize(0.11);
                Ratio->GetXaxis()->SetTitleFont(42);
                Ratio->GetXaxis()->SetTitleOffset(1.1);
                Ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                Ratio->GetXaxis()->SetLabelSize(12);

                Ratio->Draw("E1HIST");
 
                c1->SaveAs("Reco_matchedvsspectra_"+ eta[i] + "-" + eta[i+1] +".pdf");
                leg->Clear();
                c1->Update();
                
		
		

   	
    }//i
       
    
    
    
    
    
    
    
    
}//main
    
		

#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>

void Rivet_Gen_comp(){
    
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
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    TLegend *leg = new TLegend(0.7646077,0.74677,0.8973289,0.8875969,NULL,"brNDC");
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);
    
   
    
    TFile *MC_file = new TFile("output-MC-PtHat-1.root");
    TFile *Rivet_file = new TFile("P8-dijetMass-13TeV-ALL-ON.root");
    Rivet_file -> cd("CMS_Dijet_Mass_13TeV_test");
    
    
    ///// Histos booking //////

    
    TH1D* Gen_level[5];
    TH1D* Rivet_level[5];
    TH1D* Ratio;
    //TH1D* Reco_level[5];
    
    
    
    

    
    for (int i=0; i<bin_counter-1; i++) { //
    
    		char AK4Plot_no[20];
    		sprintf(AK4Plot_no, "%s%d%s","d0",i+1,"-x01-y01-AK4"); // mass plots
    		//sprintf(AK4Plot_no, "%s%d%s","d0",i+1,"-Pt-AK4");   //pt distribution
 	
		//Gen_level[i] = (TH1D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/gen_hdj_leading");
		Gen_level[i] = (TH1D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/gen_hdjmass");
		//Reco_level[i] = (TH1D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/hdjmass");
		Rivet_level[i] = (TH1D*)gDirectory->FindObjectAny(AK4Plot_no);
		
		int binmax = Rivet_level[i]->GetMaximumBin();
		double x = Rivet_level[i]->GetXaxis()->GetBinCenter(binmax);
		
		for (int k=1; k<=binmax; k++) {
                    
                	Rivet_level[i]->SetBinContent(k,0);
                        Rivet_level[i]->SetBinError(k,0);
                        
                        Gen_level[i]->SetBinContent(k,0);
                        Gen_level[i]->SetBinError(k,0);
                        
                        //Reco_level[i]->SetBinContent(k,0);
                        //Reco_level[i]->SetBinError(k,0);
                    
               }
		
		
		
		
		c1->cd();
		
               
   		
                Double_t norm = Rivet_level[i]->Integral()/Gen_level[i]->Integral();
                Gen_level[i]->Scale(norm);
                
                /*Double_t norm2 = Rivet_level[i]->Integral()/Reco_level[i]->Integral();
                Reco_level[i]->Scale(norm2);*/
                
                
                c1->cd(); 
                
                TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
            	pad1->SetBottomMargin(0.012);
            	pad1->Draw();
            	pad1-> SetLogy() ;
            	pad1-> SetLogx();
            	pad1->cd(); 
            	            
                Gen_level[i]->SetYTitle("A.U");
                Gen_level[i]->SetLineColor(kBlue);
                Gen_level[i]->SetLineWidth(1);
                //Gen_level[i]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
                //Gen_level[i]->GetXaxis()->SetNoExponent();
                Gen_level[i]->GetXaxis()->SetRangeUser(x,8000.);
                Gen_level[i]->GetYaxis()->SetLabelFont(43);
                Gen_level[i]->GetYaxis()->SetLabelSize(12);
                
                Gen_level[i]->GetYaxis()->SetRangeUser(1e-3,1e+9);
                Gen_level[i]->Draw("e1");
                
                Rivet_level[i]->SetLineColor(2);
	        Rivet_level[i]->SetLineWidth(1);
           	Rivet_level[i]->Draw("e1 same");
           	
           	/*Reco_level[i]->SetLineColor(3);
	        Reco_level[i]->SetLineWidth(1);
           	Reco_level[i]->Draw("e1 same");*/
           	
           	leg->AddEntry(Gen_level[i],"Gen","l");
           	leg->AddEntry(Rivet_level[i],"Rivet","l");
           	//leg->AddEntry(Reco_level[i],"Reco","l");
           	leg->Draw();
           	
           	TLatex lt;
                lt.SetNDC();
            	lt.SetTextSize(0.03);
                lt.DrawLatex(0.77,0.72,eta[i]+" < #eta < "+eta[i+1]);
                lt.SetTextAlign(12);
           	
           	//cout << "Rivet_BinNumber: " << Rivet_level[i]->GetSize()-2 << " Gen_BinNumber: " << Gen_level[i]->GetSize()-2 <<  endl;
           	
           	Ratio = (TH1D*)Rivet_level[i]->Clone("Ratio");
           	
           	c1->cd(); 
           	
           	
           	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
                pad2->SetTopMargin(0.012);
                pad2->SetBottomMargin(0.28);
                pad2-> SetLogx();
                pad2->SetGridy();
                pad2->Draw();
                pad2->cd();       // pad2 becomes the current pad
            
                Ratio->Divide(Gen_level[i]);
                
                //Cosmetics
                Ratio->SetLineWidth(1);
                Ratio->SetXTitle("M_{jj}(GeV)");
                //Ratio->SetXTitle("Leading jet P_{T}(GeV)");
                Ratio->GetYaxis()->SetTitleOffset(0.8);
                Ratio->SetYTitle("Rivet/Gen");
                Ratio->GetXaxis()->SetRangeUser(x,8000.);
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
                
                
	
                
                
           	
           	
           	
                
                
                c1->SaveAs("Riv_Gen_comp_"+ eta[i] + "-" + eta[i+1] +".png");
                leg->Clear();
                c1->Update();
                
		
		

   	
    }//i
       
    
    
    
    
    
    
    
    
}//main
    
		

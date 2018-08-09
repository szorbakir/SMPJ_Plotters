#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>


void Rivet_Gen_eta(){
    
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
    //c1->SetLogx();
    //c1->SetLogy();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    //c2->SetLogx();
    //c2->SetLogy();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    TCanvas * c3 = new TCanvas("c3", "c3", w, h);
    //c3->SetLogx();
    //c3->SetLogy();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    TLegend *leg = new TLegend(0.7646077,0.74677,0.8973289,0.8875969,NULL,"brNDC");
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);
    
    TLegend *leg2 = new TLegend(0.7646077,0.74677,0.8973289,0.8875969,NULL,"brNDC");
    leg2->SetBorderSize(0);
    leg2->SetLineColor(0);
    leg2->SetLineStyle(1);
    leg2->SetLineWidth(0);
    leg2->SetFillColor(0);
    leg2->SetFillStyle(1001);
    
    TLegend *leg3 = new TLegend(0.7646077,0.74677,0.8973289,0.8875969,NULL,"brNDC");
    leg3->SetBorderSize(0);
    leg3->SetLineColor(0);
    leg3->SetLineStyle(1);
    leg3->SetLineWidth(0);
    leg3->SetFillColor(0);
    leg3->SetFillStyle(1001);
    
    TFile *MC_file = new TFile("output-MC-PtHat-1.root");
    TFile *Rivet_file = new TFile("P8-dijetMass-13TeV-ALL-ON.root");
    Rivet_file -> cd("CMS_Dijet_Mass_13TeV_test");
    
    
    ///// Histos booking //////

    
    TH1D* Gen_leading_eta;
    TH1D* Gen_subleading_eta;
    TH1D* Gen_mass_eta;
    
    TH1D* Rivet_leading_eta;
    TH1D* Rivet_subleading_eta;
    TH1D* Rivet_mass_eta;
    
    TH1D* Ratio_leading;
    TH1D* Ratio_subleading;
    TH1D* Ratio_mass;
    
    for (int i=0; i<1; i++) { //
    
    		char Leading_jet[20];
    		char Subleading_jet[20];
    		char Mass[20];
    		
    		sprintf(Leading_jet, "%s","Leading_jet_eta"); // leading jet eta
    		sprintf(Subleading_jet, "%s","Subleading_jet_eta");   // subleading jet eta
    		sprintf(Mass, "%s","Dijet_mass_eta");             // dijet mass eta
 	
		Gen_leading_eta = (TH1D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/gen_hdj_leading_eta");
		Gen_subleading_eta = (TH1D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/gen_hdj_subleading_eta");
	        Gen_mass_eta = (TH1D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/gen_hdjmass_eta");
		
		Rivet_leading_eta = (TH1D*)gDirectory->FindObjectAny(Leading_jet);
		Rivet_subleading_eta = (TH1D*)gDirectory->FindObjectAny(Subleading_jet);
		Rivet_mass_eta = (TH1D*)gDirectory->FindObjectAny(Mass);
		
		
		
                    
               }
               
               c1->cd();
		
               
   		
                Double_t norm = Rivet_leading_eta->Integral()/Gen_leading_eta->Integral();
                Gen_leading_eta->Scale(norm);
                
                /*Double_t norm2 = Rivet_level[i]->Integral()/Reco_level[i]->Integral();
                Reco_level[i]->Scale(norm2);*/
                
                
                c1->cd(); 
                
                TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
            	pad1->SetBottomMargin(0.012);
            	pad1->Draw();
            	//pad1-> SetLogy() ;
            	//pad1-> SetLogx();
            	pad1->cd(); 
            	            
                Gen_leading_eta->SetYTitle("A.U");
                Gen_leading_eta->SetLineColor(kBlue);
                Gen_leading_eta->SetLineWidth(1);
                //Gen_level[i]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
                //Gen_level[i]->GetXaxis()->SetNoExponent();
                //Gen_level[i]->GetXaxis()->SetRangeUser(x,8000.);
                Gen_leading_eta->GetYaxis()->SetLabelFont(43);
                Gen_leading_eta->GetYaxis()->SetLabelSize(12);
                
                //Gen_leading_eta->GetYaxis()->SetRangeUser(1e-3,1e+9);
                Gen_leading_eta->Draw("e1");
                
                Rivet_leading_eta->SetLineColor(2);
	        Rivet_leading_eta->SetLineWidth(1);
           	Rivet_leading_eta->Draw("e1 same");
           	
           	leg->AddEntry(Gen_leading_eta,"Gen_j1_eta","l");
           	leg->AddEntry(Rivet_leading_eta,"Rivet_j1_eta","l");
           	leg->Draw();
           	
           	
           	Ratio_leading = (TH1D*)Rivet_leading_eta->Clone("Ratio_leading");
           	
           	c1->cd(); 
           	
           	
           	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
                pad2->SetTopMargin(0.012);
                pad2->SetBottomMargin(0.28);
                //pad2-> SetLogx();
                pad2->SetGridy();
                pad2->Draw();
                pad2->cd();       // pad2 becomes the current pad
            
                Ratio_leading->Divide(Gen_leading_eta);
                
                //Cosmetics
                Ratio_leading->SetLineWidth(1);
                Ratio_leading->SetXTitle("Leading jet #eta");
                Ratio_leading->GetYaxis()->SetTitleOffset(0.8);
                Ratio_leading->SetYTitle("Rivet/Gen");
                //Ratio->GetXaxis()->SetRangeUser(x,8000.);
                Ratio_leading->GetYaxis()->CenterTitle();
                Ratio_leading->SetMarkerColor(kBlue);
                Ratio_leading->SetLineColor(kBlue);
                Ratio_leading->GetYaxis()->SetNdivisions(505);
                Ratio_leading->GetXaxis()->SetNdivisions(511);
                Ratio_leading->GetYaxis()->SetTitleSize(0.11);
                Ratio_leading->GetYaxis()->SetTitleFont(42);
                Ratio_leading->GetYaxis()->SetTitleOffset(0.35);
                Ratio_leading->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                Ratio_leading->GetYaxis()->SetLabelSize(12);
                Ratio_leading->GetYaxis()->SetRangeUser(0.8,1.2);
                Ratio_leading->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    	        Ratio_leading->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
                Ratio_leading->GetXaxis()->SetTitleSize(0.11);
                Ratio_leading->GetXaxis()->SetTitleFont(42);
                Ratio_leading->GetXaxis()->SetTitleOffset(1.1);
                Ratio_leading->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                Ratio_leading->GetXaxis()->SetLabelSize(12);
	        Ratio_leading->Draw("E1HIST");
                
 
                c1->SaveAs("Riv_Gen_leading_jet_eta.png");
                leg->Clear();
                c1->Update();
                
                c2->cd();
		
               
   		
                Double_t norm2 = Rivet_subleading_eta->Integral()/Gen_subleading_eta->Integral();
                Gen_subleading_eta->Scale(norm2);
                
                /*Double_t norm2 = Rivet_level[i]->Integral()/Reco_level[i]->Integral();
                Reco_level[i]->Scale(norm2);*/
                
                
                c2->cd(); 
                
                TPad *pad3 = new TPad("pad3", "pad3", 0, 0.3, 1, 1.0);
            	pad3->SetBottomMargin(0.012);
            	pad3->Draw();
            	//pad1-> SetLogy() ;
            	//pad1-> SetLogx();
            	pad3->cd(); 
            	            
                Gen_subleading_eta->SetYTitle("A.U");
                Gen_subleading_eta->SetLineColor(kBlue);
                Gen_subleading_eta->SetLineWidth(1);
                Gen_subleading_eta->GetYaxis()->SetLabelFont(43);
                Gen_subleading_eta->GetYaxis()->SetLabelSize(12);
                
                
                Gen_subleading_eta->Draw("e1");
                
                Rivet_subleading_eta->SetLineColor(2);
	        Rivet_subleading_eta->SetLineWidth(1);
           	Rivet_subleading_eta->Draw("e1 same");
           	
           	leg2->AddEntry(Gen_subleading_eta,"Gen_j2_eta","l");
           	leg2->AddEntry(Rivet_subleading_eta,"Rivet_j2_eta","l");
           	leg2->Draw();
           	
           	
           	Ratio_subleading = (TH1D*)Rivet_subleading_eta->Clone("Ratio_subleading");
           	
           	c2->cd(); 
           	
           	
           	TPad *pad4 = new TPad("pad4", "pad4", 0, 0.05, 1, 0.3);
                pad4->SetTopMargin(0.012);
                pad4->SetBottomMargin(0.28);
                pad4->SetGridy();
                pad4->Draw();
                pad4->cd();       // pad2 becomes the current pad
            
                Ratio_subleading->Divide(Gen_subleading_eta);
                
                //Cosmetics
                Ratio_subleading->SetLineWidth(1);
                Ratio_subleading->SetXTitle("Subleading jet #eta");
                Ratio_subleading->GetYaxis()->SetTitleOffset(0.8);
                Ratio_subleading->SetYTitle("Rivet/Gen");
                //Ratio->GetXaxis()->SetRangeUser(x,8000.);
                Ratio_subleading->GetYaxis()->CenterTitle();
                Ratio_subleading->SetMarkerColor(kBlue);
                Ratio_subleading->SetLineColor(kBlue);
                Ratio_subleading->GetYaxis()->SetNdivisions(505);
                Ratio_subleading->GetXaxis()->SetNdivisions(511);
                Ratio_subleading->GetYaxis()->SetTitleSize(0.11);
                Ratio_subleading->GetYaxis()->SetTitleFont(42);
                Ratio_subleading->GetYaxis()->SetTitleOffset(0.35);
                Ratio_subleading->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                Ratio_subleading->GetYaxis()->SetLabelSize(12);
                Ratio_subleading->GetYaxis()->SetRangeUser(0.8,1.2);
                Ratio_subleading->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    	        Ratio_subleading->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
                Ratio_subleading->GetXaxis()->SetTitleSize(0.11);
                Ratio_subleading->GetXaxis()->SetTitleFont(42);
                Ratio_subleading->GetXaxis()->SetTitleOffset(1.1);
                Ratio_subleading->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                Ratio_subleading->GetXaxis()->SetLabelSize(12);
	        Ratio_subleading->Draw("E1HIST");
                
 
                c2->SaveAs("Riv_Gen_subleading_jet_eta.png");
                leg2->Clear();
                c2->Update();
                
                c3->cd();
		
               
   		
                Double_t norm3 = Rivet_mass_eta->Integral()/Gen_mass_eta->Integral();
                Gen_mass_eta->Scale(norm3);
                
                /*Double_t norm2 = Rivet_level[i]->Integral()/Reco_level[i]->Integral();
                Reco_level[i]->Scale(norm2);*/
                
                
                c3->cd(); 
                
                TPad *pad5 = new TPad("pad5", "pad5", 0, 0.3, 1, 1.0);
            	pad5->SetBottomMargin(0.012);
            	pad5->Draw();
            	//pad1-> SetLogy() ;
            	//pad1-> SetLogx();
            	pad5->cd(); 
            	            
                Gen_mass_eta->SetYTitle("A.U");
                Gen_mass_eta->SetLineColor(kBlue);
                Gen_mass_eta->SetLineWidth(1);
                Gen_mass_eta->GetYaxis()->SetLabelFont(43);
                Gen_mass_eta->GetYaxis()->SetLabelSize(12);
                Gen_mass_eta->GetXaxis()->SetRangeUser(0.,5.);
                
                
                Gen_mass_eta->Draw("e1");
                
                Rivet_mass_eta->SetLineColor(2);
	        Rivet_mass_eta->SetLineWidth(1);
           	Rivet_mass_eta->Draw("e1 same");
           	
           	leg3->AddEntry(Gen_mass_eta,"Gen_Mjj_etamax","l");
           	leg3->AddEntry(Rivet_mass_eta,"Rivet_Mjj_etamax","l");
           	leg3->Draw();
           	
           	
           	Ratio_mass = (TH1D*)Rivet_mass_eta->Clone("Ratio_mass");
           	
           	c3->cd(); 
           	
           	
           	TPad *pad6 = new TPad("pad6", "pad6", 0, 0.05, 1, 0.3);
                pad6->SetTopMargin(0.012);
                pad6->SetBottomMargin(0.28);
                pad6->SetGridy();
                pad6->Draw();
                pad6->cd();       // pad2 becomes the current pad
            
                Ratio_mass->Divide(Gen_mass_eta);
                
                //Cosmetics
                Ratio_mass->SetLineWidth(1);
                Ratio_mass->SetXTitle("Dijet system |#eta|_{max}");
                Ratio_mass->GetYaxis()->SetTitleOffset(0.8);
                Ratio_mass->SetYTitle("Rivet/Gen");
                //Ratio->GetXaxis()->SetRangeUser(x,8000.);
                Ratio_mass->GetYaxis()->CenterTitle();
                Ratio_mass->SetMarkerColor(kBlue);
                Ratio_mass->SetLineColor(kBlue);
                Ratio_mass->GetYaxis()->SetNdivisions(505);
                Ratio_mass->GetXaxis()->SetNdivisions(511);
                Ratio_mass->GetYaxis()->SetTitleSize(0.11);
                Ratio_mass->GetYaxis()->SetTitleFont(42);
                Ratio_mass->GetYaxis()->SetTitleOffset(0.35);
                Ratio_mass->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                Ratio_mass->GetYaxis()->SetLabelSize(12);
                Ratio_mass->GetYaxis()->SetRangeUser(0.8,1.2);
                Ratio_mass->GetXaxis()->SetRangeUser(0.,5.);
                Ratio_mass->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    	        Ratio_mass->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
                Ratio_mass->GetXaxis()->SetTitleSize(0.11);
                Ratio_mass->GetXaxis()->SetTitleFont(42);
                Ratio_mass->GetXaxis()->SetTitleOffset(1.1);
                Ratio_mass->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                Ratio_mass->GetXaxis()->SetLabelSize(12);
	        Ratio_mass->Draw("E1HIST");
                
 
                c3->SaveAs("Riv_Gen_dijet_eta.png");
                leg3->Clear();
                c3->Update();
                
}//main

#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>

void smp_plotter_fraction(){
    
    TString observable[] = {
        "hchf",
        "hnef",
        "hnhf",
        "hcef",
        "hmuf",
    };
    
    TString axis_name[] = {
        "CH Fraction",
        "NE Fraction",
        "NH Fraction",
        "CE Fraction",
        "MU Fraction"
    };
    
    TString eta_bins[] = {
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
    
    TString inputFile1;
    TString inputFile2;
    TString inputFile3;
    TString inputFile4;
    
    Double_t w = 1200;
    Double_t h = 1000;
    Double_t norm = 1;
    
    TH1F *h_DATA_;
    
    TH1F *h_pythia_;
    TH1F *h_madgraph_;
    TH1F *h_herwig_;
    
    TH1F *h_Ratio_pythia;
    TH1F *h_Ratio_madgraph;
    TH1F *h_Ratio_herwig;
    
    
    inputFile1.Form("output-MC-2b.root");
    inputFile2.Form("output-MDG-2b.root");
    inputFile3.Form("output-HW-2b.root");
    inputFile4.Form("output-DATA-2b.root");
    
    
    int counter = (sizeof(observable)/sizeof(*observable));
    int bin_counter = (sizeof(eta_bins)/sizeof(*eta_bins));
    
    for (int j=0; j<bin_counter - 1; j++) { //
        for (int i=0; i<counter; i++) {
    
//    for (int j=0; j<1; j++) { //
//        for (int i=0; i<1; i++) {

    
            TString hname = (TString) (observable[i]+"_"+eta_bins[j] + "-" + eta_bins[j+1]);
            TString eta_name = (TString) (eta_bins[j] + " < #eta < " + eta_bins[j+1]);
            
            //MC
            TFile* MCFile1 = TFile::Open(inputFile1.Data(), "READ");
            MCFile1 -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_pythia_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
            
            TFile* MCFile2 = TFile::Open(inputFile2.Data(), "READ");
            MCFile2 -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_madgraph_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
            
            TFile* MCFile3 = TFile::Open(inputFile3.Data(), "READ");
            MCFile3 -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_herwig_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
            
            //DATA
            TFile* DATAFile = TFile::Open(inputFile4.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_DATA_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
    
            //Bin Removing
            int ncells = h_pythia_->GetSize();  // it adds underflow and overflow bins so I have to extract those
            int MC_NBins = ncells-2;
            
            for (int k=1; k<=MC_NBins; k++) { //
                
                double check_bins = h_pythia_->GetBinContent(k);
                double Xaxis_mc = h_pythia_->GetBinCenter(k);
                double data_check_bins = h_DATA_->GetBinContent(k);
                
                if (Xaxis_mc > 1.0 && check_bins!=0){ 
                	h_pythia_->SetBinContent(k,0);
                	h_pythia_->SetBinError(k,0);
                    h_madgraph_->SetBinContent(k,0);
                    h_madgraph_->SetBinError(k,0);
                    h_herwig_->SetBinContent(k,0);
                    h_herwig_->SetBinError(k,0);
                }
            }
            
            // Basic Scaling ////
            double Int_data = (h_DATA_->Integral());
            double Int_MC1   = (h_pythia_->Integral());
            double Int_MC2   = (h_madgraph_->Integral());
            double Int_MC3   = (h_herwig_->Integral());
            
            
            h_DATA_->Scale(1/Int_data);
            h_pythia_->Scale(1/Int_MC1);
            h_madgraph_->Scale(1/Int_MC2);
            h_herwig_->Scale(1/Int_MC3);
            
            //Draw, cosmetics, etc.
            TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    
            gStyle->SetOptStat(0);
            gStyle->SetOptTitle(0);
    
            c1->cd();
    
            TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
            pad1->SetBottomMargin(0.012);
            pad1->Draw();
            pad1-> SetLogy() ;
            //pad1-> SetLogx();
            pad1->cd();
            
            h_Ratio_pythia = (TH1F*)h_pythia_->Clone("h_Ratio_pythia");
            h_Ratio_madgraph = (TH1F*)h_madgraph_->Clone("h_Ratio_madgraph");
            h_Ratio_herwig = (TH1F*)h_herwig_->Clone("h_Ratio_herwig");
            
            h_DATA_->GetXaxis()->SetLabelOffset(999);
            h_DATA_->GetXaxis()->SetLabelSize(0);
            h_DATA_->GetYaxis()->SetNdivisions(505);
            h_DATA_->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_DATA_->GetXaxis()->SetLabelSize(20);
            h_DATA_->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_DATA_->GetYaxis()->SetLabelSize(20);
            h_DATA_->SetMarkerColor(kBlack);
            h_DATA_->SetMarkerStyle(20);
            h_DATA_->SetLineColor(kBlack);
            h_DATA_->SetLineWidth(2);
            h_DATA_->SetYTitle("A.U.");
            h_DATA_->GetYaxis()->SetTitleOffset(0.8);
            h_DATA_->GetYaxis()->SetTitleSize(0.05);
            h_DATA_->GetYaxis()->SetTitleFont(42);
            
            h_pythia_->SetMarkerColor(kRed+1);
            h_pythia_->SetLineColor(kRed+1);
            h_pythia_->SetLineWidth(2);
            
            h_madgraph_->SetMarkerColor(kBlue+1);
            h_madgraph_->SetLineColor(kBlue+1);
            h_madgraph_->SetLineWidth(2);
            
            h_herwig_->SetMarkerColor(kGreen+1);
            h_herwig_->SetLineColor(kGreen+1);
            h_herwig_->SetLineWidth(2);
            
            h_DATA_->Draw("E1HIST");
            h_pythia_->Draw("E1HIST SAME");
            h_madgraph_->Draw("E1HIST SAME");
            h_herwig_->Draw("E1HIST SAME");
            
            if (observable[i] == "hchf") {
                TLegend *leg = new TLegend(0.3460601,0.1089862,0.4279967,0.3501843,NULL,"brNDC");
                leg->SetBorderSize(0);
                leg->SetTextFont(42);
                leg->SetTextSize(0.045);
                leg->AddEntry(h_pythia_,"Pythia","l");
                leg->AddEntry(h_madgraph_,"MadGraph","l");
                leg->AddEntry(h_herwig_,"Herwig","l");
                leg->AddEntry(h_DATA_,"RunBCDEFGH","lep");
                leg->AddEntry("","AK4chs Jets","");
                leg->AddEntry("",eta_name,"");
                leg->Draw();
            } else {
                TLegend *leg = new TLegend(0.7145242,0.6249084,0.8163606,0.8886447,NULL,"brNDC");
                leg->SetBorderSize(0);
                leg->SetTextFont(42);
                leg->SetTextSize(0.045);
                leg->AddEntry(h_pythia_,"Pythia","l");
                leg->AddEntry(h_madgraph_,"MadGraph","l");
                leg->AddEntry(h_herwig_,"Herwig","l");
                leg->AddEntry(h_DATA_,"RunBCDEFGH","lep");
                leg->AddEntry("","AK4chs Jets","");
                leg->AddEntry("",eta_name,"");
                leg->Draw();
            }
           

            TLatex *t6 = new TLatex();
            t6->SetNDC();
            t6->SetTextFont(42);
            t6->SetTextSize(0.05);
            t6->SetTextAlign(11);
            t6->DrawLatex(0.1,0.92,"#bf{CMS} #font[12]{Work in Progress 2016}");
            
            TLatex *t7 = new TLatex();
            t7->SetNDC();
            t7->SetTextFont(42);
            t7->SetTextSize(0.05);
            t7->SetTextAlign(11);
            t7->DrawLatex(0.68,0.92,"#sqrt{s}=13 TeV, 39.9 fb^{-1}");
            c1->cd();          // Go back to the main canvas before defining pad2
   	
            TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
            pad2->SetTopMargin(0.012);
            pad2->SetBottomMargin(0.28);
            //pad2-> SetLogx();
            pad2->SetGridy();
            pad2->Draw();
            pad2->cd();       // pad2 becomes the current pad
            
            h_Ratio_pythia->Divide(h_DATA_);
            h_Ratio_madgraph->Divide(h_DATA_);
            h_Ratio_herwig->Divide(h_DATA_);
            
            //Cosmetics
            h_Ratio_pythia->SetLineWidth(2);
            h_Ratio_pythia->SetXTitle(axis_name[i]);
            h_Ratio_pythia->GetYaxis()->SetTitleOffset(0.8);
            h_Ratio_pythia->SetYTitle("MC/Data");
            h_Ratio_pythia->GetYaxis()->CenterTitle();
            h_Ratio_pythia->SetMarkerColor(kRed+1);
            h_Ratio_pythia->SetLineColor(kRed+1);
            h_Ratio_pythia->GetYaxis()->SetNdivisions(606);
            h_Ratio_pythia->GetXaxis()->SetNdivisions(511);
            h_Ratio_pythia->GetYaxis()->SetTitleSize(0.11);
            h_Ratio_pythia->GetYaxis()->SetTitleFont(42);
            h_Ratio_pythia->GetYaxis()->SetTitleOffset(0.35);
            h_Ratio_pythia->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_Ratio_pythia->GetYaxis()->SetLabelSize(15);
            h_Ratio_pythia->GetYaxis()->SetRangeUser(0.5,1.7);
            h_Ratio_pythia->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    	    h_Ratio_pythia->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
            h_Ratio_pythia->GetXaxis()->SetTitleSize(0.11);
            h_Ratio_pythia->GetXaxis()->SetTitleFont(42);
            h_Ratio_pythia->GetXaxis()->SetTitleOffset(1.1);
            h_Ratio_pythia->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_Ratio_pythia->GetXaxis()->SetLabelSize(15);
            
            h_Ratio_madgraph->SetMarkerColor(kBlue+1);
            h_Ratio_madgraph->SetLineColor(kBlue+1);
            h_Ratio_madgraph->SetLineWidth(2);
            
            h_Ratio_herwig->SetMarkerColor(kGreen+1);
            h_Ratio_herwig->SetLineColor(kGreen+1);
            h_Ratio_herwig->SetLineWidth(2);
            
            h_Ratio_pythia->Draw("E1HIST");
            h_Ratio_madgraph->Draw("E1HISTSAME");
            h_Ratio_herwig->Draw("E1HISTSAME");

   
            c1->SaveAs(hname + ".png"); //saving...
            c1->Update();
        }
    }
}


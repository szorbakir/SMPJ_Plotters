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
    
    Double_t w = 1200;
    Double_t h = 900;
    Double_t norm = 1;
    
    TH1F *h_DATA_part1_;
    TH1F *h_DATA_part2_;
    TH1F *h_MC_;
    TH1F *h_Ratio;
    TH1F *h_Ratio_2;
    
    inputFile1.Form("output-MC-2b.root");
    inputFile2.Form("output-RunBtoFearly-2b.root");
    inputFile3.Form("output-RunFlatetoH-2b.root");
    
    int counter = (sizeof(observable)/sizeof(*observable));
    int bin_counter = (sizeof(eta_bins)/sizeof(*eta_bins));
    
    for (int j=0; j<bin_counter - 1; j++) { //
        for (int i=0; i<counter; i++) { 
            
            TString hname = (TString) (observable[i]+"_"+eta_bins[j] + "-" + eta_bins[j+1]);
            TString eta_name = (TString) (eta_bins[j] + " < #eta < " + eta_bins[j+1]);
            
            //MC
            TFile* MCFile = TFile::Open(inputFile1.Data(), "READ");
            MCFile -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_MC_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
            
            //DATA
            TFile* DATAFile = TFile::Open(inputFile2.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_DATA_part1_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
            
            TFile* DATAFile2 = TFile::Open(inputFile3.Data(), "READ");
            DATAFile2 -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_DATA_part2_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
    
            //Bin Removing
            int ncells = h_MC_->GetSize();  // it adds underflow and overflow bins so I have to extract those
            int MC_NBins = ncells-2;
            
            for (int k=1; k<=MC_NBins; k++) { //
                
                double check_bins = h_MC_->GetBinContent(k);
                double Xaxis_mc = h_MC_->GetBinCenter(k);
                double data_check_bins = h_DATA_part1_->GetBinContent(k);
                
                if (Xaxis_mc > 1.0 && check_bins!=0){ 
                	h_MC_->SetBinContent(k,0);
                	h_MC_->SetBinError(k,0);
                }
            }
            
            // Basic Scaling ////
            double Int_data = (h_DATA_part1_->Integral());
            double Int_data_2 = (h_DATA_part2_->Integral());
            double Int_MC   = (h_MC_->Integral()); //Numbers are coming from the loop above...They corresponds to first and last bin of data///
            
            h_DATA_part1_->Scale(1/Int_data);
            h_DATA_part2_->Scale(1/Int_data_2);
            h_MC_->Scale(1/Int_MC);
            
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
            
            h_Ratio = (TH1F*)h_MC_->Clone("h_Ratio");
            h_Ratio_2 = (TH1F*)h_MC_->Clone("h_Ratio_2");
            
            h_DATA_part1_->GetXaxis()->SetLabelOffset(999);
            h_DATA_part1_->GetXaxis()->SetLabelSize(0);
            h_DATA_part1_->GetYaxis()->SetNdivisions(505);
            h_DATA_part1_->GetYaxis()->SetNdivisions(505);
            h_DATA_part1_->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_DATA_part1_->GetXaxis()->SetLabelSize(20);
            h_DATA_part1_->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_DATA_part1_->GetYaxis()->SetLabelSize(20);
            h_DATA_part1_->SetMarkerColor(kBlue);
            h_DATA_part1_->SetMarkerStyle(20);
            h_DATA_part1_->SetLineColor(kBlue);
            h_DATA_part1_->SetLineWidth(1);
            h_DATA_part1_->SetYTitle("A.U.");
            h_DATA_part1_->GetYaxis()->SetTitleOffset(0.8);
            h_DATA_part1_->GetYaxis()->SetTitleSize(0.05);
            h_DATA_part1_->GetYaxis()->SetTitleFont(42);
            
            h_MC_->SetMarkerColor(kBlack);
            h_MC_->SetLineColor(kBlack);
            h_MC_->SetLineWidth(1);
            
            h_DATA_part2_->SetMarkerColor(kRed);
            h_DATA_part2_->SetLineColor(kRed);
            h_DATA_part2_->SetLineWidth(1);
            h_DATA_part2_->SetMarkerStyle(22);
           
    
            h_DATA_part1_->Draw("E1HIST");
            h_MC_->Draw("E1HIST SAME");
            h_DATA_part2_->Draw("E1HIST SAME");
            
            if (observable[i] == "hchf") {
                TLegend *leg = new TLegend(0.3060601,0.1089862,0.4079967,0.3001843,NULL,"brNDC");
                leg->SetBorderSize(0);
                leg->SetTextFont(42);
                leg->SetTextSize(0.045);
                leg->AddEntry(h_MC_,"MC","l");
                leg->AddEntry(h_DATA_part1_,"RunBCDEF_{Early}, 19.49 fb^{-1}","lep");
                leg->AddEntry(h_DATA_part2_,"RunF_{Late}GH,  16.5 fb^{-1}","lep");
                leg->AddEntry("","AK4chs Jets","");
                leg->AddEntry("",eta_name,"");
                leg->Draw();
            } else {
                TLegend *leg = new TLegend(0.6060601,0.6689862,0.7079967,0.8901843,NULL,"brNDC");
                leg->SetBorderSize(0);
                leg->SetTextFont(42);
                leg->SetTextSize(0.045);
                leg->AddEntry(h_MC_,"MC","l");
                leg->AddEntry(h_DATA_part1_,"RunBCDEF_{Early}, 19.49 fb^{-1}","lep");
                leg->AddEntry(h_DATA_part2_,"RunF_{Late}GH,  16.5 fb^{-1}","lep");
                leg->AddEntry("","AK4chs Jets","");
                leg->AddEntry("",eta_name,"");
                leg->Draw();
            }
           

            TLatex *t6 = new TLatex();
            t6->SetNDC();
            t6->SetTextFont(42);
    	    t6->SetTextSize(0.05);
    	    t6->SetTextAlign(11);
            t6->DrawLatex(0.1,0.92,"#bf{CMS} Preliminary, #sqrt{s}=13 TeV");
            
            c1->cd();          // Go back to the main canvas before defining pad2
   	
            TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
            pad2->SetTopMargin(0.012);
            pad2->SetBottomMargin(0.28);
            //pad2-> SetLogx();
            pad2->SetGridy();
            pad2->Draw();
            pad2->cd();       // pad2 becomes the current pad
            
            h_Ratio->Divide(h_DATA_part1_);
            h_Ratio_2->Divide(h_DATA_part2_);
    
            //Cosmetics
            h_Ratio->SetLineWidth(1);
            h_Ratio->SetXTitle(axis_name[i]);
            h_Ratio->GetYaxis()->SetTitleOffset(0.8);
            h_Ratio->SetYTitle("MC/Data");
            h_Ratio->GetYaxis()->CenterTitle();
            h_Ratio->SetMarkerColor(kBlue);
            h_Ratio->SetLineColor(kBlue);
            h_Ratio->GetYaxis()->SetNdivisions(505);
            h_Ratio->GetXaxis()->SetNdivisions(511);
            h_Ratio->GetYaxis()->SetTitleSize(0.11);
            h_Ratio->GetYaxis()->SetTitleFont(42);
            h_Ratio->GetYaxis()->SetTitleOffset(0.35);
            h_Ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_Ratio->GetYaxis()->SetLabelSize(15);
            h_Ratio->GetYaxis()->SetRangeUser(0.3,1.7);
            h_Ratio->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    	    h_Ratio->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
            h_Ratio->GetXaxis()->SetTitleSize(0.11);
            h_Ratio->GetXaxis()->SetTitleFont(42);
            h_Ratio->GetXaxis()->SetTitleOffset(1.1);
            h_Ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_Ratio->GetXaxis()->SetLabelSize(15);
            
            
            h_Ratio_2->SetMarkerColor(kRed);
            h_Ratio_2->SetLineColor(kRed);
            h_Ratio_2->SetLineWidth(1);


            h_Ratio->Draw("E1HIST");
            h_Ratio_2->Draw("E1HIST SAME");
   
            c1->SaveAs(hname + ".png"); //saving...
            c1->Update();
        }
    }
}


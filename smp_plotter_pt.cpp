#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>


void smp_plotter_pt(){
    
    TString observable[] = {
        "hpt",
        "hdj_leading",
        "hdj_subleading",
        "hpt1",
        "hpt2"
    };
    
    TString axis_name[] = {
        "Raw p_{T} Spectrum (GeV)",
        "Leading Jet p_{T} (GeV)",
        "Subleading Jet p_{T} (GeV)",
        "Leading Jet p_{T} (GeV)",
        "Subleading Jet p_{T} (GeV)"
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
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    
    TH1D *h_MadGraph_;
    TH1D *h_Herwig_;
    TH1D *h_Pythia_;
    
    TH1D *h_DATA_;
    
    TH1D *h_Ratio_MD_;
    TH1D *h_Ratio_HW_;
    TH1D *h_Ratio_PYT_;
    
    inputFile1.Form("output-MDG-2b.root");
    inputFile2.Form("output-HW-2b.root");
    inputFile3.Form("output-MC-2a.root");
    inputFile4.Form("output-DATA-2b.root");
    
    int counter = (sizeof(observable)/sizeof(*observable));
    int bin_counter = (sizeof(eta_bins)/sizeof(*eta_bins));
    
    for (int j=0; j<bin_counter-1; j++) {
        for (int i=0; i<counter; i++) {
        
            TString hname = (TString) (observable[i]+"_"+eta_bins[j] + "-" + eta_bins[j+1]);
            TString eta_name = (TString) (eta_bins[j] + " < #eta < " + eta_bins[j+1]);
            
            //MC
            TFile* MCFile1 = TFile::Open(inputFile1.Data(), "READ");
            MCFile1 -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_MadGraph_ = (TH1D*)gDirectory->FindObjectAny(observable[i]);
            
            //MC
            TFile* MCFile2 = TFile::Open(inputFile2.Data(), "READ");
            MCFile2 -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_Herwig_ = (TH1D*)gDirectory->FindObjectAny(observable[i]);
            
            //MC
            TFile* MCFile3 = TFile::Open(inputFile3.Data(), "READ");
            MCFile3 -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_Pythia_ = (TH1D*)gDirectory->FindObjectAny(observable[i]);
    
            //DATA
            TFile* DATAFile = TFile::Open(inputFile4.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_DATA_ = (TH1D*)gDirectory->FindObjectAny(observable[i]);
            
            double xmin = 1000000, xmax = 0;
            double ymin = 0.;
            double ymax = 0.;

            int ncells = h_DATA_->GetSize();  // it adds underflow and overflow bins so I have to extract those
            int Data_NBins = ncells-2;
            
            for (int k=1; k<=Data_NBins; k++) {
                
                double check_bins = h_DATA_->GetBinContent(k);
                
                if (check_bins!=0) {
                    
                    double Xaxis_data = h_DATA_->GetBinCenter(k);
                    
                    if (k < xmin) {xmin = k; ymin = Xaxis_data;}
                    if (k > xmax) {xmax = k; ymax = Xaxis_data;}
                    }
            }
            
            // Basic Scaling ////
            double Int_data = (h_DATA_->Integral());
            double Int_MC1   = (h_MadGraph_->Integral(xmin,xmax));
            double Int_MC2   = (h_Herwig_->Integral(xmin,xmax));
            double Int_MC3   = (h_Pythia_->Integral(xmin,xmax));
            
            double Scale_Factor_1 = 0;
            double Scale_Factor_2 = 0;
            double Scale_Factor_3 = 0;
            
            Scale_Factor_1 = Int_data/Int_MC1;
            h_MadGraph_->Scale(Scale_Factor_1);
            Scale_Factor_2 = Int_data/Int_MC2;
            h_Herwig_->Scale(Scale_Factor_2);
            Scale_Factor_3 = Int_data/Int_MC3;
            h_Pythia_->Scale(Scale_Factor_3);
            //End of scaling //
            
            //Draw, cosmetics, etc.
            gStyle->SetOptStat(0);
            gStyle->SetOptTitle(0);
    
            c1->cd();
    
            TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
            pad1->SetBottomMargin(0.010); // Upper and lower plot are joined
            pad1->Draw();             // Draw the upper pad: pad1
            pad1-> SetLogy();
            pad1-> SetLogx();
            pad1->cd();
    
            h_Ratio_MD_ = (TH1D*) h_MadGraph_->Clone("h_Ratio_MD_");
            h_Ratio_HW_ = (TH1D*) h_Herwig_->Clone("h_Ratio_HW_");
            h_Ratio_PYT_ = (TH1D*) h_Pythia_->Clone("h_Ratio_PYT_");

            h_MadGraph_->GetXaxis()->SetRangeUser(ymin,2500);
            h_MadGraph_->GetXaxis()->SetLabelOffset(999);
            h_MadGraph_->GetYaxis()->SetNdivisions(505);
            h_MadGraph_->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_MadGraph_->GetXaxis()->SetLabelSize(20);
            h_MadGraph_->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_MadGraph_->GetYaxis()->SetLabelSize(20);
            h_MadGraph_->SetMarkerColor(kBlue+1);
            h_MadGraph_->SetLineColor(kBlue+1);
            h_MadGraph_->SetLineWidth(2);
            h_MadGraph_->SetYTitle("A.U.");
            h_MadGraph_->GetYaxis()->SetTitleOffset(0.8);
            h_MadGraph_->GetYaxis()->SetTitleSize(0.05);
            h_MadGraph_->GetYaxis()->SetTitleFont(42);
            
            h_Herwig_->SetMarkerColor(kGreen+1);
            h_Herwig_->SetLineColor(kGreen+1);
            h_Herwig_->SetLineWidth(2);
            
            h_Pythia_->SetMarkerColor(kRed+1);
            h_Pythia_->SetLineColor(kRed+1);
            h_Pythia_->SetLineWidth(2);
            
            h_DATA_->SetMarkerColor(kBlack);
            h_DATA_->SetMarkerStyle(20);
            h_DATA_->SetLineColor(kBlack);
            h_DATA_->SetLineWidth(2);
            
            h_MadGraph_->Draw("E1HIST");
            h_Herwig_->Draw("E1HISTSAME");
            h_Pythia_->Draw("E1HISTSAME");
            h_DATA_->Draw("E1HIST SAME");
            
            TLegend *leg = new TLegend(0.7145242,0.6249084,0.8163606,0.8886447,NULL,"brNDC");
            leg->SetBorderSize(0);
            leg->SetTextFont(42);
            leg->SetTextSize(0.045);
            leg->AddEntry(h_MadGraph_,"MadGraph","l");
            leg->AddEntry(h_Herwig_,"Herwig","l");
            leg->AddEntry(h_Pythia_,"Pythia","l");
            leg->AddEntry(h_DATA_,"RunBCDEFGH","lp");
            leg->AddEntry("","AK4chs Jets","");
            leg->AddEntry("",eta_name,"");
            leg->Draw();
            
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
            pad2->SetTopMargin(0.005);
            pad2->SetBottomMargin(0.28);
            pad2->SetGridy();
            pad2->Draw();
            pad2->cd();       // pad2 becomes the current pad
            gPad-> SetLogx();
    
            h_Ratio_MD_->Divide(h_DATA_);
            h_Ratio_HW_->Divide(h_DATA_);
            h_Ratio_PYT_->Divide(h_DATA_);
    
            //cosmetics
            h_Ratio_MD_->GetXaxis()->SetRangeUser(ymin,2500);
            h_Ratio_MD_->SetXTitle(axis_name[i]);
            h_Ratio_MD_->SetYTitle("MC/Data");
            h_Ratio_MD_->GetYaxis()->SetTitleOffset(0.8);
            h_Ratio_MD_->GetYaxis()->CenterTitle();
            h_Ratio_MD_->SetMarkerColor(kBlue+1);
            h_Ratio_MD_->SetLineColor(kBlue+1);
            h_Ratio_MD_->SetLineWidth(2);
            h_Ratio_MD_->GetYaxis()->SetRangeUser(0.5,1.9);
            h_Ratio_MD_->GetYaxis()->SetNdivisions(606);
            h_Ratio_MD_->GetYaxis()->SetTitleSize(0.11);
            h_Ratio_MD_->GetYaxis()->SetTitleFont(42);
            h_Ratio_MD_->GetYaxis()->SetTitleOffset(0.35);
            h_Ratio_MD_->GetYaxis()->SetLabelFont(43);
            h_Ratio_MD_->GetYaxis()->SetLabelSize(18);
            h_Ratio_MD_->GetXaxis()->SetMoreLogLabels();
            h_Ratio_MD_->GetXaxis()->SetNoExponent();
            h_Ratio_MD_->GetXaxis()->SetTitleSize(0.11);
            h_Ratio_MD_->GetXaxis()->SetTitleFont(42);
            h_Ratio_MD_->GetXaxis()->SetTitleOffset(1.1);
            h_Ratio_MD_->GetXaxis()->SetLabelFont(43);
            h_Ratio_MD_->GetXaxis()->SetLabelSize(18);
            
            h_Ratio_HW_->SetMarkerColor(kGreen+1);
            h_Ratio_HW_->SetLineColor(kGreen+1);
            h_Ratio_HW_->SetLineWidth(2);
            
            h_Ratio_PYT_->SetMarkerColor(kRed+1);
            h_Ratio_PYT_->SetLineColor(kRed+1);
            h_Ratio_PYT_->SetLineWidth(2);
            
            h_Ratio_MD_->Draw("E1HIST");
            h_Ratio_HW_->Draw("E1HISTSAME");
            h_Ratio_PYT_->Draw("E1HISTSAME");
            
            c1->SaveAs(hname + ".png"); //saving...
        }
    }
}

#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>

void smp_plotter_pt(){
    
    TString observable[] = {
        "hpt",
        "hpt4"
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
        "2.5"       
    };
    
    TString inputFile1;
    
    
    Double_t w = 1200;
    Double_t h = 1000;
    Double_t norm = 1;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    
    
    
    TH1D *h_DATA_;
    TH1D *h_DATA_2;
    
    TH1D *h_Ratio_;
    
    
    
    inputFile1.Form("output-DATA-2b.root");
    
    int counter = (sizeof(observable)/sizeof(*observable));
    int bin_counter = (sizeof(eta_bins)/sizeof(*eta_bins));
    
    for (int j=0; j<bin_counter-1; j++) {
        for (int i=0; i<1; i++) {
        
            TString hname = (TString) (observable[i]+"_"+eta_bins[j] + "-" + eta_bins[j+1]);
            TString eta_name = (TString) (eta_bins[j] + " < #eta < " + eta_bins[j+1]);
            
    
            //DATA
            TFile* DATAFile = TFile::Open(inputFile1.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_DATA_ = (TH1D*)gDirectory->FindObjectAny(observable[i]);
            h_DATA_2 = (TH1D*)gDirectory->FindObjectAny(observable[i+1]);
            
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
            
            /*// Basic Scaling ////
            double Int_data = (h_DATA_->Integral());
            double Int_data2 = (h_DATA_2->Integral());
            
            
            double Scale_Factor_1 = 0;
            
            Scale_Factor_1 = Int_data/Int_data2;
            h_DATA_2->Scale(Scale_Factor_1);
            */
            
            
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
    
            h_Ratio_ = (TH1D*) h_DATA_2->Clone("h_Ratio_");
            

            h_DATA_->GetXaxis()->SetRangeUser(ymin,2500);
            h_DATA_->GetXaxis()->SetLabelOffset(999);
            h_DATA_->GetYaxis()->SetNdivisions(505);
            h_DATA_->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_DATA_->GetXaxis()->SetLabelSize(20);
            h_DATA_->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_DATA_->GetYaxis()->SetLabelSize(20);
            h_DATA_->SetMarkerColor(kBlue+1);
            h_DATA_->SetLineColor(kBlue+1);
            h_DATA_->SetLineWidth(2);
            h_DATA_->SetYTitle("A.U.");
            h_DATA_->GetYaxis()->SetTitleOffset(0.8);
            h_DATA_->GetYaxis()->SetTitleSize(0.05);
            h_DATA_->GetYaxis()->SetTitleFont(42);
            
            h_DATA_2->SetMarkerColor(kBlack);
            h_DATA_2->SetMarkerStyle(20);
            h_DATA_2->SetLineColor(kBlack);
            h_DATA_2->SetLineWidth(2);
            
            h_DATA_->Draw("E1HIST");
            h_DATA_2->Draw("E1HIST SAME");
            
            TLegend *leg = new TLegend(0.7145242,0.6249084,0.8163606,0.8886447,NULL,"brNDC");
            leg->SetBorderSize(0);
            leg->SetTextFont(42);
            leg->SetTextSize(0.045);
            leg->AddEntry(h_DATA_,"Inclusive jets","l");
            leg->AddEntry(h_DATA_2,"High Chf jets","l");
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
    
            h_Ratio_->Divide(h_DATA_);
            
    
            //cosmetics
            h_Ratio_->GetXaxis()->SetRangeUser(ymin,2500);
            h_Ratio_->SetXTitle(axis_name[i]);
            h_Ratio_->SetYTitle("Ratio");
            h_Ratio_->GetYaxis()->SetTitleOffset(0.8);
            h_Ratio_->GetYaxis()->CenterTitle();
            h_Ratio_->SetMarkerColor(kBlue+1);
            h_Ratio_->SetLineColor(kBlue+1);
            h_Ratio_->SetLineWidth(2);
            h_Ratio_->GetYaxis()->SetRangeUser(0.,0.1);
            h_Ratio_->GetYaxis()->SetNdivisions(606);
            h_Ratio_->GetYaxis()->SetTitleSize(0.11);
            h_Ratio_->GetYaxis()->SetTitleFont(42);
            h_Ratio_->GetYaxis()->SetTitleOffset(0.35);
            h_Ratio_->GetYaxis()->SetLabelFont(43);
            h_Ratio_->GetYaxis()->SetLabelSize(18);
            h_Ratio_->GetXaxis()->SetMoreLogLabels();
            h_Ratio_->GetXaxis()->SetNoExponent();
            h_Ratio_->GetXaxis()->SetTitleSize(0.11);
            h_Ratio_->GetXaxis()->SetTitleFont(42);
            h_Ratio_->GetXaxis()->SetTitleOffset(1.1);
            h_Ratio_->GetXaxis()->SetLabelFont(43);
            h_Ratio_->GetXaxis()->SetLabelSize(18);
        
            
            h_Ratio_->Draw("E1HIST");
            
            
            c1->SaveAs(hname + ".png"); //saving...
        }
    }
}

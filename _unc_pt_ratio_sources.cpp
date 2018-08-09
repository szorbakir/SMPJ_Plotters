#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>


void _unc_pt_ratio_sources(){
    
    TString observable[] = {
        "hpt",
        "hdj_leading",
        "hdj_subleading"
//        "hpt1",
//        "hpt2"
    };
    
    TString axis_name[] = {
        "Jet p_{T} (GeV)",
        "Leading Jet p_{T} (GeV)",
        "Subleading Jet p_{T} (GeV)"
//        "Leading Jet p_{T} (GeV)"
//        "Subleading Jet p_{T} (GeV)"
    };
    
    TString eta_bins[] = {
        "0.0",
        "0.5",
        "1.0",
        "1.5",
        "2.0",
        "2.5",
//        "3.0",
//        "3.2",
//        "4.7"
    };
    
    TString inputFile1;
    TString inputFile2;
    TString inputFile3;
    TString inputFile4;
    
    Double_t w = 1200;
    Double_t h = 1000;
    Double_t norm = 1;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    
    TH1D *h_Pythia_;
    TH1D *h_Pythia_up_;
    TH1D *h_Pythia_down_;
    
    TH1D *h_DATA_;
    TH1D *h_DATA_up_;
    TH1D *h_DATA_down_;
    
    TH1D *h_Ratio_PYT_;
    TH1D *h_Ratio_PYT_up_;
    TH1D *h_Ratio_PYT_down_;
    
    TH1D *h_Ratio_PYT_up_lumi_;
    TH1D *h_Ratio_PYT_down_lumi_;
    
    TH1D *h_Ratio_PYT_up_det_;
    TH1D *h_Ratio_PYT_down_det_;
    
    TH1D *h_unc_det_;
    TH1D *h_unc_lumi_;
    
    TH1D *h_DATA_up_lumi_;
    TH1D *h_DATA_down_lumi_;
    
    TH1D *h_DATA_up_det_;
    TH1D *h_DATA_down_det_;
    
    
    inputFile1.Form("datas/output-MC-2b.root");
    inputFile2.Form("datas/output-DATA-2b.root");
    inputFile3.Form("datas/output-DATA-2b-up.root");
    inputFile4.Form("datas/output-DATA-2b-down.root");
    
    int counter = (sizeof(observable)/sizeof(*observable));
    int bin_counter = (sizeof(eta_bins)/sizeof(*eta_bins));
    
    for (int j=0; j<bin_counter-1; j++) {
        for (int i=0; i<counter; i++) {
            
            TString hname = (TString) (observable[i]+"_"+eta_bins[j] + "-" + eta_bins[j+1]);
            TString eta_name = (TString) (eta_bins[j] + " < |y| < " + eta_bins[j+1]);
            
            //MC
            TFile* MCFile = TFile::Open(inputFile1.Data(), "READ");
            MCFile -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_Pythia_ = (TH1D*)gDirectory->FindObjectAny(observable[i]);
            h_Pythia_up_ = (TH1D*) h_Pythia_->Clone("h_Pythia_up_");
            h_Pythia_down_ = (TH1D*) h_Pythia_->Clone("h_Pythia_down_");
            
            //DATA
            TFile* DATAFile = TFile::Open(inputFile2.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_DATA_ = (TH1D*)gDirectory->FindObjectAny(observable[i]);
            
            //DATA_up
            TFile* DATAFile_up = TFile::Open(inputFile3.Data(), "READ");
            DATAFile_up-> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_DATA_up_ = (TH1D*)gDirectory->FindObjectAny(observable[i]);
            
            //DATA_down
            TFile* DATAFile_down = TFile::Open(inputFile4.Data(), "READ");
            DATAFile_down-> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_DATA_down_ = (TH1D*)gDirectory->FindObjectAny(observable[i]);
            
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

            h_Pythia_->Scale((h_DATA_->Integral())/(h_Pythia_->Integral(h_DATA_->FindFirstBinAbove(0,1),h_DATA_->FindLastBinAbove(0,1))));
//            h_Pythia_up_->Scale((h_DATA_up_->Integral())/(h_Pythia_up_->Integral(h_DATA_up_->FindFirstBinAbove(0,1),h_DATA_up_->FindLastBinAbove(0,1))));
//            h_Pythia_down_->Scale((h_DATA_down_->Integral())/(h_Pythia_down_->Integral(h_DATA_down_->FindFirstBinAbove(0,1),h_DATA_down_->FindLastBinAbove(0,1))));
            
            //Draw, cosmetics, etc.
            gStyle->SetOptStat(0);
            gStyle->SetOptTitle(0);
            
            c1->cd();

            h_Ratio_PYT_ = (TH1D*) h_Pythia_->Clone("h_Ratio_PYT_");
            h_Ratio_PYT_up_ = (TH1D*) h_Pythia_->Clone("h_Ratio_PYT_up_");
            h_Ratio_PYT_down_ = (TH1D*) h_Pythia_->Clone("h_Ratio_PYT_down_");
            h_Ratio_PYT_up_lumi_ = (TH1D*) h_Pythia_->Clone("h_Ratio_PYT_up_lumi_");
            h_Ratio_PYT_down_lumi_ = (TH1D*) h_Pythia_->Clone("h_Ratio_PYT_down_lumi_");
            h_Ratio_PYT_up_det_ = (TH1D*) h_Pythia_->Clone("h_Ratio_PYT_up_det_");
            h_Ratio_PYT_down_det_ = (TH1D*) h_Pythia_->Clone("h_Ratio_PYT_down_det_");
            
            h_unc_lumi_ = (TH1D*) h_DATA_->Clone("h_unc_lumi_");
            h_DATA_up_lumi_ = (TH1D*) h_DATA_->Clone("h_DATA_up_lumi_");
            h_DATA_down_lumi_ = (TH1D*) h_DATA_->Clone("h_DATA_down_lumi_");
            
            h_unc_lumi_->Scale(2.5/100.0);
            h_DATA_up_lumi_->Add(h_unc_lumi_);
            h_DATA_down_lumi_->Add(h_unc_lumi_,-1);
            
            h_unc_det_ = (TH1D*) h_DATA_->Clone("h_unc_det_");
            h_DATA_up_det_ = (TH1D*) h_DATA_->Clone("h_DATA_up_det_");
            h_DATA_down_det_ = (TH1D*) h_DATA_->Clone("h_DATA_down_det_");
            
            h_unc_det_->Scale(1./100.0);
            h_DATA_up_det_->Add(h_unc_det_);
            h_DATA_down_det_->Add(h_unc_det_,-1);
            
            
            
            c1->Draw();
            c1->cd();       // pad2 becomes the current pad
            c1-> SetLogx();
            
            h_Ratio_PYT_->Divide(h_DATA_);
            h_Ratio_PYT_up_->Divide(h_DATA_up_);
            h_Ratio_PYT_down_->Divide(h_DATA_down_);
            h_Ratio_PYT_up_lumi_->Divide(h_DATA_up_lumi_);
            h_Ratio_PYT_down_lumi_->Divide(h_DATA_down_lumi_);
            h_Ratio_PYT_up_det_->Divide(h_DATA_up_det_);
            h_Ratio_PYT_down_det_->Divide(h_DATA_down_det_);
            
            //cosmetics
            h_Ratio_PYT_->GetXaxis()->SetRangeUser(ymin,2500);
            h_Ratio_PYT_->SetXTitle(axis_name[i]);
            h_Ratio_PYT_->SetYTitle("MC/Data");
            h_Ratio_PYT_->GetYaxis()->CenterTitle();
            h_Ratio_PYT_->SetMarkerColor(kBlack);
            h_Ratio_PYT_->SetMarkerStyle(20);
            h_Ratio_PYT_->SetLineColor(kBlack);
            h_Ratio_PYT_->SetLineWidth(2);
            h_Ratio_PYT_->GetYaxis()->SetRangeUser(0.6,1.3);
            h_Ratio_PYT_->GetYaxis()->SetTitleOffset(1.3);
            h_Ratio_PYT_->GetXaxis()->SetMoreLogLabels();
            h_Ratio_PYT_->GetXaxis()->SetNoExponent();
            h_Ratio_PYT_->GetXaxis()->SetTitleOffset(1.3);

            h_Ratio_PYT_down_->SetLineColor(kRed);
            h_Ratio_PYT_down_->SetMarkerColor(kRed);
//            h_Ratio_PYT_down_->SetLineStyle(2);
            h_Ratio_PYT_down_->SetLineWidth(2);
            
            h_Ratio_PYT_up_->SetLineColor(kRed);
            h_Ratio_PYT_up_->SetMarkerColor(kRed);
//            h_Ratio_PYT_up_->SetLineStyle(2);
            h_Ratio_PYT_up_->SetLineWidth(2);
            
            h_Ratio_PYT_down_lumi_->SetLineColor(kGreen+2);
            h_Ratio_PYT_down_lumi_->SetMarkerColor(kGreen+2);
//            h_Ratio_PYT_down_lumi_->SetLineStyle(2);
            h_Ratio_PYT_down_lumi_->SetLineWidth(2);
            
            h_Ratio_PYT_up_lumi_->SetLineColor(kGreen+2);
            h_Ratio_PYT_up_lumi_->SetMarkerColor(kGreen+2);
//            h_Ratio_PYT_up_lumi_->SetLineStyle(2);
            h_Ratio_PYT_up_lumi_->SetLineWidth(2);
            
            h_Ratio_PYT_down_det_->SetLineColor(kBlue);
            h_Ratio_PYT_down_det_->SetMarkerColor(kBlue);
//            h_Ratio_PYT_down_det_->SetLineStyle(2);
            h_Ratio_PYT_down_det_->SetLineWidth(2);
            
            h_Ratio_PYT_up_det_->SetLineColor(kBlue);
            h_Ratio_PYT_up_det_->SetMarkerColor(kBlue);
//            h_Ratio_PYT_up_det_->SetLineStyle(2);
            h_Ratio_PYT_up_det_->SetLineWidth(2);
            
            
            
            h_Ratio_PYT_->Draw("E1");
            h_Ratio_PYT_down_->Draw("SAME HIST AC F");
            h_Ratio_PYT_up_->Draw("SAME HIST AC");
            h_Ratio_PYT_down_lumi_->Draw("SAME HIST AC F");
            h_Ratio_PYT_up_lumi_->Draw("SAME HIST AC");
            h_Ratio_PYT_up_det_->Draw("SAME HIST AC F");
            h_Ratio_PYT_down_det_->Draw("SAME HIST AC");
            
            
            TLegend *leg = new TLegend(0.1435726,0.6923077,0.245409,0.8882051,NULL,"brNDC");
            leg->SetBorderSize(0);
            leg->SetTextFont(42);
            leg->SetTextSize(0.03);
            leg->AddEntry(h_Ratio_PYT_,"MC:Pythia8/Data:2016","lp");
            leg->AddEntry(h_Ratio_PYT_up_,"JEC Uncertainty","lf");
            leg->AddEntry(h_Ratio_PYT_up_lumi_,"Luminosity Uncertainty","lf");
            leg->AddEntry(h_Ratio_PYT_up_det_,"Detector Effects Uncertainty","lf");
            leg->AddEntry("","AK4chs Jets","");
            leg->AddEntry("",eta_name,"");
            leg->Draw();
            
            TLatex *t6 = new TLatex();
            t6->SetNDC();
            t6->SetTextFont(42);
            t6->SetTextSize(0.035);
            t6->SetTextAlign(11);
            t6->DrawLatex(0.1,0.92,"#bf{CMS} #font[12]{Work in Progress 2016}");
            
            TLatex *t7 = new TLatex();
            t7->SetNDC();
            t7->SetTextFont(42);
            t7->SetTextSize(0.033);
            t7->SetTextAlign(11);
            t7->DrawLatex(0.68,0.92,"#sqrt{s}=13 TeV, 35.9 fb^{-1}");
            
            c1->cd();          // Go back to the main canvas before defining pad2
            
            c1->SaveAs("pngs/unc_pt_ratio_" + hname + ".png"); //saving...
            
            
        }
    }
}

#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>

void _unc_shaded_pt(){
    
    TString observable[] = {
        "hpt",
//        "hdj_leading",
//        "hdj_subleading"
//        "hpt1",
//        "hpt2"
    };
    
    TString axis_name[] = {
        "p_{T} Spectrum (GeV)",
//        "Leading Jet p_{T} (GeV)",
//        "Subleading Jet p_{T} (GeV)"
//        "Leading Jet p_{T} (GeV)"
//        "Subleading Jet p_{T} (GeV)"
    };
    
    TString eta_bins[] = {
        "0.0",
        "0.5",
        "1.0",
        "1.5",
        "2.0",
        "2.5"
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
    
    TH1D *h_Pythia_;
    TH1D *h_Pythia_up_;
    TH1D *h_Pythia_down_;
    
    TH1D *h_DATA_;
    TH1D *h_DATA_up_;
    TH1D *h_DATA_down_;
    
    TH1D *h_Ratio_PYT_;
    TH1D *h_Ratio_PYT_up_;
    TH1D *h_Ratio_PYT_down_;
    
    inputFile1.Form("datas/output-MC-2b.root");
    inputFile2.Form("datas/output-DATA-2b.root");
    inputFile3.Form("datas/output-DATA-2b-up.root");
    inputFile4.Form("datas/output-DATA-2b-down.root");
    
    TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",1200,1000);
    c1->SetLogx();
    
    int counter = (sizeof(observable)/sizeof(*observable));
    int bin_counter = (sizeof(eta_bins)/sizeof(*eta_bins));
    
    for (int j=0; j<bin_counter-1; j++) { //bin_counter-1 
        for (int i=0; i<counter; i++) {
            
            TString hname = (TString) (observable[i]+"_"+eta_bins[j] + "-" + eta_bins[j+1]);
            TString eta_name = (TString) (eta_bins[j] + " < #eta < " + eta_bins[j+1]);
            
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
            
            h_Pythia_->Scale((h_DATA_->Integral())/(h_Pythia_->Integral(h_DATA_->FindFirstBinAbove(0,1),h_DATA_->FindLastBinAbove(0,1))));
            
            double xmin = 1000000, xmax = 0;
            double ymin = 0.;
            double ymax = 0.;
            
            int ncells = h_DATA_->GetSize();  // it adds underflow and overflow bins so I have to extract those
            int Data_NBins = ncells-2;
            
            double x_axis[Data_NBins];
            double y_axis[Data_NBins];
            double xerror_axis[Data_NBins];
            double yerror_axis[Data_NBins];
            double y_axis_mc[Data_NBins];
            double ymin_axis[Data_NBins];
            double ymax_axis[Data_NBins];
            double ratio_y_data[Data_NBins];
            double ratio_y_data_up[Data_NBins];
            double ratio_y_data_down[Data_NBins];
            double error_plus[Data_NBins];
            double error_minus[Data_NBins];
			
			double unc_up[Data_NBins];
			double unc_down[Data_NBins];
            
            x_axis[Data_NBins]=0;
            y_axis[Data_NBins]=0;
            xerror_axis[Data_NBins]=0;
            yerror_axis[Data_NBins]=0;
            y_axis_mc[Data_NBins]=0;
            ymin_axis[Data_NBins]=0;
            ymax_axis[Data_NBins]=0;
            ratio_y_data[Data_NBins]=0;
            ratio_y_data_up[Data_NBins]=0;
            ratio_y_data_down[Data_NBins]=0;
            error_plus[Data_NBins]=0;
            error_minus[Data_NBins]=0;
            
            TH1 *frame = c1->DrawFrame(74,0.0,4000,2.0);
            
            for (int k=0; k<Data_NBins; k++) { //

                double check_bins = h_DATA_->GetBinContent(k);

                if (check_bins!=0) {
                    
                    x_axis[k] = h_DATA_->GetBinCenter(k);
//                    xerror_axis[k] = h_DATA_->GetBinCenterError(k);
                    y_axis[k] = h_DATA_->GetBinContent(k);
//                    yerror_axis[k] = h_DATA_->GetBinContentError(k);
                    y_axis_mc[k] = h_Pythia_->GetBinContent(k);                    
                    ymax_axis[k] = h_DATA_up_->GetBinContent(k);
                    ymin_axis[k] = h_DATA_down_->GetBinContent(k);
                    
                    ratio_y_data[k] = y_axis_mc[k]/y_axis[k];
                    ratio_y_data_up[k] = y_axis_mc[k]/ymax_axis[k];
                    ratio_y_data_down[k] = y_axis_mc[k]/ymin_axis[k];
                    
                    //min-max
                    unc_up[k] = y_axis_mc[k]/max(ymax_axis[k], ymin_axis[k]);
                    unc_down[k] = y_axis_mc[k]/min(ymax_axis[k], ymin_axis[k]);
					
                    double Xaxis_data = h_DATA_->GetBinCenter(k);

                    if (k < xmin) {xmin = k; ymin = Xaxis_data;}
                    if (k > xmax) {xmax = k; ymax = Xaxis_data;}
                }
            }
            
          
            
            
            
            TGraph *grmin = new TGraph(Data_NBins,x_axis,unc_down);
            TGraph *grmax = new TGraph(Data_NBins,x_axis,unc_up);
            TGraph *gr    = new TGraph(Data_NBins,x_axis,ratio_y_data);
            TGraph *grshade = new TGraph(2*Data_NBins);
            
            
            
            for (i=0;i<Data_NBins;i++) {
                grshade->SetPoint(i,x_axis[i],unc_up[i]);
                grshade->SetPoint(Data_NBins+i-1,x_axis[Data_NBins-i-1],unc_down[Data_NBins-i-1]);
            }
            
            frame->GetYaxis()->SetTitle("MC/Data");
            frame->GetXaxis()->SetTitle("Jet p_{T} (GeV)");
            grshade->SetFillStyle(3013);
            grshade->SetFillColor(16);
            grshade->Draw("f");
            grmin->SetLineColor(kBlack);
            grmin->Draw("l");
            grmax->SetLineColor(kBlue);
            grmax->Draw("l");
            gr->SetLineWidth(2);
            gr->SetMarkerColor(kRed);
            gr->SetMarkerStyle(21);
            gr->Draw("P");
            
           
            TLegend *leg = new TLegend(0.1435726,0.7261538,0.245409,0.8882051,NULL,"brNDC");
            leg->SetBorderSize(0);
            leg->SetTextFont(42);
            leg->SetTextSize(0.03);
//            leg->AddEntry(h_Ratio_PYT_,"MC/Data","lp");
            leg->AddEntry(gr,"MC:Pythia8/ Data:2016","lp");
            leg->AddEntry(grshade,"JEC+Lumi+Detector Effects Unc.","f");
//            leg->AddEntry(h_Ratio_PYT_down_,"Total Unc. Down","l");
            leg->AddEntry("","AK4chs Jets","");
            leg->AddEntry("",eta_name,"");
            leg->Draw();
//
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
//
//            c1->cd();          // Go back to the main canvas before defining pad2
          
//
            c1->SaveAs("pngs/unc_pt_ratio_" + hname + ".png"); //saving...
            
           
        }
    }
}

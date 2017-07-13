#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>

void smp_plotter_pt(){
    
    TString observable[] = {
        "hpt",
        "hpt1",
        "hpt2"
    };
    
    TString axis_name[] = {
        "Raw p_{T} Spectrum (GeV)",
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
    
    //int bin_holder[10000];
    
    Double_t w = 1200;
    Double_t h = 900;
    Double_t norm = 1;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    
    TH1F *h_MC_;
    
    TH1F *h_part1_DATA_;
    TH1F *h_part2_DATA_;
    
    TH1F *h_Ratio;
    TH1F *h_Ratio_2;
    
    inputFile1.Form("output-MC-2b.root");
    inputFile2.Form("output-RunBtoFearly-2b.root");
    inputFile3.Form("output-RunFlatetoH-2b.root");
    
    
    int counter = (sizeof(observable)/sizeof(*observable));
    int bin_counter = (sizeof(eta_bins)/sizeof(*eta_bins));
    
    
    for (int j=0; j<bin_counter-1; j++) { 
        
        for (int i=0; i<counter; i++) { 
        
            TString hname = (TString) (observable[i]+"_"+eta_bins[j] + "-" + eta_bins[j+1]); //Kisaltmalari ogren ve ona gore string array olustur...
            TString eta_name = (TString) (eta_bins[j] + " < #eta < " + eta_bins[j+1]);
            
            //MC
            TFile* MCFile = TFile::Open(inputFile1.Data(), "READ");
            MCFile -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_MC_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
    
            //DATA
            TFile* DATAFile = TFile::Open(inputFile2.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_part1_DATA_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
            
            TFile* DATAFile2 = TFile::Open(inputFile3.Data(), "READ");
            DATAFile2 -> cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j+1]);
            h_part2_DATA_ = (TH1F*)gDirectory->FindObjectAny(observable[i]);
            
//            cout << "asdqwdwdqwdqwd" << endl;
            
            double xmin = 1000000, xmax = 0;
            double ymin = 0.;
            double ymax = 0.;

            int ncells = h_part1_DATA_->GetSize();  // it adds underflow and overflow bins so I have to extract those
            int Data_NBins = ncells-2;
            
            for (int k=1; k<=Data_NBins; k++) {
                
                double check_bins = h_part1_DATA_->GetBinContent(k);
                
                if (check_bins!=0) {
                    
                    double Xaxis_data = h_part1_DATA_->GetBinCenter(k);
                    
                    if (k < xmin) {xmin = k; ymin = Xaxis_data;}
                    if (k > xmax) {xmax = k; ymax = Xaxis_data;}
                    
                }
            }
            
            // Basic Scaling ////
            
            double Int_data = (h_part1_DATA_->Integral());
            //double Int_data_2 = (h_part2_DATA_->Integral());
            double Int_MC   = (h_MC_->Integral(xmin,xmax)); //Numbers are coming from the loop above...They corresponds to first and last bin of data///
            double Int_data_2 = (h_part2_DATA_->Integral(xmin,xmax));
            
            double Scale_Factor_1 = 0;
            double Scale_Factor_2 = 0;
            Scale_Factor_1 = Int_data/Int_MC;
            h_MC_->Scale(Scale_Factor_1);
            Scale_Factor_2 = Int_data / Int_data_2;
            h_part2_DATA_ -> Scale(Scale_Factor_2);
            //End of scaling //
            
            //Draw, cosmetics, etc.
    
            
    
            gStyle->SetOptStat(0);
            gStyle->SetOptTitle(0);
    
            c1->cd();
    
            TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
            pad1->SetBottomMargin(0.005); // Upper and lower plot are joined
            pad1->Draw();             // Draw the upper pad: pad1
            pad1-> SetLogy();              
            pad1-> SetLogx();
            pad1->cd();
    
            h_Ratio = (TH1F*)h_MC_->Clone("h_Ratio");
            h_Ratio_2 = (TH1F*)h_MC_->Clone("h_Ratio_2");

            h_MC_->GetXaxis()->SetRangeUser(ymin,2500);
            h_MC_->GetXaxis()->SetLabelOffset(999);
            //h_MC_->GetXaxis()->SetLabelSize(0);
            h_MC_->GetYaxis()->SetNdivisions(505);
            h_MC_->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_MC_->GetXaxis()->SetLabelSize(20);
            h_MC_->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_MC_->GetYaxis()->SetLabelSize(20);
            h_MC_->SetMarkerColor(kBlack);
            h_MC_->SetLineColor(kBlack);
            h_MC_->SetLineWidth(1);
            h_MC_->SetYTitle("A.U.");
            h_MC_->GetYaxis()->SetTitleOffset(0.8);
            h_MC_->GetYaxis()->SetTitleSize(0.05);
            h_MC_->GetYaxis()->SetTitleFont(42);


            
            h_part1_DATA_->SetMarkerColor(kBlue);
            h_part1_DATA_->SetMarkerStyle(20);
            h_part1_DATA_->SetLineColor(kBlue);
            h_part1_DATA_->SetLineWidth(1);
            
            h_part2_DATA_->SetMarkerColor(kRed);
            h_part2_DATA_->SetMarkerStyle(20);
            h_part2_DATA_->SetLineColor(kRed);
            h_part2_DATA_->SetLineWidth(1);
            
            h_MC_->Draw("E1HIST");
            h_part1_DATA_->Draw("E1HIST SAME");
            h_part2_DATA_->Draw("E1HIST SAME");
            
    
            TLegend *leg = new TLegend(0.6060601,0.6689862,0.7079967,0.8901843,NULL,"brNDC");
            leg->SetBorderSize(0);
            leg->SetTextFont(42);
            leg->SetTextSize(0.045);
            leg->AddEntry(h_MC_,"MC","l");
            leg->AddEntry(h_part1_DATA_,"RunBCDEF_{Early}, 19.49 fb^{-1}","lep");
            leg->AddEntry(h_part2_DATA_,"RunF_{Late}GH,  16.5 fb^{-1}","lep");
            leg->AddEntry("","AK4chs Jets","");
            leg->AddEntry("",eta_name,"");
            leg->Draw();
            
            /*TLegend *leg2 = new TLegend(0.1519199,0.1362126,0.3105175,0.3115541,NULL,"brNDC");
            leg2->SetBorderSize(0);
            leg2->SetTextFont(42);
            leg2->SetTextSize(0.045);
            leg2->AddEntry("","AK4chs Jets","");
            leg2->AddEntry("",eta_name,"");
            leg2->AddEntry("","RunBCDEFG","");
            leg2->Draw();*/
            
            
    
            TLatex *t6 = new TLatex();
            t6->SetNDC();
            t6->SetTextFont(42);
    	    t6->SetTextSize(0.05);
    	    t6->SetTextAlign(11);
            t6->DrawLatex(0.1,0.92,"CMS Internal, #sqrt{s}=13 TeV");
            
            /*TLatex *t7 = new TLatex();
            t7->SetNDC();
            t7->SetTextFont(42);
    	    t7->SetTextSize(0.05);
    	    t7->SetTextAlign(11);
            t7->DrawLatex(0.685,0.92,"#sqrt{s}=13 TeV");*/
           
            
            c1->cd();          // Go back to the main canvas before defining pad2
   	
            TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
            pad2->SetTopMargin(0.005);
            pad2->SetBottomMargin(0.28);
            pad2->SetGridy();
            pad2->Draw();
            pad2->cd();       // pad2 becomes the current pad
            gPad-> SetLogx();
    
            h_Ratio_2->Divide(h_part2_DATA_);

            h_Ratio->Divide(h_part1_DATA_);
            
            
            //cosmetics
            h_Ratio->GetXaxis()->SetRangeUser(ymin,2500);
            h_Ratio->SetXTitle(axis_name[i]);
            h_Ratio->SetYTitle("MC/Data");
            h_Ratio->GetYaxis()->SetTitleOffset(0.8);
            h_Ratio->GetYaxis()->CenterTitle();
            h_Ratio->GetYaxis()->SetNdivisions(511);
            //h_Ratio->SetMarkerStyle(8);
            //h_Ratio->SetMarkerSize(1.0);
            h_Ratio->SetMarkerColor(kBlue);
            h_Ratio->SetLineColor(kBlue);
            h_Ratio->SetLineWidth(1);
            //h_Ratio->GetXaxis()->SetRangeUser(0.,1.);
            h_Ratio->GetYaxis()->SetRangeUser(0.5,1.5);
            
            h_Ratio->GetYaxis()->SetNdivisions(505);
            h_Ratio->GetYaxis()->SetTitleSize(0.11);
            h_Ratio->GetYaxis()->SetTitleFont(42);
            h_Ratio->GetYaxis()->SetTitleOffset(0.35);
            h_Ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_Ratio->GetYaxis()->SetLabelSize(18);
	
            h_Ratio->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    	    h_Ratio->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
            h_Ratio->GetXaxis()->SetTitleSize(0.11);
            h_Ratio->GetXaxis()->SetTitleFont(42);
            h_Ratio->GetXaxis()->SetTitleOffset(1.1);
            h_Ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_Ratio->GetXaxis()->SetLabelSize(18);
           
            h_Ratio_2->SetMarkerColor(kRed);
            h_Ratio_2->SetLineColor(kRed);
            h_Ratio_2->SetLineWidth(1);

            
            
            
            
            h_Ratio->Draw("E1HIST");
            h_Ratio_2->Draw("E1HIST SAME");
    
            c1->SaveAs(hname + ".png"); //saving...
        }
    }
}

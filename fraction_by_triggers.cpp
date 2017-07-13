#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>


//degisim yapildi 

void fraction_by_triggers(){
    
    TString jt[] = {
        "40",
        "80",
        "140",
        "200",
        "260",
        "320",
        "400",
        "450"
    };
    
    TString eta[] = {
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
    
    
    int jt_counter = (sizeof(jt)/sizeof(*jt));
    int eta_counter = (sizeof(eta)/sizeof(*eta));
    
    cout << jt_counter << eta_counter << endl;
    
    Double_t w = 1200;
    Double_t h = 900;
    
    TH1F* h_Data_;
    TH1F* h_MC_;
    THStack *hs = new THStack("hs","");

    
    TH1F* h_Ratio;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    
    TFile * MC_file = new TFile("output-MC-2a.root");
    TFile * Data_file = new TFile("output-DATA-2a.root");
//    TFile * Data_file = new TFile("output-DATA_part1-2a.root");

    
    
    for (int i=0; i< eta_counter -1; i++) { //eta_counter -1
        for (int j=0; j<=jt_counter-1; j++) { //jt_counter-1
            
            TString hname = (TString) ("Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j]+ "/hchf");
            TString eta_name = (TString) (eta[i] + " < #eta < " + eta[i+1]);
            
            h_Data_ = (TH1F*) Data_file->Get("Standard/" + hname);
            h_MC_ = (TH1F*) MC_file->Get("Standard/" + hname);
            
            h_Data_->Scale(1/h_Data_->Integral());
            h_MC_->Scale(1/h_MC_->Integral());
            
//            int binmax = h_Data_->GetMaximumBin();
//            double x = h_Data_->GetXaxis()->GetBinCenter(binmax);
            
//            cout <<endl << x << endl;
            
            h_Ratio = (TH1F*)h_MC_->Clone("h_Ratio");
            
            //Cosmetics
            
            gStyle->SetOptStat(0);
            gStyle->SetOptTitle(0);
            
            c1->cd();
            
            TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
            pad1->SetBottomMargin(0.012);
            pad1->Draw();
            pad1-> SetLogy() ;
//            pad1-> SetLogx();
            pad1->cd();
            
            h_Data_ -> SetLineColor(kRed);
            h_Data_ -> SetLineWidth(1);
            h_Data_ -> SetMarkerColor(kRed);
            h_Data_ -> SetMarkerStyle(20);
            
            h_MC_->GetXaxis()->SetLabelOffset(999);
            h_MC_->GetXaxis()->SetLabelSize(0);
            h_MC_->GetYaxis()->SetNdivisions(505);
            h_MC_->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_MC_->GetXaxis()->SetLabelSize(20);
            h_MC_->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_MC_->GetYaxis()->SetLabelSize(20);
            h_MC_->SetLineColor(kBlack);
            h_MC_->SetLineWidth(1);
            h_MC_->SetYTitle("A.U.");
            h_MC_->GetYaxis()->SetTitleOffset(0.8);
            h_MC_->GetYaxis()->SetTitleSize(0.05);
            h_MC_->GetYaxis()->SetTitleFont(42);
            
            h_MC_->Draw("e1hist");
            h_Data_->Draw("e1histsame");
            
            TLegend *leg = new TLegend(0.3060601,0.1089862,0.4079967,0.3401843,NULL,"brNDC");
            leg->SetBorderSize(0);
            leg->SetTextFont(42);
            leg->SetTextSize(0.045);
            leg->AddEntry(h_MC_,"MC, HLT_PFJet" + jt[j],"l");
//            leg->AddEntry(h_DATA_part1_,"RunBCDEFearly, 19.49 fb^{-1}","lep");
            leg->AddEntry(h_Data_,"RunF_{Late}GH, HLT_PFJet" + jt[j],"lep");
//            leg->AddEntry(h_Data_,"RunBCDEF_{Early}, HLT_PFJet" + jt[j],"lep");
//            leg->AddEntry(""," 16.5 fb^{-1}","");
            leg->AddEntry("","AK4chs Jets","");
            leg->AddEntry("",eta_name,"");
            leg->Draw();
            
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
            pad2->cd();
            
            h_Ratio->Divide(h_Data_);
            
            h_Ratio->SetLineWidth(1);
            h_Ratio->SetXTitle("CH Fraction");
            h_Ratio->GetYaxis()->SetTitleOffset(0.8);
            h_Ratio->SetYTitle("MC/Data");
            h_Ratio->GetYaxis()->CenterTitle();
            
            h_Ratio->SetMarkerColor(kRed);
            h_Ratio->SetLineColor(kRed);
            
            h_Ratio->GetYaxis()->SetNdivisions(505);
            h_Ratio->GetXaxis()->SetNdivisions(511);
            h_Ratio->GetYaxis()->SetTitleSize(0.11);
            h_Ratio->GetYaxis()->SetTitleFont(42);
            h_Ratio->GetYaxis()->SetTitleOffset(0.35);
            h_Ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_Ratio->GetYaxis()->SetLabelSize(15);
            
            h_Ratio->GetYaxis()->SetRangeUser(0.3,1.7);
//            h_Ratio->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
//            h_Ratio->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
            h_Ratio->GetXaxis()->SetTitleSize(0.11);
            h_Ratio->GetXaxis()->SetTitleFont(42);
            h_Ratio->GetXaxis()->SetTitleOffset(1.1);
            h_Ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            h_Ratio->GetXaxis()->SetLabelSize(15);
            
            
            h_Ratio->Draw("E1HIST");
            c1->SaveAs(eta[i] + "-" + eta[i+1] + "_"+ jt[j]+ "_hchf" + ".png"); //saving...
            
//           c2->cd();
//            h_Data_ -> SetLineColor(j+1);
//            hs->Add(h_Data_);
//            
////            hs->Add(h_MC_);
////                        hs->SetFillStyle(1001);
//            hs->Draw();
////            c2->SaveAs(eta[i] + "-" + eta[i+1] + "_"+ jt[j]+ "_hchf_stack" + ".png"); //saving...
////            hs->Update();
            
            
            
        }
//        c2->SaveAs(eta[i] + "-" + eta[i+1] + "__hchf_stack" + ".png"); //saving...
    }
}

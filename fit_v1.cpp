#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>

void fit_v1(){
    
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
        "2.0"
    };
    
    int counter = (sizeof(jt)/sizeof(*jt));
    int bin_counter = (sizeof(eta)/sizeof(*eta));
    
    double turn_on[bin_counter][counter];
    
    Double_t w = 1200;
    Double_t h = 1000;
    
    TF1* f1[9];
    
    TH1D* h_mass[9];
    TH1D* h_mass_ref[9];
    
    TH1D* h_trig_uncut[9];
    TH1D* h_trig_cut[9];
    
    TH1D* h_pythia[9];
    TH1D* h_madgraph[9];
    TH1D* h_herwig[9];
    
    TH1D *h_Ratio_pythia[9];
    TH1D *h_Ratio_madgraph[9];
    TH1D *h_Ratio_herwig[9];
    

    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    TCanvas * c3 = new TCanvas("c3", "c3", w, h);
    
    TFile * file = new TFile("output-DATA-2a.root");
    TFile * MC_file1 = new TFile("output-MC-2a.root");
    TFile * MC_file2 = new TFile("output-MDG-2a.root");
    TFile * MC_file3 = new TFile("output-HW-2a.root");
    
    for (int i=0; i<bin_counter-1; i++) { //bin_counter-1
        for (int j=0; j<counter-1; j++) { //counter-1
            
//    for (int i=0; i<1; i++) { //bin_counter-1
//        for (int j=0; j<1; j++) { //counter-1
        
            h_mass[j] = (TH1D*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            h_mass_ref[j] = (TH1D*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j]+ "/hdjmass");
            	
            //Fit function
            double Fit_par=1.75;
            
            //"Fit_par" the fit parameter optimisation
            if (i == 2) Fit_par = 2.0;
            if ((i == 2) && j == 2) Fit_par = 2.5;
            if ((i ==2) && (j > 2 )) Fit_par = 2.8;
            if (i == 3) Fit_par = 3.0;
            if ((i == 3) && (j == 1)) Fit_par = 3.4;
            if ((i == 3) && (j == 2)) Fit_par = 4;
            if ((i == 3) && (j == 3)) Fit_par = 4.1;
            if ((i == 3) && (j == 4)) Fit_par = 4.1;
            if ((i == 3) && (j == 5)) Fit_par = 4.3;
            if ((i == 3) && (j == 6)) Fit_par = 4.5;
            
            c1->cd();
            
            h_mass[j] -> Divide(h_mass_ref[j]);
            h_mass[j] -> GetYaxis()->SetRangeUser(0,1.3);
            h_mass[j] -> GetXaxis()->SetRangeUser(0,3500);
            h_mass[j] -> GetXaxis()->SetTitle("M_{ij} (GeV)");
            h_mass[j] -> Draw();
            
            f1[j]= new TF1("","0.5*(1+TMath::Erf((x-[0])/[1]))",atoi(jt[j+1])*Fit_par,3500);
            f1[j]->SetParameters(2*atoi(jt[j+1])*cosh(0.5),atoi(jt[j+1])*2*0.1);
            h_mass[j]->Fit(f1[j],"R");
            
            turn_on[i][j] = f1[j]->GetX(0.99);
            
            //Lines from point to each axis
            TLine *line_h = new TLine(0.,1.,2700.,1.);
            line_h->SetLineColor(kBlack);
            line_h->SetLineStyle(2);
           	line_h->SetLineWidth(1);
            line_h->Draw();
            
            TLine *line_v = new TLine(f1[j]->GetX(0.99), 0., f1[j]->GetX(0.99), 1.);
            line_v->SetLineColor(kBlack);
            line_v->SetLineStyle(2);
            line_v->SetLineWidth(1);
            line_v->Draw();
            
            //Latex
            TLatex l;
            l.SetTextSize(0.025);
            l.DrawLatex(1320.114,0.296371,Form("TurnOn Point = %f",f1[j]->GetX(0.99)));
            l.DrawLatex(1320.114,0.371371,Form("%s, %s"+jt[j+1],"RunBCDEFGH", "jt"));
            l.DrawLatex(1320.114,0.441533,Form(eta[i] + "%s" + eta[i+1]," < #eta < "));
            
            c1->SaveAs("part1_TurnOn" + eta[i] + "-" + eta[i+1] + "-" + jt[j+1] + ".png");
            
            h_mass[j]->Delete();
	      	h_mass_ref[j]->Delete();
        
//            Cutting out the left side of Turn-on Points and MERGING!!! //////
            
            h_trig_uncut[i] = (TH1D*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[0]+ "/hdjmass"); // uncut jet40 histo
            h_trig_cut[j] = (TH1D*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            
            h_pythia[i] = (TH1D*) MC_file1->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/mc"+"/hdjmass");
            h_madgraph[i] = (TH1D*) MC_file2->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/mc"+"/hdjmass");
            h_herwig[i] = (TH1D*) MC_file3->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/mc"+"/hdjmass");
            
            
            int ncells = h_trig_cut[j]->GetSize();
            int Data_NBins = ncells-2;
            
            double xmin = 1000000, xmax = 0;
            double ymin = 0.;
            double ymax = 0.;
            
            for (int k=1; k<=Data_NBins; k++) {
                
                double Xaxis_val = h_trig_cut[j]->GetBinCenter(k);
                double Xaxis_val2 = h_pythia[i]->GetBinCenter(k);
                double Xaxis_val3 = h_madgraph[i]->GetBinCenter(k);
                
                double check_bins = h_trig_uncut[i]->GetBinContent(k);
                
                if (check_bins!=0) {
                    
                    double Xaxis_data = h_trig_uncut[i]->GetBinCenter(k);
                    
                    if (k < xmin) {xmin = k; ymin = Xaxis_data;}
                    if (k > xmax) {xmax = k; ymax = Xaxis_data;}
                }
                
                int binmax = h_trig_uncut[i]->GetMaximumBin();
                double x = h_trig_uncut[i]->GetXaxis()->GetBinCenter(binmax);
                
                if (Xaxis_val < x){
                    h_pythia[i]->SetBinContent(k,0);
                    h_pythia[i]->SetBinError(k,0);
                    h_madgraph[i]->SetBinContent(k,0);
                    h_madgraph[i]->SetBinError(k,0);
                    h_herwig[i]->SetBinContent(k,0);
                    h_herwig[i]->SetBinError(k,0);
                }
                if (Xaxis_val < turn_on[i][j] ){
                    h_trig_cut[j]->SetBinContent(k,0);
                    h_trig_cut[j]->SetBinError(k,0);
                }
                if (Xaxis_val > turn_on[i][j] || Xaxis_val < x){
                    h_trig_uncut[i]->SetBinContent(k,0);
                    h_trig_uncut[i]->SetBinError(k,0);
                }
            }//k
            
            h_trig_uncut[i]->Add(h_trig_cut[j]);

            gStyle->SetOptStat(0);
            gStyle->SetOptTitle(0);
            
            if (j==6){
                
                TString eta_name = (TString) (eta[i] + " < #eta < " + eta[i+1]);
                
                double Scale1 = h_trig_uncut[i]->Integral()/h_pythia[i]->Integral();
                h_pythia[i]->Scale(Scale1);
                
                double Scale2 = h_trig_uncut[i]->Integral()/h_madgraph[i]->Integral();
                h_madgraph[i]->Scale(Scale2);
                
                double Scale3 = h_trig_uncut[i]->Integral()/h_herwig[i]->Integral();
                h_herwig[i]->Scale(Scale3);
                
                h_Ratio_pythia[i] = (TH1D*)h_pythia[i]->Clone("h_Ratio_pythia");
                h_Ratio_madgraph[i] = (TH1D*)h_madgraph[i]->Clone("h_Ratio_madgraph");
                h_Ratio_herwig[i] = (TH1D*)h_herwig[i]->Clone("h_Ratio_herwig");
            
                c3->cd();
                
                TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
                pad1->SetBottomMargin(0.010); // Upper and lower plot are joined
                pad1->Draw();             // Draw the upper pad: pad1
                pad1-> SetLogy();
                pad1-> SetLogx();
                pad1->cd();
                
                h_pythia[i]->GetXaxis()->SetRangeUser(ymin,6500);
                h_pythia[i]->GetXaxis()->SetLabelOffset(999);
                h_pythia[i]->GetYaxis()->SetNdivisions(505);
                h_pythia[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                h_pythia[i]->GetXaxis()->SetLabelSize(20);
                h_pythia[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                h_pythia[i]->GetYaxis()->SetLabelSize(20);
                h_pythia[i]->SetMarkerColor(kRed+1);
                h_pythia[i]->SetLineColor(kRed+1);
                h_pythia[i]->SetLineWidth(2);
                h_pythia[i]->SetYTitle("A.U.");
                h_pythia[i]->GetYaxis()->SetTitleOffset(0.8);
                h_pythia[i]->GetYaxis()->SetTitleSize(0.05);
                h_pythia[i]->GetYaxis()->SetTitleFont(42);
                
                h_trig_uncut[i]->SetMarkerColor(kBlack);
                h_trig_uncut[i]->SetMarkerStyle(20);
                h_trig_uncut[i]->SetLineColor(kBlack);
                h_trig_uncut[i]->SetLineWidth(2);
                
                h_madgraph[i]->SetMarkerColor(kBlue+1);
                h_madgraph[i]->SetLineColor(kBlue+1);
                h_madgraph[i]->SetLineWidth(2);
                
                h_herwig[i]->SetMarkerColor(kGreen+1);
                h_herwig[i]->SetLineColor(kGreen+1);
                h_herwig[i]->SetLineWidth(2);
                
                h_pythia[i]->Draw("HIST");
                h_madgraph[i]->Draw("E1HISTSAME");
                h_herwig[i]->Draw("HISTSAME");
                h_trig_uncut[i]->Draw("SAME");
                
                TLegend *leg = new TLegend(0.7145242,0.6249084,0.8163606,0.8886447,NULL,"brNDC");
                leg->SetBorderSize(0);
                leg->SetTextFont(42);
                leg->SetTextSize(0.045);
                leg->AddEntry(h_pythia[i],"Pythia","l");
                leg->AddEntry(h_madgraph[i],"MadGraph","l");
                leg->AddEntry(h_herwig[i],"Herwig","l");
                leg->AddEntry(h_trig_uncut[i],"RunBCDEFGH","lp");
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
                
                c3->cd();          // Go back to the main canvas before defining pad2
                
                TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
                pad2->SetTopMargin(0.005);
                pad2->SetBottomMargin(0.28);
                pad2->SetGridy();
                pad2->Draw();
                pad2->cd();       // pad2 becomes the current pad
                gPad-> SetLogx();
                
                h_Ratio_pythia[i]->Divide(h_trig_uncut[i]);
                h_Ratio_madgraph[i]->Divide(h_trig_uncut[i]);
                h_Ratio_herwig[i]->Divide(h_trig_uncut[i]);
                
                h_Ratio_pythia[i]->GetXaxis()->SetRangeUser(ymin,6500);
                h_Ratio_pythia[i]->SetXTitle("M_{ij} (GeV)");
                h_Ratio_pythia[i]->SetYTitle("MC/Data");
                h_Ratio_pythia[i]->GetYaxis()->SetTitleOffset(0.8);
                h_Ratio_pythia[i]->GetYaxis()->CenterTitle();
                h_Ratio_pythia[i]->GetYaxis()->SetNdivisions(511);
                h_Ratio_pythia[i]->SetMarkerColor(kRed+1);
                h_Ratio_pythia[i]->SetLineColor(kRed+1);
                h_Ratio_pythia[i]->SetLineWidth(2);
                h_Ratio_pythia[i]->GetYaxis()->SetRangeUser(0.5,1.9);
                h_Ratio_pythia[i]->GetYaxis()->SetNdivisions(606);
                h_Ratio_pythia[i]->GetYaxis()->SetTitleSize(0.11);
                h_Ratio_pythia[i]->GetYaxis()->SetTitleFont(42);
                h_Ratio_pythia[i]->GetYaxis()->SetTitleOffset(0.35);
                h_Ratio_pythia[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                h_Ratio_pythia[i]->GetYaxis()->SetLabelSize(18);
                h_Ratio_pythia[i]->GetXaxis()->SetMoreLogLabels();
                h_Ratio_pythia[i]->GetXaxis()->SetNoExponent();
                h_Ratio_pythia[i]->GetXaxis()->SetTitleSize(0.11);
                h_Ratio_pythia[i]->GetXaxis()->SetTitleFont(42);
                h_Ratio_pythia[i]->GetXaxis()->SetTitleOffset(1.1);
                h_Ratio_pythia[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                h_Ratio_pythia[i]->GetXaxis()->SetLabelSize(18);
                
                h_Ratio_madgraph[i]->SetMarkerColor(kBlue+1);
                h_Ratio_madgraph[i]->SetLineColor(kBlue+1);
                h_Ratio_madgraph[i]->SetLineWidth(2);
                
                h_Ratio_herwig[i]->SetMarkerColor(kGreen+1);
                h_Ratio_herwig[i]->SetLineColor(kGreen+1);
                h_Ratio_herwig[i]->SetLineWidth(2);
                
                h_Ratio_pythia[i]->Draw("E1HIST");
                h_Ratio_madgraph[i]->Draw("E1HISTSAME");
                h_Ratio_herwig[i]->Draw("E1HISTSAME");
                
                c3->SaveAs("Merged"+ eta[i] + "-" + eta[i+1]+ "-" +jt[j+1]+".png");
            } //j==6
            h_trig_cut[j]->Delete();
        }//j
    }//i
}//main

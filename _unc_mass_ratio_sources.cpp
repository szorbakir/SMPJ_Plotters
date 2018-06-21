#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>

void _unc_mass_ratio_sources(){
    
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
        "2.5"
    };
    
    int counter = (sizeof(jt)/sizeof(*jt));
    int bin_counter = (sizeof(eta)/sizeof(*eta));
    
    double turn_on[bin_counter][counter];
    double turn_on_up[bin_counter][counter];
    double turn_on_down[bin_counter][counter];
    
    Double_t w = 1200;
    Double_t h = 1000;
    
    TF1* f1[9];
    TF1* f1_up[9];
    TF1* f1_down[9];
    
    TH1D* h_mass[9];
    TH1D* h_mass_ref[9];
    
    TH1D* h_mass_up[9];
    TH1D* h_mass_ref_up[9];
    
    TH1D* h_mass_down[9];
    TH1D* h_mass_ref_down[9];
    
    TH1D* h_trig_uncut[9];
    TH1D* h_trig_cut[9];
    
    TH1D* h_trig_uncut_up[9];
    TH1D* h_trig_cut_up[9];

    TH1D* h_trig_uncut_down[9];
    TH1D* h_trig_cut_down[9];
    
    TH1D* h_pythia[9];
    TH1D* h_pythia_up[9];
    TH1D* h_pythia_down[9];

    TH1D *h_Ratio_pythia[9];
    TH1D *h_Ratio_pythia_up[9];
    TH1D *h_Ratio_pythia_down[9];
    
    TH1D *h_Ratio_pythia_up_lumi[9];
    TH1D *h_Ratio_pythia_down_lumi[9];
    
    TH1D *h_Ratio_pythia_up_det[9];
    TH1D *h_Ratio_pythia_down_det[9];
    
    TH1D *h_unc_det[9];
    TH1D *h_unc_lumi[9];
    
    TH1D *h_DATA_up_lumi[9];
    TH1D *h_DATA_down_lumi[9];
    
    TH1D *h_DATA_up_det[9];
    TH1D *h_DATA_down_det[9];
    

    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    TCanvas * c3 = new TCanvas("c3", "c3", w, h);
    TCanvas * c4 = new TCanvas("c4", "c4", w, h);
    TCanvas * c5 = new TCanvas("c5", "c5", w, h);
    
    TFile * file = new TFile("datas/output-DATA-2a.root");
    TFile * MC_file1 = new TFile("datas/output-MC-2a.root");
    TFile * DATA_up = new TFile("datas/output-DATA-2a-up.root");
    TFile * DATA_down = new TFile("datas/output-DATA-2a-down.root");
    
    for (int i=0; i<bin_counter-1; i++) { //bin_counter-1
        for (int j=0; j<counter-1; j++) { //counter-1
        
            h_mass[j] = (TH1D*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            h_mass_ref[j] = (TH1D*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j]+ "/hdjmass");
            
            h_mass_up[j] = (TH1D*) DATA_up->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            h_mass_ref_up[j] = (TH1D*) DATA_up->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j]+ "/hdjmass");
            
            h_mass_down[j] = (TH1D*) DATA_down->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            h_mass_ref_down[j] = (TH1D*) DATA_down->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j]+ "/hdjmass");
            	
//            //Fit function
            double FitPar[i][j];
            
//            Mehmet'den gelen
            FitPar[i][j]=1.55;
            if ((i == 0) && j == 1) FitPar[i][j] = 1.7;     //jt140 0.0-0.5
            if ((i == 0) && j == 2) FitPar[i][j] = 1.7;     //jt200
            if ((i == 0) && j == 3) FitPar[i][j] = 1.7;     //jt260
            if ((i == 0) && j == 4) FitPar[i][j] = 1.85;    //jt320
            if ((i == 0) && j == 5) FitPar[i][j] = 1.85;    //jt400
            if ((i == 0) && j == 6) FitPar[i][j] = 1.85;    //jt450
            
            if (i == 1) FitPar[i][j] = 1.7;                 //      0.5-1.0
            if ((i == 1) && j > 2) FitPar[i][j] = 1.9;
            
            if (i == 2) FitPar[i][j] = 2.0;                 //      1.0-1.5
            if ((i == 2) && j == 2) FitPar[i][j] = 2.5;     //jt200
            if ((i ==2) && (j >= 3)) FitPar[i][j] = 2.8;
            
            if (i == 3) FitPar[i][j] = 3.0;                 //      1.5-2.0
            if ((i == 3) && (j == 1)) FitPar[i][j] = 3.4;   //jt140
            if ((i == 3) && (j == 2)) FitPar[i][j] = 3.8;   //jt200
            if ((i == 3) && (j == 3)) FitPar[i][j] = 4.3;   //jt260
            if ((i == 3) && (j == 4)) FitPar[i][j] = 4.4;   //jt320
            if ((i == 3) && (j == 5)) FitPar[i][j] = 4.3;   //jt400
            if ((i == 3) && (j == 6)) FitPar[i][j] = 4.3;   //jt450
            
            if (i == 4 ) FitPar[i][j] = 4.3;                //jt80 2.0-2.5
            if ((i == 4) && (j == 1) ) FitPar[i][j] = 5.9;  //jt140
            if ((i == 4) && (j == 2) ) FitPar[i][j] = 5.6; //jt200
            if ((i == 4) && (j == 3) ) FitPar[i][j] = 5.3;  //jt260
            if ((i == 4) && (j == 4) ) FitPar[i][j] = 6.2;  //jt320
            if ((i == 4) && (j ==5) ) FitPar[i][j] = 5.2;   //jt400
            if ((i == 4) && (j ==6) ) FitPar[i][j] = 5.5;   //jt450
//            if ((i == 0) && j == 1) FitPar[i][j] = 1.7;     //jt140 0.0-0.5
//            if ((i == 0) && j == 2) FitPar[i][j] = 1.7;     //jt200
//            if ((i == 0) && j == 3) FitPar[i][j] = 1.7;     //jt260
//            if ((i == 0) && j == 4) FitPar[i][j] = 1.85;    //jt320
//            if ((i == 0) && j == 5) FitPar[i][j] = 1.85;    //jt400
//            if ((i == 0) && j == 6) FitPar[i][j] = 1.85;    //jt450
//
//            if (i == 1) FitPar[i][j] = 1.7;                 //0.5-1.0
//            if ((i == 1) && j > 2) FitPar[i][j] = 1.9;
//
//            if (i == 2) FitPar[i][j] = 2.0;                 //1.0-1.5
//            if ((i == 2) && j == 2) FitPar[i][j] = 2.5;     //jt200
//            if ((i ==2) && (j >= 3)) FitPar[i][j] = 2.8;
//
//            if (i == 3) FitPar[i][j] = 3.0;                 //1.5-2.0
//            if ((i == 3) && (j == 1)) FitPar[i][j] = 3.4;   //jt140
//            if ((i == 3) && (j == 2)) FitPar[i][j] = 3.8;   //jt200
//            if ((i == 3) && (j == 3)) FitPar[i][j] = 4.1;   //jt260
//            if ((i == 3) && (j == 4)) FitPar[i][j] = 4.4;   //jt320
//            if ((i == 3) && (j == 5)) FitPar[i][j] = 4.3;   //jt400
//            if ((i == 3) && (j == 6)) FitPar[i][j] = 5.0;   //jt450
//
//            if (i == 4 ) FitPar[i][j] = 4.3;                //jt80 2.0-2.5
//            if ((i == 4) && (j == 1) ) FitPar[i][j] = 5.9;  //jt140
//            if ((i == 4) && (j == 2) ) FitPar[i][j] = 5.74; //jt200
//            if ((i == 4) && (j == 3) ) FitPar[i][j] = 7.3;  //jt260
//            if ((i == 4) && (j == 4) ) FitPar[i][j] = 5.9;  //jt320
//            if ((i == 4) && (j ==5) ) FitPar[i][j] = 5.21;   //jt400
//            if ((i == 4) && (j ==6) ) FitPar[i][j] = 5.5;   //jt450
            
            c1->cd();
            
            h_mass[j] -> Divide(h_mass_ref[j]);
            h_mass[j] -> GetYaxis()->SetRangeUser(0,1.3);
            h_mass[j] -> GetXaxis()->SetRangeUser(0,4000);
            h_mass[j] -> GetXaxis()->SetTitle("M_{ij} (GeV)");
//            h_mass[j] -> Draw();
            
            f1[j]= new TF1("","0.5*(1+TMath::Erf((x-[0])/[1]))",atoi(jt[j+1])*FitPar[i][j],8000);
            f1[j]->SetParameters(2*atoi(jt[j+1])*cosh(0.5),atoi(jt[j+1])*2*0.1);
            h_mass[j]->Fit(f1[j],"R");
            
            turn_on[i][j] = f1[j]->GetX(0.99);
            
            h_mass[j]->Delete();
            h_mass_ref[j]->Delete();
// -----------------------------------------------//
            
            c4->cd();
            
            h_mass_up[j] -> Divide(h_mass_ref_up[j]);
            h_mass_up[j] -> GetYaxis()->SetRangeUser(0,1.3);
            h_mass_up[j] -> GetXaxis()->SetRangeUser(0,4000);
            h_mass_up[j] -> GetXaxis()->SetTitle("M_{ij} (GeV)");
//            h_mass_up[j] -> Draw();
            
            f1_up[j]= new TF1("","0.5*(1+TMath::Erf((x-[0])/[1]))",atoi(jt[j+1])*FitPar[i][j],8000);
            f1_up[j]->SetParameters(2*atoi(jt[j+1])*cosh(0.5),atoi(jt[j+1])*2*0.1);
            h_mass_up[j]->Fit(f1_up[j],"R");
            
            turn_on_up[i][j] = f1_up[j]->GetX(0.99);
            
            h_mass_up[j]->Delete();
            h_mass_ref_up[j]->Delete();
            
//----------------------------------------------------------//
            
           c5->cd();
            
            h_mass_down[j] -> Divide(h_mass_ref_down[j]);
            h_mass_down[j] -> GetYaxis()->SetRangeUser(0,1.3);
            h_mass_down[j] -> GetXaxis()->SetRangeUser(0,4000);
            h_mass_down[j] -> GetXaxis()->SetTitle("M_{ij} (GeV)");
//            h_mass_down[j] -> Draw();
            
            f1_down[j]= new TF1("","0.5*(1+TMath::Erf((x-[0])/[1]))",atoi(jt[j+1])*FitPar[i][j],8000);
            f1_down[j]->SetParameters(2*atoi(jt[j+1])*cosh(0.5),atoi(jt[j+1])*2*0.1);
            h_mass_down[j]->Fit(f1_down[j],"R");
            
            turn_on_down[i][j] = f1_down[j]->GetX(0.99);
            
            h_mass_down[j]->Delete();
            h_mass_ref_down[j]->Delete();
        
//          Cutting out the left side of Turn-on Points and MERGING!!! //////

            h_trig_uncut[i] = (TH1D*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[0]+ "/hdjmass"); // uncut jet40 histo
            h_trig_cut[j] = (TH1D*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            
            h_trig_uncut_up[i] = (TH1D*) DATA_up->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[0]+ "/hdjmass"); // uncut jet40 histo
            h_trig_cut_up[j] = (TH1D*) DATA_up->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");

            
            h_trig_uncut_down[i] = (TH1D*) DATA_down->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[0]+ "/hdjmass"); // uncut jet40 histo
            h_trig_cut_down[j] = (TH1D*) DATA_down->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");


            h_pythia[i] = (TH1D*) MC_file1->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/mc"+"/hdjmass");
            h_pythia_up[i] = (TH1D*)h_pythia[i]->Clone("h_pythia_up");
            h_pythia_down[i] = (TH1D*)h_pythia[i]->Clone("h_pythia_down");
           
            
            int ncells = h_trig_cut[j]->GetSize();
            int Data_NBins = ncells-2;

            double xmin = 1000000, xmax = 0;
            double ymin = 0.;
            double ymax = 0.;

            for (int k=1; k<=Data_NBins; k++) {

                double Xaxis_val = h_trig_cut[j]->GetBinCenter(k);
                double Xaxis_val_up = h_trig_cut_up[j]->GetBinCenter(k);
                double Xaxis_val_down = h_trig_cut_down[j]->GetBinCenter(k);
                
    
                double Xaxis_val2 = h_pythia[i]->GetBinCenter(k);

                double check_bins = h_trig_uncut[i]->GetBinContent(k);

                if (check_bins!=0) {

                    double Xaxis_data = h_trig_uncut[i]->GetBinCenter(k);

                    if (k < xmin) {xmin = k; ymin = Xaxis_data;}
                    if (k > xmax) {xmax = k; ymax = Xaxis_data;}
                }

                int binmax = h_trig_uncut[i]->GetMaximumBin();
                double x = h_trig_uncut[i]->GetXaxis()->GetBinCenter(binmax);
                
                int binmax_up = h_trig_uncut_up[i]->GetMaximumBin();
                double x_up = h_trig_uncut_up[i]->GetXaxis()->GetBinCenter(binmax_up);
                
                int binmax_down = h_trig_uncut_down[i]->GetMaximumBin();
                double x_down = h_trig_uncut_down[i]->GetXaxis()->GetBinCenter(binmax_down);

                if (Xaxis_val < x){
                    h_pythia[i]->SetBinContent(k,0);
                    h_pythia[i]->SetBinError(k,0);
                }
                if (Xaxis_val < turn_on[i][j] ){
                    h_trig_cut[j]->SetBinContent(k,0);
                    h_trig_cut[j]->SetBinError(k,0);
                }
                if (Xaxis_val > turn_on[i][j] || Xaxis_val < x){
                    h_trig_uncut[i]->SetBinContent(k,0);
                    h_trig_uncut[i]->SetBinError(k,0);
                }
                
                if (Xaxis_val_up < turn_on_up[i][j] ){
                    h_trig_cut_up[j]->SetBinContent(k,0);
                    h_trig_cut_up[j]->SetBinError(k,0);
                }
                if (Xaxis_val_up > turn_on_up[i][j] || Xaxis_val_up < x_up){
                    h_trig_uncut_up[i]->SetBinContent(k,0);
                    h_trig_uncut_up[i]->SetBinError(k,0);
                }
                
                if (Xaxis_val_down < turn_on_down[i][j] ){
                    h_trig_cut_down[j]->SetBinContent(k,0);
                    h_trig_cut_down[j]->SetBinError(k,0);
                }
                if (Xaxis_val_down > turn_on_down[i][j] || Xaxis_val_down < x_down){
                    h_trig_uncut_down[i]->SetBinContent(k,0);
                    h_trig_uncut_down[i]->SetBinError(k,0);
                }
            }//k

            h_trig_uncut[i]->Add(h_trig_cut[j]);
            h_trig_uncut_up[i]->Add(h_trig_cut_up[j]);
            h_trig_uncut_down[i]->Add(h_trig_cut_down[j]);

            gStyle->SetOptStat(0);
            gStyle->SetOptTitle(0);

            if (j==6){

                TString eta_name = (TString) (eta[i] + " < |y| < " + eta[i+1]);

                double Scale = h_trig_uncut[i]->Integral()/h_pythia[i]->Integral();
                h_pythia[i]->Scale(Scale);
                
//                double Scale_up = h_trig_uncut_up[i]->Integral()/h_pythia_up[i]->Integral();
//                h_pythia_up[i]->Scale(Scale_up);
//
//                double Scale_down = h_trig_uncut_down[i]->Integral()/h_pythia_down[i]->Integral();
//                h_pythia_down[i]->Scale(Scale_down);

                h_Ratio_pythia[i] = (TH1D*)h_pythia[i]->Clone("h_Ratio_pythia");
                h_Ratio_pythia_up[i] = (TH1D*)h_pythia[i]->Clone("h_Ratio_pythia_up");
                h_Ratio_pythia_down[i] = (TH1D*)h_pythia[i]->Clone("h_Ratio_pythia_down");
                h_Ratio_pythia_up_lumi[i] = (TH1D*)h_pythia[i]->Clone("h_Ratio_pythia_up_lumi");
                h_Ratio_pythia_down_lumi[i] = (TH1D*)h_pythia[i]->Clone("h_Ratio_pythia_down_lumi");
                h_Ratio_pythia_up_det[i] = (TH1D*)h_pythia[i]->Clone("h_Ratio_pythia_up_det");
                h_Ratio_pythia_down_det[i] = (TH1D*)h_pythia[i]->Clone("h_Ratio_pythia_down_det");
                
                h_unc_lumi[i] = (TH1D*) h_trig_uncut[i]->Clone("h_unc_lumi");
                h_DATA_up_lumi[i] = (TH1D*) h_trig_uncut[i]->Clone("h_DATA_up_lumi");
                h_DATA_down_lumi[i] = (TH1D*) h_trig_uncut[i]->Clone("h_DATA_down_lumi");
                
                h_unc_lumi[i]->Scale(2.5/100.0);
                h_DATA_up_lumi[i]->Add(h_unc_lumi[i]);
                h_DATA_down_lumi[i]->Add(h_unc_lumi[i],-1);
                
                h_unc_det[i] = (TH1D*) h_trig_uncut[i]->Clone("h_unc_det");
                h_DATA_up_det[i] = (TH1D*) h_trig_uncut[i]->Clone("h_DATA_up_det");
                h_DATA_down_det[i] = (TH1D*) h_trig_uncut[i]->Clone("h_DATA_down_det");
                
                h_unc_det[i]->Scale(1./100.0);
                h_DATA_up_det[i]->Add(h_unc_det[i]);
                h_DATA_down_det[i]->Add(h_unc_det[i],-1);
                
                
                c3->cd();
                c3-> SetLogx();
                
                h_Ratio_pythia[i]->Divide(h_trig_uncut[i]);
                h_Ratio_pythia_up[i]->Divide(h_trig_uncut_up[i]);
                h_Ratio_pythia_down[i]->Divide(h_trig_uncut_down[i]);
                
                h_Ratio_pythia_up_lumi[i]->Divide(h_DATA_up_lumi[i]);
                h_Ratio_pythia_down_lumi[i]->Divide(h_DATA_down_lumi[i]);
                h_Ratio_pythia_up_det[i]->Divide(h_DATA_up_det[i]);
                h_Ratio_pythia_down_det[i]->Divide(h_DATA_down_det[i]);
                
                h_Ratio_pythia[i]->GetXaxis()->SetRangeUser(ymin,ymax);
                h_Ratio_pythia[i]->SetXTitle("M_{ij} (GeV)");
                h_Ratio_pythia[i]->SetYTitle("MC/Data");
                h_Ratio_pythia[i]->GetYaxis()->CenterTitle();
                h_Ratio_pythia[i]->SetMarkerColor(kBlack);
                h_Ratio_pythia[i]->SetMarkerStyle(20);
                h_Ratio_pythia[i]->SetLineColor(kBlack);
                h_Ratio_pythia[i]->SetLineWidth(2);
                h_Ratio_pythia[i]->GetYaxis()->SetRangeUser(0.6,1.3);
                h_Ratio_pythia[i]->GetYaxis()->SetTitleOffset(1.3);
                h_Ratio_pythia[i]->GetXaxis()->SetMoreLogLabels();
                h_Ratio_pythia[i]->GetXaxis()->SetNoExponent();
                h_Ratio_pythia[i]->GetXaxis()->SetTitleOffset(1.3);
                
                h_Ratio_pythia_up[i]->SetMarkerColor(kRed);
                h_Ratio_pythia_up[i]->SetLineColor(kRed);
//                h_Ratio_pythia_up[i]->SetLineStyle(2);
                h_Ratio_pythia_up[i]->SetLineWidth(2);
                
                h_Ratio_pythia_down[i]->SetMarkerColor(kRed);
                h_Ratio_pythia_down[i]->SetLineColor(kRed);
//                h_Ratio_pythia_down[i]->SetLineStyle(2);
                h_Ratio_pythia_down[i]->SetLineWidth(2);
                
                h_Ratio_pythia_up_lumi[i]->SetLineColor(kGreen+2);
                h_Ratio_pythia_up_lumi[i]->SetMarkerColor(kGreen+2);
                //            h_Ratio_pythia_down_lumi[i]->SetLineStyle(2);
                h_Ratio_pythia_up_lumi[i]->SetLineWidth(2);
                
                h_Ratio_pythia_down_lumi[i]->SetLineColor(kGreen+2);
                h_Ratio_pythia_down_lumi[i]->SetMarkerColor(kGreen+2);
                //            h_Ratio_pythia_down_lumi[i]->SetLineStyle(2);
                h_Ratio_pythia_down_lumi[i]->SetLineWidth(2);
                
                h_Ratio_pythia_down_det[i]->SetLineColor(kBlue);
                h_Ratio_pythia_down_det[i]->SetMarkerColor(kBlue);
                //            h_Ratio_pythia_down_det[i]->SetLineStyle(2);
                h_Ratio_pythia_down_det[i]->SetLineWidth(2);
                
                h_Ratio_pythia_up_det[i]->SetLineColor(kBlue);
                h_Ratio_pythia_up_det[i]->SetMarkerColor(kBlue);
                //            h_Ratio_pythia_up_det[i]->SetLineStyle(2);
                h_Ratio_pythia_up_det[i]->SetLineWidth(2);
                
                h_Ratio_pythia[i]->Draw("E1");
                h_Ratio_pythia_up[i]->Draw("SAME HIST AC F");
                h_Ratio_pythia_down[i]->Draw("SAME HIST AC");
                
                h_Ratio_pythia_down_lumi[i]->Draw("SAME HIST AC F");
                h_Ratio_pythia_up_lumi[i]->Draw("SAME HIST AC");
                
                h_Ratio_pythia_up_det[i]->Draw("SAME HIST AC F");
                h_Ratio_pythia_down_det[i]->Draw("SAME HIST AC");
                
                TLegend *leg = new TLegend(0.1435726,0.6923077,0.245409,0.8882051,NULL,"brNDC");
                leg->SetBorderSize(0);
                leg->SetTextFont(42);
                leg->SetTextSize(0.03);
//                leg->AddEntry(h_Ratio_pythia[i],"MC/Data","lp");
                leg->AddEntry(h_Ratio_pythia[i],"MC:Pythia8/Data:2016","lp");
                leg->AddEntry(h_Ratio_pythia_up[i],"JEC Uncertainty","lf");
                leg->AddEntry(h_Ratio_pythia_up_lumi[i],"Luminosity Uncertainty","lf");
                leg->AddEntry(h_Ratio_pythia_up_det[i],"Detector Effects Uncertainty","lf");
//                leg->AddEntry(h_Ratio_pythia_down[i],"Total Unc. Down","l");
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
                
                c3->SaveAs("pngs/unc_mass_ratio_"+ eta[i] + "-" + eta[i+1]+ ".png");
                

            } //j==6
            h_trig_cut[j]->Delete();
            h_trig_cut_up[j]->Delete();
            h_trig_cut_down[j]->Delete();
        }//j
    }//i
}//main


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
    double turn_on_part2[bin_counter][counter];
    
    Double_t w = 1200;
    Double_t h = 800;
    
    TF1* f1[9];
    
    TH1F* h_mass[9];
    TH1F* h_mass_ref[9];
    
    TH1F* h_trig_uncut[9];
    TH1F* h_trig_cut[9];
    
    TH1F* h_mc[9];
    
    TH1F *h_Ratio[9];
    
    TF1* f1_part2[9];
    
    TH1F* h_mass_part2[9];
    TH1F* h_mass_ref_part2[9];
    
    TH1F* h_trig_uncut_part2[9];
    TH1F* h_trig_cut_part2[9];

    TH1F *h_Ratio_part2[9];

    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    TCanvas * c3 = new TCanvas("c3", "c3", w, h);
    
    TFile * file = new TFile("output-DATA_part1-2a.root");
    TFile * file_part2 = new TFile("output-DATA_part2-2a.root");
    TFile * MC_file = new TFile("output-MC-2a.root");
    
    for (int i=0; i<bin_counter-1; i++) { //bin_counter-1
        
        for (int j=0; j<counter-1; j++) { //counter-1
        	
            
            
            h_mass[j] = (TH1F*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            h_mass_ref[j] = (TH1F*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j]+ "/hdjmass");
            	
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
            l.DrawLatex(1320.114,0.371371,Form("%s, %s"+jt[j+1],"RunBCDEF_{Early}", "jt"));
            l.DrawLatex(1320.114,0.441533,Form(eta[i] + "%s" + eta[i+1]," < #eta < "));
            
            c1->SaveAs("part1_TurnOn" + eta[i] + "-" + eta[i+1] + "-" + jt[j+1] + ".png");
            
            h_mass[j]->Delete();
	      	h_mass_ref[j]->Delete();
            
//            c1->Close();
            

            
            /// Cutting out the left side of Turn-on Points and MERGING!!! //////
            
            h_trig_uncut[i] = (TH1F*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[0]+ "/hdjmass"); // uncut jet40 histo
            h_trig_cut[j] = (TH1F*) file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            
            h_mc[i] = (TH1F*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/mc"+"/hdjmass");
            
            int ncells = h_trig_cut[j]->GetSize();  // it adds underflow and overflow bins so I have to extract those
            int Data_NBins = ncells-2;
            
            int ncellsMC = h_mc[i]->GetSize();  // it adds underflow and overflow bins so I have to extract those
            int MC_NBins = ncellsMC-2;
            
            //cout << "Mc Bins: " << MC_NBins << " DAta bins: " << Data_NBins << endl;
            double xmin = 1000000, xmax = 0;
            double ymin = 0.;
            double ymax = 0.;
            
            for (int k=1; k<=Data_NBins; k++) {
                
                double Xaxis_val = h_trig_cut[j]->GetBinCenter(k);
                double Xaxis_val2 = h_mc[i]->GetBinCenter(k);
                
                double check_bins = h_trig_uncut[i]->GetBinContent(k);
                
                if (check_bins!=0) {
                    
                    double Xaxis_data = h_trig_uncut[i]->GetBinCenter(k);
                    
                    if (k < xmin) {xmin = k; ymin = Xaxis_data;}
                    if (k > xmax) {xmax = k; ymax = Xaxis_data;}
                }
                
                int binmax = h_trig_uncut[i]->GetMaximumBin();
                double x = h_trig_uncut[i]->GetXaxis()->GetBinCenter(binmax);
                
                int binmax2 = h_mc[i]->GetMaximumBin();
                double y = h_mc[i]->GetXaxis()->GetBinCenter(binmax2);
                
                if (Xaxis_val < x){
                    h_mc[i]->SetBinContent(k,0);
                    h_mc[i]->SetBinError(k,0);
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

//            -----------------------------------------------------------------------------
            
            c2->cd();
            
            h_mass_part2[j] = (TH1F*) file_part2->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            h_mass_ref_part2[j] = (TH1F*) file_part2->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j]+ "/hdjmass");
            
            h_mass_part2[j] -> Divide(h_mass_ref_part2[j]);
            h_mass_part2[j] -> GetYaxis()->SetRangeUser(0,1.3);
            h_mass_part2[j] -> GetXaxis()->SetRangeUser(0,3500);
//            h_mass_part2[j] -> Draw();
            
            f1_part2[j]= new TF1("","0.5*(1+TMath::Erf((x-[0])/[1]))",atoi(jt[j+1])*Fit_par,3500);
            f1_part2[j]->SetParameters(2*atoi(jt[j+1])*cosh(0.5),atoi(jt[j+1])*2*0.1);
            h_mass_part2[j]->Fit(f1_part2[j],"R");
            
            turn_on_part2[i][j] = f1_part2[j]->GetX(0.99);
            cout << endl << turn_on_part2[i][j] << endl << endl;
            
            
            //Lines from point to each axis
            TLine *line_h_part2 = new TLine(0.,1.,2700.,1.);
            line_h_part2->SetLineColor(kBlack);
            line_h_part2->SetLineStyle(2);
           	line_h_part2->SetLineWidth(1);
            line_h_part2->Draw();
            
            

            
            TLine *line_v_part2 = new TLine(f1_part2[j]->GetX(0.99), 0., f1_part2[j]->GetX(0.99), 1.);
            line_v_part2->SetLineColor(kBlack);
            line_v_part2->SetLineStyle(2);
            line_v_part2->SetLineWidth(1);
            line_v_part2->Draw();
            
            
            //Latex
            TLatex l_part2;
            l_part2.SetTextSize(0.025);
            l_part2.DrawLatex(1320.114,0.296371,Form("TurnOn Point = %f",f1_part2[j]->GetX(0.99)));
            l_part2.DrawLatex(1320.114,0.371371,Form("%s, %s"+jt[j+1],"#RunF_{Late}GH", "jt"));
            l_part2.DrawLatex(1320.114,0.441533,Form(eta[i] + "%s" + eta[i+1]," < #||{y} < "));
            
            c2->SaveAs("part2_TurnOn" + eta[i] + "-" + eta[i+1] + "-" + jt[j+1] + ".png");
            
            h_mass_part2[j]->Delete();
            h_mass_ref_part2[j]->Delete();
            
//            c2->Close();
//
            h_trig_uncut_part2[i] = (TH1F*) file_part2->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[0]+ "/hdjmass"); // uncut jet40 histo
            h_trig_cut_part2[j] = (TH1F*) file_part2->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/jt"+ jt[j+1]+ "/hdjmass");
            
//            h_mc[i] = (TH1F*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] + "/mc"+"/hdjmass");
            
            int ncells_part2 = h_trig_cut_part2[j]->GetSize();  // it adds underflow and overflow bins so I have to extract those
            int Data_NBins_part2 = ncells_part2-2;
            
//            int ncellsMC = h_mc[i]->GetSize();  // it adds underflow and overflow bins so I have to extract those
//            int MC_NBins = ncellsMC-2;
            
            //cout << "Mc Bins: " << MC_NBins << " DAta bins: " << Data_NBins << endl;
            
            double xmin_part2 = 1000000, xmax_part2 = 0;
            double ymin_part2 = 0.;
            double ymax_part2 = 0.;
            
            for (int m=1; m<=Data_NBins_part2; m++) {
               

                double Xaxis_val_part2 = h_trig_cut_part2[j]->GetBinCenter(m);
//                double Xaxis_val2_part2 = h_mc[i]->GetBinCenter(k);
                
                double check_bins_part2 = h_trig_uncut_part2[i]->GetBinContent(m);
                
                if (check_bins_part2!=0) {
                    
                    double Xaxis_data_part2 = h_trig_uncut_part2[i]->GetBinCenter(m);
                    
                    if (m < xmin_part2) {xmin_part2 = m; ymin_part2 = Xaxis_data_part2;}
                    if (m > xmax_part2) {xmax_part2 = m; ymax_part2 = Xaxis_data_part2;}
                }
                
                                 
                int binmax_part2 = h_trig_uncut_part2[i]->GetMaximumBin();
                double x_part2 = h_trig_uncut_part2[i]->GetXaxis()->GetBinCenter(binmax_part2);
                
                if (Xaxis_val_part2 < turn_on_part2[i][j] ){
                    h_trig_cut_part2[j]->SetBinContent(m,0);
                    h_trig_cut_part2[j]->SetBinError(m,0);
                }
                if (Xaxis_val_part2 > turn_on_part2[i][j] || Xaxis_val_part2 < x_part2){
                    h_trig_uncut_part2[i]->SetBinContent(m,0);
                    h_trig_uncut_part2[i]->SetBinError(m,0);
                }
            }//l
            
            h_trig_uncut_part2[i]->Add(h_trig_cut_part2[j]);
            
            
            cout << endl << endl << "ymin_part2: " << ymin_part2 << "ymin: " << ymin <<  endl << endl;
            gStyle->SetOptStat(0);
            gStyle->SetOptTitle(0);
            
            
            if (j==6){
                
                TString eta_name = (TString) (eta[i] + " < #eta < " + eta[i+1]);
                double Scale = h_trig_uncut[i]->Integral()/h_mc[i]->Integral();
                h_mc[i]->Scale(Scale);
                
                double Scale_part2 = h_trig_uncut[i]->Integral()/h_trig_uncut_part2[i]->Integral();
                h_trig_uncut_part2[i]->Scale(Scale_part2);
                
                h_Ratio[i] = (TH1F*)h_mc[i]->Clone("h_Ratio");
                h_Ratio_part2[i] = (TH1F*)h_mc[i]->Clone("h_Ratio_part2");
            
                c3->cd();
                
                TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
                pad1->SetBottomMargin(0.005); // Upper and lower plot are joined
                pad1->Draw();             // Draw the upper pad: pad1
                pad1-> SetLogy();
                pad1-> SetLogx();
                pad1->cd();
                
                
//                if (ymin < ymin_part2) {
//                    h_mc[i]->GetXaxis()->SetRangeUser(ymin,6500);
//                } else {
//                    h_mc[i]->GetXaxis()->SetRangeUser(ymin_part2,6500);
//                }
                
                h_mc[i]->GetXaxis()->SetRangeUser(ymin_part2,6500);
                h_mc[i]->GetXaxis()->SetLabelOffset(999);
                h_mc[i]->GetYaxis()->SetNdivisions(505);
                h_mc[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                h_mc[i]->GetXaxis()->SetLabelSize(20);
                h_mc[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                h_mc[i]->GetYaxis()->SetLabelSize(20);
                h_mc[i]->SetMarkerStyle(20);
                h_mc[i]->SetMarkerColor(kBlack);
                h_mc[i]->SetLineColor(kBlack);
                h_mc[i]->SetLineWidth(1);
                h_mc[i]->SetYTitle("A.U.");
                h_mc[i]->GetYaxis()->SetTitleOffset(0.8);
                h_mc[i]->GetYaxis()->SetTitleSize(0.05);
                h_mc[i]->GetYaxis()->SetTitleFont(42);
                
                h_trig_uncut[i]->SetMarkerColor(kBlue);
                h_trig_uncut[i]->SetMarkerStyle(20);
                h_trig_uncut[i]->SetMarkerSize(0.9);
                h_trig_uncut[i]->SetLineColor(kBlue);
                h_trig_uncut[i]->SetLineWidth(1);
                
                h_trig_uncut_part2[i]->SetMarkerColor(kRed);
                h_trig_uncut_part2[i]->SetMarkerStyle(20);
                h_trig_uncut_part2[i]->SetMarkerSize(0.9);
                h_trig_uncut_part2[i]->SetLineColor(kRed);
                h_trig_uncut_part2[i]->SetLineWidth(1);
                
                h_mc[i]->Draw("HIST");
                h_trig_uncut[i]->Draw("SAME");
                h_trig_uncut_part2[i]->Draw("SAME");
                
                
                TLegend *leg = new TLegend(0.6502504,0.6685714,0.7520868,0.8889796,NULL,"brNDC");
                leg->SetBorderSize(0);
                leg->SetTextFont(42);
                leg->SetTextSize(0.045);
                leg->AddEntry(h_mc[i],"MC","lep");
                leg->AddEntry(h_trig_uncut[i],"RunBCDEF_{Early}, 19.49 fb^{-1}","lep");
                leg->AddEntry(h_trig_uncut_part2[i],"RunF_{Late}GH,  16.5 fb^{-1}","lep");
                leg->AddEntry("","AK4chs Jets","");
                leg->AddEntry("",eta_name,"");
                leg->Draw();
                
                TLatex *t6 = new TLatex();
                t6->SetNDC();
                t6->SetTextFont(42);
                t6->SetTextSize(0.05);
                t6->SetTextAlign(11);
                t6->DrawLatex(0.1,0.92,"#bf{CMS} Preliminary, #sqrt{s}=13 TeV");
                
                c3->cd();          // Go back to the main canvas before defining pad2
                
                TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
                pad2->SetTopMargin(0.005);
                pad2->SetBottomMargin(0.28);
                pad2->SetGridy();
                pad2->Draw();
                pad2->cd();       // pad2 becomes the current pad
                gPad-> SetLogx();
                
                h_Ratio[i]->Divide(h_trig_uncut[i]);
                h_Ratio_part2[i]->Divide(h_trig_uncut_part2[i]);
                
                //cosmetics
//                if (ymin < ymin_part2) {
//                    h_Ratio[i]->GetXaxis()->SetRangeUser(ymin,6500);
//                } else {
//                    h_Ratio[i]->GetXaxis()->SetRangeUser(ymin_part2,6500);
//                }
                h_Ratio[i]->GetXaxis()->SetRangeUser(ymin_part2,6500);
                h_Ratio[i]->SetXTitle("M_{ij} (GeV)");
                h_Ratio[i]->SetYTitle("MC/Data");
                h_Ratio[i]->GetYaxis()->SetTitleOffset(0.8);
                h_Ratio[i]->GetYaxis()->CenterTitle();
                h_Ratio[i]->GetYaxis()->SetNdivisions(511);
                //h_Ratio[i]->SetMarkerStyle(8);
                //h_Ratio[i]->SetMarkerSize(1.0);
                h_Ratio[i]->SetMarkerColor(kBlue);
                h_Ratio[i]->SetLineColor(kBlue);
                h_Ratio[i]->SetLineWidth(2);
                //h_Ratio->GetXaxis()->SetRangeUser(0.,1.);
                h_Ratio[i]->GetYaxis()->SetRangeUser(0.5,1.5);
                
                h_Ratio[i]->GetYaxis()->SetNdivisions(505);
                h_Ratio[i]->GetYaxis()->SetTitleSize(0.11);
                h_Ratio[i]->GetYaxis()->SetTitleFont(42);
                h_Ratio[i]->GetYaxis()->SetTitleOffset(0.35);
                h_Ratio[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                h_Ratio[i]->GetYaxis()->SetLabelSize(18);
                
                h_Ratio[i]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
                h_Ratio[i]->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
                h_Ratio[i]->GetXaxis()->SetTitleSize(0.11);
                h_Ratio[i]->GetXaxis()->SetTitleFont(42);
                h_Ratio[i]->GetXaxis()->SetTitleOffset(1.1);
                h_Ratio[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                h_Ratio[i]->GetXaxis()->SetLabelSize(18);
                
                h_Ratio_part2[i]->SetMarkerColor(kRed);
                h_Ratio_part2[i]->SetLineColor(kRed);
                h_Ratio_part2[i]->SetLineWidth(2);

                
                h_Ratio[i]->Draw("E1HIST");
                h_Ratio_part2[i]->Draw("E1HISTSAME");
                
                c3->SaveAs("Merged"+ eta[i] + "-" + eta[i+1]+ "-" +jt[j+1]+".png");
            } //j==6
		
            h_trig_cut[j]->Delete();
            h_trig_cut_part2[j]->Delete();
        }//j
    }//i
}//main

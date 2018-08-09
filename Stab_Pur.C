#include "TH1F.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include <stdio.h>      
#include <stdlib.h>

void Stab_Pur(){
    
    TString eta[] = {
        "0.0",
        "0.5",
        "1.0",
        "1.5",
        "2.0",
        "2.5"
    };
    
    const int nMassBins = 103;
    double massBoundaries[nMassBins+1] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7060, 7320, 7589, 7866, 8152, 8447, 8752, 9067, 9391, 9726, 10072, 10430, 10798, 11179, 11571, 11977, 12395, 12827, 13272, 13732, 14000};
    
    int bin_counter = (sizeof(eta)/sizeof(*eta));

    
    Double_t w = 1200;
    Double_t h = 800;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    c1->SetLogx();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    TCanvas * c2 = new TCanvas("c2", "c2", w, h);
    c2->SetLogx();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    
    TCanvas * c3 = new TCanvas("c3", "c3", w, h);
    c3->SetLogx();
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    TLegend *leg = new TLegend(0.7646077,0.74677,0.8973289,0.8875969,NULL,"brNDC");
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);
    
    
    TFile *MC_file = new TFile("output-MC-PtHat-1.root");
    
    ///// Histos booking //////

    
    TH2D* R_Matrix[5];
    TProfile *p_stab[5];
    TProfile *p_pur[5];
    TProfile *combine[5];
    

    
    
    double sumgen_columns[5][100];
    double sumreco_columns[5][100];
    

    
    for (int i=0; i<bin_counter-1; i++) { 
		
		R_Matrix[i] = (TH2D*) MC_file->Get("Standard/Eta_" + eta[i] + "-" + eta[i+1] +"/mc"+"/matrix_gen_reco");
		p_stab[i]= new TProfile("stab","",nMassBins,massBoundaries);
		p_pur[i]= new TProfile("pur","",nMassBins,massBoundaries);
		
		int first_bin = R_Matrix[i]->FindFirstBinAbove();
		int last_bin = R_Matrix[i]->FindLastBinAbove();
		
		
		/////Stability
		for (int j=first_bin; j<=last_bin; j++) { 
			
			double matched_binvalue = 0 ;
			double stability = 0 ;
			
			for (int k=first_bin; k<=last_bin; k++) {
				
				double binvalue = R_Matrix[i]->GetBinContent(j,k);
				
				if (j==k) matched_binvalue = R_Matrix[i]->GetBinContent(j,k); 
				if (binvalue != 0 ) sumreco_columns[i][j] += binvalue; 
				
				
			}
			
			
			stability =  matched_binvalue/sumreco_columns[i][j] ;
			p_stab[i]->Fill(massBoundaries[j], stability);
			 
		
		}
		
		/////Purity
		for (int k=first_bin; k<=last_bin; k++) { 
			
			double matched_binvalue_2 = 0 ;
			double purity = 0 ;
			
			for (int j=first_bin; j<=last_bin; j++) {
				
				double binvalue_2 = R_Matrix[i]->GetBinContent(j,k);
				
				if (j==k) matched_binvalue_2 = R_Matrix[i]->GetBinContent(j,k); 
				if (binvalue_2 != 0 ) sumgen_columns[i][k] += binvalue_2; 
				
				
			}
			
			
			purity =  matched_binvalue_2/sumgen_columns[i][k] ;
			p_pur[i]->Fill(massBoundaries[k], purity);
			 
		
		}
		
		c1->cd();
		
		p_stab[i]->GetYaxis()->SetRangeUser(0.0,1.0);
   		p_stab[i]->GetXaxis()->SetRangeUser(31.,10000.);
   		
   		
   		p_stab[i]->GetYaxis()->SetTitle("Stability");
   		p_stab[i]->GetXaxis()->SetTitle("Mjj(GeV)");
   		p_stab[i]->GetYaxis()->SetTitleOffset(0.9);
                p_stab[i]->GetYaxis()->SetTitleSize(0.03);
                p_stab[i]->GetYaxis()->SetTitleFont(42);
                p_stab[i]->GetXaxis()->SetTitleOffset(1.2);
                p_stab[i]->GetXaxis()->SetTitleSize(0.03);
                p_stab[i]->GetXaxis()->SetTitleFont(42);
                p_stab[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                p_stab[i]->GetXaxis()->SetLabelSize(15);
                p_stab[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                p_stab[i]->GetYaxis()->SetLabelSize(15);
                p_stab[i]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    		p_stab[i]->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
    		p_stab[i]->Draw("HIST");
    		
    		TLatex lt;
                lt.SetNDC();
            	lt.SetTextSize(0.03);
                lt.DrawLatex(0.77,0.86,eta[i]+" < #eta < "+eta[i+1]);
                lt.SetTextAlign(12);
		
		c1->SaveAs("Stability_"+ eta[i] + "-" + eta[i+1] +".png");
		c1->Update();
                
		
		
		c2->cd();
		
		p_pur[i]->GetYaxis()->SetRangeUser(0.0,1.0);
   		p_pur[i]->GetXaxis()->SetRangeUser(31.,10000.);
   		
   		
   		p_pur[i]->GetYaxis()->SetTitle("Purity");
   		p_pur[i]->GetXaxis()->SetTitle("Mjj(GeV)");
   		p_pur[i]->GetYaxis()->SetTitleOffset(0.9);
                p_pur[i]->GetYaxis()->SetTitleSize(0.03);
                p_pur[i]->GetYaxis()->SetTitleFont(42);
                p_pur[i]->GetXaxis()->SetTitleOffset(1.2);
                p_pur[i]->GetXaxis()->SetTitleSize(0.03);
                p_pur[i]->GetXaxis()->SetTitleFont(42);
                p_pur[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                p_pur[i]->GetXaxis()->SetLabelSize(15);
                p_pur[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                p_pur[i]->GetYaxis()->SetLabelSize(15);
                p_pur[i]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    		p_pur[i]->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
    		p_pur[i]->Draw("HIST");
    		
    		TLatex lt;
                lt.SetNDC();
            	lt.SetTextSize(0.03);
                lt.DrawLatex(0.77,0.86,eta[i]+" < #eta < "+eta[i+1]);
                lt.SetTextAlign(12);
		
		c2->SaveAs("Purity_"+ eta[i] + "-" + eta[i+1] +".png");
                
                c2->Update();

                c3->cd();
		
		combine[i] = (TProfile*)p_stab[i]->Clone("Combine");
	        
		combine[i]->GetYaxis()->SetRangeUser(0.0,1.0);
   		combine[i]->GetXaxis()->SetRangeUser(31.,10000.);
   		
   		
   		combine[i]->GetYaxis()->SetTitle("Stability/Purity");
   		combine[i]->SetLineColor(kBlue);
   		combine[i]->GetXaxis()->SetTitle("Mjj(GeV)");
   		combine[i]->GetYaxis()->SetTitleOffset(0.9);
                combine[i]->GetYaxis()->SetTitleSize(0.03);
                combine[i]->GetYaxis()->SetTitleFont(42);
                combine[i]->GetXaxis()->SetTitleOffset(1.2);
                combine[i]->GetXaxis()->SetTitleSize(0.03);
                combine[i]->GetXaxis()->SetTitleFont(42);
                combine[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                combine[i]->GetXaxis()->SetLabelSize(15);
                combine[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
                combine[i]->GetYaxis()->SetLabelSize(15);
                combine[i]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
    		combine[i]->GetXaxis()->SetNoExponent(); //to make the x-axis a bit easier to read and see where the axis starts.
    		combine[i]->Draw("HIST");
    		
    		p_pur[i]->SetLineColor(2);
	        p_pur[i]->SetLineWidth(1);
           	p_pur[i]->Draw("e1 same");
           	
           	
           	
           	leg->AddEntry(combine[i],"Stability","l");
           	leg->AddEntry(p_pur[i],"Purity","l");
           	leg->Draw();
    		
    		TLatex lt;
                lt.SetNDC();
            	lt.SetTextSize(0.03);
                lt.DrawLatex(0.17,0.16,eta[i]+" < #eta < "+eta[i+1]);
                lt.SetTextAlign(12);
                
                c3->SaveAs("Combine"+ eta[i] + "-" + eta[i+1] +".png");
                combine[i]->Delete();
                p_stab[i]->Delete();
                p_pur[i]->Delete();
                leg->Clear();
                c3->Update();

}
}

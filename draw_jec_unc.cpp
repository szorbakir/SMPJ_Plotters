#include "TH1D.h"

void draw_jec_unc(){
    
    TString inputFile1;
    Double_t w = 1200;
    Double_t h = 1000;
    
    TH1D *h_histo1_;
    TH1D *h_histo2_;
    TH1D *h_histo3_;
    TH1D *h_histo4_;
    TH1D *h_histo5_;
    
    TFile * file = new TFile("datas/datas_draw_unc/output-DATA-2b.root");
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    
    h_histo1_ = (TH1D*) file->Get("Standard/Eta_0.0-0.5/punc_vs_pt");
    h_histo2_ = (TH1D*) file->Get("Standard/Eta_0.5-1.0/punc_vs_pt");
    h_histo3_ = (TH1D*) file->Get("Standard/Eta_1.0-1.5/punc_vs_pt");
    h_histo4_ = (TH1D*) file->Get("Standard/Eta_1.5-2.0/punc_vs_pt");
    h_histo5_ = (TH1D*) file->Get("Standard/Eta_2.0-2.5/punc_vs_pt");
    
    c1->cd();
    //Draw, cosmetics, etc.
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    h_histo1_->GetYaxis()->SetRangeUser(0.,2.5);
    h_histo1_->GetXaxis()->SetRangeUser(74,4100);
    
    h_histo1_->SetXTitle("P_{T} (GeV)");
    h_histo1_->SetYTitle("JEC Uncertainty %");
    h_histo1_->GetYaxis()->CenterTitle();
    
    
    h_histo1_->Draw("FC HIST");
    h_histo2_->Draw("FC HIST SAME");
    h_histo3_->Draw("FC HIST SAME");
    h_histo4_->Draw("FC HIST SAME");
    h_histo5_->Draw("FC HIST SAME");
    
    h_histo1_->SetLineColor(kBlack);
    h_histo2_->SetLineColor(kBlue);
    h_histo3_->SetLineColor(kRed);
    h_histo4_->SetLineColor(kGreen+2);
    h_histo5_->SetLineColor(kYellow+2);
    
    h_histo1_->SetLineWidth(2);
    h_histo2_->SetLineWidth(2);
    h_histo3_->SetLineWidth(2);
    h_histo4_->SetLineWidth(2);
    h_histo5_->SetLineWidth(2);
    
    TLegend *leg = new TLegend(0.1285476,0.6364103,0.230384,0.8876923,NULL,"brNDC");
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.03);
    leg->AddEntry(h_histo1_,"0.0<|y|<0.5","l");
    leg->AddEntry(h_histo2_,"0.5<|y|<1.0","l");
    leg->AddEntry(h_histo3_,"1.0<|y|<1.5","l");
    leg->AddEntry(h_histo4_,"1.5<|y|<2.0","l");
    leg->AddEntry(h_histo5_,"2.0<|y|<2.5","l");
    leg->AddEntry("","AK4PFchs Jets","");
    leg->Draw();
    
    TLatex *t1 = new TLatex();
    t1->SetNDC();
    t1->SetTextFont(42);
    t1->SetTextSize(0.035);
    t1->SetTextAlign(11);
    t1->DrawLatex(0.1,0.92,"#bf{CMS} #font[12]{Work in Progress 2016}");
    
    TLatex *t2 = new TLatex();
    t2->SetNDC();
    t2->SetTextFont(42);
    t2->SetTextSize(0.033);
    t2->SetTextAlign(11);
    t2->DrawLatex(0.68,0.92,"#sqrt{s}=13 TeV, 35.9 fb^{-1}");
    
    TLatex *t3 = new TLatex();
    t3->SetNDC();
    t3->SetTextFont(42);
    t3->SetTextSize(0.03);
    t3->SetTextAlign(11);
    t3->DrawLatex(0.2796327,0.8574359,"JEC: Summer16_03Feb2017_V7 ");
    
    c1->SaveAs("pngs/draw_jec_unc.png");
}

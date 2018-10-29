void _unc_pt_ratio_seperate_sources()
{

    TString observable[] = {
        "hpt",
        "hdj_leading",
        "hdj_subleading",
        "hpt1",
        "hpt2"};

    TString axis_name[] = {
        "Jet p_{T} (GeV)",
        "Dijet System - Leading Jet p_{T} (GeV)",
        "Dijet System - Subleading Jet p_{T} (GeV)",
        "Leading Jet p_{T} (GeV)",
        "Subleading Jet p_{T} (GeV)"};

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

    constexpr double w = 1500.;
    constexpr double h = 1000.;

    TCanvas *c1 = new TCanvas("c1", "c1", w, h);

    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    Color_t BackgroundColor = 10;
    gStyle->SetCanvasColor(BackgroundColor);
    gStyle->SetTitleFillColor(BackgroundColor);
    gStyle->SetStatColor(BackgroundColor);

    c1->SetFillColor(BackgroundColor);

    TH1D *h_Pythia_;

    TH1D *h_DATA_;
    TH1D *h_DATA_up_;
    TH1D *h_DATA_down_;
    TH1D *h_DATA_lumi_up_;
    TH1D *h_DATA_lumi_down_;
    TH1D *h_DATA_total_up_;
    TH1D *h_DATA_total_down_;

    TH1D *h_Ratio_PYT_;
    TH1D *h_Ratio_PYT_up_;
    TH1D *h_Ratio_PYT_down_;
    TH1D *h_Ratio_PYT_lumi_up_;
    TH1D *h_Ratio_PYT_lumi_down_;
    TH1D *h_Ratio_PYT_total_up_;
    TH1D *h_Ratio_PYT_total_down_;

    inputFile1.Form("datas/output-MC-PtHat-2b.root");
    inputFile2.Form("datas/output-DATA-2b.root");
    inputFile3.Form("datas/output-DATA-2b-up.root");
    inputFile4.Form("datas/output-DATA-2b-down.root");

    constexpr auto counter = (sizeof(observable) / sizeof(*observable));
    constexpr auto bin_counter = (sizeof(eta_bins) / sizeof(*eta_bins));

    for (size_t j = 0; j < bin_counter - 1; j++)
    {
        for (size_t i = 0; i < counter; i++)
        {

            TString hname = (TString)(observable[i] + "_" + eta_bins[j] + "-" + eta_bins[j + 1]);
            TString eta_name = (TString)(eta_bins[j] + " < |y| < " + eta_bins[j + 1]);

            //MC
            TFile *MCFile = TFile::Open(inputFile1.Data(), "READ");
            MCFile->cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j + 1]);
            h_Pythia_ = (TH1D *)gDirectory->FindObjectAny(observable[i]);

            //DATA
            TFile *DATAFile = TFile::Open(inputFile2.Data(), "READ");
            DATAFile->cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j + 1]);
            h_DATA_ = (TH1D *)gDirectory->FindObjectAny(observable[i]);

            //DATA_up
            TFile *DATAFile_up = TFile::Open(inputFile3.Data(), "READ");
            DATAFile_up->cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j + 1]);
            h_DATA_up_ = (TH1D *)gDirectory->FindObjectAny(observable[i]);

            //DATA_down
            TFile *DATAFile_down = TFile::Open(inputFile4.Data(), "READ");
            DATAFile_down->cd("Standard/Eta_" + eta_bins[j] + "-" + eta_bins[j + 1]);
            h_DATA_down_ = (TH1D *)gDirectory->FindObjectAny(observable[i]);

            h_DATA_lumi_up_ = (TH1D *)h_DATA_->Clone("h_DATA_lumi_up_");
            h_DATA_lumi_down_ = (TH1D *)h_DATA_->Clone("h_DATA_lumi_down_");

            h_DATA_total_up_ = (TH1D *)h_DATA_->Clone("h_DATA_total_up_");
            h_DATA_total_down_ = (TH1D *)h_DATA_->Clone("h_DATA_total_down_");

            auto xmin = std::numeric_limits<double>::infinity();
            auto xmax = std::numeric_limits<double>::min();
            auto ymin = std::numeric_limits<double>::infinity();
            auto ymax = std::numeric_limits<double>::min();

            auto ncells = h_DATA_->GetSize(); // it adds underflow and overflow bins so I have to extract those
            auto Data_NBins = ncells - 2;

            for (size_t k = 1; k <= Data_NBins; k++)
            {

                auto check_bins = h_DATA_->GetBinContent(k);

                if (check_bins != 0)
                {

                    auto Xaxis_data = h_DATA_->GetBinCenter(k);

                    if (k < xmin)
                    {
                        xmin = k;
                        ymin = Xaxis_data;
                    }
                    if (k > xmax)
                    {
                        xmax = k;
                        ymax = Xaxis_data;
                    }

                    auto data = h_DATA_->GetBinContent(k);

                    auto jec_up = abs(h_DATA_up_->GetBinContent(k) - h_DATA_->GetBinContent(k));
                    auto jec_down = abs(h_DATA_->GetBinContent(k) - h_DATA_down_->GetBinContent(k));
                    auto lumi_unc = data * (2.5 / 100.0);
                    auto det_unc = data * (1.0 / 100.0);

                    auto up_shift = sqrt(pow(jec_up, 2) + pow(det_unc, 2));
                    auto down_shift = sqrt(pow(jec_down, 2) + pow(det_unc, 2));

                    auto total_up_shift = sqrt(pow(jec_up, 2) + pow(lumi_unc, 2) + pow(det_unc, 2));
                    auto total_down_shift = sqrt(pow(jec_down, 2) + pow(lumi_unc, 2) + pow(det_unc, 2));

                    h_DATA_up_->SetBinContent(k, data + up_shift);
                    h_DATA_down_->SetBinContent(k, data - down_shift);
                    h_DATA_lumi_up_->SetBinContent(k, data + lumi_unc);
                    h_DATA_lumi_down_->SetBinContent(k, data - lumi_unc);
                    h_DATA_total_up_->SetBinContent(k, data + total_up_shift);
                    h_DATA_total_down_->SetBinContent(k, data - total_down_shift);
                }
            }

            h_Pythia_->Scale((h_DATA_->Integral()) / (h_Pythia_->Integral(h_DATA_->FindFirstBinAbove(0, 1), h_DATA_->FindLastBinAbove(0, 1))));

            //Draw, cosmetics, etc.

            c1->cd();

            h_Ratio_PYT_ = (TH1D *)h_Pythia_->Clone("h_Ratio_PYT_");
            h_Ratio_PYT_up_ = (TH1D *)h_Pythia_->Clone("h_Ratio_PYT_up_");
            h_Ratio_PYT_down_ = (TH1D *)h_Pythia_->Clone("h_Ratio_PYT_down_");
            h_Ratio_PYT_lumi_up_ = (TH1D *)h_Pythia_->Clone("h_Ratio_PYT_lumi_up_");
            h_Ratio_PYT_lumi_down_ = (TH1D *)h_Pythia_->Clone("h_Ratio_PYT_lumi_down_");
            h_Ratio_PYT_total_up_ = (TH1D *)h_Pythia_->Clone("h_Ratio_PYT_total_up_");
            h_Ratio_PYT_total_down_ = (TH1D *)h_Pythia_->Clone("h_Ratio_PYT_total_down_");

            c1->Draw();
            c1->cd(); // pad2 becomes the current pad
            c1->SetLogx();

            h_Ratio_PYT_total_down_->SetFillColor(kBlue - 4);
            h_Ratio_PYT_down_->SetFillColor(kOrange);
            h_Ratio_PYT_lumi_down_->SetFillColor(kGreen + 1);
            h_Ratio_PYT_lumi_up_->SetFillColor(kOrange);
            h_Ratio_PYT_up_->SetFillColor(kBlue - 4);
            h_Ratio_PYT_total_up_->SetFillColor(BackgroundColor);

            h_Ratio_PYT_->Divide(h_DATA_);
            h_Ratio_PYT_up_->Divide(h_DATA_up_);
            h_Ratio_PYT_down_->Divide(h_DATA_down_);
            h_Ratio_PYT_lumi_up_->Divide(h_DATA_lumi_up_);
            h_Ratio_PYT_lumi_down_->Divide(h_DATA_lumi_down_);
            h_Ratio_PYT_total_up_->Divide(h_DATA_total_up_);
            h_Ratio_PYT_total_down_->Divide(h_DATA_total_down_);

            //cosmetics
            h_Ratio_PYT_total_down_->GetXaxis()->SetRangeUser(ymin, 2500);
            h_Ratio_PYT_total_down_->SetXTitle(axis_name[i]);
            h_Ratio_PYT_total_down_->SetYTitle("MC/Data");
            h_Ratio_PYT_total_down_->GetYaxis()->CenterTitle();
            h_Ratio_PYT_->SetMarkerColor(kBlack);
            h_Ratio_PYT_->SetMarkerStyle(20);
            h_Ratio_PYT_->SetLineColor(kBlack);
            h_Ratio_PYT_->SetLineWidth(2);
            h_Ratio_PYT_total_down_->GetYaxis()->SetRangeUser(0.6, 1.6);
            h_Ratio_PYT_total_down_->GetYaxis()->SetTitleOffset(1.3);
            h_Ratio_PYT_total_down_->GetXaxis()->SetMoreLogLabels();
            h_Ratio_PYT_total_down_->GetXaxis()->SetNoExponent();
            h_Ratio_PYT_total_down_->GetXaxis()->SetTitleOffset(1.3);

            h_Ratio_PYT_down_->SetLineColor(kOrange);
            h_Ratio_PYT_down_->SetMarkerColor(kOrange);

            h_Ratio_PYT_up_->SetLineColor(kOrange);
            h_Ratio_PYT_up_->SetMarkerColor(kOrange);

            h_Ratio_PYT_lumi_up_->SetLineColor(kGreen + 1);
            h_Ratio_PYT_lumi_up_->SetMarkerColor(kGreen + 1);

            h_Ratio_PYT_lumi_down_->SetLineColor(kGreen + 1);
            h_Ratio_PYT_lumi_down_->SetMarkerColor(kGreen + 1);

            h_Ratio_PYT_total_up_->SetLineColor(kBlue - 4);
            h_Ratio_PYT_total_up_->SetMarkerColor(kBlue - 4);

            h_Ratio_PYT_total_down_->SetLineColor(kBlue - 4);
            h_Ratio_PYT_total_down_->SetMarkerColor(kBlue - 4);

            h_Ratio_PYT_total_down_->Draw("HIST C");
            h_Ratio_PYT_down_->Draw("SAME HIST C");
            h_Ratio_PYT_lumi_down_->Draw("SAME HIST C");
            h_Ratio_PYT_lumi_up_->Draw("SAME HIST C");
            h_Ratio_PYT_up_->Draw("SAME HIST C");
            h_Ratio_PYT_total_up_->Draw("SAME HIST C");
            h_Ratio_PYT_->Draw("SAME E1");

            c1->SetTickx(1);
            c1->SetTicky(1);
            gPad->RedrawAxis();

            TLegend *leg = new TLegend(0.1435726, 0.6861538, 0.245409, 0.8882051, NULL, "brNDC");
            leg->SetBorderSize(0);
            leg->SetTextFont(42);
            leg->SetTextSize(0.03);
            leg->AddEntry(h_Ratio_PYT_, "MC:Pythia8/Data:2016", "lp");
            leg->AddEntry(h_Ratio_PYT_down_, "JEC + Detector Uncertainty", "lf");
            leg->AddEntry(h_Ratio_PYT_lumi_down_, "Luminosity Uncertainty", "lf");
            leg->AddEntry(h_Ratio_PYT_total_down_, "JEC + Detector + Luminosity Uncertainty", "lf");
            leg->AddEntry("", "AK4chs Jets", "");
            leg->AddEntry("", eta_name, "");
            leg->Draw();

            TLatex *t6 = new TLatex();
            t6->SetNDC();
            t6->SetTextFont(42);
            t6->SetTextSize(0.035);
            t6->SetTextAlign(11);
            t6->DrawLatex(0.1, 0.92, "#bf{CMS} #font[12]{Work in Progress 2016}");

            TLatex *t7 = new TLatex();
            t7->SetNDC();
            t7->SetTextFont(42);
            t7->SetTextSize(0.033);
            t7->SetTextAlign(11);
            t7->DrawLatex(0.75, 0.92, "35.9 fb^{-1} (13 TeV)");

            c1->cd(); // Go back to the main canvas before defining pad2

            c1->SaveAs("pngs/unc_pt_ratio_" + hname + ".png"); //saving...
        }
    }
}

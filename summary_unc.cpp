void summary_unc()
{
    TCanvas *c1 = new TCanvas("c1", "c1", 1200, 1000); //Print plots switches
    bool print_RawMassPlots = false;
    bool print_TurnOnPlots = false;
    bool print_OnlyDataSpectrumPlots = false; //only data mass spectrum etaBYeta, triggers with different colors
    bool print_MC_DATA_massSpectrum = true;   //Data mass eta merged and MC included

    TString eta_bins[] = {"0.0", "0.5", "1.0", "1.5", "2.0", "2.5", "3.0", "3.2", "4.7"};
    TString jt[] = {"40", "80", "140", "200", "260", "320", "400", "450"};
    //List of colors = kBlcak=1, kRed=632, kSpring+5=825, kBlue=600, kOrange=800, kMagenta+1=616, kCyan+1=433, kGreen-2=414
    int ListColors[] = {1, 632, 825, 600, 800, 616, 433, 414};
    int ListMrkStyle[] = {20, 21, 29, 34, 33, 23, 22, 27};
    gStyle->SetOptStat(0);

    TLegend *leg = new TLegend(0.6994992, 0.6619355, 0.8731219, 0.8722581);
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);

    TString inputFile1;
    TString inputFile3;
    TString inputFile4;

    inputFile1.Form("datas/output-DATA-2a.root");

    TH1F *h_djmassRef[8]; //to get turnOn point for each trigger on dijet mass, denominator
    TH1F *h_djmass[8];    //numerator
    TH1F *h_djmassCut[5][8];

    constexpr auto bin_counter = (sizeof(eta_bins) / sizeof(*eta_bins));
    constexpr auto counter = (sizeof(jt) / sizeof(*jt));

    //Fixed title latex settings for each loop (positions, alignments, etc.)
    TLatex *t1 = new TLatex();
    t1->SetNDC();
    t1->SetTextFont(42);
    t1->SetTextSize(0.04);
    t1->SetTextAlign(11);

    TLatex *t2 = new TLatex();
    t2->SetNDC();
    t2->SetTextFont(42);
    t2->SetTextSize(0.03);
    t2->SetTextAlign(11);

    //------------------------/ get Turn-On point values wrt corresponding HLT_PFJet /------------------------------//

    double turnOn[8][7];
    double FitPar[8][7];

    double turnOn_up[8][7];
    double FitPar_up[8][7];

    double turnOn_down[8][7];
    double FitPar_down[8][7];

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            //open desired files and directories
            TFile *DATAFile = TFile::Open(inputFile1.Data(), "READ");
            DATAFile->cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i + 1] + "/jt" + jt[j] + "/");
            h_djmassRef[j] = (TH1F *)gDirectory->FindObjectAny("hdjmass");

            DATAFile->cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i + 1] + "/jt" + jt[j + 1] + "/");
            h_djmass[j] = (TH1F *)gDirectory->FindObjectAny("hdjmass");

            h_djmass[j]->Divide(h_djmassRef[j]);

            FitPar[i][j] = 1.55;
            if ((i == 0) && j == 1)
                FitPar[i][j] = 1.7; //jt140 0.0-0.5
            if ((i == 0) && j == 2)
                FitPar[i][j] = 1.7; //jt200
            if ((i == 0) && j == 3)
                FitPar[i][j] = 1.7; //jt260
            if ((i == 0) && j == 4)
                FitPar[i][j] = 1.85; //jt320
            if ((i == 0) && j == 5)
                FitPar[i][j] = 1.85; //jt400
            if ((i == 0) && j == 6)
                FitPar[i][j] = 1.85; //jt450

            if (i == 1)
                FitPar[i][j] = 1.7; //      0.5-1.0
            if ((i == 1) && j > 2)
                FitPar[i][j] = 1.9;

            if (i == 2)
                FitPar[i][j] = 2.0; //      1.0-1.5
            if ((i == 2) && j == 2)
                FitPar[i][j] = 2.5; //jt200
            if ((i == 2) && (j >= 3))
                FitPar[i][j] = 2.8;

            if (i == 3)
                FitPar[i][j] = 3.0; //      1.5-2.0
            if ((i == 3) && (j == 1))
                FitPar[i][j] = 3.4; //jt140
            if ((i == 3) && (j == 2))
                FitPar[i][j] = 3.8; //jt200
            if ((i == 3) && (j == 3))
                FitPar[i][j] = 4.3; //jt260
            if ((i == 3) && (j == 4))
                FitPar[i][j] = 4.4; //jt320
            if ((i == 3) && (j == 5))
                FitPar[i][j] = 4.3; //jt400
            if ((i == 3) && (j == 6))
                FitPar[i][j] = 4.2; //jt450

            if (i == 4)
                FitPar[i][j] = 4.3; //jt80 2.0-2.5
            if ((i == 4) && (j == 1))
                FitPar[i][j] = 5.9; //jt140
            if ((i == 4) && (j == 2))
                FitPar[i][j] = 5.6; //jt200
            if ((i == 4) && (j == 3))
                FitPar[i][j] = 5.3; //jt260
            if ((i == 4) && (j == 4))
                FitPar[i][j] = 6.2; //jt320
            if ((i == 4) && (j == 5))
                FitPar[i][j] = 5.2; //jt400
            if ((i == 4) && (j == 6))
                FitPar[i][j] = 5.5; //jt450

            //Fit function
            TF1 *f1 = new TF1("f1", "0.5*(1+TMath::Erf((x-[0])/[1]))", atoi(jt[j + 1]) * FitPar[i][j], 8000.);

            f1->SetParameters(2 * atoi(jt[j + 1]) * cosh(0.5), atoi(jt[j + 1]) * 2 * 0.1);
            f1->SetLineWidth(1);
            c1->cd();
            h_djmass[j]->Fit(f1, "R");
            c1->Close();
            f1->GetX(0.99);

            turnOn[i][j] = f1->GetX(0.99);

        } // j of turON loop
    }     // i of TurnOn plots

    //-----------------------------------------|| Mass spectrum with MC comparison ||--------------------------------

    gStyle->SetOptStat(0);

    constexpr double w = 1200.;
    constexpr double h = 1000.;

    Color_t BackgroundColor = 10;
    gStyle->SetCanvasColor(BackgroundColor);
    gStyle->SetTitleFillColor(BackgroundColor);
    gStyle->SetStatColor(BackgroundColor);

    TCanvas *c4 = new TCanvas("c4", "c4", w, h);
    c4->SetLogy();
    c4->SetLogx();
    c4->SetTickx(1);
    c4->SetTicky(1);
    c4->SetFillColor(BackgroundColor);

    TString inputFile11;
    TH1F *eta0_up;
    TH1F *eta0_down;
    TH1F *eta1_up;
    TH1F *eta1_down;
    TH1F *eta2_up;
    TH1F *eta2_down;
    TH1F *eta3_up;
    TH1F *eta3_down;
    TH1F *eta4_up;
    TH1F *eta4_down;

    inputFile11.Form("hist.root");

    TFile *DATAFile11 = TFile::Open(inputFile11.Data(), "READ");
    DATAFile11->cd();
    eta0_up = (TH1F *)gDirectory->FindObjectAny("eta0_up");
    eta0_down = (TH1F *)gDirectory->FindObjectAny("eta0_down");
    eta1_up = (TH1F *)gDirectory->FindObjectAny("eta1_up");
    eta1_down = (TH1F *)gDirectory->FindObjectAny("eta1_down");
    eta2_up = (TH1F *)gDirectory->FindObjectAny("eta2_up");
    eta2_down = (TH1F *)gDirectory->FindObjectAny("eta2_down");
    eta3_up = (TH1F *)gDirectory->FindObjectAny("eta3_up");
    eta3_down = (TH1F *)gDirectory->FindObjectAny("eta3_down");
    eta4_up = (TH1F *)gDirectory->FindObjectAny("eta4_up");
    eta4_down = (TH1F *)gDirectory->FindObjectAny("eta4_down");

    eta0_up->Scale(1e8);
    eta0_down->Scale(1e8);
    eta1_up->Scale(1e6);
    eta1_down->Scale(1e6);
    eta2_up->Scale(1e4);
    eta2_down->Scale(1e4);
    eta3_up->Scale(1e2);
    eta3_down->Scale(1e2);
    eta4_up->Scale(1e0);
    eta4_down->Scale(1e0);

    eta0_up->SetFillColor(kYellow);
    eta0_down->SetFillColor(BackgroundColor);
    eta1_up->SetFillColor(kYellow);
    eta1_down->SetFillColor(BackgroundColor);
    eta2_up->SetFillColor(kYellow);
    eta2_down->SetFillColor(BackgroundColor);
    eta3_up->SetFillColor(kYellow);
    eta3_down->SetFillColor(BackgroundColor);
    eta4_up->SetFillColor(kYellow);
    eta4_down->SetFillColor(BackgroundColor);

    eta0_up->SetFillStyle(3001);
    eta1_up->SetFillStyle(3001);
    eta2_up->SetFillStyle(3001);
    eta3_up->SetFillStyle(3001);
    eta4_up->SetFillStyle(3001);

    c4->cd();

    eta0_up->GetXaxis()->SetTitle("M_{jj} (GeV)");
    eta0_up->GetXaxis()->SetTitleSize(0.025);
    eta0_up->GetYaxis()->SetTitle("A.U.");
    eta0_up->GetYaxis()->SetTitleSize(0.025);
    eta0_up->GetXaxis()->SetNoExponent();
    eta0_up->GetXaxis()->SetMoreLogLabels();
    //h_djmassCut[i][0]->GetXaxis()->SetRangeUser(/*h_djmassCut[i][0]->GetMaximumBin()*/70,14000);
    eta0_up->GetXaxis()->SetTitleOffset(1.7);
    eta0_up->GetYaxis()->SetTitleOffset(1.7);

    eta0_up->GetXaxis()->SetRangeUser(/*h_djmassCut[i][0]->GetMaximumBin()*/ 88, 14000);
    //    eta0_up->SetMaximum(2e14);
    eta0_up->SetMaximum(2e14);
    eta0_up->SetMinimum(1e-8);

    eta0_up->SetMarkerColor(kYellow);
    eta0_up->SetLineColor(kYellow);
    eta0_down->SetMarkerColor(kYellow);
    eta0_down->SetLineColor(kYellow);
    eta1_up->SetMarkerColor(kYellow);
    eta1_up->SetLineColor(kYellow);
    eta1_down->SetMarkerColor(kYellow);
    eta1_down->SetLineColor(kYellow);
    eta2_up->SetMarkerColor(kYellow);
    eta2_up->SetLineColor(kYellow);
    eta2_down->SetMarkerColor(kYellow);
    eta2_down->SetLineColor(kYellow);
    eta3_up->SetMarkerColor(kYellow);
    eta3_up->SetLineColor(kYellow);
    eta3_down->SetMarkerColor(kYellow);
    eta3_down->SetLineColor(kYellow);
    eta4_up->SetMarkerColor(kYellow);
    eta4_up->SetLineColor(kYellow);
    eta4_down->SetMarkerColor(kYellow);
    eta4_down->SetLineColor(kYellow);

    eta0_up->Draw("hist ");
    eta0_down->Draw("samehist ");
    eta1_up->Draw("samehist ");
    eta1_down->Draw("samehist ");
    eta2_up->Draw("samehist ");
    eta2_down->Draw("samehist ");
    eta3_up->Draw("samehist ");
    eta3_down->Draw("samehist ");
    eta4_up->Draw("samehist ");
    eta4_down->Draw("samehist ");

    //    c4->Close();

    if (print_MC_DATA_massSpectrum == true)
    {

        //legend of triggers and their colors
        TLegend *leg2 = new TLegend(0.523474, 0.6619355, 0.697183, 0.8722581);
        leg2->SetBorderSize(0);
        leg2->SetLineColor(0);
        leg2->SetLineStyle(1);
        leg2->SetLineWidth(0);
        leg2->SetFillColor(0);
        leg2->SetFillStyle(1001);

        //legend but goes down more
        TLegend *leg3 = new TLegend(0.6894992, 0.6219355, 0.8631219, 0.8722581);
        leg3->SetBorderSize(0);
        leg3->SetLineColor(0);
        leg3->SetLineStyle(1);
        leg3->SetLineWidth(0);
        leg3->SetFillColor(0);
        leg3->SetFillStyle(1001);

        TH1F *MC_massSpectrum[5];

        TString inputFile2;
        inputFile2.Form("datas/output-MC-PtHat-2a.root");

        for (size_t i = 0; i < 5; i++)
        {
            //open MC file
            TFile *MCFile = TFile::Open(inputFile2.Data(), "READ");
            MCFile->cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i + 1] + "/");
            MC_massSpectrum[i] = (TH1F *)gDirectory->FindObjectAny("hdjmass");

            for (size_t j = 0; j < counter; j++)
            {

                //open DATA file
                TFile *DATAFile = TFile::Open(inputFile1.Data(), "READ");
                DATAFile->cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i + 1] + "/jt" + jt[j] + "/");
                h_djmassCut[i][j] = (TH1F *)gDirectory->FindObjectAny("hdjmass");

                // cut the right and left of DATA wrt triggers
                int ncells = h_djmassCut[i][j]->GetSize(); //it adds undrfolw and overflow bins so I have to extract those
                int Data_Nbins = ncells - 2;

                for (size_t k = 1; k <= Data_Nbins; k++)
                {
                    auto check_bins = h_djmassCut[i][j]->GetBinContent(k);
                    //double check_err  = h_djmassCut[i][j]->GetBinError(k);

                    if (check_bins != 0)
                    {
                        auto binmax = h_djmassCut[i][0]->GetMaximumBin();
                        auto x = h_djmassCut[i][0]->GetXaxis()->GetBinCenter(binmax);
                        auto Xaxis_val = h_djmassCut[i][0]->GetBinCenter(k);

                        if ((j == 0) && (Xaxis_val < x))
                        {
                            h_djmassCut[i][0]->SetBinContent(k, 0.);
                            h_djmassCut[i][0]->SetBinError(k, 0.);
                        } // cut left side of jt40

                        if ((k < h_djmassCut[i][j]->GetXaxis()->FindBin(turnOn[i][j - 1])) && (j > 0))
                        {
                            h_djmassCut[i][j]->SetBinContent(k, 0.);
                            h_djmassCut[i][j]->SetBinError(k, 0.);
                        } //cut leftside >jt40

                        if ((k >= h_djmassCut[i][j]->GetXaxis()->FindBin(turnOn[i][j])) && (j < 7))
                        {
                            h_djmassCut[i][j]->SetBinContent(k, 0.);
                            h_djmassCut[i][j]->SetBinError(k, 0.);
                        } //cut right side <jt450
                    }     //check bin
                }         //k

                // cut the right left side of jt40 and rigt side of jt450 for MC
                auto ncellsMC = MC_massSpectrum[i]->GetSize(); //it adds undrfolw and overflow bins so I have to extract those
                auto MC_Nbins = ncellsMC - 2;

                for (size_t k = 0; k <= MC_Nbins; k++)
                {

                    if ((j == 0) && (k < h_djmassCut[i][0]->FindFirstBinAbove(0, 1)))
                    {
                        //cout << k << endl;
                        MC_massSpectrum[i]->SetBinError(k, 0.);
                        MC_massSpectrum[i]->SetBinContent(k, 0.);
                    }

                    if ((j == 7) && (k > h_djmassCut[i][7]->FindLastBinAbove(0, 1)))
                    {
                        //cout << k << endl;
                        MC_massSpectrum[i]->SetBinError(k, 0.);
                        MC_massSpectrum[i]->SetBinContent(k, 0.);
                    }
                }

                //Draw  dijetMasss of jt40 histogram after cut
                if (j == 0)
                {
                    h_djmassCut[i][0] = (TH1F *)gDirectory->FindObjectAny("hdjmass");
                    h_djmassCut[i][0]->SetLineColor(1);
                    h_djmassCut[i][0]->GetXaxis()->SetTitle("M_{jj} (GeV)");
                    h_djmassCut[i][0]->GetXaxis()->SetTitleSize(0.025);
                    h_djmassCut[i][0]->GetXaxis()->SetNoExponent();
                    h_djmassCut[i][0]->GetXaxis()->SetMoreLogLabels();
                    h_djmassCut[i][0]->GetYaxis()->SetTitle("A.U.");
                    h_djmassCut[i][0]->GetYaxis()->SetTitleSize(0.025);
                    //h_djmassCut[i][0]->GetXaxis()->SetRangeUser(/*h_djmassCut[i][0]->GetMaximumBin()*/70,14000);
                    h_djmassCut[i][0]->GetXaxis()->SetTitleOffset(1.7);
                    h_djmassCut[i][0]->GetYaxis()->SetTitleOffset(1.7);
                    h_djmassCut[i][0]->Draw("same");
                }

                h_djmassCut[i][j] = (TH1F *)gDirectory->FindObjectAny("hdjmass");
                h_djmassCut[i][j]->SetLineColor(ListColors[j]);
                h_djmassCut[i][j]->SetMarkerColor(ListColors[j]);

                //DATA scale... MarkerStyle...
                if (i == 0)
                    h_djmassCut[0][j]->Scale(1e8);
                if (i == 1)
                    h_djmassCut[1][j]->Scale(1e6);
                if (i == 2)
                    h_djmassCut[2][j]->Scale(1e4);
                if (i == 3)
                    h_djmassCut[3][j]->Scale(1e2);
                if (i == 4)
                    h_djmassCut[4][j]->Scale(1e0);

                if (i == 0)
                    h_djmassCut[0][j]->SetMarkerSize(1.0);
                if (i == 1)
                    h_djmassCut[1][j]->SetMarkerSize(1.0);
                if (i == 2)
                    h_djmassCut[2][j]->SetMarkerSize(1.3);
                if (i == 3)
                    h_djmassCut[3][j]->SetMarkerSize(1.);
                if (i == 4)
                    h_djmassCut[4][j]->SetMarkerSize(1.4);
                h_djmassCut[i][j]->SetMarkerStyle(ListMrkStyle[i]);

                h_djmassCut[i][j]->SetMaximum(2e14);
                h_djmassCut[i][j]->SetMinimum(1e-8);
                //Draw raw dijetMass of jt>40 histograms on the same canvas with jt40
                if (j > 0)
                    h_djmassCut[i][j]->Draw("same");

                //Draw , scale, make-up MC
                if (j == 7)
                {
                    //sum of the integrals of each triggers after cut to scale MC
                    double IntegralDATA = 0;
                    IntegralDATA = h_djmassCut[i][0]->Integral() + h_djmassCut[i][1]->Integral() +
                                   h_djmassCut[i][2]->Integral() + h_djmassCut[i][3]->Integral() +
                                   h_djmassCut[i][4]->Integral() + h_djmassCut[i][5]->Integral() +
                                   h_djmassCut[i][6]->Integral() + h_djmassCut[i][7]->Integral();

                    MC_massSpectrum[i]->Scale(IntegralDATA / MC_massSpectrum[i]->Integral());
                    MC_massSpectrum[i]->SetLineColor(kRed + 2);
                    MC_massSpectrum[i]->SetLineWidth(1);
                    MC_massSpectrum[i]->SetMarkerColor(kRed + 2);
                    MC_massSpectrum[i]->Draw("samehist");

                    h_djmassCut[i][0]->GetXaxis()->SetRangeUser(/*h_djmassCut[i][0]->GetMaximumBin()*/ 88, 14000);
                } //
            }     //j (jet loop)
        }         //i (eta loop)

        leg3->AddEntry(h_djmassCut[0][0], "0.0 < #||{y} < 0.5 (#times 10^{8})", "lp");
        leg3->AddEntry(h_djmassCut[1][0], "0.5 < #||{y} < 1.0 (#times 10^{6})", "lp");
        leg3->AddEntry(h_djmassCut[2][0], "1.0 < #||{y} < 1.5 (#times 10^{4})", "lp");
        leg3->AddEntry(h_djmassCut[3][0], "1.5 < #||{y} < 2.0 (#times 10^{2})", "lp");
        leg3->AddEntry(h_djmassCut[4][0], "2.0 < #||{y} < 2.5 (#times 10^{0})", "lp");
        leg3->AddEntry(MC_massSpectrum[0], "Pythia8 CUETP8M", "l");
        leg3->AddEntry(eta0_up, "JEC+Detector+Lumi Unc.", "f");
        leg3->AddEntry("", "", "");
        leg3->AddEntry("", "Anti-k_{T} R=0.4", "");
        leg3->AddEntry("", "PF+CHS", "");
        leg3->AddEntry("", "Detector Level", "");
        leg3->Draw();

        t1->SetTextSize(0.035);
        t1->DrawLatex(0.1, 0.92, "#bf{CMS} #font[12]{Work in Progress 2016}");

        t2->SetTextSize(0.033);
        t2->DrawLatex(0.71, 0.92, "35.9 fb^{-1} (13 TeV)");

        leg2->AddEntry(h_djmassCut[0][0], "HLT_PFJet40", "p");
        leg2->AddEntry(h_djmassCut[0][1], "HLT_PFJet80", "p");
        leg2->AddEntry(h_djmassCut[0][2], "HLT_PFJet140", "p");
        leg2->AddEntry(h_djmassCut[0][3], "HLT_PFJet200", "p");
        leg2->AddEntry(h_djmassCut[0][4], "HLT_PFJet260", "p");
        leg2->AddEntry(h_djmassCut[0][5], "HLT_PFJet320", "p");
        leg2->AddEntry(h_djmassCut[0][6], "HLT_PFJet400", "p");
        leg2->AddEntry(h_djmassCut[0][7], "HLT_PFJet450", "p");
        leg2->Draw();

        gPad->RedrawAxis();

        c4->SaveAs("pngs/All_DATA-MC_DiJetMassSpectrum_.png");
        c4->Update();
        //        c4->Close();
    } //print_MC_DATA_massSpectrum
} //main

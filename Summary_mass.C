

void Summary_mass(){
    

    
    TString eta_bins[]  = {"0.0", "0.5", "1.0", "1.5", "2.0", "2.5", "3.0", "3.2", "4.7"};
    TString jt[]        = {"40", "80", "140", "200", "260", "320", "400", "450"};
    
    TString scales[]    =  {"10^{6}", "10^{5}", "10^{4}", "10^{3}", "10^{2}"};
    
    gStyle->SetOptStat(0);
    Double_t w = 1200;
    Double_t h = 800;
    
    
    
    TLegend *leg = new TLegend(0.6994992,0.6619355,0.8731219,0.8722581);
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);
    
    TLegend *leg2 = new TLegend(0.1602671,0.1950904,0.3338898,0.4056848,NULL,"brNDC");
    leg2->SetBorderSize(0);
    leg2->SetLineColor(0);
    leg2->SetLineStyle(1);
    leg2->SetLineWidth(0);
    leg2->SetFillColor(0);
    leg2->SetFillStyle(1001);
    
    TLegend *leg3 = new TLegend(0.6994992,0.6619355,0.8731219,0.8722581);
    leg3->SetBorderSize(0);
    leg3->SetLineColor(0);
    leg3->SetLineStyle(1);
    leg3->SetLineWidth(0);
    leg3->SetFillColor(0);
    leg3->SetFillStyle(1001);
    
    
    TString inputFile1, inputFile2;
    inputFile1.Form("output-DATA-2a.root");
    inputFile2.Form("output-MC-2b.root");
    
    
    TH1F *h_RAWdjmass[8];
    TH1F *h_djmassRef[8];
    TH1F *h_djmass[8]   ;
    TH1F *h_djmassCut[8];
    TH1F *h_djmassSummary[8];
    TH1F *h_djmassMC[8];
    
    
    int bin_counter = (sizeof(eta_bins)/sizeof(*eta_bins));
    int counter = (sizeof(jt)/sizeof(*jt));

  
    
    /////////////////////////////////////////////////////////////////////
    //get Turn-On point values wrt corresponding HLT_PFJet
    /////////////////////////////////////////////////////////////////////
    double turnOn[5][7];
    TString etaJet[5][7];
    float FitPar[8][7];
    //float a[4][7];
    
    
    
    for (int i=0; i<4; i++) {
        
        TString bbbb;
        
        for (int j=0; j<7; j++) {
            
            
            //open desired files and directories
            TFile* DATAFile = TFile::Open(inputFile1.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i+1] + "/jt" + jt[j] + "/");
            h_djmassRef[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
            
            DATAFile -> cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i+1] + "/jt" + jt[j+1] + "/");
            h_djmass[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
            
            
            TString xname = (TString)("jt"+jt[j+1]+"/jt"+jt[j]);
            
            h_djmass[j]->Divide(h_djmassRef[j]);
            
            
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
            if ((i == 3) && (j == 3)) FitPar[i][j] = 4.1;   //jt260
            if ((i == 3) && (j == 4)) FitPar[i][j] = 4.4;   //jt320
            if ((i == 3) && (j == 5)) FitPar[i][j] = 4.3;   //jt400
            if ((i == 3) && (j == 6)) FitPar[i][j] = 5.0;   //jt450
            if (i == 4 ) FitPar[i][j] = 4.3;                //jt80 2.0-2.5
            if ((i == 4) && (j == 1) ) FitPar[i][j] = 5.9;  //jt140
            if ((i == 4) && (j == 2) ) FitPar[i][j] = 5.74; //jt200
            if ((i == 4) && (j == 3) ) FitPar[i][j] = 7.3;  //jt260
            if ((i == 4) && (j == 4) ) FitPar[i][j] = 5.9;  //jt320
            if ((i == 4) && (j ==5) ) FitPar[i][j] = 5.21;   //jt400
            if ((i == 4) && (j ==6) ) FitPar[i][j] = 5.5;   //jt450
           
            
           /* //"a" the fit parameter optimisation
            a[i][j]=1.75;
            
            if (i == 1 && j == 6) a[i][j] = 2.0;
            if (i == 2) a[i][j] = 2.0;
            if ((i == 2) && j == 2) a[i][j] = 2.5;
            if ((i ==2) && (j >= 3)) a[i][j] = 2.8;
            if (i == 3) a[i][j] = 3.0;
            if ((i == 3) && (j == 1)) a[i][j] = 3.4;
            if ((i == 3) && (j == 2)) a[i][j] = 4;
            if ((i == 3) && (j == 3)) a[i][j] = 4.1;
            if ((i == 3) && (j == 4)) a[i][j] = 4.1;
            if ((i == 3) && (j == 5)) a[i][j] = 4.3;
            if ((i == 3) && (j == 6)) a[i][j] = 4.5;*/
            
            //Fit function
            TF1 *f1 = new TF1("f1","0.5*(1+TMath::Erf((x-[0])/[1]))",atoi(jt[j+1])*FitPar[i][j],3500.);
            
            f1->SetParameters(2*atoi(jt[j+1])*cosh(0.5),atoi(jt[j+1])*2*0.1);
            f1->SetLineWidth(1);
            h_djmass[j]->Fit(f1,"R");
            f1->GetX(0.99);
            
            turnOn[i][j] = f1->GetX(0.99);
            bbbb = (TString) ("eta_"+eta_bins[i]+"-"+eta_bins[i+1]+"_jt"+jt[j+1]+"/jt"+jt[j]);
            etaJet[i][j] = bbbb;
            
        }// j of TurnOn loop
    }// i of TurnOn plots
    
    /////////////////////////////////////////////////////////////////////
    // cut the right and left of massSpectrum wrt turnOn points
    /////////////////////////////////////////////////////////////////////
    for (int i=0; i<5; i++) {
        
        
        TCanvas * c3 = new TCanvas("c3", "c3", w, h);
        c3->SetLogy();
        c3->SetLogx();
        c3->SetTickx(1);
        c3->SetTicky(1);
        
        
        for (int j=0; j<counter; j++) {
            
            //open desired files and directories
            TFile* DATAFile = TFile::Open(inputFile1.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i+1] + "/jt" + jt[j] + "/");
            h_djmassCut[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
            
            TFile* MCFile = TFile::Open(inputFile2.Data(), "READ");
            MCFile -> cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i+1] + "/");
            h_djmassMC[i] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
            
            
            // cut the right and left
            int ncells = h_djmassCut[j]->GetSize(); //it adds undrfolw and overflow bins so I have to extract those
            int Data_Nbins = ncells-2;
            double x;
            
            for (int k=1; k<=Data_Nbins; k++) {
                double check_bins = h_djmassCut[j]->GetBinContent(k);
                double check_err  = h_djmassCut[j]->GetBinError(k);
                
                if (check_bins != 0){

                    double Xaxis_val = h_djmassCut[0]->GetBinCenter(k);
                    int binmax = h_djmassCut[0]->GetMaximumBin();
                    x = h_djmassCut[0]->GetXaxis()->GetBinCenter(binmax);
                    
                    
                    
                    if (Xaxis_val < x){
                        h_djmassCut[0]->SetBinContent(k,0);
                        h_djmassCut[0]->SetBinError(k,0);
                        h_djmassMC[i]->SetBinContent(k,0);
                        h_djmassMC[i]->SetBinError(k,0);
                        
                    }

                    if ((k<h_djmassCut[j]->GetXaxis()->FindBin(turnOn[i][j-1])) && (j>0)){
                        
                        h_djmassCut[j]->SetBinContent(k, 0.);
                        h_djmassCut[j]->SetBinError(k, 0.);
                    }//cut leftside >jt40
                    
                    if ((k>=h_djmassCut[j]->GetXaxis()->FindBin(turnOn[i][j])) && (j<7)){
                        
                        h_djmassCut[j]->SetBinContent(k, 0.);
                        h_djmassCut[j]->SetBinError(k, 0.);
                    }//cut right side <jt450
                }//check bin
            }//k
            
            //Draw raw dijetMasss of jt40 histogram
            if(j == 0){
                h_djmassCut[0] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
                h_djmassCut[0]->SetLineColor(1);
                h_djmassCut[0]->SetLineWidth(2);
                h_djmassCut[0]->GetXaxis()->SetTitle("M_{jj} (GeV)");
                h_djmassCut[0]->GetYaxis()->SetTitle("A.U.");
                h_djmassCut[0]->GetXaxis()->SetRangeUser(x,10000);
                h_djmassCut[0]->SetMaximum(2e5);
                h_djmassCut[0]->SetMinimum(1e-6);
                h_djmassCut[0]->GetXaxis()->SetTitleOffset(1.2);
                h_djmassCut[0]->GetXaxis()->SetMoreLogLabels(); //to make the x-axis a bit easier to read and see where the axis starts.
                h_djmassCut[0]->GetXaxis()->SetNoExponent();
               // h_djmassCut[0]->Draw();
                h_djmassSummary[i] = (TH1F*)h_djmassCut[0]->Clone("h_djmassSummary");
            }
            
            
            
            //add other entries to TLegend
            h_djmassCut[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
            h_djmassCut[j]->SetLineColor(j+1);
            if (j == 2) h_djmassCut[j]->SetLineColor(kSpring+5);
            if (j == 4) h_djmassCut[j]->SetLineColor(kOrange);
            if (j == 6) h_djmassCut[j]->SetLineColor(kCyan+1);
            if (j == 7) h_djmassCut[j]->SetLineColor(kGreen-2);
            
            h_djmassCut[j]->SetLineWidth(2);
            
            //Draw raw dijetMass of jt>40 histograms on the same canvas with jt40
            
             
            
            if (j>0) h_djmassSummary[i]->Add(h_djmassCut[j]);
           // if (j>0) h_djmassCut[j]->Draw("same");
            
            
           
            
            
            
            //TLegend Settings
            leg->AddEntry(h_djmassCut[j],"HLT_PFJet"+jt[j],"l");
        }//j of cut
        
        
        
        //leg->Draw();
        
        
        
        

        //c3->SaveAs("MassSpectrum_"+eta_bins[i]+"-"+eta_bins[i+1]+"_.png");
        //c3->SaveAs("MassSpectrum_"+eta_bins[i]+"-"+eta_bins[i+1]+"_.png");
        c3->Update();
        leg->Clear(); //After listing one turn of eta clear legend otherwise all eta legends will be stacked together
        
        
        h_djmassSummary[i]->SetMarkerStyle(22+i);
        h_djmassSummary[i]->SetLineColor(1+i);
        
        h_djmassMC[i]->SetLineColor(1);
        //h_djmassMC[i]->SetMarkerStyle(7);
        h_djmassMC[i]->SetLineStyle(2);
        
        
        
        leg2->AddEntry(h_djmassSummary[i],eta_bins[i]+" < |y| < "+eta_bins[i+1]+" (" +scales[i]+ " )","lp");
        //leg3->AddEntry(h_djmassSummary[i]," DATA2016 ","lp");
        leg3->AddEntry(h_djmassMC[i]," Pythia ","lp");
        if (i != 4) leg3->Clear();
        
        //if (i>0) h_djmassSummary[i]->Draw("same");

        
        
   // h_djmassSummary[i]->Draw();    
    }//i of MassSpectrum after cutting right and left
    
    ///// Making one Summary Plot for all ETA bins ////
        
        TCanvas * c4 = new TCanvas("c4", "c4", w, h);
        c4->SetLogy();
        c4->SetLogx();
        c4->SetTickx(1);
        c4->SetTicky(1);
        
        h_djmassSummary[0]->SetMinimum(1e-20);
        //h_djmassSummary[0]->GetXaxis()->SetRangeUser(100,7000);
        h_djmassSummary[0]->Scale(1000000);       
        h_djmassMC[0]->Scale(h_djmassSummary[0]->Integral()/h_djmassMC[0]->Integral());
     
        h_djmassSummary[0]->Draw();
        h_djmassMC[0]->Draw("histsame");
        
        
        h_djmassSummary[1]->Scale(100000);
        h_djmassMC[1]->Scale(h_djmassSummary[1]->Integral()/h_djmassMC[1]->Integral());

        h_djmassSummary[1]->Draw("same");
        h_djmassMC[1]->Draw("histsame");
        
        
        h_djmassSummary[2]->Scale(10000);
        h_djmassMC[2]->Scale(h_djmassSummary[2]->Integral()/h_djmassMC[2]->Integral());
   
        h_djmassSummary[2]->Draw("same");
        h_djmassMC[2]->Draw("histsame");
        
        
        
        h_djmassSummary[3]->Scale(1000);
        h_djmassMC[3]->Scale(h_djmassSummary[3]->Integral()/h_djmassMC[3]->Integral());
   
        
        h_djmassSummary[3]->Draw("same");
        h_djmassMC[3]->Draw("histsame");
        
        h_djmassSummary[4]->Scale(100);
        h_djmassMC[4]->Scale(h_djmassSummary[4]->Integral()/h_djmassMC[4]->Integral());
        h_djmassSummary[4]->Draw("same");
        h_djmassMC[4]->Draw("histsame");
        
        TLatex *t6 = new TLatex();
        t6->SetNDC();
        t6->SetTextFont(42);
        t6->SetTextSize(0.035);
        t6->SetTextAlign(11);
        t6->DrawLatex(0.1,0.92,"#bf{CMS} Preliminary, #sqrt{s}=13 TeV");
        
        
        
        
        leg2->Draw();
        leg3->Draw();
        
        //c3->SaveAs("Summary_"+eta_bins[i]+"-"+eta_bins[i+1]+"_.png");
        c4->SaveAs("Summary.png");
    
        
}

from ROOT import TFile, TF1, TUnfold, TUnfoldDensity
from ROOT import gDirectory, TH1F, TH1D, TH2D, TGraph, TSpline3, TH2F
from ROOT import gROOT, TCanvas, gStyle, TPad, TLine, TLegend
from ROOT import TMath, TText, TLatex, gApplication, gPad
from array import *
import math,sys
import ROOT
import subprocess

gStyle.SetOptStat(0)
gStyle.SetPaintTextFormat("5.1f")

### RESPONSE matrix ####
label = sys.argv[1]
print label
if (label=='MC'):
   toyMC = TFile("output-MC-PtHat-1.root")
elif (label=='MCherwig'):
   toyMC = TFile('TTbar-Events-Herwig.root')
elif (label=='MCfxfx'):
   toyMC = TFile('TTbar-Events-CUEP8M2T4-FxFx.root')
elif (label=='MCcuetm2'):
   toyMC = TFile('TTbar-Events-CUETP8M2-UpJER-CentralBTagging.root')
else:
    print 'Allowed arguments are MCcuetm1, MCherwig, MCfxfx, MCcuetm2'
    quit()

dataFile = TFile("hist.root")
global bineta
bineta = ["0.0", "0.5", "1.0", "1.5", "2.0", "2.5"]
numberBinsGen = [24, 23, 22, 20, 17]
numberBinsReco = [51, 49, 46, 42, 37]
 
for i in range(0,5):

    toyMC.cd('Standard/Eta_'+bineta[i]+'-'+bineta[i+1]+'/mc')
    resM = gDirectory.Get('matrix_gen_reco')
    #resM.Draw()
    #dLevelMC = dataFile.Get('mc_etabin' +str(i)+'_mass') # mc gen level  
    dLevelMC = gDirectory.Get('gen_hdjmass')
    #dLevelMC.Draw() 
    ## RECO level data and covariance matrix ###
    #dLevel = dataFile.Get('data_etabin' +str(i)+'_mass') # mc reco level 
    dLevel = gDirectory.Get('hdjmass')
    #dLevel.Draw()
    #dLevel.Scale(7539.457)

    print "Unfolding dijet mass spectra"

    #nbinsGen = 8
    #nbinsRec = 16
    nbinsGen = numberBinsGen[i]
    nbinsRec = numberBinsReco[i]

    ### Correlation matrix after unfolding
    #genbinning = [395, 548, 737, 967, 1248, 1588, 2000, 2500, 4252]
    genbinning = [[67, 88, 119, 156, 197, 244, 296, 354, 419, 489, 565, 649, 740, 838, 944, 1181, 1607, 2132, 2775, 3558, 4509, 5663, 7060, 8752, 10798], [88, 119, 156, 197, 244, 296, 354, 419, 489, 565, 649, 740, 838, 944, 1181, 1607, 2132, 2775, 3558, 4509, 5663, 7060, 8752, 10798], [137, 176, 220, 270, 325, 386, 453, 526, 606, 693, 788, 890, 1000, 1383, 1856, 2438, 3147, 4010, 5058, 6328, 7866, 9726, 11977],[220, 270, 325, 386, 453, 526, 606, 693, 788, 890, 1000, 1383, 1856, 2438, 3147, 4010, 5058, 6328, 7866, 9726, 11977], [354, 419, 489, 565, 649, 740, 838, 944, 1181, 1607, 2132, 2775, 3558, 4509, 5663, 7060, 8752, 10798]]
 
    correlationMatrix = TH2D("correlationMatrix"+str(i)+"bin","correlationMatrix"+str(i)+"bin",nbinsGen,array('d', genbinning[i]),nbinsGen,array('d', genbinning[i]))

    #recobinning = [395, 471.5, 548, 642.5, 737, 852, 967, 1107.5, 1248, 1418, 1588, 1794, 2000, 2250, 2500, 3376, 4252]
    recobinning = [[67, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1181, 1383, 1607, 1856, 2132, 2438, 2775, 3147, 3558, 4010, 4509, 5058, 5663, 6328, 7060, 7866, 8752, 9726, 10798, 11977, 13272, 14000], [88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1181, 1383, 1607, 1856, 2132, 2438, 2775, 3147, 3558, 4010, 4509, 5058, 5663, 6328, 7060, 7866, 8752, 9726, 10798, 11977, 13272, 14000], [137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1181, 1383, 1607, 1856, 2132, 2438, 2775, 3147, 3558, 4010, 4509, 5058, 5663, 6328, 7060, 7866, 8752, 9726, 10798, 11977, 13272, 14000], [220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1181, 1383, 1607, 1856, 2132, 2438, 2775, 3147, 3558, 4010, 4509, 5058, 5663, 6328, 7060, 7866, 8752, 9726, 10798, 11977, 13272, 14000], [354, 386, 419, 453,489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1181, 1383, 1607, 1856, 2132, 2438, 2775, 3147, 3558, 4010, 4509, 5058, 5663, 6328, 7060, 7866, 8752, 9726, 10798, 11977, 13272, 14000]]


    dCovMatrix =  TH2D("dCovMatrix"+str(i)+"bin","dCovMatrix"+str(i)+"bin",nbinsRec,array('d', recobinning[i]),nbinsRec,array('d', recobinning[i]))
    for t in range(1,nbinsRec):
        dCovMatrix.SetBinContent(t,t,dLevel.GetBinError(t))    

    ## SET ERRORS OF DETECTOR LEVEL SPECTRA WITH COVARIANCE MATRIX OF THE MEASUREMENT  
    unfold = TUnfoldDensity(resM,TUnfold.kHistMapOutputVert,TUnfold.kRegModeNone,TUnfold.kEConstraintNone)

    ## SET INPUT HERE
    unfold.SetInput(dLevel,0,0,dCovMatrix)

    reguScan= False
    fixedTau = False
    noReg = True

    if reguScan:
    ##Regularize Curvature (it is done when the spectrum is rapidly falling)
        for b in range(2,nbinsGen):
            fnlbins = b
            down = dLevelMC.GetBinContent(fnlbins-1)
            up = dLevelMC.GetBinContent(fnlbins+1)
            
        unfold.RegularizeCurvature(b-1, b, b+1, 1.00/down,1.00/up)
        
        nScan=30;
        logTauX = TSpline3()
        logTauY = TSpline3()
        lCurve = TGraph()
        
        iBest=unfold.ScanLcurve(nScan,1e-04,1e-03,lCurve,logTauX,logTauY);
        print "( " + str(unfold.GetChi2A()) + "+" + str(unfold.GetChi2L()) +  ") / " +  str(unfold.GetNdf())
        print "Tau : ", unfold.GetTau()
        
        folderName = "ReguScan_"+str(unfold.GetTau())+"y-fineGEN"+label
        subprocess.call(["mkdir",folderName])
        
        l =[0.00,0.00]
        x=array('d', l)
        y=array('d', l)
        t=array('d', l)

        #print iBest
        logTauX.GetKnot(iBest,t,x)
        logTauY.GetKnot(iBest,t,y)

        bestLCurve = TGraph(1,x,y)
        bestLogTauLogChi2 = TGraph(1,t,x)
        bestLogTauLogChi2L2 = TGraph(1,t,y)
        
        c1 = TCanvas()
        lCurve.Draw("AL")
        bestLCurve.SetMarkerColor(2)
        bestLCurve.Draw("*")
        c1.Print(folderName+"/Lcurve.pdf")

        c4 = TCanvas()
        logTauX.Draw()
        bestLogTauLogChi2.SetMarkerColor(2)
        bestLogTauLogChi2.Draw("*")
        c4.Print(folderName+"/bestLogtauL1"+str(unfold.GetTau())+".pdf")

        c5 = TCanvas()
        logTauY.Draw()
        bestLogTauLogChi2L2.SetMarkerColor(2)
        bestLogTauLogChi2L2.Draw("*")
        c5.Print(folderName+"/bestLogtauL2"+str(unfold.GetTau())+".pdf")
        
    elif fixedTau :
        ##Regularize Curvature
        for b in range(2,nbinsGen):
            fnlbins = b
            down = dLevelMC.GetBinContent(fnlbins-1)
            up = dLevelMC.GetBinContent(fnlbins+1)
	#print 1.00/down
        unfold.RegularizeCurvature(b-1, b, b+1, 1.00/down,1.00/up)

        unfold.DoUnfold(0.01)
        print "( " + str(unfold.GetChi2A()) + "+" + str(unfold.GetChi2L()) +  ") / " +  str(unfold.GetNdf())
        print "Tau : ", unfold.GetTau()
        folderName = "FixedTau_"+str(unfold.GetTau())+"y"+label
        subprocess.call(["mkdir",folderName])

    elif noReg :
        unfold.DoUnfold(0.00)
        print "( " + str(unfold.GetChi2A()) + "+" + str(unfold.GetChi2L()) +  ") / " +  str(unfold.GetNdf())
        folderName = "NoReg_"+str(unfold.GetTau())+"_y"+label
        subprocess.call(["mkdir",folderName])
        if (i=='0.0'):
            subprocess.call(["rm",folderName+"/unfoldedMjjSpectra0.0.root"])
        
    unfoldedHisty1 = unfold.GetOutput("HistoOutput"+str(i)+"bin")
    unfoldedErrory1 = unfold.GetEmatrixInput("unfolding stat error matrix")

    for ii in range(1,nbinsGen+1):
        ei = math.sqrt(unfoldedErrory1.GetBinContent(ii,ii))
        unfoldedHisty1.SetBinError(ii,math.sqrt(unfoldedErrory1.GetBinContent(ii,ii)))
        if ei == 0:
            continue
        for j in range(1,nbinsGen+1):
            ej = math.sqrt(unfoldedErrory1.GetBinContent(j,j))
            if ej == 0:
                continue
            correlationMatrix.SetBinContent(ii,j,unfoldedErrory1.GetBinContent(ii,j)/ei/ej)

    lx = unfold.GetLxMinusBias("Lx"+str(i)+"bin")
    c2 = TCanvas()
    lx.Draw()
    c2.Print(folderName+"/Lx"+str(i)+"Bin.pdf")

    c3 = TCanvas()
    correlationMatrix.GetXaxis().SetMoreLogLabels()
    correlationMatrix.GetYaxis().SetMoreLogLabels()
    correlationMatrix.GetYaxis().SetNoExponent()
    correlationMatrix.GetXaxis().SetNoExponent()

    correlationMatrix.SetTitle("#tau : "+str(unfold.GetTau()))
    correlationMatrix.Draw("colz")
    c3.SetLogy()
    c3.SetLogx()

    c3.Print(folderName+"/"+str(i)+"binsCorr"+str(unfold.GetTau())+".pdf")

    f = TFile(folderName+"/unfoldedSpectra-Mjj-Tau_"+str(unfold.GetTau())+".root","update")

    unfoldedHisty1.Write()
    dLevel.Write("hdjmass_bin"+str(i))
    resM.Write("matrix_gen_reco_bin"+str(i))
    correlationMatrix.Write()
    lx.Write()
    f.Close()

# SMPJ_Plotters
Various source codes to plot histograms from SMP-Jet official NTuples.

General notes;
    Please use the last version of ROOT.
    These source codes can be used both locally and on LXPLUS.
    If you change the Data versions do not forget Legends :)

Future updates;
    Bash script to run all source code with one command line.

How to use;
    Basically do "root -l -b -q <X.cpp>"

---rawMassMerged.cpp---
Source code use only one Data version.First, change and "SELECT" the Data version you want to use. Data version should be "normalized".
Then source code do following;
    Plotting raw mass distributions for each jet trigger.
    Reconstructing "Dijet Mass Spectrum" then plotting.

---fit_v1.cpp---
Source code use two Data version and MC (versions should be "normalized") then do following;
    Plotting TurnOn point histograms for each jet trigger and rapidity bins.
    Reconstructing "Dijet Mass Spectrum" then comparing Data versions with MC.

---smp_plotter_pt.cpp---
Source code use two Data version and MC (versions should be "combined") then do following;
    Plotting raw pT spectrum, pt of inclusive leading and subleading jets for each rapidity bins.

---smp_plotter_fractions.cpp---
Source code use two Data version and MC (versions should be "combined") then do following;
    Plotting fractions for each rapidity bins.

---fraction_by_triggers.cpp---
Source code use only one Data version and trigger simulated MC (versions should be "normalized"). First, change and "SELECT" the Data version you want to use.
Then source code do following;
    Plotting fractions for Data and MC. By default it plots only CHF for each triggers and rapidity bins.

# SMPJ_Plotters
Various source codes to plot histograms from SMP-Jet official NTuples.

General notes;
    Please use the last version of ROOT.
    These source codes can be used both locally and on LXPLUS.
    Please create  pngs/ and datas/ folders since the new source codes needs them. Please add your output's to datas folder. Or change the input and output paths in cpp's.

Future updates;
    Basic ratio plots for mass and ratio. Probably will do it using TRatioPlot class.
    Improve Summary_unc.cpp.

How to use;
    Basically do "root -l -b -q <X.cpp>"

_unc_pt_ratio.cpp                                       --> Plot pTs with total unc. on hand.
_unc_pt_ratio_seperate_sources.cpp         --> Plot pTs with seperate unc. sources.
_unc_mass_ratio.cpp                                  --> Plot mass with total unc on hand.
_unc_mass_ratio_seperate_sources.cpp    --> Plot mass with seperate unc. sources.
summary.cpp                                               --> Plot Summary without unc.
summary_unc.cpp                                       --> Plot Summary with unc.


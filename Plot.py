###########################################
# LZ OD-OCS testing: Plot script for NIM paper.
# Harvey Birch 
# hbirch@hep.ph.liv.ac.uk
###########################################
#import ROOT as R
from ROOT import *
import math
import sys 

filename = sys.argv[1]
print("Processing file: " + filename)
f = TFile(filename,"read")

H_fpgaNum = f.Get("fpgaNum")
fpgaNumI = int(H_fpgaNum.GetMean())
fpgaNum = str(fpgaNumI)

H_PBNum = f.Get("PBNum")
PBNumI = int(H_PBNum.GetMean())
PBNum = str(PBNumI)

c1 = TCanvas('c1','TrigWvslnNph',1920,1080)
H_TrigW = f.Get("lnnph mean vs TriggerWidth mean")  
H_TrigW.Draw()
gStyle.SetOptStat(0)
gStyle.SetOptFit(0111)
H_TrigW.SetMarkerStyle(20)
Title = "Trigger width versus ln(Number of photons per pulse) for FPGA{}CH{}".format(fpgaNum,PBNum)
H_TrigW.SetTitle(Title)
H_TrigW.SetXTitle('ln(Number of photons per pulse)')
H_TrigW.SetYTitle('Trigger width (ns)')
c1.SetGrid()
c1.Update()
c1fname = "FPGA{}_CH{}_lnNPHvsTrigW.png".format(fpgaNum,PBNum)
c1.Print(c1fname)

c2 = TCanvas('c2','NphvsPulseW',1920,1080)
H_PulseW = f.Get("PMTPulseWidth mean vs nph mean")  
H_PulseW.Draw()
H_PulseW.SetMarkerStyle(20)
Title = "Number of photons per pulse versus PMT Pulse width for FPGA{}CH{}".format(fpgaNum,PBNum)
H_PulseW.SetTitle(Title)
H_PulseW.SetXTitle('PMT Pulse width (ns)')
H_PulseW.SetYTitle('Number of photons per pulse')
c2.SetGrid()
c2.Update()
c2fname = "FPGA{}_CH{}_NPHvsPulseW.png".format(fpgaNum,PBNum)
c2.Print(c2fname)

c3 = TCanvas('c3','PD_Value',1920,1080)
H_PD_Value = f.Get("pdValue vs nph")  
H_PD_Value.Draw()
H_PD_Value.SetMarkerStyle(20)
Title = "Photodiode output versus Number of photons per pulse for FPGA{}CH{}".format(fpgaNum,PBNum)
H_PD_Value.SetTitle(Title)
H_PD_Value.SetYTitle('Number of photons per pulse')
H_PD_Value.SetXTitle('Photodiode output')
c3.SetGrid()
c3.Update()
c3fname = "FPGA{}_CH{}_PD_ValuevsNPH.png".format(fpgaNum,PBNum)
c3.Print(c3fname)

c4 = TCanvas('c4','PD_ValuevsPMTPW',1920,1080)
H_PD_ValuevsPMTPW = f.Get("PMTPulseWidth mean vs pdValue")  
H_PD_ValuevsPMTPW.Draw()
H_PD_ValuevsPMTPW.SetMarkerStyle(20)
Title = "PMT Pulse width versus Photodiode output for FPGA{}CH{}".format(fpgaNum,PBNum)
H_PD_ValuevsPMTPW.SetTitle(Title)
H_PD_ValuevsPMTPW.SetXTitle('PMT Pulse width (ns)')
H_PD_ValuevsPMTPW.SetYTitle('Photodiode output')
c4.SetGrid()
c4.Update()
c4fname = "FPGA{}_CH{}_PMTPWvsPD_Value.png".format(fpgaNum,PBNum)
c4.Print(c4fname)

c5 = TCanvas('c5','TWvsPMTPW',1920,1080)
H_TWvsPMTPW = f.Get("Trigger Width mean vs PMTPulseWidth mean")  
H_TWvsPMTPW.Draw()
H_TWvsPMTPW.SetMarkerStyle(20)
Title = "Trigger width versus PMT pulse width for FPGA{}CH{}".format(fpgaNum,PBNum)
H_TWvsPMTPW.SetTitle(Title)
H_TWvsPMTPW.SetXTitle('Trigger width (ns)')
H_TWvsPMTPW.SetYTitle('PMT Pulse width (ns)')
c5.SetGrid()
c5.Update()
c5fname = "FPGA{}_CH{}_TriggerWidthvsPMTPulseWidth.png".format(fpgaNum,PBNum)
c5.Print(c5fname)


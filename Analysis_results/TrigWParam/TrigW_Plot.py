###########################################
# LZ OD-OCS testing: Plot script for NIM paper.
# Harvey Birch
# hbirch@hep.ph.liv.ac.uk
###########################################
#import ROOT as R
from ROOT import TCanvas, TGraph, TMultiGraph
from ROOT import gROOT
import math
from array import array
import numpy as np

c1 = TCanvas( 'c1', 'c1', 3840, 2160)
c1.SetGrid()

#a,b,c,d,e,f,g = np.loadtxt('Values.txt',delimiter=',', unpack=True) # a = 1k, b = 10k, c = 50k, d = 100k, e = 250k, f = 500k and g = 1M
x = array('d',[1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15.,16.,17.,18.,19.,20.,21.,22.,23.,24.,25.,26.,27.,28.,29.,30.,31.,32.,33.,34.,35.,36.,37.,38.,39.,40.])

n = 40
#x = array('d')
#for i in range( 1, 40 ):
#   x.append( i )

a = array('d',[24.56, 22.49, 23.81, 22.46, 23.31, 21.46, 22.69, 21.53, 23.3, 22.97, 22.71, 23.13, 22.13, 23.44, 21.37, 20.21, 22.28, 22.87, 22.33, 24.73, 22.42, 23.04, 21.36, 20.92, 21.93, 23.19, 22.71, 23.63, 21.77, 22.64, 21.29, 22.2, 22.73, 23.96, 24.08, 24.1, 22.81, 22.83, 23.35, 22.])
a_bar = np.mean(a)

b = array('d',[28.09, 26.11, 27.36, 26.24, 27.15, 25.38, 25.73, 25.53, 26.99, 26.71, 26.42, 26.75, 26.66, 27.22, 25.42, 24.85, 26.19, 26.06, 26.31, 28.64, 26.55, 26.57, 25.55, 25.03, 26.15, 27.23, 26.66, 27.54, 25.92, 26.11, 25.67, 26.04, 27.55, 28.2, 27.84, 27.86, 26.70,26.86, 26.69, 25.84])
b_bar = np.mean(b)

c = array('d',[32.76, 31.31, 32.53, 31.53, 31.58, 30.69, 30.01, 31.08, 32.63, 31.47, 31.22, 31.99, 32.08, 31.82, 30.1, 30.22, 31.18, 30.75, 31., 34.32, 32.11, 30.99, 30.53, 29.99, 31.22, 32.62, 31.54, 32.9, 30.73, 31.08, 30.6, 30.74, 33.15, 33.56, 33.26, 32.84, 31.53, 31.3, 30.95, 29.99])
c_bar = np.mean(c)

d = array('d',[43.13, 34.85, 36.3, 35.11, 34.9, 34.22, 33.18, 34.79, 36.79, 34.95, 34.74, 35.89, 35.97, 35.03, 33.4, 33.99, 34.71, 34.22, 34.41, 38.33, 36.12, 34.18, 34.1, 33.49, 34.85, 36.52, 35.03, 36.8, 34.25, 34.7, 34.14, 34.14, 37.11, 37.31, 37.21, 36.47, 34.97, 34.53, 34.1, 32.92])
d_bar = np.mean(d)

e = array('d',[43.13, 41.52, 44.01, 41.86, 41.9, 40.8, 39.81, 41.69, 45.39, 42.15, 42.07, 44.08, 43.93, 41.46, 40.07, 41.61, 41.87, 41.43, 41.47, 46.18, 44.36, 40.71, 41.4, 40.56, 42.29, 44.55, 42.18, 44.85, 41.58, 42.18, 41.37, 41.11, 45.19, 44.78, 45.34, 43.99, 41.97, 41.21, 40.69, 38.84])
e_bar = np.mean(e)

f = array('d',[51.67, 48.76, 53.33, 49.23, 50.57, 47.93, 48.04, 49.12, 55.85, 50.99, 51.12, 54.29, 53.62, 49.06, 48.05, 50.74, 50.46, 50.33, 50.1, 55.19, 54.4, 48.63, 50.25, 49.03, 51.32, 54.36, 50.83, 54.72, 50.58, 51.3, 50.14, 49.59, 54.93, 53.61, 55., 53.23, 50.4, 49.35, 48.84, 45.92])
f_bar = np.mean(f)

g = array('d',[65.2,  58.83, 67.89, 59.57, 64.4, 57.85, 61.28, 59.38, 72.24, 65.07, 65.61, 70.79, 68.88, 60.76, 60.42, 64.9, 63.85, 64.51, 63.73, 68.48, 70.28, 61.09, 64.14, 62.17, 65.52, 69.9, 64.43, 70.42, 64.85, 65.71, 63.85, 62.95, 70.18, 67.17, 70.71, 67.94, 63.58, 62.13, 61.86, 56.83])
g_bar = np.mean(g)

name = ["FPGA1CH1","FPGA1CH2","FPGA1CH3","FPGA1CH4","FPGA1CH5","FPGA1CH6","FPGA1CH7","FPGA1CH8","FPGA2CH1","FPGA2CH2","FPGA2CH3","FPGA2CH4","FPGA2CH5","FPGA2CH6","FPGA2CH7","FPGA2CH8","FPGA3CH1","FPGA3CH2","FPGA3CH3","FPGA3CH4","FPGA3CH5","FPGA3CH6","FPGA3CH7","FPGA3CH8","FPGA4CH1","FPGA4CH2","FPGA4CH3","FPGA4CH4","FPGA4CH5","FPGA4CH6","FPGA4CH7","FPGA4CH8","FPGA5CH1","FPGA5CH2","FPGA5CH3","FPGA5CH4","FPGA5CH5","FPGA5CH6","FPGA5CH7","FPGA5CH8"]



ag = TGraph( n, x, a )
ag.SetMarkerColor(2)
ag.SetMarkerStyle(20)

bg = TGraph( n, x, b )
bg.SetMarkerColor(3)
bg.SetMarkerStyle(20)

cg = TGraph( n, x, c )
cg.SetMarkerColor(4)
cg.SetMarkerStyle(20)

dg = TGraph( n, x, d )
dg.SetMarkerColor(6)
dg.SetMarkerStyle(20)

eg = TGraph( n, x, e )
eg.SetMarkerColor(7)
eg.SetMarkerStyle(20)

fg = TGraph( n, x, f )
fg.SetMarkerColor(1)
fg.SetMarkerStyle(20)

gg = TGraph( n, x, g )
gg.SetMarkerColor(8)
gg.SetMarkerStyle(20)

mg = TMultiGraph("mg","mg")
mg.Add(ag,"P")
mg.Add(bg,"P")
mg.Add(cg,"P")
mg.Add(dg,"P")
mg.Add(eg,"P")
mg.Add(fg,"P")
mg.Add(gg,"P")
mg.Draw("A PMC")
mg.SetTitle("Specific trigger width for each channel for different number of photons.; Pulser board number; Trigger width (ns)");
mg.Draw("A PMC")

c1.Update()
c1.Print("Global_TrigW.png")
c1.Print("Global_TrigW.root")

# here we load the shared library created using the Makefile
from ROOT import gSystem
gSystem.Load("Analysis.so")

# now we can create instance of the class EventLoop
from ROOT import EventLoop
eventLoop = EventLoop()

import sys
fname = sys.argv[1]
#fname = 'FPGA1_CH1_Freq4000.0khz_Date_20191106_13_22_31.root'

# set the attributes. All public attributes are accessible
eventLoop.treeName = "tree"

# The data are stored in multiple files
eventLoop.inputFiles.push_back('/Users/harveybirch/Desktop/FST_SEP/' + fname)

# initialize and execute the event loop
eventLoop.initialize()
eventLoop.execute()

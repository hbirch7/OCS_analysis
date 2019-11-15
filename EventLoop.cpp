#include "EventLoop.h"
#include <iostream>
#include <stdexcept>
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH2F.h"
#include "TPaveStats.h"
#include "TStyle.h"


using namespace std;

EventLoop::EventLoop() {
}

//Declaring fitting functions
Double_t fitf0(Double_t *x,Double_t *par);
Double_t fitf0(Double_t *x,Double_t *par){
  Double_t arg = x[0];
  Double_t tw = par[0] + TMath::Exp(par[1]*arg + par[2]*arg*arg + par[3]*arg*arg*arg);
  return tw;
}

Double_t fitf1(Double_t *x,Double_t *par);
Double_t fitf1(Double_t *x,Double_t *par){
  Double_t arg = x[0];
  Double_t tw = par[0] + TMath::Log(par[1]*arg - par[2]*arg*arg + par[3]*arg*arg*arg);
  return tw;
}

void EventLoop::initialize() {
 // create an instance of the TChain class
 m_chain = new TChain(treeName);
 
 // loop through the input files and add them to the chain
 for(auto inputFile : inputFiles) {
   m_chain->Add(inputFile);
   std::cout << "Added file: " << inputFile << std::endl;
 }
}


// Iterate from 0 (smallest possible set point)  to  n (largest possible set point) in increments of 1 for every possible set point.
int EventLoop::calcWidthSetTotal(int widthSetLarge, int widthSetSmall){
  return ((widthSetLarge-5) * 64) + widthSetSmall;
}

void EventLoop::execute() {
 // sanity check. m_chain must not be zero
 if(!m_chain) {
   throw std::runtime_error("Calling execute while the event loop was not initialized.");
 }

 

 // first we create a c++ variables for the entries in the root file
 Double_t        nph;
 Double_t        nphe;
 Double_t        tsec;
 Double_t        temp;
 Double_t        width1; // Electrical Width
 Double_t        triggerWidth; // Electrical Width
 Double_t        amp1;
 Double_t        charge1;
 Double_t        intg1;
 Double_t        pkpos1;
 Double_t        width2;  // PMT Pulse Width
 Double_t        PMTPulseWidth;  // PMT Pulse Width
 Double_t        amp2;
 Double_t        charge2;
 Double_t        intg2;
 Double_t        pkpos2;
 Double_t        width3;
 Double_t        widthsetLarge;
 Double_t        widthsetSmall;
 Double_t        vref;
 Double_t        fpgaBoardNumOut;
 Double_t        pulserboardnumOut;
 Double_t        pdValue;




 //then we pass them into the tree
 m_chain->SetBranchAddress("nph", &nph);
 m_chain->SetBranchAddress("nphe", &nphe);
 m_chain->SetBranchAddress("tsec", &tsec);
 m_chain->SetBranchAddress("temp", &temp);
 m_chain->SetBranchAddress("width1", &width1);
 m_chain->SetBranchAddress("width1", &triggerWidth);
 m_chain->SetBranchAddress("amp1", &amp1);
 m_chain->SetBranchAddress("charge1", &charge1);
 m_chain->SetBranchAddress("intg1", &intg1);
 m_chain->SetBranchAddress("pkpos1", &pkpos1);
 m_chain->SetBranchAddress("width2", &width2);
 m_chain->SetBranchAddress("width2", &PMTPulseWidth);
 m_chain->SetBranchAddress("amp2", &amp2);
 m_chain->SetBranchAddress("charge2", &charge2);
 m_chain->SetBranchAddress("intg2", &intg2);
 m_chain->SetBranchAddress("pkpos2", &pkpos2);
 m_chain->SetBranchAddress("width3", &width3);
 m_chain->SetBranchAddress("widthsetLarge", &widthsetLarge);
 m_chain->SetBranchAddress("widthsetSmall", &widthsetSmall);
 m_chain->SetBranchAddress("vref", &vref);
 m_chain->SetBranchAddress("fpgaBoardNumOut", &fpgaBoardNumOut);
 m_chain->SetBranchAddress("pulserboardnumOut", &pulserboardnumOut);
 m_chain->SetBranchAddress("pdValue", &pdValue);

  
 for(int eventNum = 1; eventNum <= 1; ++eventNum) { //Looping over first event to get FPGANum and PBNum for output file name
   m_chain->GetEntry(eventNum); // Get the event data
   int fpgaNum = fpgaBoardNumOut; 
   int PBNum = pulserboardnumOut;
   string filenamestr = "FPGA" + to_string(fpgaNum) + "_" + "CH" + to_string(PBNum) + "_Results.root";
   char filename[filenamestr.size() + 1]; //String to Char converson for filename
   filenamestr.copy(filename, filenamestr.size() + 1);
   filename[filenamestr.size()] = '\0';

   // make the output file to save histograms to
   TFile *outFile = new TFile(filename,"RECREATE");
 
   // create a subdirectory "PMTPulses" in this file
   TDirectory *dnph = outFile->mkdir("nph");
   TDirectory *dPMTPulses = outFile->mkdir("PMTPulses");
   TDirectory *dTrigPulses = outFile->mkdir("TriggerPulses");


   // First plot things like nph, trigger width and pmt pulse width for each set point
   // and find the mean with a gaussian

   // DEFINE SOME HISTOGRAMS
   TH1F *H_nph = new TH1F("h_nph", "nph", 75000, 0, 750000);
   TH1F *H_nph_stdDev = new TH1F("h_nph_stdDev", "nph stdDev", 500, 0, 250);

   TH1F *H_triggerWidth = new TH1F("triggerWidth", "triggerWidth", 3000, 0, 75);
   TH1F *H_PMTPulseWidth = new TH1F("pmtPulseWidth", "PMT Pulse Width", 750, 0, 37.5);

   TH1F *H_triggerWidth_div_PMTPulseWidth = new TH1F("triggerWidth_div_PMTPulseWidth", "triggerWidth_div_PMTPulseWidth", 500, 0, 10);

   TH2F *H_TriggerWidth_vs_nph = new TH2F("TriggerWidth mean vs nph mean", "TriggerWidth mean vs nph mean", 750, 0, 75, 1000, 0, 750000);
   TH2F *H_nph_vs_TriggerWidth = new TH2F("nph mean vs TriggerWidth mean", "nph mean vs TriggerWidth mean", 1000, 0, 750000, 750, 0, 75);
   TH2F *H_lnnph_vs_TriggerWidth = new TH2F("lnnph mean vs TriggerWidth mean", "lnnph mean vs TriggerWidth mean", 1000, 0, 20, 750, 0, 75);
   TH2F *H_PMTPulseWidth_vs_nph = new TH2F("PMTPulseWidth mean vs nph mean", "PMTPulseWidth mean vs nph mean", 750, 0, 75, 1000, 0, 750000);

   TH2F *H_TrigWidthmean_vs_PMTPulseWidthmean = new TH2F("Trigger Width mean vs PMTPulseWidth mean", "Trigger Width mean vs PMTPulseWidth mean", 150, 0, 75, 50, 0, 25);

   TH2F *H_pdValue_vs_nph = new TH2F("pdValue vs nph", "pdValue vs nph", 50, 0, 5, 1000, 0, 750000);
   TH2F *H_tsec_vs_pdValue = new TH2F("tsec vs pdValue", "tsec vs pdValue", 70000, 0, 7000, 50, 0, 5);//
   TH2F *H_PMTPulseWidth_vs_pdValue = new TH2F("PMTPulseWidth mean vs pdValue", "PMTPulseWidth mean vs pdValue", 750, 0, 75, 50, 0, 5);//
   TH2F *H_totalWidthSet_vs_pdValue = new TH2F("totalWidthSet vs pdValue", "total WidthSet vs pdValue", 750, 0, 750, 5, 0, 5);

   TH2F *H_tsec_vs_SmallWidthSet = new TH2F("tsec_vs_SmallWidthSet", "tsec vs SmallWidthSet", 500, 0, 25000, 71, 0, 70);
   TH1F *H_SmallWidthSet = new TH1F("SmallWidthSet", "SmallWidthSet", 71, -0.5, 70.5);
   TH1F *H_LargeWidthSet = new TH1F("LargeWidthSet", "LargeWidthSet", 32, -0.5, 15.5);

   TH1F *H_fpgaNum = new TH1F("fpgaNum", "fpgaNum", 10, 0, 9);
   TH1F *H_PBNum = new TH1F("PBNum", "PBNum", 10, 0, 9);

   int numMeasurementsAtEachPoint = 4000; //This is need for Full range widthsetLarge[1-63] widthsetSmall[5-11] (ie Slow test procedure)
   //int numMeasurementsAtEachPoint = 10000; //This is need for Full range widthsetLarge[1-63] widthsetSmall[5-11] (ie Slow test procedure)
   // int numMeasurementsAtEachPoint = 1000;  //This is need for short range widthsetLarge[1,15,45] widthsetSmall[5-11]  (ie Fast test procedure)

   int repeatReadingAtWidth = 0;

   // EVENT LOOP - Getting the Mean of each set point
   for(int eventNum = 0; eventNum < m_chain->GetEntries(); ++eventNum) {
     m_chain->GetEntry(eventNum); // Get the event data

     H_tsec_vs_SmallWidthSet->Fill(tsec, widthsetSmall);
     H_SmallWidthSet->Fill(widthsetSmall);
     H_LargeWidthSet->Fill(widthsetLarge);

     // calculate the fractional error
     double fracError = nphe/nph;

     // fill nph histo if frac error < 0.1  (or if widthSetLarge < 6 where nph error is large compared to nph)    or the PMT Pulse Width did not work
     // these histograms are filled with data from each set point, once a new set point is reached a gaussian will be fitted to these histograms
     // fracError set to 0.1 to exclude the first measurement on each small width set where widthsetLarge < 6
     // PMTPulseWidth set to 1.5 to exclude noise at the lower limit
     if (((fracError < 0.1) && PMTPulseWidth > 1.5 ) | ( (widthsetLarge < 6) & (repeatReadingAtWidth > 1000)  )){
       H_nph->Fill(nph);
       H_triggerWidth->Fill(triggerWidth);
       H_PMTPulseWidth->Fill(PMTPulseWidth);
       H_triggerWidth_div_PMTPulseWidth->Fill(triggerWidth / PMTPulseWidth);
     }

     // once eventNum (the event number loop) has reached the number of iterations in each measurement then fit a gaussian, get the mean and clear the histograms
     // if eventNumber exactly divisible by numMeasurementsAtEachPoint
     // if H_nph has entries, try and fit it.  10,000 = how many entries per step point where data is taken 
     // NOTE : number of entries in histo may not be 10000 based on error. 
     if ( eventNum % numMeasurementsAtEachPoint == 0 && eventNum!=0){
       // Some vars to use
       int LargeStepPoint = widthsetLarge;
       int SmallStepPoint = widthsetSmall;
       std::string histoSetPointNum = "Large_" + std::to_string(LargeStepPoint) + "_Small_" + std::to_string(SmallStepPoint);
       repeatReadingAtWidth = 0;

       int totalWidthSet = calcWidthSetTotal(widthsetLarge, widthsetSmall);


       // NPHs
       // CALC NPH Stat variables that may be of use
       std::cout<< " Got " << H_nph->GetEntries() << "  entries in histo. Mean is " << H_nph->GetMean() << " at width point " << totalWidthSet << " (large width " << widthsetLarge << ", small width " << widthsetSmall << ") \n";
       dnph->cd();
       // Too few entries from power meter (nph) to fit gaussian of the nph data - just get mean of hist
       double nph_mean = H_nph->GetMean();
       double lnnph_mean = TMath::Log(nph_mean);
       double nph_spread = H_nph->GetStdDev();
       H_nph_stdDev->Fill(nph_spread);

       TH1* nph_setPoint = (TH1*)H_nph->Clone();
       nph_setPoint->GetXaxis()->SetRangeUser( (nph_mean - (nph_spread*5)) , (nph_mean + (nph_spread*5)));
       std::string nph_histoSetPointName = "nph_" + histoSetPointNum;
       nph_setPoint->SetName(nph_histoSetPointName.c_str());

     
       // TRIG PULSES     ---   Fit Gaussian of the Trigger Width data from this set point 
       dTrigPulses->cd();
       int binmax = H_triggerWidth->GetMaximumBin(); int x_high = H_triggerWidth->GetXaxis()->GetBinCenter(binmax);
       int binmin = H_triggerWidth->GetMinimumBin(); int x_low = H_triggerWidth->GetXaxis()->GetBinCenter(binmin);
       TF1* f2 = new TF1("f2", "gaus",  x_low, x_high);
       H_triggerWidth->Fit("f2", "S");
       double triggerWidth_mean = f2->GetParameter(1);
       double triggerWidth_spread = f2->GetParameter(2);

       // Clone this histogram after fitting it, that way we can save a histo for this set point to the root file. H_triggerWidth histo will be cleared and filled for the next set point
       TH1* trigWidth_setPoint = (TH1*)H_triggerWidth->Clone();
       trigWidth_setPoint->GetXaxis()->SetRangeUser( (triggerWidth_mean - (triggerWidth_spread*5)) , (triggerWidth_mean + (triggerWidth_spread*5))); // set the range +/- 5 stdDevs around mean
       std::string trigWidth_histoSetPointName = "trigWidth_" + histoSetPointNum;
       trigWidth_setPoint->SetName(trigWidth_histoSetPointName.c_str()); // set the name to be unique so dont save over it, set the name to be the large / small widths
       trigWidth_setPoint->Write();

       // PMT PULSES
       // Fit Gaussian of the PMT Width data from this set point 
       dPMTPulses->cd();    // make the "PMTPulses" directory the current directory
       binmax = H_PMTPulseWidth->GetMaximumBin(); x_high = H_PMTPulseWidth->GetXaxis()->GetBinCenter(binmax);
       binmin = H_PMTPulseWidth->GetMinimumBin(); x_low = H_PMTPulseWidth->GetXaxis()->GetBinCenter(binmin);
       TF1* f3 = new TF1("f3", "gaus",  x_low, x_high);
       H_PMTPulseWidth->Fit("f3", "S");
       //     f1->Draw("same");
       double PMTPulseWidth_mean = f3->GetParameter(1);
       double PMTPulseWidth_spread = f3->GetParameter(2);

       // Clone this histogram after fitting it, that way we can save a histo for this set point to the root file. H_triggerWidth histo will be cleared and filled for the next set point
       TH1* PMTPulseWidth_setPoint = (TH1*)H_PMTPulseWidth->Clone();
       PMTPulseWidth_setPoint->GetXaxis()->SetRangeUser( (PMTPulseWidth_mean - (PMTPulseWidth_spread*5)) , (PMTPulseWidth_mean + (PMTPulseWidth_spread*5))); // set the range +/- 5 stdDevs around mean
       std::string PMTPulseWidth_histoSetPointName = "PMTPulseWidth_" + histoSetPointNum;
       PMTPulseWidth_setPoint->SetName(PMTPulseWidth_histoSetPointName.c_str()); // set the name to be unique so dont save over it, set the name to be the large / small widths
       PMTPulseWidth_setPoint->Write();

       outFile->cd();
       ///
       H_TriggerWidth_vs_nph->Fill(triggerWidth_mean, nph_mean);
       H_PMTPulseWidth_vs_nph->Fill(PMTPulseWidth_mean, nph_mean);
       H_pdValue_vs_nph->Fill(pdValue, nph_mean);
       H_tsec_vs_pdValue->Fill(tsec, pdValue);
       H_PMTPulseWidth_vs_pdValue->Fill(PMTPulseWidth_mean, pdValue);
       H_totalWidthSet_vs_pdValue->Fill(totalWidthSet, pdValue);
       H_TrigWidthmean_vs_PMTPulseWidthmean->Fill(triggerWidth_mean , PMTPulseWidth_mean );
       //fit nph
       //     H_nph->Draw();
       //     getchar(); // press enter to continue
       // try and fit it, then store the fit in an array with totalWidthSet?
       H_nph_vs_TriggerWidth->Fill(nph_mean, triggerWidth_mean);
       H_lnnph_vs_TriggerWidth->Fill(lnnph_mean, triggerWidth_mean);

       H_nph->Reset();
       H_triggerWidth->Reset();
       H_PMTPulseWidth->Reset();
     }

     repeatReadingAtWidth++;
   }
 






   /// LOOP OVER EVENTS AGAIN PLOTTING SOME OTHER STUFF
   TH1F *H_nph_2 = new TH1F("nph_2", "nph_2", 1000, 0, 250000);
   TH2F *H_nph_vs_nphe = new TH2F("nph_vs_nphe", "nph vs nphe", 1000, 0, 2500000, 1000, 0, 5000);
   TH2F *H_nph_vs_pdVal = new TH2F("nph_vs_pdVal", "nph vs pdVal", 1000, 0, 500000, 50, 0, 5);

   TH2F *H_TrigWidth_vs_nph_2 = new TH2F("TrigWidth_vs_nph", "H_TrigWidth_vs_nph", 500, 0, 50, 1000, 0, 500000);
   TH2F *H_PMTPulseWidth_vs_nph_2 = new TH2F("PMT Pulse Width vs phd", "PMT Pulse Width vs phd", 500, 0, 50, 1000, 0, 500000);
   TH2F *H_widthSet_vs_nph = new TH2F("H_widthSet_vs_nph", "H_widthSet_vs_nph", 700, 0, 700, 1000, 0, 500000);
   TH2F *H_widthSet_vs_TrigWidth = new TH2F("H_widthSet_vs_TrigWidth", "H_TrigWidth_vs_widthSet", 700, 0, 700, 150, 0, 75);
   TH2F *H_TrigWidth_vs_PMTPulseWidth = new TH2F("H_TrigWidth_vs_PMTPulseWidth", "H_TrigWidth_vs_PMTPulseWidth", 150, 0, 75, 50, 0, 25);


   TH1F *H_nph_PDHigh = new TH1F("nph PDHigh", "nph when PD High", 1000, 0, 250000);
   TH1F *H_nphe_PDHigh = new TH1F("nphe PDHigh", "nph when PD High", 1000, 0, 250000);
   TH1F *H_tsec_PDHigh = new TH1F("tsec PDHigh", "tsec when PD High", 1000, 0, 20000);

   // event loop
   for(int i=0; i< m_chain->GetEntries(); ++i) {
     // here the values from the file get copied into memory
     m_chain->GetEntry(i);
   
     // now we can work with the variables, e.g. print them
     //   std::cout << nph << ", " << nphe << std::endl;
     H_nph_vs_nphe->Fill(nph, nphe);
     // calc fractional error
     double fracError = nphe/nph;

     if (fracError < 0.1 && PMTPulseWidth > 1.5){
       H_nph_2->Fill(nph);
       H_nph_vs_pdVal->Fill(nph, pdValue);
       H_nph_vs_nphe->Fill(nph, nphe);
       H_TrigWidth_vs_nph_2->Fill(triggerWidth, nph);
       H_PMTPulseWidth_vs_nph_2->Fill(PMTPulseWidth, nph);

       H_widthSet_vs_nph->Fill(calcWidthSetTotal(widthsetLarge, widthsetSmall), nph);
       H_widthSet_vs_TrigWidth->Fill(calcWidthSetTotal(widthsetLarge, widthsetSmall), triggerWidth);

       H_TrigWidth_vs_PMTPulseWidth->Fill(triggerWidth, PMTPulseWidth);

     }

     // Look at when pd value is high, anything intersting?
     if (pdValue > 2.5){
       H_tsec_PDHigh->Fill(tsec);
       H_nph_PDHigh->Fill(nph);
       H_nphe_PDHigh->Fill(nphe);
     }

     if(i%100000==0) {
       std::cout << "Event " << i << std::endl;
     }
   }
   /*
     int binmax =  H_lnnph_vs_TriggerWidth->GetMaximumBin();
     double maxlnnph = H_lnnph_vs_TriggerWidth->GetXaxis()->GetBinCenter(binmax);
     cout << "Max lnNph: " << maxlnnph << endl;
     int binmin =  H_lnnph_vs_TriggerWidth->GetMinimumBin();
     double minlnnph = H_lnnph_vs_TriggerWidth->GetXaxis()->GetBinCenter(binmin);
     cout << "Min lnNph: " << minlnnph << endl;
   */
   TF1 *fitf_0 = new TF1("fitf_0",fitf0,6,12,4); //Trigger Width vs lnNph fit function
   fitf_0->SetParameters(19.,0.25,-0.006,0.0006);
   H_lnnph_vs_TriggerWidth->Fit(fitf_0);

   H_fpgaNum->Fill(fpgaBoardNumOut);
   Int_t fpga_Num = H_fpgaNum->GetMean();
   H_PBNum->Fill(pulserboardnumOut);
   Int_t PB_Num = H_PBNum->GetMean();

   TF1 *fitparam = (TF1*)H_lnnph_vs_TriggerWidth->GetListOfFunctions()->FindObject("fitf_0");
   double fitparam0 = fitparam -> GetParameter(0);
   double fitparam1 = fitparam -> GetParameter(1);
   double fitparam2 = fitparam -> GetParameter(2);
   double fitparam3 = fitparam -> GetParameter(3);
   // cout << "Fit parameters: " << fitparam0 << ", " << fitparam1 << ", " << fitparam2 << " and " << fitparam3 << endl; 

   string TrigWstr = "FPGA" + to_string(fpgaNum) + "_" + "CH" + to_string(PBNum) + "_TrigW_fit_parameters.txt";
   char TrigWfile[TrigWstr.size() + 1]; //String to Char converson for filename
   TrigWstr.copy(TrigWfile, TrigWstr.size() + 1);
   TrigWfile[TrigWstr.size()] = '\0';
   
   FILE *afile = fopen (TrigWfile, "w+");
   fprintf (afile,"%d\t%d\t%3.6f\t%3.6f\t%3.6f\t%3.6f", fpga_Num, PB_Num, fitparam0, fitparam1, fitparam2, fitparam3);
   fclose(afile);

   /* 
      TF1 *fitf_1 = new TF1("fitf_1",fitf1,1.,750e3,4);
      fitf_1->SetParameters(30,1,1,1);
      H_nph_vs_TriggerWidth->Fit(fitf_1);
   */

   outFile->Write();
   outFile->Close();
 }
}

////////////////////////////////////
//This is 2nd Version of the Full System Test Analysis script
// Harvey Birch harveyjohnbirch@gmail.com 
//
//To run the analysis, use the following terminal command:
//$ root -l FST_Analysis_V2.C+(\"PB99_Freq5000.0khz_Date_20190423_17_36_28.root\",99);
//
////////////////////////////////////

#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TTree.h"
#include <iostream>
#include "TProfile.h"
#include "TGraphErrors.h"
#include <cmath>
#include <cstdio>
#include "TROOT.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TPaveLabel.h"
#include "TPad.h"
#include "TFile.h"
#include "TF1.h"
#include "TAxis.h"
#include "TMath.h"
#include "TPaveStats.h"
#include "TObject.h"
#include "TString.h"
#include "TGraph.h"
#include "TH1.h"
#include "TRandom.h"
#include "TLine.h"

using namespace std;

Double_t fitf_TWNPH(Double_t *x,Double_t *par);

Double_t fitf_TWNPH(Double_t *x,Double_t *par) 
{
  Double_t arg = x[0];
  Double_t fitval_TWNPH = par[0] + TMath::Exp(par[1]*arg + par[2]*arg*arg + par[3]*arg*arg*arg);
  return fitval_TWNPH;
} 

void FST_Analysis_V2(char* filename, int board, int channel);

void FST_Analysis_V2(char* filename, int board, int channel)
{

  TFile *_file0 = TFile::Open(filename);
  TTree* tree = (TTree*)_file0->Get("tree");

  char titlename[30];
  sprintf(titlename,"FPGA%02dCh%02d",board, channel);

  float maxNph = 800.e3;

  const int nPointsLarge = 7; 
  const int nPointsSmall = 64;
  const int nPointsTot = nPointsLarge * nPointsSmall;
  const int startPointLarge = 5;
  const int startPointSmall = 1;
  
  TCanvas *c1 = new TCanvas("c1", "c1",800,1500);
  TPaveLabel* title = new TPaveLabel(0.1,0.96,0.9,0.99,titlename);
  title->Draw();
  gStyle->SetOptFit(kTRUE);

//  int nRows = (int)round((float)(nPointsLarge-startPointLarge) / 2.);
  int nRows = 4;
  TPad* graphPad = new TPad("Graphs","Graphs",0.01,0.05,0.95,0.95);
  graphPad->Draw();
  graphPad->cd();
  graphPad->Divide(2,nRows);

  float nphPMT[nPointsTot]; //number of photos at PMT
  float ernphPMT[nPointsTot]; // uncertainties of nphPMT
  float nphe[nPointsTot]; //error on power meter nph reading

  float lnphPMT[nPointsTot]; //number of photos at PMT
  float erlnphPMT[nPointsTot]; // uncertainties of nphPMT

  float trigW[nPointsTot]; //trigger width
  float ertrigW[nPointsTot]; //uncertainties of trigw
  float trigWres[nPointsTot]; //trigger width calculated residuals
  float trigWcalc[nPointsTot]; //Calc trigw from Fitf
   float trigWdelta[nPointsTot]; //Diff between trigW and trigWcalc

  float pw[nPointsTot]; //pulse width
  float erpw[nPointsTot]; // uncertainties of pw
  float pwMax = 0.0; // Max pulse width

  float widthsetlarge[nPointsTot];
  float widthsetsmall[nPointsTot];
  float widthsetdec[nPointsTot];

  float vref[nPointsTot];


  const float maxPD = 2.0; //was previously set to 2
  float pd[nPointsTot]; //photodiode board output
  float erpd[nPointsTot]; // uncertainties of pd

  int i_looped = 0;

  for (int ipoint = 0; ipoint < nPointsLarge; ipoint++){
    for (int jpoint = 0; jpoint < nPointsSmall; jpoint++){
      int trigSetLarge = startPointLarge + ipoint;  // trigger set 1 ~ 5ns
      int trigSetSmall = startPointSmall + jpoint;
      
      graphPad->cd(ipoint+1);
      
      char selection[80];
      

      if ((board==1 || board==9 || board==17 || board==25) && trigSetLarge > 4){
	sprintf(selection,"widthsetLarge==%d&&widthsetSmall==%d&&amp2<-40&&width2<40&&width2>3&&nph/nphe>1",trigSetLarge, trigSetSmall); // widthset is the trigger width set and pulseWidth is the pulse width.
      } 
      else {
	sprintf(selection,"widthsetLarge==%d&&widthsetSmall==%d&&amp2<-10&&width2<40&&width2>2&&nph/nphe>1",trigSetLarge, trigSetSmall); // widthset is the trigger width set and pulseWidth is the pulse width.
      }
      
      /*
      if ((board==1 || board==9 || board==17 || board==25) && trigSetLarge > 4){
	sprintf(selection,"widthsetLarge==%d&&widthsetSmall==%d&&amp2<-40&&width2<40&&width2>3&&nphe/nph < 0.2",trigSetLarge, trigSetSmall); // widthset is the trigger width set and pulseWidth is the pulse width.
      } 
      else {
	sprintf(selection,"widthsetLarge==%d&&widthsetSmall==%d&&amp2<-10&&width2<40&&width2>2&&nphe/nph < 0.2",trigSetLarge, trigSetSmall); // widthset is the trigger width set and pulseWidth is the pulse width.
      }
      */
      TH1F* trwHist = new TH1F("trwHist","",500,0.,500); // values for the trigger width graph
      tree->Draw("width1>>trwHist",selection,""); //trigWidth -> width1
      float nEntries = trwHist->GetEntries();
      cout << "nEntries: " << nEntries << endl;
      if (nEntries < 1) continue;
      float trwMean=trwHist->GetMean();
      float ertrwSigma=trwHist->GetRMS();
      trigW[i_looped] = trwMean;
      ertrigW[i_looped] = ertrwSigma;
      //std::cout << "trigW=" << trwMean << std::endl;

      TH1F* pwHist = new TH1F("pwHist","",100,0.,100); // values for the pulse width graph
      tree->Draw("width2>>pwHist",selection,""); //pulseWidth -> width2
      float pwMean=pwHist->GetMean();
      float erpwSigma=pwHist->GetRMS();
      pw[i_looped] = pwMean;
      erpw[i_looped] = erpwSigma;
      if (pwMean > pwMax) pwMax = pwMean;
      //std::cout << "pwMean=" << pwMean << std::endl;
      
      TH1F* LargeWHist = new TH1F("LargeWHist","",20,0.,20.);
      tree->Draw("widthsetLarge>>LargeWHist",selection,""); 
      float LargeWval = trigSetLarge;
      widthsetlarge[i_looped] = LargeWval;
      std::cout << "Large Width: " <<  LargeWval << endl;      
      
      TH1F* SmallWHist = new TH1F("SmallWHist","",20,0.,20.);
      tree->Draw("widthsetSmall>>SmallWHist",selection,""); 
      float trigSetSmallf = trigSetSmall; 
      float SmallWval = (trigSetSmallf/nPointsSmall);
      widthsetsmall[i_looped] = SmallWval;
      std::cout << "Small Width: " <<  trigSetSmallf << endl;
      std::cout << "Small Width: " <<  SmallWval << endl;
      
      float widthsetdecVal = LargeWval + SmallWval;
      widthsetdec[i_looped] = widthsetdecVal;
      std::cout << "Calc Width: " <<  widthsetdecVal << endl;
      
      TH1F* npHist = new TH1F("npHist","nphe/nph < 0.01",10000,0.,maxNph); // number of photons in a given scan point
      tree->Draw("nph>>npHist",selection,""); 
      float nphMean=npHist->GetMean();
      cout << "nphVal: " << nphMean << endl;
      
      TH1F* npheHist = new TH1F("npheHist","",10000,0.,maxNph); // number of photons in a given scan point
      tree->Draw("nphe>>npheHist",selection,""); 
      float npheMean=npheHist->GetMean();
      nphe[i_looped] = npheMean;
      cout << "npheVal: " << npheMean << endl;
      
      TH1F* pdHist = new TH1F("pdHist","",1000,0.,maxPD); // PD in a given scan point
      tree->Draw("pdValue>>pdHist",selection,""); //pdReadingCh1 is not a leaf in the new tree
      float pdMean=pdHist->GetMean();
      float pdSigma=pdHist->GetRMS();
      //if (pdSigma < 1.0e-5) pdSigma = 1.0e-5;
      pd[i_looped] = pdMean;
      erpd[i_looped] = pdSigma;
      cout << "PD: " << pd[i_looped] << " " << erpd[i_looped] << endl;
      
      TH1F* intgHist = new TH1F("intgHist","",10000,0.,maxNph);// integral of the pmt signal which should be ~ to the number of photons but on the pulse-by-pulse basis
      tree->Draw("abs(intg2)>>intgHist",selection,""); //pulseIntg -> intg2
      float intgMean=intgHist->GetMean();
      float intgSigma=intgHist->GetRMS();   // a peak width which will be used to determine the histogram range for a given scan point
      float lb = (intgMean - 4 * intgSigma) * nphMean / intgMean;  // +/- 4sigmas
      float hb = (intgMean + 4 * intgSigma) * nphMean / intgMean;
      //std::cout << "intgMean=" << intgMean << "  intgSigma=" << intgSigma << std::endl;
      //std::cout << "nphMean=" << nphMean << std::endl;
      //std::cout << "lb=" << lb << "  hb=" << hb << std::endl;

      TH1F* npmtHist = new TH1F("npmtHist",titlename,100,lb,hb);
      char ca[50];
      sprintf(ca,"abs(intg2)*%f/%f>>npmtHist",nphMean,intgMean); //pulseIntg -> intg2
      tree->Draw(ca,selection);
      float nPMTMean=npmtHist->GetMean();
      float nPMTSigma=npmtHist->GetRMS();
      nphPMT[i_looped] = nPMTMean;
      ernphPMT[i_looped] = nPMTSigma;
      lnphPMT[i_looped] = TMath::Log(nPMTMean);
      erlnphPMT[i_looped] = nPMTSigma/nPMTMean;
      lnphPMT[i_looped] = (nPMTMean > 0) ? TMath::Log(nPMTMean) : 0.0;
      erlnphPMT[i_looped] = (nPMTMean > 0) ?  nPMTSigma/nPMTMean : 0.0;
      //std::cout << "nph: " << nphPMT[i_looped] << " " << "enph: " << ernphPMT[i_looped] << endl;
      
      char axname[50];
      sprintf(axname,"Nph for trigger set %d.%d",trigSetLarge,trigSetSmall);
      npmtHist->GetXaxis()->SetTitle(axname);
      npmtHist->SetTitle("");
      npmtHist->Fit("gaus");
      
      c1->Update();
    
      i_looped = i_looped+1;
      cout << "FINISHING SMALL STEP:" << jpoint << endl;
    }
  }
  char plotname[30];  
  sprintf(plotname,"nph_FPGA%02dCh%02d.png",board, channel);
  c1->Print(plotname);    // a plot with distributions for all large width sets    
  
  TGraphErrors* nphVSpw = new TGraphErrors(nPointsTot,pw,nphPMT,erpw,ernphPMT); //Nph VS pulse width: R-160606
  sprintf(titlename,"FPGA%02dCh%02d",board, channel);
  TCanvas *c2 = new TCanvas("c2", "c2",10,65,700,500);
  c2->SetGrid();
  TH2F* NPHvsPW = new TH2F("NPHvsPW",titlename,100,0.,pwMax+2.,1000,1,maxNph);
  NPHvsPW->SetStats(kFALSE);
  NPHvsPW->GetYaxis()->SetTitle("Log number of photons per pulse");
  NPHvsPW->GetXaxis()->SetTitle("Pulse width (ns)");
  NPHvsPW->Draw();
  nphVSpw->SetMarkerStyle(20);
  nphVSpw->Draw("P same");
  c2->Update();  
  sprintf(plotname,"NphVSPulseWidth_FPGA%02dCh%02d.png",board, channel);
  c2->Print(plotname);

  TGraphErrors* nphVStw = new TGraphErrors(nPointsTot,trigW,nphPMT,ertrigW,ernphPMT);  //Nph VS Trigger Width: R-160601
  sprintf(titlename,"FPGA%02dCh%02d",board, channel);
  TCanvas *c3 = new TCanvas("c3","c3",10,65,700,500);
  c3->SetGrid();
  TH2F* NPHvsTW = new TH2F("NPHvsTW",titlename,500,15,70,10000,0,maxNph);
  NPHvsTW->SetStats(kFALSE);
  NPHvsTW->GetXaxis()->SetTitle("Trigger width (ns)");
  NPHvsTW->GetYaxis()->SetTitle("Number of photons per pulse");
  NPHvsTW->Draw();
  nphVStw->SetMarkerStyle(20);
  nphVStw->Draw("P same");
  c3->Update();
  sprintf(plotname,"NphvsTriggerWidth_FPGA%02dCh%02d.png",board, channel);
  c3->Print(plotname);
  
  TGraphErrors* twVSlnph = new TGraphErrors(nPointsTot,lnphPMT,trigW,erlnphPMT,ertrigW); //ln(Nph) VS Trigger Width: R-160605 nPointsTot -> nPointsLarge
  sprintf(titlename,"FPGA%02dCh%02d",board, channel);
  TCanvas *c4 = new TCanvas("c4","c4",10,65,700,500);
  c4->SetGrid();
  //  TH2F* trigWVSlnphPMT = new TH2F("trigWVSlnphPMT",titlename,1000,1,TMath::Log(maxNph),65,0,65);
  TH2F* trigWVSlnphPMT = new TH2F("trigWVSlnphPMT",titlename,1000,1.,TMath::Log(maxNph),500,15,70);
  trigWVSlnphPMT->SetStats(kFALSE);
  trigWVSlnphPMT->GetXaxis()->SetTitle("Natural log number of photons per pulse");
  trigWVSlnphPMT->GetYaxis()->SetTitle("Trigger width (ns)");
  trigWVSlnphPMT->Draw();
  twVSlnph->SetMarkerStyle(20);
  //twVSlnph->Draw("nphe > 10e3");
  
  TF1 *fitf = new TF1("fitf",fitf_TWNPH,6.,TMath::Log(maxNph),4); //Trigger Width vs lnNph fit function
  //  fitf->SetParameters(19.1892,0.2436,-0.006633,0.0006565);
  fitf->SetParameters(19.1892,0.2436,-0.006633,0.0006565);
  twVSlnph->Fit(fitf);
  twVSlnph->Draw("P same");
  gStyle->SetStatX(0.55);
  gStyle->SetStatY(0.9);
  c4->Update();
  sprintf(plotname,"lnNphVSTriggerWidth_FPGA%02dCh%02d.png",board, channel);
  c4->Print(plotname);
  
  TF1 *fitparam = (TF1*)twVSlnph->GetListOfFunctions()->FindObject("fitf"); //Extracting parameters from the function which is fitted to TWvslnNph
  cout << "Getting fit parameters from Trigger Width VS Nph plot" << endl;
  double fitparam0 = fitparam -> GetParameter(0);
  cout << fitparam0 << endl;
  double fitparam1 = fitparam -> GetParameter(1);
  cout << fitparam1 << endl;
  double fitparam2 = fitparam -> GetParameter(2);
  cout << fitparam2 << endl;
  double fitparam3 = fitparam -> GetParameter(3);
  cout << fitparam3 << endl;

  FILE *afile = fopen ("TrigW_fit_parameters.txt", "w+");
  fprintf (afile,"%02d\t%3.6f\t%3.6f\t%3.6f\t%3.6f", board, fitparam0, fitparam1, fitparam2, fitparam3);
  fclose(afile);

  
  TCanvas *c5 = new TCanvas("c5","c5",10,65,700,500);  //ln(Nph) VS Trigger Width with residual plot: R-160605
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0.1);
  pad1->SetGrid();
  pad1->Draw();
  pad1->cd();
  trigWVSlnphPMT->Draw();
  twVSlnph->Draw("P same");
  pad1->Update();
  pad1->RedrawAxis();
  twVSlnph->Draw("P same");
  c5->cd();
  TPad *pad2 = new TPad("pad2", "pad2", 0.02, 0, 1, 0.3);
  pad2->Draw();
  pad2->cd();
  pad2->SetLeftMargin(0.08);
  pad2->SetTopMargin(0.035);
  pad2->SetBottomMargin(0.19);
  pad2->Range(-0.2586662,-7.368421,15.41745,5.438596);
  pad2->SetGridy();
  
  
  for (int iPoints = 0; iPoints <= nPointsTot; iPoints++) {
    Double_t xval[1];
    xval[iPoints] = lnphPMT[iPoints];
    trigWcalc[iPoints] = fitparam0 + TMath::Exp(fitparam1*xval[iPoints] + fitparam2*xval[iPoints]*xval[iPoints] + fitparam3*xval[iPoints]*xval[iPoints]*xval[iPoints]);
  }
  for (int i = 0; i < nPointsTot; i++){
    trigWdelta[i] = trigW[i] - trigWcalc[i];
      }
/*  for (int iPoints = 0; iPoints <= nPointsTot; iPoints++) {
    Double_t xval[1];
    xval[iPoints] = lnphPMT[iPoints];
    trigWres[iPoints] =  (trigW[iPoints] - fitf->Eval(xval[iPoints]) );
    }*/
  
  TGraph* twRes = new TGraph(nPointsTot,lnphPMT,trigWdelta); //residual plot for TWvslnNph fit
  twRes->SetTitle("");
  twRes->GetXaxis()->SetTitle("Log Number of Photons per pulse");
  twRes->GetXaxis()->SetTitleSize(0.08);
  twRes->GetXaxis()->SetTitleOffset(1);
  twRes->GetXaxis()->SetLabelSize(0.085);
  twRes->GetYaxis()->SetTitle("Trigger Width Residual");
  twRes->GetYaxis()->SetLabelSize(0.085);
  twRes->GetYaxis()->SetTitleSize(0.085);
  twRes->GetYaxis()->SetTitleOffset(0.35);
  twRes->GetYaxis()->CenterTitle(true);
  twRes->GetXaxis()->SetLimits(1.,TMath::Log(maxNph));
  //  twRes->GetXaxis()->SetLimits(1.,TMath::Log(maxNph));
  //  float maxRes = twRes->GetMaximum();
  //  float minRes = twRes->GetMinimum();
  twRes->SetMinimum(-2); // NEED  ADD IN GET MAX/MIN FUNC AND THEN SET MAX/MIN OFF THIS. This will perform better over the Full set of data.
  twRes->SetMaximum(2); //
  twRes->SetMarkerColor(1);
  twRes->SetMarkerSize(1);
  twRes->SetMarkerStyle(7); //8
  twRes->Draw("AP");
  //TLine *line = new TLine(4.,0,11.,0);
  //line->SetLineColor(kBlack);
  //line->Draw();
  c5->cd();
  c5->Update();
  sprintf(plotname,"twVSlnNph_res_FPGA%02dCh%02d.png",board, channel);
  c5->Print(plotname);
/*
  for (int i = 0; i < nPointsTot; i++){
    cout << "lnNph: " << " " << lnphPMT[i] << ", TriggerWidth: " << " " << trigW[i] << ", Calculated  TriggerWidth: " << " " << trigWcalc[i]  << ", DeltatrigW: " << trigWdelta[i] << endl;
  }
*/

  /*  TCanvas *c7 = new TCanvas("c7", "c7",10,65,700,500);//Pulse width data extraction 
  TH1F* TrigWFitDiff = new TH1F("TrigWFitDiff","",20,0.,20.);
  TGraph* TrigFit = new TGraph(nPointsTot,nphPMT,trigWdelta);
  TrigWFitDiff->Draw();
  TrigFit->Draw("AP");
  c7->Update();
  */

  /*
  TCanvas *c6 = new TCanvas("c6", "c6",10,65,700,500);//Pulse width data extraction 
  TH1F* PulseWidthVer = new TH1F("PulseWidthVer","",20,0.,20.);
  tree->Draw("width2>>PulseWidthVer","nph<1100 && nph>900");
  float PulseWidthMean = PulseWidthVer->GetMean();
  PulseWidthVer->Draw();
  c6->Update();  
  FILE *pfile = fopen ("PulseWidth1000nph.txt", "w"); //Output for OCS paper. When FST data is taken, file path to text file MUST be changed.
  //    FILE *pfile = fopen ("/user/hbirch/LZ_experiment/Firmware_testing/FST_V4/OCS_data_analysis/PulseWidth1000nph.txt", "r+"); //Output for OCS paper. When FST data is taken, file path to text file MUST be changed.
  fprintf (pfile,"%3.6f\n", PulseWidthMean);
  fclose(pfile);
  */
  TGraphErrors* nphVSpd = new TGraphErrors(nPointsTot,nphPMT,pd,ernphPMT,erpd); //pulse width VS number of photons /////////////////////////////////////////////////////
  sprintf(titlename,"PB%02d",board);
  TCanvas *c8 = new TCanvas("c2", "c2",10,65,700,500);
  c8->SetGridy();
  //c2->SetLogy(0);
  //c8->SetLogx();
  TH2F* NPHvsPD = new TH2F("PWvsNPH",titlename,10000,1,maxNph,100,0,maxPD);
  NPHvsPD->SetStats(kFALSE);
  NPHvsPD->GetYaxis()->SetTitle("Photodiode Value");
  NPHvsPD->GetXaxis()->SetTitle("Number of photons per pulse");
  NPHvsPD->Draw();
  nphVSpd->SetMarkerStyle(20);
  nphVSpd->Draw("P same");
  sprintf(plotname,"nphVSpdValue_FPGA%02dCh%02d.png",board, channel); 
  c8->Print(plotname);








}




/*
  CODE DUMP 

  TGraphErrors* nphVSwidthsetL = new TGraphErrors(nPointsTot,widthsetlarge,nphPMT,0,ernphPMT);  //nph VS width set large 
  sprintf(titlename,"PB%02d",board);
  TCanvas *c7 = new TCanvas("c7","c7",10,65,700,500);
  c7->SetGridy();
  //c7->SetLog();
  //c7->Range(1,0,maxNph,65);
  TH2F* NPHvsWIDTHSETL = new TH2F("NPHvsWIDTHSETL",titlename,12,9,21,1000,1,maxNph);
  NPHvsWIDTHSETL->SetStats(kFALSE);
  NPHvsWIDTHSETL->GetXaxis()->SetTitle("Large Width Set");
  NPHvsWIDTHSETL->GetYaxis()->SetTitle("nph");
  NPHvsWIDTHSETL->Draw();
  nphVSwidthsetL->SetMarkerStyle(20);
  nphVSwidthsetL->Draw("P same");
  //sprintf(plotname,"TriggerWidthVSnphPB%02d.png",board);
  //c7->Print(plotname);
  //sprintf(plotname,"TriggerWidthVSnphPB%02d.root",board);
  //c7->Print(plotname);
  
  TGraphErrors* nphVSwidthsetS = new TGraphErrors(nPointsTot,widthsetsmall,nphPMT,0,ernphPMT);  //nph VS width set small 
  sprintf(titlename,"PB%02d",board);
  TCanvas *c8 = new TCanvas("c8","c8",10,65,700,500);
  c8->SetGridy();
  //c8->SetLog();
  //c8->Range(1,0,maxNph,65);
  TH2F* NPHvsWIDTHSETS = new TH2F("NPHvsWIDTHSETS",titlename,120,9,21,1000,1,maxNph);
  NPHvsWIDTHSETS->SetStats(kFALSE);
  NPHvsWIDTHSETS->GetXaxis()->SetTitle("Small Width Set");
  NPHvsWIDTHSETS->GetYaxis()->SetTitle("nph");
  NPHvsWIDTHSETS->Draw();
  nphVSwidthsetS->SetMarkerStyle(20);
  nphVSwidthsetS->Draw("P same");
  //sprintf(plotname,"TriggerWidthVSnphPB%02d.png",board);
  //c8->Print(plotname);
  //sprintf(plotname,"TriggerWidthVSnphPB%02d.root",board);
  //c8->Print(plotname);
  
  TGraphErrors* nphVSwidthsetD = new TGraphErrors(nPointsTot,widthsetdec,nphPMT,0,ernphPMT);  //nph VS combined widthset ////////////////////////////////////////////////////////////
  sprintf(titlename,"PB%02d",board);
  TCanvas *c9 = new TCanvas("c9","c9",10,65,700,500);
  c9->SetGridy();
  //c9->SetLog();
  //c9->Range(1,0,maxNph,65);
  TH2F* NPHvsWIDTHSETD = new TH2F("NPHvsWIDTHSETD",titlename,640,9,21,1000,1,maxNph);
  NPHvsWIDTHSETD->SetStats(kFALSE);
  NPHvsWIDTHSETD->GetXaxis()->SetTitle("Width set");
  NPHvsWIDTHSETD->GetYaxis()->SetTitle("Number of photons");
  NPHvsWIDTHSETD->Draw();
  nphVSwidthsetD->SetMarkerStyle(20);
  nphVSwidthsetD->Draw("P same");
  sprintf(plotname,"nphVSWidthSetPB%02d.png",board);
  c9->Print(plotname);
*/

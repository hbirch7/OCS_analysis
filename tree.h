//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct  1 13:13:51 2019 by ROOT version 6.06/08
// from TTree tree/tree
//////////////////////////////////////////////////////////

#ifndef tree_h
#define tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class tree {
public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  String_t fname = FPGA1_CH1_Freq4000.0khz_Date_20191114_02_59_46.root;
  
  // Fixed size dimensions of array or collections stored in the TTree if any.
  
  // Declaration of leaf types
  Double_t        nph;
  Double_t        nphe;
  Double_t        tsec;
  Double_t        temp;
  Double_t        width1;
  Double_t        amp1;
  Double_t        charge1;
  Double_t        intg1;
  Double_t        pkpos1;
  Double_t        width2;
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

  // List of branches
  TBranch        *b_nph;   //!
  TBranch        *b_nphe;   //!
  TBranch        *b_tsec;   //!
  TBranch        *b_temp;   //!
  TBranch        *b_width1;   //!
  TBranch        *b_amp1;   //!
  TBranch        *b_charge1;   //!
  TBranch        *b_intg1;   //!
  TBranch        *b_pkpos1;   //!
  TBranch        *b_width2;   //!
  TBranch        *b_amp2;   //!
  TBranch        *b_charge2;   //!
  TBranch        *b_intg2;   //!
  TBranch        *b_pkpos2;   //!
  TBranch        *b_width3;   //!
  TBranch        *b_widthsetLarge;   //!
  TBranch        *b_widthsetSmall;   //!
  TBranch        *b_vref;   //!
  TBranch        *b_fpgaBoardNumOut;   //!
  TBranch        *b_pulserboardnumOut;   //!
  TBranch        *b_pdValue;   //!

  tree(TTree *tree=0);
  virtual ~tree();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tree_cxx
tree::tree(TTree *tree) : fChain(0) 
{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fname);
    if (!f || !f->IsOpen()) {
      f = new TFile(fname);
    }
    f->GetObject("tree",tree);

  }
  Init(tree);
}

tree::~tree()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t tree::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t tree::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void tree::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("nph", &nph, &b_nph);
  fChain->SetBranchAddress("nphe", &nphe, &b_nphe);
  fChain->SetBranchAddress("tsec", &tsec, &b_tsec);
  fChain->SetBranchAddress("temp", &temp, &b_temp);
  fChain->SetBranchAddress("width1", &width1, &b_width1);
  fChain->SetBranchAddress("amp1", &amp1, &b_amp1);
  fChain->SetBranchAddress("charge1", &charge1, &b_charge1);
  fChain->SetBranchAddress("intg1", &intg1, &b_intg1);
  fChain->SetBranchAddress("pkpos1", &pkpos1, &b_pkpos1);
  fChain->SetBranchAddress("width2", &width2, &b_width2);
  fChain->SetBranchAddress("amp2", &amp2, &b_amp2);
  fChain->SetBranchAddress("charge2", &charge2, &b_charge2);
  fChain->SetBranchAddress("intg2", &intg2, &b_intg2);
  fChain->SetBranchAddress("pkpos2", &pkpos2, &b_pkpos2);
  fChain->SetBranchAddress("width3", &width3, &b_width3);
  fChain->SetBranchAddress("widthsetLarge", &widthsetLarge, &b_widthsetLarge);
  fChain->SetBranchAddress("widthsetSmall", &widthsetSmall, &b_widthsetSmall);
  fChain->SetBranchAddress("vref", &vref, &b_vref);
  fChain->SetBranchAddress("fpgaBoardNumOut", &fpgaBoardNumOut, &b_fpgaBoardNumOut);
  fChain->SetBranchAddress("pulserboardnumOut", &pulserboardnumOut, &b_pulserboardnumOut);
  fChain->SetBranchAddress("pdValue", &pdValue, &b_pdValue);
  Notify();
}

Bool_t tree::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void tree::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t tree::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}
#endif // #ifdef tree_cxx

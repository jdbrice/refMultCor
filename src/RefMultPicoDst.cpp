#include "RefMultPicoDst.h"
#include <iostream>
using namespace std;


RefMultPicoDst::RefMultPicoDst(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      cout << "[RefMultPicoDst] Error Invalid Tree " << endl;
      return;
   }
   Init(tree);
}

RefMultPicoDst::~RefMultPicoDst()
{
   //if (!fChain) return;
   //delete fChain->GetCurrentFile();
}

Int_t RefMultPicoDst::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t RefMultPicoDst::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void RefMultPicoDst::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normaly not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

    fChain->SetBranchAddress("runId", &runId, &b_runId);
   fChain->SetBranchAddress("refMult", &refMult, &b_refMult);

   fChain->SetBranchAddress("nPrimary", &nPrimary, &b_nPrimary);
   fChain->SetBranchAddress("nGlobal", &nGlobal, &b_nGlobal);
   fChain->SetBranchAddress("tofMult", &tofMult, &b_tofMult);
   fChain->SetBranchAddress("nTofMatch", &nTofMatch, &b_nTofMatch);
   
   fChain->SetBranchAddress("vertexX", &vertexX, &b_vertexX);
   fChain->SetBranchAddress("vertexY", &vertexY, &b_vertexY);
   fChain->SetBranchAddress("vertexZ", &vertexZ, &b_vertexZ);

   fChain->SetBranchAddress("tofMatchFlag", tofMatchFlag, &b_tofMatchFlag);
   
   fChain->SetBranchAddress("bbcCoinRate", &bbcCoinRate, &b_bbcCoinRate);
   fChain->SetBranchAddress("zdcCoinRate", &zdcCoinRate, &b_zdcCoinRate);

   fChain->SetBranchAddress("charge", charge, &b_charge);
   fChain->SetBranchAddress("pt", pt, &b_pt);
   fChain->SetBranchAddress("eta", eta, &b_eta);
   fChain->SetBranchAddress("phi", phi, &b_phi);
   fChain->SetBranchAddress("pX", pX, &b_pX);
   fChain->SetBranchAddress("pY", pY, &b_pY);
   fChain->SetBranchAddress("pZ", pZ, &b_pZ);
   fChain->SetBranchAddress("p", p, &b_p);
   
   Notify();
}

Bool_t RefMultPicoDst::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void RefMultPicoDst::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t RefMultPicoDst::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

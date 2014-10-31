#ifndef REF_MULT_PICO_DST_H
#define REF_MULT_PICO_DST_H

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "PicoDataStore.h"

const Int_t kMaxHits = 5000;
const Int_t kMaxTriggers = 400;

class RefMultPicoDst : public PicoDataStore {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

  UInt_t         runId;
   UShort_t       refMult;
   UShort_t       nTracks;
   UShort_t       nPrimary;
   UShort_t       nGlobal;
   UShort_t       tofMult;
   UShort_t       nTofMatch;
   
   Float_t         vertexX;
   Float_t         vertexY;
   Float_t         vertexZ;

   Float_t         bbcCoinRate;
   Float_t         zdcCoinRate;
   
   Int_t           tofMatchFlag[kMaxHits];   //[nTracks]
   
   Int_t           charge[kMaxHits];   //[nTracks]
   Float_t         pt[kMaxHits];   //[nTracks]
   Float_t         eta[kMaxHits];   //[nTracks]
   Float_t         phi[kMaxHits];   //[nTracks]
   Float_t         pX[kMaxHits];   //[nTracks]
   Float_t         pY[kMaxHits];   //[nTracks]
   Float_t         pZ[kMaxHits];   //[nTracks]
   Float_t         p[kMaxHits];   //[nTracks]
   
   

   // List of branches
   TBranch        *b_runId;   //!
   TBranch        *b_eventId;   //!

   TBranch        *b_refMult;   //!

   TBranch        *b_vertexX;   //!
   TBranch        *b_vertexY;   //!
   TBranch        *b_vertexZ;   //!
   
   TBranch        *b_tofMult;   //!
   TBranch        *b_nGlobal;   //!
   TBranch        *b_nPrimary;   //!
   TBranch        *b_nTofMatch;   //!
   TBranch        *b_tofMatchFlag;   //!

   TBranch        *b_charge;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_pX;   //!
   TBranch        *b_pY;   //!
   TBranch        *b_pZ;   //!
   TBranch        *b_p;   //!

   TBranch        *b_bbcCoinRate;   //!
   TBranch        *b_zdcCoinRate;   //!


   RefMultPicoDst(TTree *tree=0);

   virtual ~RefMultPicoDst();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   //virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);


   /**
    * Implement the PicoDataStore Interface
    */
   
   virtual TTree * getTree() { return fChain; }
   
   virtual Double_t trackPt( Int_t iHit ) { return pt[ iHit ]; }
   virtual Double_t trackP( Int_t iHit ){ return p[ iHit ]; }
   virtual Double_t trackPx( Int_t iHit ){ return pX[ iHit ]; }
   virtual Double_t trackPy( Int_t iHit ){ return pY[ iHit ]; }

   virtual Double_t trackEta( Int_t iHit ) { return eta[ iHit ]; }
   virtual Int_t trackTofMatch( Int_t iHit ){ return tofMatchFlag[ iHit ]; }

   virtual Double_t eventVertexX( ){ return vertexX; }
   virtual Double_t eventVertexY( ){ return vertexY; }
   virtual Double_t eventVertexZ( ){ return vertexZ; }

   virtual UInt_t eventRunId(){ return runId; }

   virtual Short_t eventTofMult(){return tofMult; }
   virtual UShort_t eventRefMult(){ return refMult; }
   virtual Short_t eventNumPrimary(){ return nPrimary; }
   virtual Short_t eventNumTofMatched(){ return nTofMatch; }
   virtual Short_t eventNumGlobal(){ return nGlobal; }

   virtual Float_t eventZDC(){ return zdcCoinRate; }
   virtual Float_t eventBBC(){ return bbcCoinRate; } 


};

#endif


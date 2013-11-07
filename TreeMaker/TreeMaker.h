#ifndef TreeMaker_h
#define TreeMaker_h 1

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TStopwatch.h"
#include "TDatime.h"
#include "TMap.h"
#include "TMath.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <map>
using namespace std;

class TreeMaker
{
  public:
    static TreeMaker* GetTreeMakerPointer(void); // Creates TreeMaker object and returns a pointer to it.

    void SpecifyNumberOfEvents(Int_t nEvents); // Only used for timer display purposes

    void CreateNewFileAndTree( TString absolutePath, TString fileName, Int_t compression = 1, TString treeName = "tree" );
    void CloseCurrentFile(void);
    void FillBranch(TString varName, Int_t    *val, const Int_t nEls = 1);
    void FillBranch(TString varName, Double_t *val, const Int_t nEls = 1);
    void FillBranch(TString varName, TString  *val);
    void FillTree(void);
    void SaveTree(void);

    void SetDisplayInterval(Int_t interval /*in seconds*/);
    void UpdateDisplay(Int_t nEvent);

    void Destroy(void);

  private:
    void CreateNewFile(TString path, TString name, Int_t compression);
    void CreateNewTree(TString name);
    void CreateBranch(TString varName, TString varType, Int_t nEls);
    void StartFillTimer(Bool_t reset=1);
    void DisplayHeader(void);
    void DisplayETF(void);
    void DisplayFooter(void);
    TString GetDHMS(Double_t timeInSecs);
    void GetDateAndTime(void);

  private:
    TString fClassName;
    TString fMethodName;

    TreeMaker(); // Private constructor. It is invoked only by GetTreeMakerPointer().
    static TreeMaker *fPtr; // Pointer to the one-and-only TreeMaker object.

    // Timer and Timer Display Variables
    TDatime *fDateAndTime;
    TStopwatch *fTimer;
    static const Int_t fNumberOfFillsToWait = 100;
    Int_t fNumFills;  // Number of times FillTree() was called.
    Int_t fNumEvents; // Number of events specified.
    Int_t fNEvent;    // The number of the current event.
    Int_t fDeltaN;
    Int_t fPreviousN;
    Int_t fPreviousNFill;
    Int_t fBefore; // Used in UpdateDisplay().
    Int_t fNow;  // Used in UpdateDisplay().
    Double_t fElapsedTime; // Accumulated elapsed time since first start of the timer.
    Int_t fInterval;
    Double_t fDeltaT;
    Double_t fPreviousT;
    Int_t fMonth, fDay, fYear;
    Int_t fHr, fMin, fSec;
    TString fBeginHr, fBeginMin, fBeginSec;

    // File and Tree Variables
    TString fFileName;
    TString fPathAndName;
    TFile *fFile;
    TString fTreeName;
    TString fTreeTitle;
    TTree *fTree;
    static const Long64_t fMaxTreeSize = 53687091200LL;  // Sets maximum size of the ROOT file
                                                         // containing the tree (in bytes).
    static const Long64_t fAutoSaveSize = 104857600LL;   // Sets when the TTree header and
                                                         // TBranch buffers are flushed to disk.
                                                         // They are flushed when this
                                                         // size (in bytes) is reached.
    static const TString fVersionDate;

    // Branches and Buffer Variables
    std::map<TString,TString> fBranchName;
    std::map<TString,Int_t*> fVarInt;
    std::map<TString,Double_t*> fVarDouble;
    std::map<TString,Char_t*> fVarChar;

    Bool_t fHeaderDisplayed;

}; //END of class TreeMaker

#endif



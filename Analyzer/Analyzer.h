#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TExec.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TGraphSmooth.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TBrowser.h"
#include "TEllipse.h"
#include "TLine.h"
#include "TRandom.h"
#include "TRandom1.h"
#include "TMath.h"
#include "TF1.h"
#include "TFolder.h"

#include "ReplayTimer.h"

#ifndef Analyzer_h
#define Analyzer_h 1

class Analyzer
{
  public:
    Analyzer();
    virtual ~Analyzer(){;};

    void SetPathToRunData(TString path);
    void SetPathToSimData(TString path);
    void SetPathToStorageFile(TString path);
    void SetStorageFileTag(TString name);

    TString GetPathToRunData(void) { return fRunFilePath; };
    TString GetPathToSimData(void) { return fSimFilePath; };
    TString GetPathToStorageFile(void) { return fStorageFilePath; };
    TString GetStorageFileTag(void) { return fStorageFileTag; };

    virtual void ReplayRun(TString fileTag) {;};
    virtual void ReplaySim(TString fileTag) {;};

  protected:
    void OpenRunFile(TString fileTag, TString option = "read", Bool_t verbose = kTRUE);
    void CloseRunFile(Bool_t verbose = kTRUE);
    void OpenSimFile(TString fileTag, TString option = "read", Bool_t verbose = kTRUE);
    void CloseSimFile(Bool_t verbose = kTRUE);
    void OpenStorageFile(TString option = "read", Bool_t verbose = kTRUE);
    void CloseStorageFile(Bool_t verbose = kTRUE);
    void CdStorageDirectory(TString dirName, TString dirTitle = "", Bool_t verbose = kTRUE);
    void DeleteStorageDirectory(TString dirName);
    TString SaveStorageObject(TString name, TObject* o);
    TString SaveStorageHistogram(TH1* h);
    TString SaveStorageGraph(TGraph* g);
    TH1* GetStorageHistogram(TString hName);
    TObject* GetStorageObject(TString oName);

    TString fClassName;
    TString fMethodName;
    TString fHName;
    TString fHTitle;

    static const Double_t PI;

    TFile* fRunFile;
    TFile* fSimFile;
    TFile* fStorageFile;

    TString fTreeName;
    TTree* fTree;
    Int_t fNEntries;
    TString fBranchName;

    ReplayTimer* fReplayTimer;

  private:
    Bool_t StorageFileExists(void);
    void RecreateStorageFile(TString fileNameTag);

    TString fRunFileTag;
    TString fRunFilePath;
    TString fRunFileName;

    TString fSimFileTag;
    TString fSimFilePath;
    TString fSimFileName;

    TString fStorageFilePath;
    TString fStorageFileTag;
    TString fStorageFileName;

};//END of class Analyzer

#endif

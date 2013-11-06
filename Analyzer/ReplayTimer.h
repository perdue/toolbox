#include <iostream>
#include <iomanip>
using namespace std;

#include "TStopwatch.h"
#include "TDatime.h"
#include "TString.h"
#include "TMath.h"

#ifndef ReplayTimer_h
#define ReplayTimer_h 1

class ReplayTimer
{
  public:
    ReplayTimer();
    void Check(Int_t entry, Int_t nentries);

  private:
    void Start(Bool_t reset=kTRUE);
    void GetDateAndTime(void);
    void DisplayETF(void);
    TString GetDHMS(Double_t timeInSecs);

    const TString fClassName;
    TString fMethodName;

    TStopwatch *fTimer;
    TDatime *fDateAndTime;
    Int_t fNEntries;
    Int_t fEntry;
    Int_t fMonth, fDay, fYear;
    Int_t fHour, fMinute, fSecond;
    Int_t fInterval;
    Double_t fElapsedTime; // Accumulated elapsed time since first start of the timer.
    Int_t fNowTime;
    Int_t fBeforeTime;
    Int_t fDeltaN;
    Int_t fPreviousN;
    Double_t fDeltaT;
    Double_t fPreviousT;

};//END of class ReplayTimer

#endif

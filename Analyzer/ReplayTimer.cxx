#include "ReplayTimer.h"

ReplayTimer::ReplayTimer()
 :
  fClassName("ReplayTimer")
{
  fTimer = new TStopwatch();
  fDateAndTime = new TDatime();
  fNEntries    =  0;
  fEntry       =  0;
  fElapsedTime =  0;
  fBeforeTime  =  0;
  fInterval    =  3; // Update replay display about every fInterval seconds.
  fPreviousN   =  0;
  fPreviousT   =  0;
  fMonth       =  0;
  fDay         =  0;
  fYear        =  0;

}//END of constructor

////////////////////
void ReplayTimer::Check(Int_t entry, Int_t nentries)
{
  fMethodName = "Check";

  fNEntries = nentries;
  fEntry = entry + 1;

  if( fEntry==1 || fEntry==fNEntries )
  {
    if( fEntry==1 ) Start();
    GetDateAndTime();
    DisplayETF();
  }

  fElapsedTime = fTimer->RealTime(); // Stops the timer and returns the elapsed time.
  if( fElapsedTime>0 ) fTimer->Continue(); // Continues the timing without a reset.

  // This block of code keeps track of when to update the display.
  fNowTime = (Int_t)fElapsedTime;
  Int_t deltaTime = fNowTime-fBeforeTime;
  if( deltaTime>=fInterval )
  {
    DisplayETF();

    fBeforeTime = fNowTime;

  }//END of if

}//END of Check()

////////////////////
void ReplayTimer::Start(Bool_t reset)
{
  fMethodName = "Start";

  fTimer->Start(reset);

  cout << fClassName << "::" << fMethodName << " - ";
  cout << "Timer ";
  if( reset ) cout << "reset and ";
  cout << "started." << endl;

}//END of Start()

////////////////////
void ReplayTimer::GetDateAndTime(void)
{
  fDateAndTime->Set();
  fMonth = fDateAndTime->GetMonth();
  fDay   = fDateAndTime->GetDay();
  fYear  = fDateAndTime->GetYear();

  fHour   = fDateAndTime->GetHour();
  fMinute = fDateAndTime->GetMinute();
  fSecond = fDateAndTime->GetSecond();

}//END of GetDateAndTime()

////////////////////
void ReplayTimer::DisplayETF(void)
{
  fMethodName = "DisplayETF";

  fDeltaN = fEntry - fPreviousN;
  fPreviousN = fEntry;

  fDeltaT = fElapsedTime - fPreviousT;
  fPreviousT = fElapsedTime;

  Double_t fillRate = 0;
  Double_t etf = 0;
  if( fDeltaT>0 )
  {
    fillRate = ((Double_t)fDeltaN)/fDeltaT;
    etf = ((Double_t)(fNEntries-fEntry))/fillRate;
  }

  GetDateAndTime();

  if( fEntry==1 )
  {
    cout << endl;
    cout << "\tReplaying Began On ";
    cout << fMonth << "/" << fDay << "/" << fYear << "\n";
    cout << "\tNumber of Entries to Replay: ";
    cout << ((fNEntries>0)?Form("%d",fNEntries):"???") << endl;
    cout << endl;

    cout << setw(21) << "Entry Number  " << " - ";
    cout << "    Replay Rate     "       << " - ";
    cout << setw(8)  << "  Clock  "     << " - ";
    cout << setw(11) << "  Elapsed  "   << " - ";
    cout << setw(11) << " To Finish ";
    cout << endl;
    cout << setw(50) << "HH" << ":" << setw(2) << "MM" << ":" << setw(2) << "SS";
    cout << " - ";
    cout << setw(2) << "DD" << ":" << setw(2) << "HH" << ":" << setw(2) << "MM" << ":" << setw(2) << "SS";
    cout << " - ";
    cout << setw(2) << "DD" << ":" << setw(2) << "HH" << ":" << setw(2) << "MM" << ":" << setw(2) << "SS";
    cout << endl;


  }//END of if

  cout << "\r";
  cout << setw(19) << fEntry << "   - ";
  cout << setw(7) << (Int_t)fillRate << " entries/sec  - ";
  cout << setw(3) << ((fHour<=9)?Form("0%d",fHour):Form("%d",fHour)) << ":";
  cout << setw(2) << ((fMinute<=9)?Form("0%d",fMinute):Form("%d",fMinute)) << ":";
  cout << setw(2) << ((fSecond<=9)?Form("0%d",fSecond):Form("%d",fSecond));
  cout << " - ";
  if( fElapsedTime>0 ) cout << GetDHMS(fElapsedTime) << " - ";
  if( fNEntries>0 ) cout << GetDHMS(etf);
  cout.flush();

  if( fEntry==fNEntries && fNEntries>0 )
  {
    cout << "\r";
    cout << setw(19) << fEntry << "   - ";
    cout << setw(7) << fEntry/fElapsedTime << " entries/sec  - ";
    cout << setw(3) << ((fHour<=9)?Form("0%d",fHour):Form("%d",fHour)) << ":";
    cout << setw(2) << ((fMinute<=9)?Form("0%d",fMinute):Form("%d",fMinute)) << ":";
    cout << setw(2) << ((fSecond<=9)?Form("0%d",fSecond):Form("%d",fSecond));
    cout << " - ";
    if( fElapsedTime>0 ) cout << GetDHMS(fElapsedTime) << " - ";
    if( fNEntries>0 ) cout << GetDHMS(etf);
    cout.flush();

    cout << endl;
    cout << endl;
    cout << "\tReplaying Ended On ";
    cout << fMonth << "/" << fDay << "/" << fYear << "\n";
    cout << "\tNumber of Entries Replayed: ";
    cout << fEntry << endl;

  }//END of if

  fMonth = fDay = fYear = 0;

}//END of DisplayETF()

////////////////////
TString ReplayTimer::GetDHMS(Double_t timeInSecs)
{
  fMethodName = "GetDHMS";

  Int_t t = TMath::Nint(timeInSecs);

  Int_t days = 0;
  Int_t hrs  = 0;
  Int_t mins = 0;
  Int_t secs = 0;

  Int_t remaining_s = 0;

  days = t/86400;
  remaining_s = t%86400;
  hrs = remaining_s/3600;
  remaining_s = remaining_s%3600;
  mins = remaining_s/60;
  remaining_s = remaining_s%60;
  secs = remaining_s;

  TString DD,HH,MM,SS;
  DD = (days<=9)?Form("0%d:",days):Form("%d:",days);
  HH = (hrs<=9) ?Form("0%d:",hrs): Form("%d:",hrs );
  MM = (mins<=9)?Form("0%d:",mins):Form("%d:",mins);
  SS = (secs<=9)?Form("0%d", secs):Form("%d", secs);

  return (DD+HH+MM+SS);

}//END of GetDHMS()


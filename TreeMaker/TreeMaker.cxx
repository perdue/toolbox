#include "TreeMaker.h"

TreeMaker * TreeMaker::fPtr = 0; // Initializes pointer to NULL.

const TString TreeMaker::fVersionDate = "04/14/2011";

////////////////////////////////////////////////////////////////////////////////
TreeMaker::TreeMaker()
 :
  fClassName("TreeMaker")
{
  cout << "Instance of " << fClassName << " Constructed!" << endl;
  cout << "   TreeMaker Version Date: " << fVersionDate << endl;

  // Pointers
  fTimer = 0;
  fFile = 0;
  fTree = 0;

  fTimer = new TStopwatch();
  fDateAndTime = new TDatime();

  // Variables
  fNumFills      =  0;
  fBefore        = -1;
  fNow           =  0;
  fElapsedTime   =  0;
  fInterval      = 10;
  fNumEvents     =  0;
  fPreviousN     =  0;
  fPreviousNFill =  0;
  fPreviousT     =  0;
  fMonth         =  0;
  fDay           =  0;
  fYear          =  0;
  fHeaderDisplayed = kFALSE;

  GetDateAndTime();

  fBeginHr  = (fHr<= 9) ? Form("0%d",fHr ) : Form("%d",fHr);
  fBeginMin = (fMin<=9) ? Form("0%d",fMin) : Form("%d",fMin);
  fBeginSec = (fSec<=9) ? Form("0%d",fSec) : Form("%d",fSec);

  StartFillTimer();

}//END of constructor

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::Destroy(void)
{
  cout << "Instance of " << fClassName << " Destructed!" << endl;

  Int_t size;

  size = fVarInt.size();
  std::map<TString,Int_t*>::iterator iit;
  iit = fVarInt.begin();
  for( Int_t i=0; i<size; i++)
  {
    TString name = iit->first;
    delete fVarInt[name];
    iit++;
  }

  size = fVarDouble.size();
  std::map<TString,Double_t*>::iterator dit;
  dit = fVarDouble.begin();
  for( Int_t i=0; i<size; i++)
  {
    TString name = dit->first;
    delete fVarDouble[name];
    dit++;
  }

  size = fVarChar.size();
  std::map<TString,Char_t*>::iterator cit;
  cit = fVarChar.begin();
  for( Int_t i=0; i<size; i++)
  {
    TString name = cit->first;
    delete fVarChar[name];
    cit++;
  }

  delete fTimer;
  delete fDateAndTime;
  delete fFile;
  delete fTree;
  delete fPtr;
  fPtr = 0;

}//END of Destroy()

////////////////////////////////////////////////////////////////////////////////
TreeMaker* TreeMaker::GetTreeMakerPointer(void)
{
  if( !fPtr )
    fPtr = new TreeMaker;

  return fPtr;

}//END of GetTreeMakerPointer()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::StartFillTimer(Bool_t reset)
{
  fMethodName = "StartFillTimer";

  fTimer->Start(reset);

  cout << "   " << fClassName << "::" << fMethodName << " - ";
  cout << "Timer ";
  if( reset ) cout << "reset and ";
  cout << "started." << endl;

}//END of StartFillTimer()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::SpecifyNumberOfEvents(Int_t nEvents)
{
//  fMethodName = "SpecifyNumberOfEvents";

  fNumEvents = nEvents;

}//END of SpecifyNumberOfEvents()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::CreateNewFileAndTree( TString absolutePath, TString fileName, Int_t compression, TString treeName )
{
//  fMethodName = "CreateNewFileAndTree";

  CreateNewFile( absolutePath, fileName, compression );
  CreateNewTree( treeName );

  fNumFills =  0;

  FillBranch("month", &fMonth);
  FillBranch("day",   &fDay  );
  FillBranch("year",  &fYear );
  FillBranch("elapsedRealTime", &fElapsedTime);
  FillBranch("fillNumber", &fNumFills);

}//END of CreateNewFileAndTree()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::CreateNewFile( TString path, TString name, Int_t compression )
{
  fMethodName = "CreateNewFile";

  if( !fFile )
  {
    TString ext = ".root";
    fFileName = name+ext;
    fPathAndName = path+name+ext;

    fFile = new TFile(fPathAndName,"recreate",fFileName);
    fFile->SetCompressionLevel(compression);

    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "New File Created: " << fPathAndName << endl;
  }
  else
  {
    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Warning! Cannot create a new file while current file is open." << endl;
  }

}//END of CreateNewFile()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::CloseCurrentFile( void )
{
  fMethodName = "CloseCurrentFile";

  Long64_t file_size;
  TString display;
  Int_t compress;

  if( fFile )
  {
    file_size = fFile->GetSize();
    if(      file_size > 1073741824 ) display = Form("%.2f GiB",(Double_t)file_size/1073741824);
    else if( file_size > 1048576    ) display = Form("%.2f MiB",(Double_t)file_size/1048576);
    else if( file_size > 1024       ) display = Form("%.2f KiB",(Double_t)file_size/1024);
    else                              display = Form("%d B",   (Int_t)file_size);
    compress  = fFile->GetCompressionLevel();

    fFile->Close();

    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Closed File: " << fPathAndName;
    cout << "\n                                 Compression Level: " << compress;
    cout << "\n                                         File Size: " << display << endl << endl;
  }

//  delete fFile;
  fFile = 0;
  fTree = 0;

}//END of CloseCurrentFile()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::CreateNewTree(TString name)
{
  fMethodName = "CreateNewTree";

  if( !fTree )
  {
    fTreeName = name;
    fTreeTitle = fTreeName;

    fTree = new TTree(fTreeName,fTreeTitle);
    fTree->SetMaxTreeSize(fMaxTreeSize);
    fTree->SetAutoSave(fAutoSaveSize);
    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "New Tree Created: " << fTreeName << endl;
    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Max Size of File: " << Form("%.0f GiB",(Double_t)fMaxTreeSize/1073741824.0) << endl;
    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Auto Save Size: " << Form("%.0f MiB",(Double_t)fTree->GetAutoSave()/1048576.0) << endl;
  }
  else
  {
    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Warning! Cannot create a new tree while current file is open." << endl;
  }

}//END of CreateNewTree()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::FillBranch(TString varName, Int_t *val, const Int_t nEls)
{
//  fMethodName = "FillBranch";

  TString varType = "I";

  TString i = varName;

  // If the branch named varName does not zexist, then create it and its buffer variable.
  if( !fBranchName[i].Contains(varName) ) CreateBranch(i,varType,nEls);

  // Fill each element of the buffer variable for the branch named varName.
  for(Int_t j=0; j<nEls; j++) fVarInt[i][j] = val[j];

}//END of FillBranch()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::FillBranch(TString varName, Double_t *val, const Int_t nEls)
{
//  fMethodName = "FillBranch";

  TString varType = "D";

  TString i = varName;

  // If the branch named varName does not exist, then create it and its buffer variable.
  if( !fBranchName[i].Contains(varName) ) CreateBranch(i,varType,nEls);

  // Fill each element of the buffer variable for the branch named varName.
  for(Int_t j=0; j<nEls; j++) fVarDouble[i][j] = val[j];

}//END of FillBranch()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::FillBranch(TString varName, TString *val)
{
//  fMethodName = "FillBranch";

  TString varType = "C";

  TString i = varName;

  // If the branch named varName does not exist, then create it and its buffer variable.
  if( !fBranchName[i].Contains(varName) ) CreateBranch(i,varType,1);

  // Fill the buffer variable for the branch named varName.
  sprintf(fVarChar[i],"%s",val->Data());

}//END of FillBranch()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::CreateBranch(TString varName, TString varType, Int_t nEls)
{
  fMethodName = "CreateBranch";

  TString i = varName;
  TString specify = "";

  if( fTree )
  {
    fBranchName[i] = i;

    if( nEls==1 )
      specify = Form("%s/%s",     fBranchName[i].Data(), varType.Data());
    else
      specify = Form("%s[%d]/%s", fBranchName[i].Data(), nEls, varType.Data());

    if( varType=="I" )
    {
      fVarInt[i] = new Int_t[nEls];
      fTree->Branch(fBranchName[i],fVarInt[i],specify);
    }
    else if( varType=="D" )
    {
      fVarDouble[i] = new Double_t[nEls];
      fTree->Branch(fBranchName[i],fVarDouble[i],specify);
    }
    else if( varType=="C" )
    {
      fVarChar[i] = new Char_t[100];
      fTree->Branch(fBranchName[i],fVarChar[i],specify);
    }
    else
    {
      cout << "   " << fClassName << "::" << fMethodName << " - ";
      cout << "Error! Only these branch types are accepted: \"I\", \"D\", \"C\"" << endl;
      exit(0);
    }

    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Created Branch: " << specify << endl;
  }
  else
  {
    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Error! No tree exists." << endl;
    exit(0);
  }

}//END of CreateBranch()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::FillTree(void)
{
  fMethodName = "FillTree";

  fNumFills++;

  FillBranch("month", &fMonth);
  FillBranch("day",   &fDay  );
  FillBranch("year",  &fYear );
  FillBranch("elapsedRealTime", &fElapsedTime);
  FillBranch("fillNumber", &fNumFills);
  fMethodName = "FillTree";

  if( fTree ) fTree->Fill();
  else
  {
    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Error! No tree exists." << endl;
    exit(0);
  }

}//END of FillTree()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::SaveTree(void)
{
  fMethodName = "SaveTree";

  if( fTree )
  {
    fTree->Write();

    if( !fHeaderDisplayed && fNumFills<=fNumberOfFillsToWait )
      DisplayHeader();
    DisplayETF();
    DisplayFooter();

    cout << endl;
    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Tree Saved: " << fTreeName << endl;
  }
  else
  {
    cout << "   " << fClassName << "::" << fMethodName << " - ";
    cout << "Error! No tree exists." << endl;
    exit(0);
  }

}//END of SaveTree()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::SetDisplayInterval(Int_t interval)
{
//  fMethodName = "SetDisplayInterval";

  if( interval>1 )
    fInterval = interval;
  else
    fInterval = 2;

}//END of SetDisplayInterval()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::UpdateDisplay(Int_t nEvent)
{
//  fMethodName = "UpdateDisplay";

  fNEvent = nEvent;

  if( !fHeaderDisplayed && fNumFills>fNumberOfFillsToWait )
  {
    DisplayHeader();
    fHeaderDisplayed = kTRUE;

  }//END of if

  fElapsedTime = fTimer->RealTime(); // Stops the timer and returns the elapsed time.
  if( fElapsedTime>0 ) fTimer->Continue(); // Continues the timing without a reset.

  fNow = (Int_t(fElapsedTime))%fInterval;

  // This block keeps track of when to update the display.
  // It will update every time period specified by "fInterval".
  if( fNow != fBefore )
  {
    if( fNow==0 && fNumFills>fNumberOfFillsToWait ) DisplayETF();

    fBefore = fNow;

  }//END of if

}//END of UpdateDisplay()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::DisplayHeader(void)
{
//  fMethodName = "DisplayHeader";

  cout << endl;
  cout << "     Filling Began on ";
  cout << fMonth << "/" << fDay << "/" << fYear << " at ";
  cout << fBeginHr << ":" << fBeginMin << ":" << fBeginSec << "\n";
  cout << "     Number of Events Specified: ";
  cout << ((fNumEvents>0)?Form("%d",fNumEvents):"Not Specified") << "\n";
  cout << endl;

  cout << setw(17) << "Event Number" << " - ";
  cout << "Events/Sec" << " - ";
  cout << "Fill Number" << " - ";
  cout << "Fills/Sec" << " - ";
  cout << "Size (MiB)" << " - ";
  cout << setw(8)  << "  Clock  "     << " - ";
  cout << setw(11) << "  Elapsed  "   << " - ";
  cout << setw(11) << " To Finish ";
  cout << endl;

  cout << setw(75) << "HH" << ":" << setw(2) << "MM" << ":" << setw(2) << "SS";
  cout << " - ";
  cout << setw(2) << "DD" << ":" << setw(2) << "HH" << ":" << setw(2) << "MM" << ":" << setw(2) << "SS";
  cout << " - ";
  cout << setw(2) << "DD" << ":" << setw(2) << "HH" << ":" << setw(2) << "MM" << ":" << setw(2) << "SS";
  cout << endl;

}//END of DisplayHeader()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::DisplayETF(void)
{
//  fMethodName = "DisplayETF";

  fDeltaN = fNEvent - fPreviousN;
  fPreviousN = fNEvent;

  fDeltaT = fElapsedTime - fPreviousT;
  fPreviousT = fElapsedTime;

  Double_t eventRate = ((Double_t)fDeltaN)/fDeltaT;
  Double_t etf = ((Double_t)(fNumEvents-fNEvent))/eventRate;

  fDeltaN = fNumFills - fPreviousNFill;
  fPreviousNFill = fNumFills;

  Double_t fillRate = ((Double_t)fDeltaN)/fDeltaT;

  GetDateAndTime();

  cout << "\r";
  cout << setw(17) << fNEvent << " - ";
  cout << setw(10) << (Int_t)eventRate << " - ";
  cout << setw(11) << fNumFills << " - ";
  cout << setw(9) << (Int_t)fillRate << " - ";
  cout << setw(13) << Form("%.0f - ",(Double_t)fFile->GetSize()/1048576.0);
  cout << setw(3) << ((fHr<=9)?Form("0%d",fHr):Form("%d",fHr)) << ":";
  cout << setw(2) << ((fMin<=9)?Form("0%d",fMin):Form("%d",fMin)) << ":";
  cout << setw(2) << ((fSec<=9)?Form("0%d",fSec):Form("%d",fSec));
  cout << " - ";
  if( fElapsedTime>0 ) cout << GetDHMS(fElapsedTime) << " - ";
  if( fNumEvents>0 ) cout << GetDHMS(etf);
  cout.flush();

}//END of DisplayETF()

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::DisplayFooter(void)
{
//  fMethodName = "DisplayFooter";

  cout << endl;
  cout << endl;
  cout << "     Filling Ended On ";
  cout << fMonth << "/" << fDay << "/" << fYear << " at ";
  cout << ((fHr<=9 )?Form("0%d",fHr ):Form("%d",fHr) ) << ":";
  cout << ((fMin<=9)?Form("0%d",fMin):Form("%d",fMin)) << ":";
  cout << ((fSec<=9)?Form("0%d",fSec):Form("%d",fSec)) << "\n";
  cout << "     Number of Fills:" << setw(11) << fNumFills << endl;
  cout << "     Number of Events:" << setw(10) << fNEvent   << endl;

}//END of DisplayFooter()

////////////////////////////////////////////////////////////////////////////////
TString TreeMaker::GetDHMS(Double_t timeInSecs)
{
//  fMethodName = "GetDHMS";

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

////////////////////////////////////////////////////////////////////////////////
void TreeMaker::GetDateAndTime(void)
{
//  fMethodName = "GetDateAndTime";

  fDateAndTime->Set();
  fMonth = fDateAndTime->GetMonth();
  fDay   = fDateAndTime->GetDay();
  fYear  = fDateAndTime->GetYear();

  fHr  = fDateAndTime->GetHour();
  fMin = fDateAndTime->GetMinute();
  fSec = fDateAndTime->GetSecond();

}//END of GetDateAndTime()

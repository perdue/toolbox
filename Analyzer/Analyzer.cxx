#include "Analyzer.h"

const Double_t Analyzer::PI = TMath::Pi();

////////////////////////////////////////////////////////////////////////////////
Analyzer::Analyzer()
 :
  fClassName("Analyzer")
{
  TExec e;
  e.Exec(".x /home/perdue/.rootlogon");

  SetPathToRunData("./");
  SetPathToSimData("./");
  SetPathToStorageFile("./");
  SetStorageFileTag("storage");

  fStorageFile = 0;

  cout << endl;

}//END of constructor

////////////////////////////////////////////////////////////////////////////////
void Analyzer::SetPathToRunData(TString path)
{
  Int_t length = path.Length();
  TString last = path[length-1];
  if( last!="/" )  path += "/";

  fRunFilePath = path;

  fMethodName = "SetPathToRunData";

  cout << fClassName << "::" << fMethodName << " - Path to data: " << fRunFilePath << endl;

}//END of SetPathToRunData()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::SetPathToSimData(TString path)
{
  Int_t length = path.Length();
  TString last = path[length-1];
  if( last!="/" )  path += "/";

  fSimFilePath = path;

  fMethodName = "SetPathToSimData";

  cout << fClassName << "::" << fMethodName << " - Path to data: " << fSimFilePath << endl;

}//END of SetPathToSimData()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::SetPathToStorageFile(TString path)
{
  Int_t length = path.Length();
  TString last = path[length-1];
  if( last!="/" )  path += "/";

  fStorageFilePath = path;

  fMethodName = "SetPathToStorageFile";

  cout << fClassName << "::" << fMethodName << " - Path to data: " << fStorageFilePath << endl;

}//END of SetPathToStorageFile()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::SetStorageFileTag(TString name)
{
  fStorageFileTag = name;

  fMethodName = "SetStorageFileTag";

  cout << fClassName << "::" << fMethodName << " - Storage file tag: " << fStorageFileTag << endl;

}//END of SetStorageFileTag()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::OpenRunFile(TString fileTag, TString option, Bool_t verbose)
{
  fRunFileTag = fileTag;

  fRunFileName = fRunFileTag + ".root";
  fRunFile = new TFile(fRunFilePath + fRunFileName,option);
  if( !fRunFile->IsOpen() )
  {
    cerr << fClassName << "::" << fMethodName << " - Error! Could not open file: " << fRunFileName << endl;
    exit(0);
  }

  if( option=="update" )
  {
    if( verbose )
      cout << fClassName << "::" << fMethodName << " - Updating file: " << fRunFileName << endl;
  }
  else
  {
    if( verbose )
      cout << fClassName << "::" << fMethodName << " - Reading file: " << fRunFileName << endl;
  }

}//END of OpenRunFile()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::CloseRunFile(Bool_t verbose)
{
  fRunFile->Close();
  delete fRunFile;
  fRunFile = 0;

  if( verbose )
    cout << fClassName << "::" << fMethodName << " - Closed file: " << fRunFileName << endl;

}//END of CloseRunFile()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::OpenSimFile(TString fileTag, TString option, Bool_t verbose)
{
  fSimFileTag = fileTag;

  fSimFileName = fSimFileTag + ".root";
  fSimFile = new TFile(fSimFilePath + fSimFileName,option);
  if( !fSimFile->IsOpen() )
  {
    cerr << fClassName << "::" << fMethodName << " - Error! Could not open file: " << fSimFileName << endl;
    exit(0);
  }

  if( option=="update" )
  {
    if( verbose )
      cout << fClassName << "::" << fMethodName << " - Updating file: " << fSimFileName << endl;
  }
  else
  {
    if( verbose )
      cout << fClassName << "::" << fMethodName << " - Reading file: " << fSimFileName << endl;
  }

}//END of OpenSimFile()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::CloseSimFile(Bool_t verbose)
{
  fSimFile->Close();
  delete fSimFile;
  fSimFile = 0;

  if( verbose )
    cout << fClassName << "::" << fMethodName << " - Closed file: " << fSimFileName << endl;

}//END of CloseSimFile()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::OpenStorageFile(TString option, Bool_t verbose)
{
  if( !StorageFileExists() )
    RecreateStorageFile( fStorageFileTag );

  fStorageFileName = fStorageFileTag + ".root";
  fStorageFile = new TFile(fStorageFilePath + fStorageFileName,option);
  if( !fStorageFile->IsOpen() )
  {
    cerr << fClassName << "::" << fMethodName << " - Error! Could not open file: " << fStorageFileName << endl;
    exit(0);
  }

  if( option=="update" )
  {
    if( verbose )
      cout << fClassName << "::" << fMethodName << " - Updating file: " << fStorageFileName << endl;
  }
  else
  {
    if( verbose )
      cout << fClassName << "::" << fMethodName << " - Reading file: " << fStorageFileName << endl;
  }

}//END of OpenStorageFile()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::CloseStorageFile(Bool_t verbose)
{
  fStorageFile->Close();
  delete fStorageFile;
  fStorageFile = 0;

  if( verbose )
    cout << fClassName << "::" << fMethodName << " - Closed file: " << fStorageFileName << endl;

}//END of CloseStorageFile()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::CdStorageDirectory(TString dirName, TString dirTitle, Bool_t verbose)
{
  fStorageFile->cd();
  TDirectoryFile* dirFile = (TDirectoryFile*)fStorageFile->Get(dirName);

  if( !dirFile )
  {
    dirFile = new TDirectoryFile(dirName,dirTitle);
    if( verbose ) cout << fClassName << "::" << fMethodName << " - Created directory: " << dirName << endl;
  }

  if( verbose ) cout << fClassName << "::" << fMethodName << " - Current directory: " << dirName << endl;

  dirFile->SetTitle(dirTitle);
  fStorageFile->cd(dirName);

}//END of CdStorageDirectory()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::DeleteStorageDirectory(TString dirName)
{
  fStorageFile->cd();

  TDirectoryFile* dir = (TDirectoryFile*)fStorageFile->Get(Form("%s;1",dirName.Data()));
  if( dir )
  {
    fStorageFile->Delete(Form("%s;1",dirName.Data()));
    dir = (TDirectoryFile*)fStorageFile->Get(Form("%s;1",dirName.Data()));

    if( !dir )
    {
      cout << fClassName << "::" << fMethodName << " - Deleted directory: " << dirName << endl;
    }
  }

}//END of DeleteStorageDirectory()

////////////////////////////////////////////////////////////////////////////////
TString Analyzer::SaveStorageObject(TString name, TObject* o)
{
  o->Write(name, TObject::kOverwrite);

  return name;

}//END of SaveStorageObject()

////////////////////////////////////////////////////////////////////////////////
TString Analyzer::SaveStorageHistogram(TH1* h)
{
  TString hName = h->GetName();
  SaveStorageObject(hName, h);

  return hName;

}//END of SaveStorageHistogram()

////////////////////////////////////////////////////////////////////////////////
TString Analyzer::SaveStorageGraph(TGraph* g)
{
  TString gName = g->GetName();
  g->Write(gName, TObject::kOverwrite);

  return gName;

}//END of SaveStorageGraph()

////////////////////////////////////////////////////////////////////////////////
TH1* Analyzer::GetStorageHistogram(TString hName)
{
  if( !fStorageFile )
  {
    cerr << fClassName << "::" << fMethodName << " - Error! No storage file has been opened. " << endl;
    exit(0);
  }

  TH1* h = (TH1*)fStorageFile->Get(hName);
  if( !h )
  {
    cerr << fClassName << "::" << fMethodName << " - Error! Could not locate histogram: " << hName << endl;
    exit(0);
  }

  return h;

}//END of GetStorageHistogram()

////////////////////////////////////////////////////////////////////////////////
TObject* Analyzer::GetStorageObject(TString oName)
{
  TObject* o = (TObject*)fStorageFile->Get(oName);
  if( !o )
  {
    cerr << fClassName << "::" << fMethodName << " - Error! Could not locate object: " << oName << endl;
    exit(0);
  }

  return o;

}//END of GetStorageObject()

////////////////////////////////////////////////////////////////////////////////
Bool_t Analyzer::StorageFileExists(void)
{
  TFile f(fStorageFilePath+fStorageFileTag+".root","read");

  Bool_t exists = f.IsOpen();
  f.Close();

  return exists;

}//END of StorageFileExists()

////////////////////////////////////////////////////////////////////////////////
void Analyzer::RecreateStorageFile(TString fileNameTag)
{
  fStorageFileTag = fileNameTag;

  cout << fClassName << "::" << fMethodName << " - Path to data: " << fStorageFilePath << endl;

  fStorageFileName = fStorageFileTag + ".root";
  fStorageFile = new TFile(fStorageFilePath + fStorageFileName,"recreate");
  fStorageFile->Close();
  delete fStorageFile;
  fStorageFile = 0;

  cout << fClassName << "::" << fMethodName << " - Recreated file: " << fStorageFileName << endl;

}//END of RecreateStorageFile()

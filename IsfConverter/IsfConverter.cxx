#include "IsfConverter.h"

//const Double_t IsfConverter::PI = TMath::Pi();

////////////////////////////////////////////////////////////////////////////////
IsfConverter::IsfConverter()
 :
  fClassName("IsfConverter")
{
  TExec e;
  e.Exec(".x /home/perdue/.rootlogon");

   fNrPt    =  0;  // number of points in waveform
   fBytNr   =  0;  // number of bytes per data point
   fBitNr   =  0;  // number of bits per data point
   fEncdg   = "";  // type of encoding
   fBnFmt   = "";  // format of binary data
   fBytOr   = "";  // byte order of data points (MSB or LSB)
   fWfid    = "";  // string describing acquisition parameters
   fPtFmt   = "";  // point format
   fXunit   = "";  // horizontal unit
   fXincr   =  0;  // horizontal sampling interval
   fXzero   =  0;  // time of first point in wavform
   fPtOff   =  0;  // equals 0 if data source was displayed
   fYunit   = "";  // vertical unit
   fYmult   =  0;  // vertical scale factor per digitizing level
   fYoff    =  0;  // vertical position in digitizing levels
   fYzero   =  0;  // vertical offset
   fVscale  =  0;  // vertical scale (per division)
   fHscale  =  0;  // horizontal scale (per division)
   fVpos    =  0;
   fVoffset =  0;
   fHdelay  =  0;
   fCurve   = "";


   fWaveform = new TGraph();


//  SetPathToRunData("./");
//  SetPathToSimData("./");
//  SetPathToStorageFile("./");
//  SetStorageFileTag("storage");

//  fStorageFile = 0;

//  cout << endl;

}//END of constructor

////////////////////////////////////////////////////////////////////////////////
TGraph* IsfConverter::Read(TString name, Bool_t verbose)
{
  fMethodName = "Read";

  // Delete all points in the graph.
  for(Int_t i=fWaveform->GetN()-1; i>=0; i--)
  {
    fWaveform->RemovePoint(i);

  }//END of i loop

  // Open .isf file.
  ifstream f(name.Data(), ios::in | ios::binary);
  if( !f.is_open() )
  {
    cerr << fClassName << "::" << fMethodName << " - ";
    cerr << "Error!  Could not open file: " << name << endl;
    exit(0);

  }//END of if

  // Read and parse the file header,
  // which is written in plain text.
  const Int_t NCHARS = 1000;
  char header[NCHARS];
  f.read(header,NCHARS);
  Parse(header,verbose);

  // Find the start of the binary waveform data.
  streamoff offset = (streamoff)(-fNrPt * fBytNr); // There are fBytNr number
                                                   // of bytes per point and
                                                   // fNrPt number of points.
  f.seekg(offset, ios::end); // Seek to beginning of binary data
                             // from the end of the file.

  const Int_t NBYTES = (Int_t)fBytNr;
  for(Int_t i=0; i<(Int_t)fNrPt; i++)
  {
    char buffer[NBYTES];
    f.read(buffer, NBYTES);

    int data;
    if( NBYTES==2 && fBytOr=="MSB" )
      data = (buffer[0]<<8) + buffer[1];
    else if( NBYTES==2 && fBytOr=="LSB" )
      data = (buffer[1]<<8) + buffer[0];
    else if( NBYTES==1 )
      data = buffer[0];

    Double_t x = fXzero + i*fXincr;
    Double_t y = fYmult*(data - fYoff) + fYzero;

    fWaveform->SetPoint(i,x,y);

  }//END of i loop

  TString xunit = fXunit(1);
  fWaveform->GetXaxis()->SetTitle( "Time (" + xunit + ")");
  fWaveform->GetXaxis()->CenterTitle();

  TString yunit = fYunit(1);
  fWaveform->GetYaxis()->SetTitle( "Signal (" + yunit + ")");
  fWaveform->GetYaxis()->CenterTitle();

  fWaveform->SetMarkerStyle(20);

  return fWaveform;

}//END of Read()

////////////////////////////////////////////////////////////////////////////////
void IsfConverter::Parse(string header, Bool_t verbose)
{
  fMethodName = "Parse";

  fNrPt    = GetD(header,   "NR_PT",  verbose);
  fBytNr   = GetD(header,  "BYT_NR",  verbose);
  fBitNr   = GetD(header,  "BIT_NR",  verbose);
  fEncdg   = GetS(header,   "ENCDG",  verbose);
  fBnFmt   = GetS(header,  "BN_FMT",  verbose);
  fBytOr   = GetS(header,  "BYT_OR",  verbose);
  fWfid    = GetS(header,    "WFID",  verbose);
  fPtFmt   = GetS(header,  "PT_FMT",  verbose);
  fXunit   = GetS(header,   "XUNIT",  verbose);
  fXincr   = GetD(header,   "XINCR",  verbose);
  fXzero   = GetD(header,   "XZERO",  verbose);
  fPtOff   = GetD(header,  "PT_OFF",  verbose);
  fYunit   = GetS(header,   "YUNIT",  verbose);
  fYmult   = GetD(header,   "YMULT",  verbose);
  fYoff    = GetD(header,    "YOFF",  verbose);
  fYzero   = GetD(header,   "YZERO",  verbose);
  fVscale  = GetD(header,  "VSCALE",  verbose);
  fHscale  = GetD(header,  "HSCALE",  verbose);
  fVpos    = GetD(header,    "VPOS",  verbose);
  fVoffset = GetD(header, "VOFFSET",  verbose);
  fHdelay  = GetD(header,  "HDELAY",  verbose);
//  fCurve   = GetS(header,   "CURVE",  verbose);

}//END of Parse()

////////////////////////////////////////////////////////////////////////////////
Double_t IsfConverter::GetD(string header, string param, Bool_t verbose)
{
  fMethodName = "GetD";

  int start = header.find(param);
  int middle = header.find(" ", start);
  int end = header.find(";", middle);
  double value = atof(header.substr(middle+1,end-middle-1).c_str());

  if( verbose )
  {
    cout << setiosflags( ios::scientific | ios::right );
    cout << setprecision(4);
    cout << setw(10) << param << ":" << setw(13) << value << endl;
  }

  return value;

}//END of GetD()

////////////////////////////////////////////////////////////////////////////////
TString IsfConverter::GetS(string header, string param, Bool_t verbose)
{
  fMethodName = "GetS";

  int start = header.find(param);
  int middle = header.find(" ", start);
  int end = header.find(";", middle);
  TString value = header.substr(middle+1,end-middle-1).c_str();

  if( verbose )
  {
    cout << setiosflags( ios::scientific | ios::right );
    cout << setprecision(4);
    if( param=="WFID" )
      cout << setw(10) << param << ":" << setw(73) << value << endl;
    else
      cout << setw(10) << param << ":" << setw(13) << value << endl;
  }

  return value;

}//END of GetS()

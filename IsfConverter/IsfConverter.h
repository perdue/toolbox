#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iomanip>
using namespace std;

#include "TString.h"
#include "TExec.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TAxis.h"

#ifndef IsfConverter_h
#define IsfConverter_h 1

class IsfConverter
{
  public:
    IsfConverter();
    virtual ~IsfConverter(){;};

    TGraph* Read(TString name, Bool_t verbose=kFALSE);

  protected:
    TString fClassName;
    TString fMethodName;
    TString fHName;
    TString fHTitle;

  private:
    void Parse(string header, Bool_t verbose);
    Double_t GetD(string header, string param, Bool_t verbose);
    TString GetS(string header, string param, Bool_t verbose);

    Double_t fNrPt;   // number of points in waveform
    Double_t fBytNr;  // number of bytes per data point
    Double_t fBitNr;  // number of bits per data point
    TString fEncdg;   // type of encoding
    TString fBnFmt;   // format of binary data
    TString fBytOr;   // byte order of data points (MSB or LSB)
    TString fWfid;    // string describing acquisition parameters
    TString fPtFmt;   // point format
    TString fXunit;   // horizontal unit
    Double_t fXincr;  // horizontal sampling interval
    Double_t fXzero;  // time of first point in wavform
    Double_t fPtOff;  // equals 0 if data source was displayed
    TString fYunit;   // vertical unit
    Double_t fYmult;  // vertical scale factor per digitizing level
    Double_t fYoff;   // vertical position in digitizing levels
    Double_t fYzero;  // vertical offset
    Double_t fVscale; // vertical scale (per division)
    Double_t fHscale; // horizontal scale (per division)
    Double_t fVpos;
    Double_t fVoffset;
    Double_t fHdelay;
    TString fCurve;

    TGraph* fWaveform;

};//END of class IsfConverter

#endif

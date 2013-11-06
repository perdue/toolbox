#include "Rkin.h"

#include <iostream>
#include <cstdlib>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////
Rkin::Rkin()
 :
  fClassName("Rkin")
{
  fMethodName = "Rkin";

  // Load the nuclear mass table
  loademass_();

  fInAng   = 0;
  fInEn    = 0;
  fInReac  = "";
  fInEx    = 0;

  fQ       = 0;
  fEn3     = 0;
  fLabAng3 = 0;
  fCmAng3  = 0;
  fEn4     = 0;
  fLabAng4 = 0;
  fRatioP  = 0;


}//END of constructor

/////////////////////////////////////////////////////////////////////////////////////////////////
void Rkin::Run(void)
{
  fMethodName = "Run";

  if( fInReac!="" )
  {
    // Call it with internal variables set with the Set methods.
    // Use the "Get" methods to retrieve the results.
    Run(fInAng,fInEn,fInReac,fInEx,fQ,fEn3,fLabAng3,fCmAng3,fEn4,fLabAng4,fRatioP);
  }
  else
  {
    cerr << fClassName << "::" << fMethodName << " - Error! Reaction not specified." << endl;
    exit(0);
  }

}//END of Run()

/////////////////////////////////////////////////////////////////////////////////////////////////
void Rkin::Run(Double_t inang, Double_t inen,
               TString inreac, Double_t inex,
               Double_t &Q, Double_t &en3,
               Double_t &labang3, Double_t &cmang3,
               Double_t &en4, Double_t &labang4, Double_t &ratiop)
{
  fMethodName = "Run";

  // Call it like you would in a C program.  The Fortran routine will have to do the error checking.
  char reac[255];
  sprintf(reac,"%-25s",inreac.Data());
  rkin_(inang,inen,reac,inex,Q,en3,labang3,cmang3,en4,labang4,ratiop);

}//END of Run()

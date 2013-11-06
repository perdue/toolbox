#ifndef LightOutput_h
#define LightOutpu_h 1

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
using namespace std;

#include "TString.h"
#include "TObjArray.h"
#include "TMath.h"
#include "TRandom2.h"

////////////////////////////////////////////////////////////////////////////////
class LightOutput
{
  public:
    LightOutput(TString dataPath);
    void SetPathToData(TString dataPath);
    void SetResolutionParameters(Double_t alpha, Double_t beta, Double_t gamma); // as fractions.
    Double_t GetLightOutput(TString particleName, Double_t initialEnergy, Double_t energyDeposited);
    void SetQuiet(void);

  private:
    void ReadData(void);
    TString GetTableParticle(TString particleName);
    Double_t GetLightOut(TString tableParticle);
    Double_t Interpolate(TString particle, Double_t energy);
    Double_t Smear(Double_t lightOut);

  private:
    const TString fClassName;
    TString fMethodName;

    TString fMaterial; // Material of detector (NE213, BC501, BC505, EJ301, etc).
    TString fDataPath; // Path to light output tables.

    TRandom2* fRandGen; // Random number generator.

    // The light output is randomly smeared according to a Guassian distribution
    // using a FWHM = L * sqrt( fAlpha^2 + fBeta^2/L + fGamma^2/L^2 ).
    // fAlpha:  Locus-dependent light transmission from scintillator to photocathode.
    // fBeta:   Statistical effects of light production, attenuation, photon-to-electron
    //          conversion, and electron amplification.
    // fGamma:  Noise contributions
    // Ref:     G. Dietze and H. Klein, NIM 193 (1982) 549
    Double_t fAlpha, fBeta, fGamma; // specified as a fraction.

    // List of light output tables.
    vector< TString > fTable;

    // Names of particles for which light output tables exist.
    vector< TString > fTableParticle;

    // Ordered pair (particle, light output table)
    map< TString, map< Double_t, Double_t > > fParticleLightTable;

    Double_t fInitialEnergy;
    Double_t fEnergyDeposited;

    Bool_t fIsVerbose;

};//END of class LightOutput

#endif

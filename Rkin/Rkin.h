/////////////////////////////////////////////////////////////////////////////////////////////////
// Setting up interface to rkin Fortran subroutines.
/////////////////////////////////////////////////////////////////////////////////////////////////

// loademass_: Reads in values from MASS83A.DAT to an array
extern "C" void loademass_();

// emass1_: Finds the mass difference in MeV of a nucleus given A and Z
// by reading the file MASS83A.DAT using relative record number A.
extern "C" double emass1_(int &A, int &Z);

// rkin_: Program interface of rkin.for.
//        inang  = input lab angle in degrees
//        inen   = input lab energy in MeV
//        inreac = reaction in the form A(a,b) or A(a,b)B
//        inex   = excitation energy of final state in MeV
//        Q      = Q-value calculated by rkin() in MeV
//        en3    = lab energy of outgoing particle in MeV
//        labang3 = lab angle of outgoing particle in degrees
//        cmang3  = CM angle of outgoing particle in degrees
//        en4     = lab energy of recoiling nucleus in MeV
//        labang4 = lab angle of recoiling nucleus in degrees
//        ratiop  = SigCM / SigLab
extern "C" void rkin_(double &inang, double &inen,
                      char inreac[], double &inex,
                      double &Q, double &en3,
                      double &labang3, double &cmang3,
                      double &en4, double &labang4, double &ratiop);

/////////////////////////////////////////////////////////////////////////////////////////////////
// A class interface to the rkin program.
/////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef Rkin_h
#define Rkin_h 1

#include "TString.h"

class Rkin
{
  public:
    Rkin();

    void Run(void);
    void Run(Double_t inang, Double_t inen,
             TString inreac, Double_t inex,
             Double_t &Q, Double_t &en3,
             Double_t &labang3, Double_t &cmang3,
             Double_t &en4, Double_t &labang4, Double_t &ratiop);


    void SetDetectorAngle(Double_t angle)     { fInAng  = angle;    }; // degrees
    void SetBeamEnergy(Double_t energy)       { fInEn   = energy;   }; // MeV
    void SetReaction(TString reaction)        { fInReac = reaction; };
    void SetExcitationEnergy(Double_t energy) { fInEx   = energy;   }; // MeV

    Double_t GetBeamEnergy(void)       { return fInEn;    }; // MeV
    TString& GetReaction(void)         { return fInReac;  };
    Double_t GetExcitationEnergy(void) { return fInEx;    }; // MeV

    Double_t GetQValue(void)    { return fQ;       }; // MeV
    Double_t GetEnergy3(void)   { return fEn3;     }; // MeV
    Double_t GetLabAngle3(void) { return fLabAng3; }; // degrees
    Double_t GetCmAngle3(void)  { return fCmAng3;  }; // degress
    Double_t GetEnergy4(void)   { return fEn4;     }; // MeV
    Double_t GetLabAngle4(void) { return fLabAng4; }; // degrees
    Double_t GetRatioP(void)    { return fRatioP;  };

  private:
    const TString fClassName;
    TString fMethodName;

    Double_t fInAng;
    Double_t fInEn;
    TString fInReac;
    Double_t fInEx;
    Double_t fQ;
    Double_t fEn3;
    Double_t fLabAng3;
    Double_t fCmAng3;
    Double_t fEn4;
    Double_t fLabAng4;
    Double_t fRatioP;

}; //END of class Rkin

#endif

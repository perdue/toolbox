#ifndef Utility_h
#define Utility_h 1

#include <math.h>

#include "Rtypes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
class Utility
{
  public:
    static const Double_t SpeedOfLight(void); // Returns speed of light in cm/ns.

    static const Double_t NeutronMass(void); // Returns neutron mass in MeV/c^2.

    static const Double_t ProtonMass(void); // Returns proton mass in MeV/c^2.

    static Double_t NeutronTof(Double_t d, Double_t Tn); // Returns ntof in ns for d in cm and Tn in MeV.

    static Double_t NeutronEnergy(Double_t d, Double_t ntof); // Returns Tn in MeV for d in cm and ntof in ns.

    static Double_t CalcMultError(Double_t A, Double_t B, Double_t eA, Double_t eB);

    static Double_t CalcDiviError(Double_t A, Double_t B, Double_t eA, Double_t eB);

    static Double_t TimeOfFlight(Double_t chan, Double_t nsPerChan, Double_t gammaChan, Double_t d); // Converts from time difference in channels to TOF in ns.

};//END of class Utility

#endif

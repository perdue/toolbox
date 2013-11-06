#include <iostream>
using namespace std;

#include "Rkin.h"

int main(void)
{

  Rkin rk;

  // Set inputs to rkin.
  rk.SetDetectorAngle(150);
  rk.SetBeamEnergy(0.16);
  rk.SetReaction("10B(d,n)");

  cout << "Eb:       " << rk.GetBeamEnergy()       << " MeV" << endl;
  cout << "Reaction: " << rk.GetReaction()         << endl;
  cout << "Ex:       " << rk.GetExcitationEnergy() << " MeV" << endl;

  // Run rkin after inputs are set.
  rk.Run();

  cout << "Q:        " << rk.GetQValue()    << " MeV" << endl;
  cout << "En3:      " << rk.GetEnergy3()   << " MeV" << endl;
  cout << "LabAng3:  " << rk.GetLabAngle3() << " deg" << endl;
  cout << "CmAng3:   " << rk.GetCmAngle3()  << " deg" << endl;
  cout << "En4:      " << rk.GetEnergy4()   << " MeV" << endl;
  cout << "LabAng4:  " << rk.GetLabAngle4() << " deg" << endl;
  cout << "RatioP:   " << rk.GetRatioP()    << endl;

  // Look at Rkin.h to see how to interface to the rkin program
  // the "Fortran" way.

  return 0;
}

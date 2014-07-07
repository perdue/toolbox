#include "Utility.h"

///////////////////////////////////////////////////////////
const Double_t Utility::SpeedOfLight(void)
{
  return 29.9792458; // cm/ns

}//END of SpeedOfLight()

///////////////////////////////////////////////////////////
const Double_t Utility::NeutronMass(void)
{
  return 939.573;

}//END of NeutronMass()

///////////////////////////////////////////////////////////
const Double_t Utility::ProtonMass(void)
{
  return 938.280;

}//END of ProtonMass()

///////////////////////////////////////////////////////////
Double_t Utility::NeutronTof(Double_t d, Double_t Tn)
{
  Double_t c = SpeedOfLight(); // cm/ns
  Double_t En = NeutronMass(); // MeV

  // Relativistic formula
  Double_t ntof = d/c * (Tn + En) / sqrt( Tn*Tn + 2*Tn*En ); // ns

  return ntof;

}//END of NeutronTof()

///////////////////////////////////////////////////////////
Double_t Utility::NeutronEnergy(Double_t d, Double_t ntof)
{
  Double_t c = SpeedOfLight(); // cm/ns
  Double_t v = d/ntof;         // cm/ns
  Double_t En = NeutronMass(); // MeV

  // Relativistic formula
  Double_t gamma = 1 / sqrt(1 - v*v/c/c);
  Double_t Tn = En*(gamma - 1); // MeV

  return Tn;

}//END of NeutronEnergy()

///////////////////////////////////////////////////////////
Double_t Utility::CalcMultError(Double_t A, Double_t B, Double_t eA, Double_t eB)
{
  return sqrt( A*A*eB*eB + B*B*eA*eA );

}//END of CalcMultError()

//////////////////////////////////////////////////////////////
Double_t Utility::CalcDiviError(Double_t A, Double_t B, Double_t eA, Double_t eB)
{
  return 1/B*sqrt( eA*eA + A*A*eB*eB/(B*B) );

}//END of CalcDiviError()

///////////////////////////////////////////////////////////
Double_t Utility::TimeOfFlight(Double_t chan, Double_t nsPerChan, Double_t gammaChan, Double_t d)
{
  Double_t c = SpeedOfLight(); // cm/ns

  Double_t tof = nsPerChan * (chan - gammaChan) + d/c;

  return tof;

}//END of TimeOfFlight()

#include "LightOutput.h"

////////////////////////////////////////////////////////////////////////////////
LightOutput::LightOutput(TString dataPath)
 :
  fClassName("LightOutput")
{
  SetPathToData( dataPath );
  ReadData();

  fAlpha = 0;
  fBeta  = 0;
  fGamma = 0;

  fRandGen = new TRandom2();

  fIsVerbose = kTRUE;

}//END of constructor

////////////////////////////////////////////////////////////////////////////////
void LightOutput::SetPathToData(TString dataPath)
{
  fMethodName = "SetPathToData";

  Int_t length = dataPath.Length();
  if( dataPath[length-1] != '/' )
    dataPath += "/";

  fDataPath = dataPath;

  cout << fClassName << "::" << fMethodName << " - ";
  cout << "Reading light output tables from path: ";
  if( fDataPath.Length()>5 ) cout << "\n\t\t\t     " << fDataPath << endl;
  else                               cout << fDataPath << endl;

}//END of SetPathToData()

////////////////////////////////////////////////////////////////////////////////
void LightOutput::ReadData(void)
{
  fMethodName = "ReadData";

  TString cmd = Form("cd %s; ls light_table* > table_list", fDataPath.Data());
  system(cmd.Data());

  TString fName = Form("%stable_list",fDataPath.Data());
  ifstream f_in;
  f_in.open(fName);
  if( !f_in.is_open() )
  {
    cerr << fClassName << "::" << fMethodName << " - Error!  Could not open file: " << fName << endl;
    exit(0);

  }//END of if

  TString tableName;
  TObjArray* oa;
  while( !f_in.eof() ) // Loop until at the end-of-file (eof).
  {
    f_in >> tableName;

    // Strip away the name of the particle from the table name.
    oa = tableName.Tokenize(".");
    if( oa->At(2) )
    {
      fTable.push_back( tableName );
      fTableParticle.push_back( oa->At(2)->GetName() );
    }

  }//END of while loop

  f_in.close();

  // Ordered pair (energy deposited, light output).
  map< Double_t, Double_t > lightTable;

  for(Int_t p=0; p<(Int_t)fTableParticle.size(); p++)
  {
    // Open light output table file for reading.
    fName = fDataPath + fTable[p];
    f_in.open( fName.Data() );
    if( !f_in.is_open() )
    {
      cerr << fClassName << "::" << fMethodName << " - Error!  Could not open file: " << fName << endl;
      exit(0);

    }//END of if

    // Skip over the 2 comment lines.
    string tmp;
    getline(f_in, tmp);
    getline(f_in, tmp);

    Double_t edep; // Energy deposited.
    Double_t lo;   // Light output
    while( !f_in.eof() ) // Loop until at the end-of-file (eof).
    {
      f_in >> edep >> lo; // in MeV

      // Add ordered pair (edep, lo).
      lightTable[ edep ] = lo;

    }//END of while loop

    // Add ordered pair (particle, light output table).
    fParticleLightTable[ fTableParticle[p] ] = lightTable;
    lightTable.clear();

    f_in.close();

  }//END of p loop

  cmd = Form("cd %s; rm table_list", fDataPath.Data());
  system(cmd.Data());

  cout << fClassName << "::" << fMethodName << " - ";
  cout << "Light output tables available for : ";
  for(Int_t p=0; p<(Int_t)fTableParticle.size(); p++)
  {
    cout << fTableParticle[p] << " ";
    if( (p+1)%4==0 )
    {
      cout << endl;
      cout << "\t\t\t\t\t\t\t    ";
    }

  }//END of p loop
  cout << endl;

}//END of ReadData()

////////////////////////////////////////////////////////////////////////////////
void LightOutput::SetResolutionParameters(Double_t alpha, Double_t beta, Double_t gamma)
{
  fMethodName = "SetResolutionParameters";

  fAlpha = alpha;
  fBeta = beta;
  fGamma = gamma;

  cout << fClassName << "::" << fMethodName << " - ";
  cout << Form("(alpha, beta, gamma) = (%.2f, %.2f, %.2f)\n",fAlpha,fBeta,fGamma);

}//END of SetResolutionParameters()

////////////////////////////////////////////////////////////////////////////////
Double_t LightOutput::GetLightOutput(TString particleName, Double_t initialEnergy,
                                     Double_t energyDeposited)
{
  fMethodName = "GetLightOutput";

  // Assign values to private class variables.
  fInitialEnergy = initialEnergy;
  fEnergyDeposited = energyDeposited;

  // Find the name of the table particle that corresponds to the particle
  // that was being tracked in the sensitive volume of the detector.
  TString tableParticle = GetTableParticle( particleName );

  // Get the light output for the table particle.
  Double_t lo = GetLightOut( tableParticle );

  // Apply detector resolution
  Double_t smeared = Smear( lo );

  return smeared;

}//END of GetLightOutput()

////////////////////////////////////////////////////////////////////////////////
TString LightOutput::GetTableParticle(TString particleName)
{
  fMethodName = "GetTableParticle";

  TString particle;

  if( particleName.Contains("e-") || particleName.Contains("e+") )
  {
    particle = "electron";
  }
  else if( particleName.Contains("H3") )
  {
    particle = "triton";
  }
  else
  {
    // Check to see if the particle has a light output table available.
    Bool_t hasTable = kFALSE;
    Int_t pMax = 0;
    for(Int_t p=0; p<(Int_t)fTableParticle.size(); p++)
    {
      hasTable = particleName.Contains( fTableParticle[p] );
      pMax = p;

      if( hasTable )
      {
        particle = fTableParticle[p];
        break;
      }

    }//END of p loop

    // If the particle doesn't have a light output table available,
    // then print out a warning message.
    if( !hasTable )
    {
      particle = particleName + " table not found";

      if( fIsVerbose )
      {
        cout << endl;
        cout << endl;
        cout << fClassName << "::" << fMethodName << " - ";
        cout << "Warning!  No light output table found for \"" << particleName << "\"" << endl;
      }
    }

  }//END of else

  return particle;

}//END of GetTableParticle()

////////////////////////////////////////////////////////////////////////////////
Double_t LightOutput::GetLightOut(TString tableParticle)
{
  fMethodName = "GetLightOut";

  Double_t lo  = 0.0;
  Double_t lo1 = 0.0;
  Double_t lo2 = 0.0;

  if( tableParticle.Contains("table not found") )
  {
      Int_t length = tableParticle.Length();
      TString particleName = tableParticle(0, length-16);

      lo = 0.0;

      if( fIsVerbose )
      {
        cout << fClassName << "::" << fMethodName << " - ";
        cout << "Warning!  Light output set to zero for \"" << particleName << "\"" << endl;
        cout << endl;
      }
  }
  else if( tableParticle=="electron" )
  {
    lo = fEnergyDeposited;
  }
  else
  {
    lo1 = Interpolate( tableParticle, fInitialEnergy );
    lo2 = Interpolate( tableParticle, fInitialEnergy - fEnergyDeposited );

    lo = lo1 - lo2;
  }

  return lo;

}//END of GetLightOut()

////////////////////////////////////////////////////////////////////////////////
Double_t LightOutput::Interpolate(TString particle, Double_t energy)
{
  Double_t engLowerBound = 0;
  Double_t engUpperBound = 0;
  Double_t lowerLightOut = 0;
  Double_t upperLightOut = 0;
  Double_t light         = 0;
  Double_t m             = 1;
  Double_t b             = 0;

  // Ordered pair (energy deposited, light output).
  map< Double_t, Double_t >& lightTable = fParticleLightTable[ particle ];

  // Interator used to access elements in the light output table.
  map< Double_t, Double_t >::iterator iter;

  if( energy > 0 )
  {
    iter = lightTable.upper_bound( energy ); // Get iterator of element with "key" greater than "energy".
    engUpperBound = iter->first;             // Get key (the energy value) from the map element.
    upperLightOut = iter->second;            // Get val (the light output value) from the map element.
    if( iter != lightTable.begin() ) // If the iterator is not at the first element in the map, ...
    {
      iter--;                        // Point iterator at one element below the current one.
      engLowerBound = iter->first;   // Get key
      lowerLightOut = iter->second;  // Get val
    }

    // Linear interpolation between light output data points.
    m = (upperLightOut - lowerLightOut)/(engUpperBound - engLowerBound); // slope: m = (y2-y1)/(x2-x1)
    b = upperLightOut - m*engUpperBound;                                 // intercept: b = y2 - m*x2
    light = m*energy + b;                                                // y = m*x + b

  }//END of if

  return light;

}//END of Interpolate()

////////////////////////////////////////////////////////////////////////////////
Double_t LightOutput::Smear(Double_t lightOut)
{
  Double_t fwhm = lightOut * TMath::Sqrt( fAlpha*fAlpha
                                          + fBeta*fBeta/lightOut
                                          + fGamma*fGamma/lightOut/lightOut );

  Double_t sigma = fwhm/2.35482;

  Double_t smeared = lightOut;
  if( lightOut>0 ) smeared += fRandGen->Gaus(0,sigma);

  return smeared;

}//END of Smear()

////////////////////////////////////////////////////////////////////////////////
void LightOutput::SetQuiet(void)
{
  fMethodName = "SetQuiet";

  fIsVerbose = kFALSE;

  cout << fClassName << "::" << fMethodName << " - ";
  cout << "Warning! All additional warnings have been suppressed" << endl;

}//END of SetMaterial()


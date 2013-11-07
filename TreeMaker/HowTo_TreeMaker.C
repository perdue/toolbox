/*
    To run this example macro:
    $ root
    root [0] .L TreeMaker.cc+
    root [1] .x HowTo_TreeMaker.C
    root [2] .q

    Note: The TreeMaker class cannot be used uncompiled.  This was
          not by design.  I used the C++ standard container "map"
          to dynamically allocate TBranch variables in the TreeMaker
          class.  The ROOT interpreter (cint) seems to have trouble
          with maps, but can handle compiled code that uses maps.

    Brent Perdue, 04/20/2011
*/

#include "TBrowser.h"
#include "TreeMaker.h"

void HowTo_TreeMaker(void)
{
  // This calls the private constructor.  This is the
  // only way to construct a TreeMaker object.  Once
  // a TreeMaker object has been constructed, that
  // will be the only one that will be created in your
  // program (singleton class).
  TreeMaker *t0 = TreeMaker::GetTreeMakerPointer();

  // Creates storage ROOT file and tree inside that file.
  TString path = "./";
  TString fileName = "OutFile";
  TString treeName = "tree";
  Int_t compression = 3;
  t0->CreateNewFileAndTree( path, fileName, compression, treeName );

  t0->SetDisplayInterval( 2 /*seconds*/ ); // Should be 2 or greater.

  Int_t nEvents = 100000;
  t0->SpecifyNumberOfEvents(nEvents); // Used for display purposes only.

  // Define new variables to store event information.
  Int_t event;
  Double_t varD;
  const Int_t nEls = 3;
  Double_t array[nEls];
  TString name;

  for(Int_t i=0; i<nEvents; i++)
  {
    event = i+1; // Event number

    t0->UpdateDisplay(event);

    varD = (Double_t)i*0.33;  // Giving varD some value of this event.

    for(Int_t el=0; el<nEls; el++) // Giving the elements of the array
      array[el] = el*el*0.345;     // some values.

    name = Form("text%d",i%2);     // Giving the string a value.
    if( i%5==0 ) name = "text2";

    // FillBranch creates a branch in the tree and the
    // associated branch variable if needed. Then it
    // fills the branch variable with the correct values.
    t0->FillBranch("event", &event);           // Storing an integer.
    t0->FillBranch("varD",  &varD);            // Storing a double.
    t0->FillBranch("array", &array[0],nEls);   // Storing an array of doubles.
    t0->FillBranch("name",  &name);            // Storing a string of characters.

    t0->FillTree();// Adds new variable row to the tree.

  }//END of i loop

  // This pointer, t1, will point to the same object as t0.
  // This demonstrates how you would get another pointer to
  // the TreeMaker object, inside a different class for example.
  // In this macro, two pointers to one object is not needed.
  TreeMaker *t1 = TreeMaker::GetTreeMakerPointer();
  t1->SaveTree();  // Flushed last nonfull buffer.  This is needed.
  t1->CloseCurrentFile(); // Close the ROOT storage file.
  t1->Destroy();  // Explicitly destroy the TreeMaker object.

  // Viewing the resulting tree.
  TFile *f = new TFile(fileName + ".root");
  new TBrowser;

  TTree *t = (TTree*)f->Get(treeName);
  t->Scan("event:varD:array[1]:name");

}//END of HowTo_TreeMaker()

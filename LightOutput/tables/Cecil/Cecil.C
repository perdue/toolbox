void Cecil(void)
{
  write_proton_table();
  write_alpha_table();

}//END of Cecil()

void write_proton_table(void)
{
  // Cecil coefficients
  Double_t a1 = 0.83;
  Double_t a2 = 2.82;
  Double_t a3 = 0.25;
  Double_t a4 = 0.93;

  TGraph* g_light = new TGraph();
  ofstream table("light_table.Cecil.proton");

  table << "#Cecil parameterization of light output in MeV for proton\n";
  table << "#Generated from parameterization of NIM 161 (1979) 439-447.\n";

  Int_t j = 0;


  for(Double_t E=0.0; E<0.09; E+=0.01)
  {
    Double_t x = E;
    Double_t yy = a1*0.09 - a2*( 1.0 - exp( -a3*pow(0.09,a4) ) );

    Double_t m = yy/0.09;

    Double_t y = m*x;

    g_light->SetPoint(j,x,y);
    j++;

    table << Form("%.2f\t\t",x);
    table << Form("%.4f\n",y);

  }//END of E loop

  for(Double_t E=0.09; E<20.0; E+=0.01)
  {
    Double_t x = E;
    Double_t y = a1*E - a2*( 1.0 - exp( -a3*pow(E,a4) ) );

    g_light->SetPoint(j,x,y);
    j++;

    table << Form("%.2f\t\t",x);
    table << Form("%.4f\n",y);

  }//END of E loop

  for(Double_t E=20; E<=350; E+=1)
  {
    Double_t x = E;
    Double_t y = a1*E - a2*( 1.0 - exp( -a3*pow(E,a4) ) );

    g_light->SetPoint(j,x,y);
    j++;

    table << Form("%.0f\t\t",x);
    table << Form("%.2f\n",y);

  }//END of E loop

  g_light->SetName("g_proton");
  g_light->SetTitle("Cecil parameterization for proton light output");
  g_light->GetXaxis()->SetTitle("E_{p} (MeV)");
  g_light->GetYaxis()->SetTitle("L_{p} (MeVee)");
  g_light->GetXaxis()->CenterTitle();
  g_light->GetYaxis()->CenterTitle();

  table.close();

  new TCanvas;
  g_light->Draw("al");

}//END of write_proton_table()

void write_alpha_table(void)
{
  // Cecil coefficients
  Double_t a1 = 0.410;
  Double_t a2 = 5.900;
  Double_t a3 = 0.065;
  Double_t a4 = 1.010;

  TGraph* g_light = new TGraph();

  ofstream table("light_table.Cecil.alpha");

  table << "#Cecil parameterization of light output in MeV for alpha\n";
  table << "#Generated from parameterization of NIM 161 (1979) 439-447.\n";

  Int_t j = 0;
  for(Double_t E=0.0; E<20.0; E+=0.01)
  {
    Double_t x = E;
    Double_t y = a1*E - a2*( 1.0 - exp( -a3*pow(E,a4) ) );

    if( y<0 ) y=0;

    g_light->SetPoint(j,x,y);
    j++;

    table << Form("%.2f\t\t",x);
    table << Form("%.4f\n",y);

  }//END of E loop

  for(Double_t E=20; E<=350; E+=1)
  {
    Double_t x = E;
    Double_t y = a1*E - a2*( 1.0 - exp( -a3*pow(E,a4) ) );

    g_light->SetPoint(j,x,y);
    j++;

    table << Form("%.0f\t\t",x);
    table << Form("%.2f\n",y);

  }//END of E loop

  g_light->SetName("g_alpha");
  g_light->SetTitle("Cecil parameterization for alpha light output");
  g_light->GetXaxis()->SetTitle("E_{#alpha} (MeV)");
  g_light->GetYaxis()->SetTitle("L_{#alpha} (MeVee)");
  g_light->GetXaxis()->CenterTitle();
  g_light->GetYaxis()->CenterTitle();

  table.close();

  new TCanvas;
  g_light->Draw("al");

}//END of write_alpha_table()

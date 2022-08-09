/**
Written by Austin de St Croix
Calculating the true threshold for SBC, or the
Threshold Transformation from heat to kinetic energy
(see README for more physics background)
**/

#include <iostream>
#include <stdlib.h>
#include <format>

#include "NEST.hh"
#include "VDetector.hh"

#include "LArDetector.hh"
#include "LArNEST.hh"

#include "TH1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"

#include "TF1.h"

using namespace std;
// using namespace NEST;

// helpful code, functions and constants (imported in a lazy way. no compiling or linking)
#include "helpers.cc"
#include "sbctt_ds.cc"

// ---- Physics Notes ---- //
// Right now ignoring fluctuations and photon quantization


// gap energies taken from Aprile review paper
sbc_medium makeArgon(float rho){
  sbc_medium ar;
  ar.wl = 128.;
  ar.egap = 14.3;
  ar.wval = 23.6; // probably lower
  ar.rho = rho;
  
  ar.pdimer = -1;
  ar.excimer = -1;
  return ar;
}


int main(int argc, char** argv){
  
  cout << "hello austin!" << endl;
  TApplication app("ROOT Application", &argc, argv);
  
  sbc_medium ar(128., 14.3, 23.6, 1.4);
  
  uint64_t seed = time(NULL);

  double density = 1.4;
  vector<double> efieldList = {1, 100, 250, 500, 1000};
  
  // in keV
  double lowE = 0.1;
  double highE = 1e3;
  double estep = 0.1;
  // estep = 5;
  int numSteps = (highE - lowE)/estep;
  vector<double> energy_vals = MCH::MakeVector_log(-1,3,100);
  
  // vector<double> energy_vals = MCH::MakeVector_linear(lowE, highE, estep);
  // vector<double> energy_vals = MCH::MakeVector_log(lowE, highE, 10);
  
  

  // TGraph * gly = new TGraph(); // Light Yield
  // TGraph * gqy = new TGraph(); // Light Yield
  // TGraph * glNPE = new TGraph(); // Light Yield
  // 
  // TGraph * gKvsHeat_light = new TGraph(); // Kinetic Energy vs Heat
  // TGraph * gKvsHeat_lightcharge = new TGraph(); // Kinetic Energy vs Heat
  // 
  // TGraph * gKvsHeat_A = new TGraph();
  // TGraph * gKvsHeat_B = new TGraph();
  // TGraph * gKvsHeat_C = new TGraph();
  
  // TGraph * gKvsHeat2 = new TGraph(); // Kinetic Energy vs Heat
  TMultiGraph * mg_KvsHeat_light = new TMultiGraph();
  vector<TGraph*> vg_kvh_light;
  TMultiGraph * mg_KvsHeat_lightcharge = new TMultiGraph();
  vector<TGraph*> vg_kvh_lightcharge;
  
  vector<TGraph*> vg_totalYields;
  vector<TGraph*> vg_totalYieldsTrue;
  vector<TGraph*> vg_lightYields;
  vector<TGraph*> vg_chargeYields;
  
  // Nest Stuff //
  LArDetector* detector = new LArDetector();
  // Construct NEST class using detector object
  NEST::LArNEST larnest(detector);
  RandomGen::rndm()->SetSeed(seed);   // Set random seed of RandomGen class
  
  NEST::LArInteraction nr = NEST::LArInteraction::NR;
  NEST::LArInteraction er = NEST::LArInteraction::ER;
  // ER - field from 1-9500
  // NR - field from 1-2000
  // Alpha      from 1-20000
  
  // Construct NEST objects for storing calculation results
  NEST::LArNESTResult result;
  
  // cout << larnest.GetNRPhotonYields(0.1, 0.001) << endl;
  // cout << larnest.GetNRPhotonYields(1, 1) << endl;
  // cout << larnest.GetNRPhotonYields(10, 1) << endl;
  // cout << larnest.GetNRPhotonYields(100, 1) << endl;
  // cout << larnest.GetNRPhotonYields(1000, 1) << endl;
  
  for(int iefield = 0; iefield < efieldList.size(); iefield++){
    double Efield = efieldList[iefield];
    cout << Efield << endl;
    
    TGraph * gkvh_light = new TGraph();
    TGraph * gkvh_lightcharge = new TGraph();
    
    TGraph * gTotalYields = new TGraph();
    TGraph * gTotalYieldsTrue = new TGraph();
    TGraph * gLightYields = new TGraph();
    TGraph * gChargeYields = new TGraph();
    
    for(int i = 0; i < energy_vals.size(); i++){
    
      double K = energy_vals[i];
      result = larnest.FullCalculation(NEST::LArInteraction::ER, K, Efield, density, false);
    
      // gly->AddPoint(K, result.yields.LightYield);
      // gqy->AddPoint(K, result.yields.QuantaYield);
      // glNPE->AddPoint(K, result.yields.Nph);
      
      // doing heat calculation
      double npe = result.yields.Nph;
      double E_light = npe * ar.wleV / keV;
      
      double nq = result.yields.Ne;
      double E_charge = nq * ar.egap / keV;
      if(K > 1. && K < 1. + estep){
        printf("NL = %f, Nq = %f\n", npe, nq);
        printf("K = %f, E_l = %f, E_q = %f\n", K, E_light, E_charge);
        printf("Elost = E_l + E_q = %f", E_light + E_charge);
        printf("\n\n");
      }

      // yields
      gTotalYields->AddPoint(K, result.yields.TotalYield);
      gLightYields->AddPoint(K, result.yields.LightYield);
      gChargeYields->AddPoint(K, result.yields.QuantaYield);
      gTotalYieldsTrue->AddPoint(K, result.yields.QuantaYield + result.yields.LightYield);
      
      // gKvsHeat_light->AddPoint(K, K-E_light);
      // gKvsHeat_lightcharge->AddPoint(K, K - E_light - E_charge);
      gkvh_light->AddPoint(K, (K - E_light)/K);
      gkvh_lightcharge->AddPoint(K, (K - E_light-E_charge)/K);
    }
  
  // total yields //
  vg_totalYields.emplace_back(gTotalYields);
  vg_totalYieldsTrue.emplace_back(gTotalYieldsTrue);
  vg_lightYields.emplace_back(gLightYields);
  vg_chargeYields.emplace_back(gChargeYields);
  
  // head production
  mg_KvsHeat_light->Add(gkvh_light);
  mg_KvsHeat_lightcharge->Add(gkvh_lightcharge);
  vg_kvh_light.emplace_back(gkvh_light);
  vg_kvh_lightcharge.emplace_back(gkvh_lightcharge);
  // mg->Add(gly);
  }
  
  //////////// drawing ////////////
  TCanvas * c1 = new TCanvas();

  auto gb = new TGraph();
  gb->SetMinimum(0.0);
  // gb->SetMaximum(TMath::MaxElement(vg_totalYields[0]->GetN(),vg_totalYields[0]->GetY()));
  gb->SetMaximum(100.);
  gb->AddPoint(lowE, 0);
  gb->AddPoint(highE, 0);
  gb->Draw("");
  
  TLegend * leg = new TLegend();
  leg->SetHeader("E fields (V/cm)", "C");
  leg->SetNColumns(2);
    
  for(int iE = 0; iE < efieldList.size(); iE++){
  
    int color = iE + 1;
    vg_totalYields[iE]->SetLineColor(color);
    vg_totalYields[iE]->SetLineWidth(3);
    vg_totalYields[iE]->SetLineStyle(1);
    vg_totalYields[iE]->Draw("L");
    
    vg_totalYieldsTrue[iE]->SetLineColor(color);
    vg_totalYieldsTrue[iE]->SetLineWidth(3);
    vg_totalYieldsTrue[iE]->SetLineStyle(7);
    vg_totalYieldsTrue[iE]->Draw("L");
  
    vg_chargeYields[iE]->SetLineColor(color);
    vg_chargeYields[iE]->SetLineWidth(3);
    vg_chargeYields[iE]->SetLineStyle(4);
    vg_chargeYields[iE]->Draw("C");
  
    vg_lightYields[iE]->SetLineColor(color);
    vg_lightYields[iE]->SetLineStyle(9);
    vg_lightYields[iE]->SetLineWidth(3);
    vg_lightYields[iE]->Draw("C");
    
    string a = to_string((int)efieldList[iE]);
    leg->AddEntry(vg_totalYields[iE], a.c_str(), "L");
  }
  TLegend * leg2 = new TLegend();
  leg2->SetNColumns(2);
  leg2->AddEntry(vg_totalYields[0], "total (NEST?)", "L");
  leg2->AddEntry(vg_totalYieldsTrue[0], "total (summed)", "L");
  leg2->AddEntry(vg_chargeYields[0], "charge", "L");
  leg2->AddEntry(vg_lightYields[0], "light", "L");

  gb->GetXaxis()->SetTitle("Energy (keV)");
  gb->GetYaxis()->SetTitle("Yield (quanta/keV)");
  gb->SetTitle("NEST LAr yields, vSomething");
  
  leg->Draw("same");
  leg2->Draw("same");
  c1->SetLogx();
  c1->Update();
  
  //////////// Heat vs K ////////////
  auto c = new TCanvas();
  gStyle->SetPalette(kSolar);
  leg = new TLegend();
  leg->SetHeader("Energy removal source", "C");
  leg->SetNColumns(2);
  
  int nef = efieldList.size() - 1;
  
  vg_kvh_light[0]->SetMaximum(1.01);
  vg_kvh_light[0]->SetMinimum(0.0);
  
  
  vg_kvh_light[0]->SetLineColor(1);
  vg_kvh_light[0]->SetLineStyle(2);
  vg_kvh_light[0]->SetLineWidth(3);
  vg_kvh_light[0]->Draw("AC"); // low E field
  
  vg_kvh_light[nef]->SetLineColor(2); // high E field
  vg_kvh_light[nef]->SetLineStyle(2);
  vg_kvh_light[nef]->SetLineWidth(3);
  vg_kvh_light[nef]->Draw("C");

  vg_kvh_lightcharge[0]->SetLineColor(1);
  vg_kvh_lightcharge[0]->SetLineWidth(3);
  vg_kvh_lightcharge[0]->Draw("C");
  
  vg_kvh_lightcharge[nef]->SetLineColor(2);
  vg_kvh_lightcharge[nef]->SetLineWidth(3);
  vg_kvh_lightcharge[nef]->Draw("C");
  
  // TF1 fline("fline", "x", lowE, highE);
  // // fline.Draw("same");
  
  leg->AddEntry(vg_kvh_light[0], ("light, E = " +
  to_string((int)efieldList[0]) + " V/cm").c_str(), "L");
  leg->AddEntry(vg_kvh_light[nef], ("light, E = " +
  to_string((int)efieldList[nef]) + " V/cm").c_str(), "L");
  
  leg->AddEntry(vg_kvh_lightcharge[0], ("light+charge, E = " +
  to_string((int)efieldList[0]) + " V/cm").c_str(), "L");
  leg->AddEntry(vg_kvh_lightcharge[nef], ("light+charge, E =" +
  to_string((int)efieldList[nef]) + " V/cm").c_str(), "L");
  
  vg_kvh_light[0]->GetXaxis()->SetTitle("Kinetic Energy (keV)");
  vg_kvh_light[0]->GetYaxis()->SetTitle("Fraction as heat (keV)");
  vg_kvh_light[0]->SetTitle("Primary energy conversion to Heat");
  
  leg->Draw("same");
  
  // c->SetLogy();
  c->SetLogx();
  c->Update();
  // std::cout << std::format("Hello {}!\n", "world");
  // string s = str( format("%2% %2% %1%\n") % "world" % "hello" );
  
  app.Run();

  return 0;
}
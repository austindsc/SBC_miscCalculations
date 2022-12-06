/**
Written by Austin de St Croix
Calculating the true threshold for SBC, or the
Threshold Transformation from heat to kinetic energy
(see README for more physics background)
**/

#include <iostream>
#include <stdlib.h>
#include <format> // doesn't work
#include <map>

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
  ar.ionInLiquid = 1.5;
  
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
  vector<double> efieldList = {1, 250, 1000};
  
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
  vector<TGraph*> vg_kvh_lightcharge_xenon;
  
  vector<TGraph*> vg_er_kvh_light;
  vector<TGraph*> vg_er_kvh_lightcharge;
  
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
    TGraph * gkvh_lightcharge_xenon = new TGraph();
    
    TGraph * gTotalYields = new TGraph();
    TGraph * gTotalYieldsTrue = new TGraph();
    TGraph * gLightYields = new TGraph();
    TGraph * gChargeYields = new TGraph();
    
    TGraph * g_er_kvh_light = new TGraph();
    TGraph * g_er_kvh_lightcharge = new TGraph();
    
    for(int i = 0; i < energy_vals.size(); i++){
    
      double K = energy_vals[i];
      result = larnest.FullCalculation(NEST::LArInteraction::NR, K, Efield, density, false);
      // gly->AddPoint(K, result.yields.LightYield);
      // gqy->AddPoint(K, result.yields.QuantaYield);
      // glNPE->AddPoint(K, result.yields.Nph);
      
      // doing heat calculation
      double npe = result.yields.Nph;
      double E_light = npe * ar.wleV / keV;
      double E_light_Xe = npe*1240/178/keV;
      
      double nq = result.yields.Ne;
      double E_charge = nq * (ar.egap + ar.ionInLiquid)/ keV;
      if(K > 10. && K < 10. + estep){
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
      gkvh_lightcharge_xenon->AddPoint(K, (K - E_light_Xe - E_charge)/K);
      
      // doing ERs
      result = larnest.FullCalculation(NEST::LArInteraction::ER, K, Efield, density, false);
      E_light = result.yields.Nph * ar.wleV/keV;
      E_charge = result.yields.Ne * ar.egap/keV;
      g_er_kvh_light->AddPoint(K, (K - E_light)/K);
      g_er_kvh_lightcharge->AddPoint(K, (K - E_light - E_charge)/K);
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
  vg_kvh_lightcharge_xenon.emplace_back(gkvh_lightcharge_xenon);
  
  vg_er_kvh_light.emplace_back(g_er_kvh_light);
  vg_er_kvh_lightcharge.emplace_back(g_er_kvh_lightcharge);
  }
  
  //////////// drawing ////////////
  gStyle->SetLegendTextSize(0.02);
  
  TCanvas * c1 = new TCanvas();

  auto gb = new TGraph();
  gb->SetMinimum(0.0);
  // gb->SetMaximum(TMath::MaxElement(vg_totalYields[0]->GetN(),vg_totalYields[0]->GetY()));
  gb->SetMaximum(30.);
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
  std::map<std::string, int> pcolors { {"er", 9}, {"nr", 15} };
  std::map<std::string, int> efStyle { {"low", 1}, {"mid", 3}, {"high", 7} };
  std::map<std::string, int> efColor { {"low", 1}, {"mid", 4}, {"high", 2} };
  // particle type
  // efield 
  // model type
  
  auto c = new TCanvas();
  gStyle->SetPalette(kSolar);
  
  gb = new TGraph();
  gb->SetMinimum(0.0);
  // gb->SetMaximum(TMath::MaxElement(vg_totalYields[0]->GetN(),vg_totalYields[0]->GetY()));
  gb->SetMaximum(1.01);
  gb->AddPoint(lowE, 0.);
  gb->AddPoint(highE, 1.01);
  gb->SetLineWidth(0);
  gb->Draw("");
  
  leg = new TLegend();
  leg->SetHeader("Physics Process", "C");
  leg->SetNColumns(2);
  
  int nef = efieldList.size() - 1;
  
  // for(int iE = 0; iE < efieldList.size(); iE++){
  
  // vg_kvh_light[0]->SetLineColor(efColor["low"]);
  // vg_kvh_light[0]->SetLineStyle(2);
  // vg_kvh_light[0]->SetLineWidth(3);
  // vg_kvh_light[0]->Draw("C"); // low E field, incomplete model

  // vg_kvh_light[nef]->SetLineColor(efColor["high"]); // high E field, incomplete
  // vg_kvh_light[nef]->SetLineStyle(2);
  // vg_kvh_light[nef]->SetLineWidth(3);
  // vg_kvh_light[nef]->Draw("C");

  vg_kvh_lightcharge[0]->SetLineColor(kRed); // low E field, full model
  // vg_kvh_lightcharge[0]->SetLineStyle(9);
  vg_kvh_lightcharge[0]->SetLineWidth(3);
  vg_kvh_lightcharge[0]->Draw("C");
  
  vg_kvh_lightcharge[nef]->SetLineColor(kBlue); // high E field, full model
  // vg_kvh_lightcharge[nef]->SetLineStyle(9);
  vg_kvh_lightcharge[nef]->SetLineWidth(3);
  vg_kvh_lightcharge[nef]->Draw("C");


  vg_kvh_lightcharge_xenon[0]->SetLineColor(kRed); // Xe light
  vg_kvh_lightcharge_xenon[0]->SetLineWidth(3);
  vg_kvh_lightcharge_xenon[0]->SetLineStyle(9);
  vg_kvh_lightcharge_xenon[0]->Draw("C");

  vg_kvh_lightcharge_xenon[nef]->SetLineColor(kBlue);
  vg_kvh_lightcharge_xenon[nef]->SetLineWidth(3);
  vg_kvh_lightcharge_xenon[nef]->SetLineStyle(9);
  vg_kvh_lightcharge_xenon[nef]->Draw("C");
  
  // vg_er_kvh_lightcharge[0]->SetLineColor(efColor["low"]); // ER, low field, full model
  // vg_er_kvh_lightcharge[0]->SetLineStyle(9);
  // vg_er_kvh_lightcharge[0]->SetLineWidth(4);
  // vg_er_kvh_lightcharge[0]->Draw("C");
  
  // vg_er_kvh_lightcharge[nef]->SetLineColor(efColor["high"]); // ER, high field, full model
  // vg_er_kvh_lightcharge[nef]->SetLineStyle(9);
  // vg_er_kvh_lightcharge[nef]->SetLineWidth(4);
  // vg_er_kvh_lightcharge[nef]->Draw("C");
  
  TF1 fline("fline", "1./x", lowE, highE);
  fline.SetLineWidth(6);
  fline.SetLineColorAlpha(kGreen, 0.3);
  fline.Draw("same");

  TF1 fline2("fline", "0.1/x", lowE, highE);
  fline2.SetLineWidth(6);
  fline2.SetLineColorAlpha(kMagenta, 0.3);
  fline2.Draw("same");
  
  // leg->AddEntry(vg_kvh_light[0], ("light, E = " +
  // to_string((int)efieldList[0]) + " V/cm").c_str(), "L");
  // leg->AddEntry(vg_kvh_light[nef], ("Ar light, E = " +
  // to_string((int)efieldList[nef]) + " V/cm").c_str(), "L");
  
  leg->AddEntry(vg_kvh_lightcharge[0], ("Ar light + charge, E = " +
  to_string((int)efieldList[0]) + " V/cm").c_str(), "L");

  leg->AddEntry(vg_kvh_lightcharge[nef], ("Ar light + charge, E =" +
  to_string((int)efieldList[nef]) + " V/cm").c_str(), "L");

  leg->AddEntry(vg_kvh_lightcharge_xenon[0], ("Xe light + charge, E = " +
  to_string((int)efieldList[0]) + " V/cm").c_str(), "L");

  leg->AddEntry(vg_kvh_lightcharge_xenon[nef], ("Xe light + charge, E = " +
  to_string((int)efieldList[nef]) + " V/cm").c_str(), "L");
  
  // leg->AddEntry(vg_er_kvh_lightcharge[0], ("ER: light+charge, E = " +
  // to_string((int)efieldList[0]) + " V/cm").c_str(), "L");
  // leg->AddEntry(vg_er_kvh_lightcharge[nef], ("ER: light+charge, E =" +
  // to_string((int)efieldList[nef]) + " V/cm").c_str(), "L");
  
  leg->AddEntry(&fline, "1 keV heat threshold", "L");
  leg->AddEntry(&fline2, "100 eV heat threshold", "L");
  
  gb->GetXaxis()->SetTitle("NR Kinetic Energy (keV)");
  gb->GetYaxis()->SetTitle("Fraction as heat (keV)");
  gb->SetTitle("NR: Energy converted to Heat via NEST Yields");
  
  leg->Draw("same");
  
  // c->SetLogy();
  c->SetGrid();
  c->SetLogx();
  c->Update();
  // std::cout << std::format("Hello {}!\n", "world");
  // string s = str( format("%2% %2% %1%\n") % "world" % "hello" );
  
  app.Run();

  return 0;
}
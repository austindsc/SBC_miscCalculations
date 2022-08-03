/**
Written by Austin de St Croix
Calculating the true threshold for SBC, or the
Threshold Transformation from heat to kinetic energy
(see README for more physics background)
**/

#include <iostream>
#include <stdlib.h>

#include "NEST.hh"
#include "VDetector.hh"

#include "LArDetector.hh"
#include "LArNEST.hh"

#include "TH1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"

#include "TF1.h"

using namespace std;
// using namespace NEST;

// helpful code, functions and constants (imported in a lazy way. no compiling or linking)
#include "helpers.cc"
#include "sbctt_ds.cc"


// Right now ignoring fluctuations and photon quantization



int main(int argc, char** argv){
  
  cout << "hello austin!" << endl;
  TApplication app("ROOT Application", &argc, argv);
  
  uint64_t seed = time(NULL);

  double density = 1.1;
  double Efield = 0.001;
  NEST::LArInteraction ptype = NEST::LArInteraction::NR;
  // ER - field from 1-9500
  // NR - field from 1-2000
  // Alpha      from 1-20000
  
  double lowE = 0.1;
  double highE = 1e3;
  double estep = 0.5;
  int numSteps = (highE - lowE)/estep;
  vector<double> energy_vals = MCH::MakeVector_linear(lowE, highE, estep);
  
  
  TGraph * gly = new TGraph(); // Light Yield
  TGraph * glNPE = new TGraph(); // Light Yield
  
  TGraph * gKvsHeat = new TGraph(); // Kinetic Energy vs Heat
  TGraph * gKvsHeat2 = new TGraph(); // Kinetic Energy vs Heat
  auto mg = new TMultiGraph();
  
  // TGraph * straight
  // std::vector<NEST::LArInteraction> particle_types = {
  //     NEST::LArInteraction::NR, NEST::LArInteraction::ER, NEST::LArInteraction::Alpha
  // };
  // 
  
  LArDetector* detector = new LArDetector();
  // Construct NEST class using detector object
  NEST::LArNEST larnest(detector);
  // Set random seed of RandomGen class
  RandomGen::rndm()->SetSeed(seed);
  
  // cout << GetELost(10);
  // cout << larnest.GetNRPhotonYields(0.1, 0.001) << endl;
  // cout << larnest.GetNRPhotonYields(1, 1) << endl;
  // cout << larnest.GetNRPhotonYields(10, 1) << endl;
  // cout << larnest.GetNRPhotonYields(100, 1) << endl;
  // cout << larnest.GetNRPhotonYields(1000, 1) << endl;

  // Construct NEST objects for storing calculation results
  NEST::LArNESTResult result;
  
  
  for(int i = 0; i < energy_vals.size(); i++){
  
    double K = energy_vals[i];
    result = larnest.FullCalculation(ptype, K, Efield, density, false);
  
    double ly = result.yields.LightYield;
    double qy = result.yields.QuantaYield;
  
    gly->AddPoint(K, ly);
    glNPE->AddPoint(K, result.yields.Nph);
    // gqy->AddPoint(E, nq);
    
    double npe = result.yields.Nph;
    double el_light = GetELost_Light(npe, 128.);
    double Eleft = K - el_light;
    
    gKvsHeat->AddPoint(K, Eleft);
  
  }
  
  mg->Add(gKvsHeat);
  // mg->Add(gly);
  
  // drawing //
  gKvsHeat->SetLineWidth(3);
  
  auto c = new TCanvas();
  gStyle->SetPalette(kSolar);
  // gly->SetMinimum(0.);
  // gly->SetLineColor(2);
  // 
  // gly->Draw("AL");
  // 
  // glNPE->SetLineColor(3);
  // // gqy->Draw("L same");
  // // gly2->Draw("L same");


  mg->Draw("A pmc plc");
  
  TF1 fline("fline", "x", lowE, highE);
  fline.Draw("same");
  // mg->SetMinimum(0.1);
  
  // c->SetLogy();
  // c->SetLogx();
  c->Update();
  
  
  app.Run();

  return 0;
}
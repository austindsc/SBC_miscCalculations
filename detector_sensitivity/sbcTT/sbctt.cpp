#include <iostream>
#include <stdlib.h>

#include "TH1.h"

#include "NEST.hh"
#include "VDetector.hh"

#include "LArDetector.hh"
#include "LArNEST.hh"

using namespace std;
// using namespace NEST;

extern const float hc = 1239.8;

// right now ignore fluctuations and photon quantization

// get energy lost to photons
double GetELost(float npe, float wl = 128.){
  float ep = hc/wl;
  return npe*ep;
}

int main(){
  
  cout << "heello austin!" << endl;
  
  // VDetector* det = new VDetector();
  // 
  // NESTcalc nc(det);
  
  
  double density = 1.4;
  double Efield = 0.001;
  NEST::LArInteraction ptype = NEST::LArInteraction::NR;
  // ER - field from 1-9500
  // NR - field from 1-2000
  
  uint64_t seed = time(NULL);
  
  double lowE = 1;
  double highE = 1000;
  double estep = 0.5;
  int numSteps = (highE - lowE)/estep;
  vector<double> energy_vals;
  
  for(int i = 0; i < numSteps; i++){
    energy_vals.emplace_back(lowE + estep*i);
  }
  
  // std::vector<NEST::LArInteraction> particle_types = {
  //     NEST::LArInteraction::NR, NEST::LArInteraction::ER, NEST::LArInteraction::Alpha
  // };
  // 
  
  
  LArDetector* detector = new LArDetector();
  // Construct NEST class using detector object
  NEST::LArNEST larnest(detector);
  // Set random seed of RandomGen class
  RandomGen::rndm()->SetSeed(seed);
  
  cout << GetELost(10);
  // cout << larnest.GetNRPhotonYields(0.1, 0.001) << endl;
  // cout << larnest.GetNRPhotonYields(1, 1) << endl;
  // cout << larnest.GetNRPhotonYields(10, 1) << endl;
  // cout << larnest.GetNRPhotonYields(100, 1) << endl;
  // cout << larnest.GetNRPhotonYields(1000, 1) << endl;

  
  // Construct NEST objects for storing calculation results
  NEST::LArNESTResult result;
  
  
  for(int i = 0; i < energy_vals.size(); i++){
  
    double E = energy_vals[i];
    result = larnest.FullCalculation(ptype, E, Efield, density, false);
  
    double npe = result.yields.LightYield;
    double nq = result.yields.QuantaYield;
  
  
  }
  
  
  
  
  
  
  
  return 0;
}
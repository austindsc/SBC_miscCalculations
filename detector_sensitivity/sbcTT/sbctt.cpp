#include <iostream>
#include <stdlib.h>

#include "TH1.h"

#include "NEST.hh"
#include "VDetector.hh"

#include "LArDetector.hh"
#include "LArNEST.hh"

// helpful code, functions and constants
#include "helpers.cc"

using namespace std;
// using namespace NEST;

// Right now ignoring fluctuations and photon quantization


int main(){
  
  cout << "hello austin!" << endl;
  
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
  vector<double> energy_vals = MCH::MakeVector_linear(lowE, highE, estep);
  
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
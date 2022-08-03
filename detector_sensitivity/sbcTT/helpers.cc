#include <iostream>

using namespace std;

//// Constants and Numberrs ////
extern const float hc = 1239.8;
extern const float kev = 1000.;

extern const float wlAr = 128.0;
extern const float wlXe = 176.0;

// get energy lost to photons, in eV
double GetELost_Light(float npe, float wl = wlAr){
  float ep = hc/wl;
  return npe*ep/kev;
}

// // get energy lost to photons
// double GetELost_Light(NEST::LArNESTResult yield, float wl = wlAr){
//   float npe = yield.yields.Nph;
//   float ep = hc/wl;
//   return npe*ep;
// }


// My Code Helper
namespace MCH{
  
  void test(){
    cout << "YO WE IN NEW MCH" << endl;
    return;
  }
  
  vector<double> MakeVector_linear(float min, float max, float gap){
    std::vector<double> v;
    double a = min;
    if(min > max){ cout << "Error: In MakeVector_linear, min >= max" << endl; exit(1); }
    while(a < max+gap){ v.emplace_back(a); a = a + gap; }
    return v;
  }
  
}
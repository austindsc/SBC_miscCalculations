#include <iostream>

using namespace std;

//// Constants and Numberrs ////
extern const float hc = 1239.8;

extern const wlAr = 128.0;
extern const wlXe = 176.0;

// get energy lost to photons
double GetELost(float npe, float wl = wlAr){
  float ep = hc/wl;
  return npe*ep;
}

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
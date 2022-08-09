#include <iostream>

#include<cmath> 

//for generator
#include<vector>
#include<algorithm>
#include<iterator>

using namespace std;

//// Constants and Numberrs ////
extern const float hc = 1239.8;
extern const float keV = 1000.;

extern const float wlAr = 128.0;
extern const float wlXe = 176.0;

// get energy lost to photons, in eV
double GetE_Light(float npe, float wl = wlAr){
  float ep = hc/wl;
  return npe*ep/keV;
}

// double GetE_Charge(float nq, float Egap = 10){
//   return nq
// }

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
  
  std::vector<double> MakeVector_log(double a, double b, int k){
    std::vector<double> logspace;

    double gap = (b - a)/k;
    std::vector<double> linspace = MakeVector_linear(a, b, gap);
    for(int i = 0; i < linspace.size()-1; i++){
      logspace.push_back(pow(10, linspace[i]));
    }
    return logspace;
  }
  
  
}

// template<typename T = double>
// class Logspace{
// private:
//     T curValue, base, step;
// 
// public:
//     Logspace(T first, T last, int num, T base = 10.0) : curValue(first), base(base){
//        step = (last - first)/(num-1);
//     }
// 
//     T operator()() {
//         T retval = pow(base, curValue);
//         curValue += step;
//         return retval;
//     }
// };
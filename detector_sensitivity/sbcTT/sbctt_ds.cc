
struct sbc_medium{
  float wl;     // wavelength (nm)
  float wleV;
  float egap;   // bandgap (eV)
  float wval;   // w value (eV)
  float rho;    // density (g/cm3)
  
  float pdimer; // charged dimer energy (see kubota paper and others)
  float ionInLiquid;
  float excimer;// excimer energy
  
  sbc_medium(){}
  sbc_medium(float wl, float eg, float w, float rho) :
    wl{wl}, egap{eg}, wval{w}, rho{rho}{
      this->wleV = hc/wl;
    }
};

struct sbc_seitz{
  float rc;
  float Ethresh;
  float field;
  
  float T;
  float P;
  sbc_seitz(){}
  // sbc_seitz(
};

struct ArEvent{
  float K;    // kinetic energy
  NEST::LArYieldResult nestYield;
  double LET;
  
  // void SetLET(){};
};

struct sbc_run{
  sbc_seitz bubble;
  sbc_medium target;
  vector<ArEvent> events;
};

// struct ArEvent: public Event{
//   NEST::LArYieldResult nestYield;
// };
// 
// struct XeEvent: public Event{
//   NEST::YieldResult nestYield;
// };

struct sbc_medium{
  float wl;     // wavelength
  float egap;   // bandgap
  float Wval;   // w value
  
  float pdimer; // charged dimer energy (see kubota paper and others)
  float excimer;// excimer energy
};

struct sbc_seitz{
  float rc;
  float Ethresh;
  float field;
  
  float T;
  float P;
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
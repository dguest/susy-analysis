#ifndef BTAG_DEFS_HH
#define BTAG_DEFS_HH
namespace btag { 
  enum Uncertainty { None, Total, Statistical, Systematic };
  enum Tagger { CNN_LOOSE, CNN_MEDIUM, CNN_TIGHT }; 
  enum Flavor { B, C, U, T, DATA}; 
}


#endif 

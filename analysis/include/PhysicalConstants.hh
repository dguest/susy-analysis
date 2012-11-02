#ifndef PHYSICAL_CONSTANTS_H
#define PHYSICAL_CONSTANTS_H

const double MeV = 1.0; 
const double GeV = 1e3; 
const double W_MASS = 80.385*GeV; 

// ok, not physical constants, but probably won't change
enum Flavor { 
  CHARM = 4, 
  BOTTOM = 5, 
  LIGHT = 0, 
  TAU = 15
}; 


#endif 

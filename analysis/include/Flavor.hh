#ifndef FLAVORS_HH
#define FLAVORS_HH

enum class Flavor { CHARM = 4, BOTTOM = 5, LIGHT = 0, TAU = 15, NONE = -1}; 

bool operator<(Flavor, Flavor); 
const char* as_char(Flavor); 

#endif 

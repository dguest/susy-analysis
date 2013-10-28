#ifndef BOSON_PT_REWEIGHTER_HH
#define BOSON_TRUTH_FILTER_HH

// right now just a wrapper for Mark's pt correction function, may be more 
// advanced in the future 

class SusyBuffer; 

class BosonPtReweighter
{
public: 
  float get_boson_weight(const SusyBuffer*); 
}; 

#endif 

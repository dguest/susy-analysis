#ifndef PRESELECTIONINFO_HH
#define PRESELECTIONINFO_HH

#include "typedefs.hh"

struct PreselectionInfo
{
  ull_t bits;
  float pileup_weight;
  float trigger_sf;
  unsigned random_run;
};

#endif

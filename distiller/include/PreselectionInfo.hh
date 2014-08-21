#ifndef PRESELECTIONINFO_HH
#define PRESELECTIONINFO_HH

#include "typedefs.hh"
#include "ScaleFactor.hh"

struct PreselectionInfo
{
  ull_t bits;
  ScaleFactor pileup_weight;
  float trigger_sf;
  unsigned random_run;
};

#endif

#ifndef REGION_BOSON_PT_HISTOGRAMS_HH
#define REGION_BOSON_PT_HISTOGRAMS_HH

struct EventObjects; 
struct RegionConfig; 
namespace H5 { 
  class CommonFG; 
}

#include "IRegionHistograms.hh"

class RegionBosonPtHistograms: public IRegionHistograms
{
public: 
  RegionBosonPtHistograms(const RegionConfig&, unsigned); 
  virtual ~RegionBosonPtHistograms(); 
  virtual void fill(const EventObjects& objects); 
  virtual void write_to(H5::CommonFG&) const; 
}; 

#endif 

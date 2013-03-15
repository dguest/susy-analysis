#ifndef REGION_EVENT_FILTER_HH
#define REGION_EVENT_FILTER_HH

struct EventObjects; 

#include "RegionConfig.hh"

class RegionEventFilter 
{
public: 
  RegionEventFilter(const RegionConfig& config, unsigned build_flags = 0); 
  bool pass(const EventObjects& objects) const; 
private: 
  const RegionConfig& m_region_config; 
}; 

#endif 

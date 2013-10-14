#ifndef REGION_EVENT_FILTER_HH
#define REGION_EVENT_FILTER_HH

// Intended to house all the event-wise rescaling and filtering code. 

struct EventObjects; 
class JetTagRescaler; 
class IJetTagFilter; 

#include "RegionConfig.hh"

class RegionEventFilter 
{
public: 
  RegionEventFilter(const RegionConfig& config, unsigned build_flags = 0); 
  ~RegionEventFilter(); 
  RegionEventFilter(const RegionEventFilter&) = delete; 
  RegionEventFilter& operator=(const RegionEventFilter&) = delete; 
  bool pass(const EventObjects& objects) const; 
  double jet_scalefactor(const EventObjects& objects) const; 
  float lepton_scalefactor(const EventObjects& objects) const; 
private: 
  const RegionConfig& m_region_config; 
  const JetTagRescaler* m_jet_rescaler; 
  const IJetTagFilter* m_jet_tag_filter; 
}; 

#endif 

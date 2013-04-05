#ifndef OUTPUT_FILTER_HH
#define OUTPUT_FILTER_HH

#include "typedefs.hh"

class RunInfo; 

class OutputFilter 
{
public: 
  OutputFilter(const RunInfo& info, unsigned run_flags); 
  bool should_save_event(const ull_t event_mask); 
private: 
  bool m_save_all_events; 
}; 

#endif 

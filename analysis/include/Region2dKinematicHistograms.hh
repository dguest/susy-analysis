#ifndef REGION_2D_KINEMATIC_HISTOGRAMS_HH
#define REGION_2D_KINEMATIC_HISTOGRAMS_HH

#include <boost/noncopyable.hpp>
#include <vector> 

#include "typedefs.hh"
#include "RegionConfig.hh"
#include "RegionEventFilter.hh"
#include "IRegionHistograms.hh"

class Histogram; 

namespace H5 { 
  class CommonFG; 
}

namespace { 
  const float MAX_LEADING_JET_PT = 500e3; 
  const float MAX_MET = 500e3; 
  const float MEV_PER_BIN = 5.0e3; 
}; 

class Region2dKinematicHistograms: 
  public boost::noncopyable, 
  public IRegionHistograms
{
public: 
  Region2dKinematicHistograms(const RegionConfig& config, 
			      const unsigned build = 0); 
  ~Region2dKinematicHistograms(); 
  virtual void fill(const EventObjects& objects);
  virtual void write_to(H5::CommonFG&) const; 
private: 
  int get_n_bins(double low, double high) const; 
  const RegionConfig m_region_config; 
  const RegionEventFilter m_event_filter; 
  const unsigned m_build_flags; 

  Histogram* m_leading_pt_vs_met; 
  Histogram* m_leading_pt_vs_met_sum_wt2; 
}; 


#endif 

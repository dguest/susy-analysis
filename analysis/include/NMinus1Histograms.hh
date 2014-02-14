#ifndef N_MINUS_ONE_HISTOGRAMS_HH
#define N_MINUS_ONE_HISTOGRAMS_HH

#include <vector> 
#include <string>
#include <map>
#include "IRegionHistograms.hh"

class Histogram; 

namespace H5 { 
  class CommonFG; 
}

namespace nminus { 
  const float MAX_LEADING_JET_PT = 500e3; 
  const float MAX_MET = 500e3; 
  const float MEV_PER_BIN = 5.0e3; 
  struct Selection { 
    std::string name;
    double max;
    double min;
  }; 
  class NMinusHist { 
  public: 
    NMinusHist(const std::string& name, const std::vector<Selection>&);
    NMinusHist(NMinusHist&) = delete; 
    NMinusHist(NMinusHist&&); 
    ~NMinusHist(); 
    NMinusHist operator=(NMinusHist&) = delete; 
    void fill(const std::map<std::string, double>& values); 
  private: 
    Histogram* m_histogram; 
    Selection m_this_selection; 
    std::vector<Selection> m_cuts; 
  }; 
}; 

class NMinus1Histograms: 
  public IRegionHistograms
{
public: 
  NMinus1Histograms(const RegionConfig& config, 
		    const unsigned build = 0); 
  NMinus1Histograms(NMinus1Histograms&) = delete;
  ~NMinus1Histograms(); 
  NMinus1Histograms& operator=(NMinus1Histograms&) = delete; 
  virtual void fill(const EventObjects& objects);
  virtual void write_to(H5::CommonFG&) const; 
private: 
  const RegionConfig* m_region_config; 
  const RegionEventFilter* m_event_filter; 
  const unsigned m_build_flags; 

  std::vector<NMinusHist> m_hists; 
}; 


#endif 

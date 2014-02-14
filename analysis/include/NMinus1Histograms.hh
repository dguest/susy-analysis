#ifndef N_MINUS_ONE_HISTOGRAMS_HH
#define N_MINUS_ONE_HISTOGRAMS_HH

#include <vector> 
#include <string>
#include <map>
#include "IRegionHistograms.hh"

class Histogram; 
struct Axis; 
class RegionConfig;

namespace H5 { 
  class CommonFG; 
}

namespace nminus { 
  const float MAX_LEADING_JET_PT = 500e3; 
  const float MAX_MET = 500e3; 
  const float MEV_PER_BIN = 5.0e3; 
  struct Selection { 
    Selection(); 
    double max;
    double min;
  }; 
  class NMinusHist { 
  public: 
    // NOTE: could generalize this by making it an initalizer list
    NMinusHist(const Axis&, const std::map<std::string, Selection>&);
    ~NMinusHist(); 
    NMinusHist(NMinusHist&) = delete; 
    NMinusHist(NMinusHist&&); 
    NMinusHist operator=(NMinusHist&) = delete; 
    void fill(const std::map<std::string, double>& values); 
    void write_to(H5::CommonFG&, std::string name = "") const; 
  private: 
    Histogram* m_histogram; 
    // NOTE: generalization would also require something new here
    Selection m_selection; 
    std::string m_name; 
    std::vector<std::pair<std::string,Selection> > m_cuts; 
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
  const unsigned m_build_flags; 

  std::vector<nminus::NMinusHist> m_hists; 
}; 


#endif 

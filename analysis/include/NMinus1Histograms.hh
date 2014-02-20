#ifndef N_MINUS_ONE_HISTOGRAMS_HH
#define N_MINUS_ONE_HISTOGRAMS_HH

#include "constants_physical.hh"
#include "IRegionHistograms.hh"

#include <vector> 
#include <string>
#include <map>


class Histogram; 
struct Axis; 
class RegionConfig;
class ISelection;

namespace H5 { 
  class CommonFG; 
}

namespace nminus { 
  const size_t N_BINS = 100;
  const double MAX_ENERGY = 1_TeV;

  // all units are in MeV
  const std::string EUNIT = "MeV"; 


  // names for internal use (and saving)
  const std::string MET = "met"; 
  const std::string DPHI = "jetmet_dphi"; 
  const std::string MCT = "mass_ct"; 
  const std::string MET_EFF = "met_eff"; 
  const std::string MCC = "mass_cc"; 

  // naming functions
  std::string jeta(int jn); 
  std::string jpt(int jn); 
  std::string jantib(int jn); 
  std::string jantiu(int jn); 

  struct Selection { 
    Selection(); 
    Selection(double, double); 
    double min;
    double max;
  }; 
  class NMinusHist { 
  public: 
    // NOTE: could generalize this by making it an initalizer list
    NMinusHist(const Axis&, const std::map<std::string, Selection>&);
    ~NMinusHist(); 
    NMinusHist(NMinusHist&) = delete; 
    NMinusHist(NMinusHist&&); 
    NMinusHist operator=(NMinusHist&) = delete; 
    void fill(const std::map<std::string, double>& values, double weight); 
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
  const ISelection* m_selection; 
  const unsigned m_build_flags; 

  std::vector<nminus::NMinusHist> m_hists; 
}; 


#endif 

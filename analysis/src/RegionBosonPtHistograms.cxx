#include "RegionBosonPtHistograms.hh"
#include "HistBuilderFlags.hh"
#include "EventObjects.hh"
#include "RegionConfig.hh"
#include "RegionEventFilter.hh"
#include "H5Cpp.h"
#include "Histogram.hh"

namespace { 
  const float MAX_PT = 1e6; 
  const float N_BINS = 100; 
}


RegionBosonPtHistograms::RegionBosonPtHistograms(
  const RegionConfig& config, unsigned flags): 
  m_region_config(new RegionConfig(config)), 
  m_event_filter(new RegionEventFilter(config, flags)), 
  m_build_flags(flags), 
  m_boson_pt(0), 
  m_boson_truth_pt(0), 
  m_reco_vs_truth_pt(0)
{ 
  
  m_boson_pt = new Histogram(N_BINS, 0, MAX_PT, "MeV", hist::wt2); 
  if ( (!flags & buildflag::is_data)) { 
    m_boson_truth_pt = new Histogram(N_BINS, 0, MAX_PT, "MeV", hist::wt2); 
    m_reco_vs_truth_pt = new Histogram(
      { {"truth", 50, 0, MAX_PT, "MeV"}, {"reco", 50, 0, MAX_PT, "MeV"}}); 
  }
}
RegionBosonPtHistograms::~RegionBosonPtHistograms() {
  delete m_region_config; 
  delete m_event_filter; 
  delete m_boson_pt; 
  delete m_boson_truth_pt; 
  delete m_reco_vs_truth_pt; 

}
void RegionBosonPtHistograms::fill(const EventObjects& obj) {
  typedef std::vector<Jet> Jets; 

  if (!m_event_filter->pass(obj)) return; 
  bool do_mu_met = m_region_config->region_bits & reg::mu_met; 

  const MetFlavors& mets = obj.met.get_syst(m_region_config->systematic); 
  const TVector2 met = do_mu_met ? mets.muon: mets.bare; 

  double weight = obj.weight; 
  bool use_electron_jet = m_region_config->region_bits & reg::electron_jet; 
  const Jets jets = use_electron_jet ? obj.jets_with_eljet : obj.jets; 

  if (! (m_build_flags & buildflag::is_data)) { 
    weight *= m_event_filter->lepton_scalefactor(obj); 
  }

  TVector2 reco_boson = met; 

}
void RegionBosonPtHistograms::write_to(H5::CommonFG&) const {}




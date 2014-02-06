#include "RegionBosonPtHistograms.hh"
#include "HistBuilderFlags.hh"
#include "EventObjects.hh"
#include "RegionConfig.hh"
#include "RegionEventFilter.hh"
#include "H5Cpp.h"
#include "Histogram.hh"

#include <cassert>

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

  assert(false); 
  // this needs to be fixed...
  bool one_muon =     true; //obj.event_mask & pass::control_muon; 
  bool one_electron = true; //obj.event_mask & pass::control_electron; 
  bool z_muons =      true; //obj.event_mask & pass::os_zmass_mu_pair; 

  int n_cr_passed = one_muon + one_electron + z_muons; 
  if ( n_cr_passed != 1) return; 
  
  if (!m_event_filter->pass(obj)) return; 
  bool do_mu_met = m_region_config->region_bits & reg::mu_met; 

  const MetFlavors& mets = obj.met.get_syst(m_region_config->systematic); 
  const TVector2 met = do_mu_met ? mets.muon: mets.bare; 

  double weight = obj.weight; 
  if (! (m_build_flags & buildflag::is_data)) { 
    // no jet jet SF are needed, since we're not tagging
    weight *= m_event_filter->lepton_scalefactor(obj); 
    weight *= m_event_filter->boson_scalefactor(obj); 
  }

  TVector2 reco_boson_p2 = obj.boson_child; 
  if (one_electron || one_muon) { 
    reco_boson_p2 += met; 
  }
  const double reco_boson_pt = reco_boson_p2.Mod(); 
  m_boson_pt->fill(reco_boson_pt, weight); 
  if (m_boson_truth_pt) { 
    m_boson_truth_pt->fill(obj.true_boson_pt, weight); 
    m_reco_vs_truth_pt->fill({obj.true_boson_pt, reco_boson_pt}, weight); 
  }

}
void RegionBosonPtHistograms::write_to(H5::CommonFG& file) const {
  using namespace H5; 
  Group region(file.createGroup(m_region_config->name)); 
  m_boson_pt->write_to(region, "recoBosonPt"); 
  if (m_boson_truth_pt) { 
    m_boson_truth_pt->write_to(region, "truthBosonPt"); 
    m_reco_vs_truth_pt->write_to(region, "bosonResponse"); 
  }
}




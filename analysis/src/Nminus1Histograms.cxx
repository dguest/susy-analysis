#include "NMinus1Histograms.hh"
#include "HistBuilderFlags.hh"
#include "Histogram.hh"
#include "typedefs.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "H5Cpp.h"
#include <stdexcept>
#include <cmath>
#include <boost/format.hpp>

#include "TVector2.h"

NMinus1Histograms
::NMinus1Histograms(const RegionConfig& config, 
			      const unsigned flags) : 
  m_region_config(config), 
  m_event_filter(config), 
  m_build_flags(flags), 

  m_leading_pt_vs_met(0), 
  m_leading_pt_vs_met_sum_wt2(0)
{ 
  using namespace kin2d; 
  if (config.output_name.size() == 0) { 
    throw std::runtime_error("output histograms not named, quitting"); 
  }
  if (config.name.size() == 0) { 
    throw std::runtime_error("no name for region, quitting"); 
  }

  Axis leading_jet_pt; 
  leading_jet_pt.name = "leadingJetPt"; 
  leading_jet_pt.n_bins = get_n_bins(config.leading_jet_pt, 
				     MAX_LEADING_JET_PT); 
  leading_jet_pt.low = config.leading_jet_pt;  
  leading_jet_pt.high = MAX_LEADING_JET_PT; 
  leading_jet_pt.units = "MeV"; 
  
  Axis met; 
  met.name = "met"; 
  met.n_bins = get_n_bins(config.met, MAX_MET); 
  met.low = config.met; 
  met.high = MAX_MET; 
  met.units = "MeV"; 
  std::vector<Axis> axes = {leading_jet_pt, met}; 
  m_leading_pt_vs_met = new Histogram(axes); 
  if (!(flags & buildflag::is_data)) { 
    m_leading_pt_vs_met_sum_wt2 = new Histogram(axes); 
  }

}

NMinus1Histograms::~NMinus1Histograms() { 
  delete m_leading_pt_vs_met; 
  delete m_leading_pt_vs_met_sum_wt2; 
}

void NMinus1Histograms::fill(const EventObjects& obj) { 
  typedef std::vector<Jet> Jets; 

  double weight = obj.weight; 
  const Jets jets = obj.jets; 

  if (!m_event_filter.pass(obj)) return; 
  const std::vector<Jet> tagged_jets = m_event_filter.tagged_jets(jets); 
  bool needs_tags = m_region_config.jet_tag_requirements.size() > 0; 
  if (tagged_jets.size() == 0 && needs_tags) return; 

  if (! (m_build_flags & buildflag::is_data)) { 
    weight *= m_event_filter.jet_scalefactor(tagged_jets); 
    weight *= m_event_filter.lepton_scalefactor(obj); 
    weight *= m_event_filter.boson_scalefactor(obj); 
  }
  const TVector2& met = obj.met; 

  assert(jets.size() > 0); 
  const std::vector<double> ptmet = {jets.at(0).Pt(), met.Mod()}; 
  m_leading_pt_vs_met->fill(ptmet,  weight); 
  if (m_leading_pt_vs_met_sum_wt2) { 
    m_leading_pt_vs_met_sum_wt2->fill(ptmet, weight*weight); 
  }
}

void NMinus1Histograms::write_to(H5::CommonFG& file) const { 
  using namespace H5; 
  Group region(file.createGroup(m_region_config.name)); 

  m_leading_pt_vs_met->write_to(region, "kinematics");
  if (m_leading_pt_vs_met_sum_wt2) { 
    m_leading_pt_vs_met_sum_wt2->write_to(region, "kinematicWt2"); 
  }
}


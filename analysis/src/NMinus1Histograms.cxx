#include "NMinus1Histograms.hh"
#include "HistBuilderFlags.hh"
#include "ISelection.hh"
#include "SignalSelection.hh"
#include "typedefs.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "hdf_helpers.hh"
#include "constants_scharmcuts.hh"

#include "Histogram.hh"

#include "H5Cpp.h"

#include <stdexcept>
#include <cassert>
#include <limits>

namespace nminus { 

  Selection::Selection() : 
    min(std::numeric_limits<double>::quiet_NaN()),
    max(std::numeric_limits<double>::quiet_NaN()) 
  { 
  }
  Selection::Selection(double mn, double mx): min(mn), max(mx)
  {
  }
  
  NMinusHist::NMinusHist(const Axis& axis, 
			 const std::map<std::string,Selection>& selection) : 
    m_histogram(new Histogram({axis})), 
    m_selection(selection.at(axis.name)), 
    m_name(axis.name)
  { 
    for (const auto& sel: selection) { 
      assert(sel.second.min < sel.second.max); 
      if (sel.first != axis.name) { 
	m_cuts.push_back(sel); 
      }
    }
  }
  NMinusHist::~NMinusHist() { 
    delete m_histogram; 
  }
  NMinusHist::NMinusHist(NMinusHist&& old): 
    m_histogram(old.m_histogram), 
    m_selection(std::move(old.m_selection)),
    m_name(std::move(old.m_name)),
    m_cuts(std::move(old.m_cuts))
  {
  }
  void NMinusHist::fill(const std::map<std::string, double>& values) { 
    for (const auto& cut: m_cuts) { 
      const auto& var = cut.first; 
      const auto& sel = cut.second; 
      const auto& val = values.at(var); 
      if (sel.min > val || val > sel.max) return; 
    }
    m_histogram->fill(values); 
  }
  void NMinusHist::write_to(H5::CommonFG& file, std::string name) const { 
    if (name.size() == 0) name = m_name; 
    m_histogram->write_to(file, name); 
    H5::DataSet the_hist(file.openDataSet(name)); 
    h5::write_attr(the_hist, "selection_min", m_selection.min); 
    h5::write_attr(the_hist, "selection_max", m_selection.max); 
  }
}

namespace nminus {
  std::map<std::string, Selection> get_selections(const RegionConfig&); 
  ISelection* selection_factory(const RegionConfig&); 
}

NMinus1Histograms
::NMinus1Histograms(const RegionConfig& config, 
		    const unsigned flags) : 
  m_region_config(new RegionConfig(config)), 
  m_selection(nminus::selection_factory(config)), 
  m_build_flags(flags)
{ 
  
}

NMinus1Histograms::~NMinus1Histograms() { 
  delete m_selection; 
  delete m_region_config; 
}

void NMinus1Histograms::fill(const EventObjects& obj) { 

  const EventRecoParameters& reco = obj.reco; 
  if (!reco.pass_event_quality) return; 

  // double weight = obj.weight; 
  // const Jets jets = obj.jets; 

  // if (!m_event_filter.pass(obj)) return; 
  // const std::vector<Jet> tagged_jets = m_event_filter.tagged_jets(jets); 
  // bool needs_tags = m_region_config.jet_tag_requirements.size() > 0; 
  // if (tagged_jets.size() == 0 && needs_tags) return; 

  // if (! (m_build_flags & buildflag::is_data)) { 
  //   weight *= m_event_filter.jet_scalefactor(tagged_jets); 
  //   weight *= m_event_filter.lepton_scalefactor(obj); 
  //   weight *= m_event_filter.boson_scalefactor(obj); 
  // }
  // const TVector2& met = obj.met; 

  // assert(jets.size() > 0); 
  // const std::vector<double> ptmet = {jets.at(0).Pt(), met.Mod()}; 
  // m_leading_pt_vs_met->fill(ptmet,  weight); 
  // if (m_leading_pt_vs_met_sum_wt2) { 
  //   m_leading_pt_vs_met_sum_wt2->fill(ptmet, weight*weight); 
  // }
}

void NMinus1Histograms::write_to(H5::CommonFG& file) const { 
  using namespace H5; 
  Group region(file.createGroup(m_region_config->name)); 

  // m_leading_pt_vs_met->write_to(region, "kinematics");
  // if (m_leading_pt_vs_met_sum_wt2) { 
  //   m_leading_pt_vs_met_sum_wt2->write_to(region, "kinematicWt2"); 
  // }
}

namespace nminus { 
  std::map<std::string, Selection> get_selections(const RegionConfig& cfg) 
  { 
    std::map<std::string, Selection> sel = {
      {J1_PT, {cfg.leading_jet_pt, INFINITY} } , 
      {MET, {cfg.met, INFINITY} }
    }; 
    assert(cfg.selection == reg::Selection::SIGNAL); 
    return { {"nothing", Selection(100,200) } } ; 
  }
  ISelection* selection_factory(const RegionConfig& cfg) { 
    assert(cfg.selection == reg::Selection::SIGNAL); 
    return new NMinusSignalSelection(cfg); 
  }
}

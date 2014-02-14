#include "NMinus1Histograms.hh"
#include "HistBuilderFlags.hh"
#include "ISelection.hh"
#include "SignalSelection.hh"
#include "typedefs.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "hdf_helpers.hh"
#include "constants_scharmcuts.hh"
#include "constants_tagging.hh"

#include "Histogram.hh"

#include "H5Cpp.h"
#include "TVector2.h"

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
    if (!values.count(m_name)) return;
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
  // ACHTUNG: work do here
}

NMinus1Histograms::~NMinus1Histograms() { 
  delete m_selection; 
  delete m_region_config; 
}

void NMinus1Histograms::fill(const EventObjects& obj) { 

  const EventRecoParameters& reco = obj.reco; 
  if (!reco.pass_event_quality) return; 

  if (!m_selection->pass(obj)) return;
  
  // if (! (m_build_flags & buildflag::is_data)) { 
  //   weight *= m_event_filter.jet_scalefactor(tagged_jets); 
  //   weight *= m_event_filter.lepton_scalefactor(obj); 
  //   weight *= m_event_filter.boson_scalefactor(obj); 
  // }
  const TVector2& met = obj.met; 

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
    // basic kinematics
    std::map<std::string, Selection> sel = {
      {J1_PT, {cfg.leading_jet_pt, INFINITY} } , 
      {J3_PT, {-INFINITY, SIGNAL_JET_3_MAX_PT} }, 
      {MET, {cfg.met, INFINITY} }, 
      {DPHI, {MIN_DPHI_JET_MET, INFINITY} }, 
    }; 
    // add tagging cuts
    for (auto jn: {0,1} ) { 
      const auto& antib = btag::JFC_MEDIUM_ANTI_B_CUT; 
      const auto& antiu = btag::JFC_MEDIUM_ANTI_U_CUT; 
      sel.insert({jabseta(jn), {0, btag::TAG_ETA}});
      sel.insert({jantib(jn), {antib, INFINITY} }); 
      sel.insert({jantiu(jn), {antiu, INFINITY} }); 
    }

    switch (cfg.selection) { 
    case reg::Selection::SIGNAL: sel.insert( 
      { 
	{MCT, {SR_MCT_MIN, INFINITY} }, 
	{MET_EFF, {MET_EFF_MIN, INFINITY} }, 
	{MCC, {M_CC_MIN, INFINITY} } 
      }); 
      return sel; 
    default: throw std::invalid_argument("unknown selection in " __FILE__); 
    }
  }
  ISelection* selection_factory(const RegionConfig& cfg) { 
    assert(cfg.selection == reg::Selection::SIGNAL); 
    return new NMinusSignalSelection(cfg); 
  }

  std::string jabseta(int jn) { 
    return "j" + std::to_string(jn) + "_abseta"; 
  }
  std::string jpt(int jn) { 
    return "j" + std::to_string(jn) + "_pt"; 
  }
  std::string jantib(int jn) {
    return "j" + std::to_string(jn) + "_antib"; 
  }
  std::string jantiu(int jn) {
    return "j" + std::to_string(jn) + "_antiu"; 
  }



}

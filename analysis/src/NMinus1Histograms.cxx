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
    m_name(axis.name)
  { 
    if (!selection.count(axis.name)) { 
      m_selection = {-INFINITY, INFINITY};
    } else {
      m_selection = selection.at(axis.name);
    }

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
    m_histogram(0), 
    m_selection(std::move(old.m_selection)),
    m_name(std::move(old.m_name)),
    m_cuts(std::move(old.m_cuts))
  {
    std::swap(m_histogram, old.m_histogram);
  }
  void NMinusHist::fill(const std::map<std::string, double>& values, 
			double weight) { 
    if (!values.count(m_name)) return;
    for (const auto& cut: m_cuts) { 

      const auto& cut_variable = cut.first; 

      // skip cut if the value is missing
      if (!values.count(cut_variable)) continue;

      const auto& selection = cut.second; 
      const auto& value = values.at(cut_variable); 
      if (selection.min > value || value > selection.max) return; 
    }
    m_histogram->fill(values, weight); 
  }
  void NMinusHist::write_to(H5::CommonFG& file, std::string name) const { 
    if (name.size() == 0) name = m_name; 
    m_histogram->write_to(file, name); 
    H5::DataSet the_hist(file.openDataSet(name)); 
    h5::write_attr_vec(the_hist, "selection_min", 
		       std::vector<double>{m_selection.min}); 
    h5::write_attr_vec(the_hist, "selection_max", 
		       std::vector<double>{m_selection.max}); 
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
  using namespace nminus;
  const auto sel = get_selections(config); 
  m_hists.emplace_back(Axis{MET, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel); 
  m_hists.emplace_back(Axis{DPHI, 80, 0.0, 3.2}, sel); 
  m_hists.emplace_back(Axis{MCT, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel);
  m_hists.emplace_back(Axis{MET_EFF, N_BINS, 0, 10}, sel); 
  m_hists.emplace_back(Axis{MCC, N_BINS, 0.0, 100_GeV, EUNIT}, sel); 
  for (int jn: {0,1,2}) { 
    m_hists.emplace_back(Axis{jeta(jn), N_BINS, -2.8, 2.8}, sel);
    m_hists.emplace_back(Axis{jpt(jn), N_BINS, 0, MAX_ENERGY, EUNIT}, sel);
    m_hists.emplace_back(Axis{jantib(jn), N_BINS, -10, 10}, sel); 
    m_hists.emplace_back(Axis{jantiu(jn), N_BINS, -10, 10}, sel); 
  }
}

NMinus1Histograms::~NMinus1Histograms() { 
  delete m_selection; 
  delete m_region_config; 
}

void NMinus1Histograms::fill(const EventObjects& obj) { 

  const EventRecoParameters& reco = obj.reco; 
  if (!reco.pass_event_quality) return; 

  if (!m_selection->pass(obj)) return;

  double weight = obj.weight;

  // if (! (m_build_flags & buildflag::is_data)) { 
  //   weight *= m_event_filter.jet_scalefactor(tagged_jets); 
  //   weight *= m_event_filter.lepton_scalefactor(obj); 
  //   weight *= m_event_filter.boson_scalefactor(obj); 
  // }
  const TVector2& met = obj.met; 

  using namespace nminus;
  std::map<std::string, double> values{
    {MET, met.Mod()}, 
    {DPHI, reco.min_jetmet_dphi}, 
    {MCT, reco.mct}, 
    {MET_EFF, reco.met_eff}, 
    {MCC, reco.mcc} 
  };
  int jn = 0;
  for (const auto& jet: obj.jets) { 
    double pb = jet.flavor_weight(Flavor::BOTTOM);
    double pc = jet.flavor_weight(Flavor::CHARM);
    double pu = jet.flavor_weight(Flavor::LIGHT);
    values.insert( { 
    	{jeta(jn), jet.Eta()}, 
    	{jpt(jn), jet.Pt()}, 
    	{jantib(jn), log(pc/pb)}, 
    	{jantiu(jn), log(pc/pu)} } );
  }
  for (auto& hist: m_hists) { 
    hist.fill(values, weight);
  }
}

void NMinus1Histograms::write_to(H5::CommonFG& file) const { 
  using namespace H5; 
  Group region(file.createGroup(m_region_config->name)); 
  
  for (const auto& hist: m_hists) { 
    hist.write_to(region);
  } 

}

namespace nminus { 
  std::map<std::string, Selection> get_selections(const RegionConfig& cfg) 
  { 
    // basic kinematics
    std::map<std::string, Selection> sel = {
      {jpt(0), {cfg.leading_jet_pt, INFINITY} } , 
      {jpt(2), {-INFINITY, SIGNAL_JET_3_MAX_PT} },
      {MET, {cfg.met, INFINITY} }, 
      {DPHI, {MIN_DPHI_JET_MET, INFINITY} }, 
    }; 
    // add tagging cuts
    for (auto jn: {0,1} ) { 
      const auto& antib = btag::JFC_MEDIUM_ANTI_B_CUT; 
      const auto& antiu = btag::JFC_MEDIUM_ANTI_U_CUT; 
      sel.insert({jpt(jn), {30_GeV, INFINITY}});
      sel.insert({jeta(jn), {-btag::TAG_ETA, btag::TAG_ETA}});
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

  std::string jeta(int jn) { 
    return "j" + std::to_string(jn) + "_eta"; 
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

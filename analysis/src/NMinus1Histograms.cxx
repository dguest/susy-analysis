#include "NMinus1Histograms.hh"
#include "HistBuilderFlags.hh"
#include "ISelection.hh"
#include "SignalSelection.hh"
#include "CR1LSelection.hh"
#include "OSDFSelection.hh"
#include "OSSFSelection.hh"
#include "CRZSelection.hh"
#include "QualityEventSelection.hh"
#include "typedefs.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "EventScalefactors.hh"
#include "hdf_helpers.hh"
#include "constants_scharmcuts.hh"
#include "constants_tagging.hh"

#include "Histogram.hh"

#include "H5Cpp.h"
#include "TVector2.h"

#include <stdexcept>
#include <cassert>
#include <limits>
#include <set>

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
  m_build_flags(flags), 
  m_make_lepton_plots(false), 
  m_make_dilep_plots(false)
{ 
  std::set<reg::Selection> onelep_regions {reg::Selection::CR_1L};
  std::set<reg::Selection> dilep_regions { 
    reg::Selection::CR_SF, reg::Selection::CR_DF, reg::Selection::CR_Z, 
      reg::Selection::QUALITY_EVENT};
  auto lepton_regions(onelep_regions);
  lepton_regions.insert(dilep_regions.begin(), dilep_regions.end());

  if (lepton_regions.count(config.selection)) m_make_lepton_plots = true;
  if (dilep_regions.count(config.selection)) m_make_dilep_plots = true;

  using namespace nminus;
  const auto sel = get_selections(config); 
  m_hists.emplace_back(Axis{MET, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel); 
  m_hists.emplace_back(Axis{DPHI, 80, 0.0, 3.2}, sel); 
  m_hists.emplace_back(Axis{MCT, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel);
  m_hists.emplace_back(Axis{MET_EFF, N_BINS, 0, 1.0}, sel); 
  m_hists.emplace_back(Axis{MCC, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel); 
  for (int jn: {0,1,2}) { 
    m_hists.emplace_back(Axis{jeta(jn), 112, -2.8, 2.8}, sel);
    m_hists.emplace_back(Axis{jpt(jn), N_BINS, 0, MAX_ENERGY, EUNIT}, sel);
    m_hists.emplace_back(Axis{jantib(jn), 300, -7.5, 7.5}, sel); 
    m_hists.emplace_back(Axis{jantiu(jn), 300, -7.5, 7.5}, sel); 
  }
  
  if (m_make_lepton_plots) { 
    m_hists.emplace_back(Axis{LLPT, N_BINS, 0.0, 500_GeV, EUNIT}, sel);
    m_hists.emplace_back(Axis{MT, N_BINS, 0.0, 500_GeV, EUNIT}, sel);
  }
  if (m_make_dilep_plots) { 
    m_hists.emplace_back(Axis{MLL, 200, 0.0, 200_GeV, EUNIT}, sel);
  }
}

NMinus1Histograms::~NMinus1Histograms() { 
  delete m_selection; 
  delete m_region_config; 
}


namespace nminus { 
  void insert_jets(const std::vector<Jet>&, 
		   std::map<std::string, double>& values);
}

void NMinus1Histograms::fill(const EventObjects& obj) { 

  const EventRecoParameters& reco = obj.reco; 
  if (!reco.pass_event_quality) return; 
  if (!m_selection->pass(obj)) return;

  double weight = obj.weight;
  if (! (m_build_flags & buildflag::is_data)) { 
    // --- apply scalefactors ---
    auto syst = m_region_config->systematic;
    assert(obj.jets.size() >= 2);
    for (auto jn = 0; jn < 2; jn++) { 
      weight *= obj.jets.at(jn).get_scalefactor(syst);
    }
    weight *= obj.event_scalefactors->get_sf(EventSyst::ELECTRON, syst);
    weight *= obj.event_scalefactors->get_sf(EventSyst::MUON, syst);
    if (m_region_config->boson_pt_correction == reg::MARKS) {
      weight *= obj.marks_boson_pt_weight;
    }
    // --- end of scalefactors ---
  }

  const TVector2& met = obj.met; 

  using namespace nminus;
  std::map<std::string, double> values{
    {MET, met.Mod()}, 
    {DPHI, reco.min_jetmet_dphi}, 
    {MCT, reco.mct}, 
    {MET_EFF, reco.met_eff}, 
    {MCC, reco.mcc} 
  };

  insert_jets(obj.jets, values);

  if (m_make_lepton_plots) { 
    values.insert( { {LLPT, reco.max_lepton_pt}, {MT, reco.mt} } );
  }
  if (m_make_dilep_plots) values.insert({ { MLL, reco.mll} });
  for (auto& hist: m_hists) { 
    hist.fill(values, weight);
  }
}


namespace nminus { 
  void insert_jets(const std::vector<Jet>& jets, 
		   std::map<std::string, double>& values) {
    int jn = 0;
    for (const auto& jet: jets) { 
      double pb = jet.flavor_weight(Flavor::BOTTOM);
      double pc = jet.flavor_weight(Flavor::CHARM);
      double pu = jet.flavor_weight(Flavor::LIGHT);
      values.insert( { 
	  {jeta(jn), jet.Eta()}, 
	  {jpt(jn), jet.Pt()}, 
	  {jantib(jn), log(pc/pb)}, 
	  {jantiu(jn), log(pc/pu)} } );

      jn++; 
    }
  } // end insert_jets
}

void NMinus1Histograms::write_to(H5::CommonFG& file) const { 
  using namespace H5; 
  Group region(file.createGroup(m_region_config->name)); 
  
  for (const auto& hist: m_hists) { 
    hist.write_to(region);
  } 

}

namespace nminus { 
  std::map<std::string, Selection> get_common_selection(
    const RegionConfig& cfg) { 
    
    // basic kinematics
    std::map<std::string, Selection> sel = {
      {jpt(0), {cfg.leading_jet_pt, INFINITY} },
      {jpt(1), {cfg.second_jet_pt, INFINITY} },
      {MET, {cfg.met, INFINITY} },
    };
    return sel;
  }
  void add_tagging_cuts(std::map<std::string, Selection>& sel) {
    // add tagging cuts (use of `insert` prevents overwrite)
    for (auto jn: {0,1} ) { 
      const auto& antib = btag::JFC_MEDIUM_ANTI_B_CUT; 
      const auto& antiu = btag::JFC_MEDIUM_ANTI_U_CUT; 
      sel.insert({jeta(jn), {-btag::TAG_ETA, btag::TAG_ETA}});
      sel.insert({jantib(jn), {antib, INFINITY} }); 
      sel.insert({jantiu(jn), {antiu, INFINITY} }); 
    }
  }

  std::map<std::string, Selection> get_selections(const RegionConfig& cfg) 
  { 
    auto sel = get_common_selection(cfg);

    switch (cfg.selection) { 
    case reg::Selection::SIGNAL: {
      sel.insert( { 
	  {MCT, {SR_MCT_MIN, INFINITY} },
	  {MET_EFF, {MET_EFF_MIN, INFINITY} },
	  {MCC, {M_CC_MIN, INFINITY} },
	  {DPHI, {MIN_DPHI_JET_MET, INFINITY} },
	}); 
      add_tagging_cuts(sel);
      return sel; 
    }
    case reg::Selection::CR_1L: {
      using namespace cr1l; 
      sel.insert(
	{
	  {MT, {M_T_MIN, M_T_MAX} },
	  {MCT, {SR_MCT_MIN, INFINITY} },
	});
      add_tagging_cuts(sel);
      return sel;
    } 
    case reg::Selection::CR_Z: 	// intentional fallthrough
    case reg::Selection::CR_SF: { 
      using namespace crsf;
      sel.insert(
	{
	  {LLPT, {LEPTON_PT_MIN, INFINITY} },
	  {MLL, {M_LL_MIN, M_LL_MAX} },
	});
      add_tagging_cuts(sel);
      return sel;
    } 
    case reg::Selection::CR_DF: {
      using namespace crdf;
      sel.insert(
	{
	  {MLL, {M_LL_MIN, INFINITY} },
	});
      add_tagging_cuts(sel);
      return sel; 
    } 
    case reg::Selection::QUALITY_EVENT: 
      sel.insert({ 
	  {DPHI, {MIN_DPHI_JET_MET, INFINITY} },
	    });
      return sel;
    default: throw std::invalid_argument("unknown selection in " __FILE__); 
    }
  }
  ISelection* selection_factory(const RegionConfig& cfg) { 
    switch (cfg.selection) { 
    case reg::Selection::SIGNAL: return new NMinusSignalSelection(cfg); 
    case reg::Selection::CR_1L: return new NMinusCR1LSelection(cfg);
    case reg::Selection::CR_SF: return new NMinusOSSFSelection(cfg);
    case reg::Selection::CR_Z: return new NMinusCRZSelection(cfg);
    case reg::Selection::CR_DF: return new NMinusOSDFSelection(cfg);
    case reg::Selection::QUALITY_EVENT: return new QualityEventSelection(cfg);
    default: throw std::invalid_argument("unknown selection in " __FILE__);
    }
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

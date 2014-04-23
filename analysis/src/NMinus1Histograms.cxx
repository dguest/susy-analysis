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
#include "constants_scharmcuts.hh"
#include "constants_tagging.hh"

#include "Histogram.hh"

#include "H5Cpp.h"
#include "TVector2.h"

#include <stdexcept>
#include <cassert>
#include <set>


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
  std::set<reg::Selection> onelep_regions {
    reg::Selection::CR_1L, reg::Selection::CR_W};
  std::set<reg::Selection> dilep_regions {
    reg::Selection::CR_SF, reg::Selection::CR_DF,
      reg::Selection::CR_Z, reg::Selection::CR_T,
      reg::Selection::QUALITY_EVENT};
  auto lepton_regions(onelep_regions);
  lepton_regions.insert(dilep_regions.begin(), dilep_regions.end());

  if (lepton_regions.count(config.selection)) m_make_lepton_plots = true;
  if (dilep_regions.count(config.selection)) m_make_dilep_plots = true;

  using namespace nminus;
  const auto sel = get_selections(config);
  // save wt^2 for the met hist so we can use it for limit setting
  m_hists.emplace_back(
    Axis{MET, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel, hist::wt2);
  // other hists don't need wt2 info... or... do... they....
  const auto hf = hist::wt2;
  m_hists.emplace_back(
    Axis{NSJET, SJET_RANGE, -0.5, SJET_RANGE - 0.5}, sel, hf);
  m_hists.emplace_back(Axis{DPHI, 80, 0.0, 3.2}, sel, hf);
  m_hists.emplace_back(Axis{MCT, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel, hf);
  m_hists.emplace_back(Axis{MET_EFF, N_BINS, 0, 1.0}, sel, hf);
  m_hists.emplace_back(Axis{MCC, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel, hf);
  for (int jn: {0,1,2}) {
    m_hists.emplace_back(Axis{jeta(jn), 112, -2.8, 2.8}, sel, hf);
    m_hists.emplace_back(Axis{jpt(jn), N_BINS, 0, MAX_ENERGY, EUNIT},
			 sel, hf);
    m_hists.emplace_back(Axis{jantib(jn), 300, -7.5, 7.5}, sel, hf);
    m_hists.emplace_back(Axis{jantiu(jn), 300, -7.5, 7.5}, sel, hf);
  }

  if (m_make_lepton_plots) {
    m_hists.emplace_back(Axis{LLPT, N_BINS, 0.0, 500_GeV, EUNIT}, sel, hf);
    m_hists.emplace_back(Axis{MT, N_BINS, 0.0, 500_GeV, EUNIT}, sel, hf);
  }
  if (m_make_dilep_plots) {
    m_hists.emplace_back(Axis{MLL, 200, 0.0, 200_GeV, EUNIT}, sel, hf);
  }
}

NMinus1Histograms::~NMinus1Histograms() {
  delete m_selection;
  delete m_region_config;
}


namespace nminus {
  void insert_jets(const std::vector<Jet>&,
		   std::map<std::string, double>& values);
  void throw_if_nan(const std::map<std::string, double>& values);
}

void NMinus1Histograms::fill(const EventObjects& obj) {

  const EventRecoParameters& reco = obj.reco;
  if (!reco.pass_event_quality) return;
  if (!m_selection->pass(obj)) return;

  double weight = obj.weight;
  if (! (m_build_flags & buildflag::is_data)) {
    // --- apply scalefactors ---
    auto syst = m_region_config->systematic;
    size_t n_jets = std::min(2UL, obj.jets.size());
    for (size_t jn = 0; jn < n_jets; jn++) {
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
    {NSJET, reco.n_signal_jets},
    {MET, met.Mod()},
    {DPHI, reco.min_jetmet_dphi},
    {MCT, reco.mct},
    {MET_EFF, reco.met_eff},
    {MCC, reco.mcc},
  };

  insert_jets(obj.jets, values);

  if (m_make_lepton_plots) {
    values.insert( { {LLPT, reco.max_lepton_pt}, {MT, reco.mt} } );
  }
  if (m_make_dilep_plots) values.insert({ { MLL, reco.mll} });

  // check for nan values
  throw_if_nan(values);
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
  void throw_if_nan(const std::map<std::string, double>& values) {
    for (const auto itr: values) {
      if (std::isnan(itr.second)) {
	throw std::invalid_argument(itr.first + " is nan");
      }
    }
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

  // _______________________________________________________________________
  // cut adding fucntions (shouldn't call each other)

  std::map<std::string, Selection> get_common_selection(
    const RegionConfig& cfg) {

    // basic kinematics (jets don't have to exist to pass this cut, but
    // the events will fail the n_jet selection)
    std::map<std::string, Selection> sel = {
      {jpt(0), {cfg.leading_jet_pt, INFINITY, Selection::Missing::ACCEPT} },
      {jpt(1), {cfg.second_jet_pt, INFINITY, Selection::Missing::ACCEPT} },
      {MET, {cfg.met, INFINITY} },
    };
    // SJET_RANGE goes from -0.5, so we only show a limit if the max jets
    // are < the range (i.e. 0 to 7 is a range of 8, don't show if max is 8)
    if (cfg.max_signal_jets < SJET_RANGE) {
      sel[NSJET] = {MIN_SIGNAL_JETS - 0.5, cfg.max_signal_jets + 0.5};
    } else if (cfg.max_signal_jets < SJET_INF_THRESHOLD){
      throw std::logic_error(
	"specified a cut above " + std::to_string(SJET_RANGE) + ", but below"
	" the 'infinite' value of " + std::to_string(SJET_INF_THRESHOLD));
    } else {
      sel[NSJET] = {MIN_SIGNAL_JETS - 0.5, INFINITY};
    }
    return sel;
  }
  void add_tagging_cuts(std::map<std::string, Selection>& sel) {
    // accept events where these jets are missing
    // (they will be rejected by the njet cut anyway)
    auto miss = Selection::Missing::ACCEPT;
    // add tagging cuts (use of `insert` prevents overwrite)
    for (auto jn: {0,1} ) {
      const auto& antib = btag::JFC_MEDIUM_ANTI_B_CUT;
      const auto& antiu = btag::JFC_MEDIUM_ANTI_U_CUT;
      sel.insert({jeta(jn), {-btag::TAG_ETA, btag::TAG_ETA, miss}});
      sel.insert({jantib(jn), {antib, INFINITY, miss} });
      sel.insert({jantiu(jn), {antiu, INFINITY, miss} });
    }
  }
  void add_1l_cuts(std::map<std::string, Selection>& sel) {
    using namespace cr1l;
    sel.insert(
      {
	{MT, {M_T_MIN, M_T_MAX} },
	{MCT, {SR_MCT_MIN, INFINITY} },
	  });
  }
  void add_sf_cuts(std::map<std::string, Selection>& sel) {
    using namespace crsf;
    sel.insert(
      {
	{LLPT, {LEPTON_PT_MIN, INFINITY} },
	{MLL, {M_LL_MIN, M_LL_MAX} },
	  });
  }
  void add_df_cuts(std::map<std::string, Selection>& sel) {
    using namespace crdf;
    sel.insert(
      {
	{MLL, {M_LL_MIN, INFINITY} },
	  });
  }
  void add_sr_cuts(std::map<std::string, Selection>& sel) {
    sel.insert( {
	{MCT, {SR_MCT_MIN, INFINITY} },
	{MET_EFF, {MET_EFF_MIN, INFINITY} },
	{MCC, {M_CC_MIN, INFINITY} },
	{DPHI, {MIN_DPHI_JET_MET, INFINITY} },
	  });
  }
  void add_presel_cuts(std::map<std::string, Selection>& sel) {
    // this case has phantom cuts to indicate where SR cuts _would_ be
    const auto PHANTOM = Selection::Missing::PHANTOM;
    sel.insert({
	{DPHI, {MIN_DPHI_JET_MET, INFINITY} },
	{MCT, {SR_MCT_MIN, INFINITY, PHANTOM} },
	{MET_EFF, {MET_EFF_MIN, INFINITY, PHANTOM} },
	{MCC, {M_CC_MIN, INFINITY, PHANTOM} },
	{DPHI, {MIN_DPHI_JET_MET, INFINITY, PHANTOM} },
	  });
  }

  // ______________________________________________________________________
  // top level selection builder (calls the above functions)

  std::map<std::string, Selection> get_selections(const RegionConfig& cfg)
  {
    auto sel = get_common_selection(cfg);
    switch (cfg.selection) {
    case reg::Selection::SIGNAL: {
      add_sr_cuts(sel);
      add_tagging_cuts(sel);
      return sel;
    }
      // tagged control selections
    case reg::Selection::CR_W: {
      add_tagging_cuts(sel);
      add_1l_cuts(sel);
      return sel;
    }
    case reg::Selection::CR_Z: {
      add_sf_cuts(sel);
      add_tagging_cuts(sel);
      return sel;
    }
    case reg::Selection::CR_T: {
      add_df_cuts(sel);
      add_tagging_cuts(sel);
      return sel;
    }

      // untagged control selections
    case reg::Selection::CR_1L: {
      add_1l_cuts(sel);
      return sel;
    }
    case reg::Selection::CR_SF: {
      add_sf_cuts(sel);
      return sel;
    }
    case reg::Selection::CR_DF: {
      add_df_cuts(sel);
      return sel;
    }

    case reg::Selection::QUALITY_EVENT: {
      add_presel_cuts(sel);
      return sel;
    }

    default: throw std::invalid_argument("unknown selection in " __FILE__);
    }
  }
  ISelection* selection_factory(const RegionConfig& cfg) {
    switch (cfg.selection) {
    case reg::Selection::SIGNAL: return new NMinusSignalSelection(cfg);

    case reg::Selection::CR_W:	// fallthrough
    case reg::Selection::CR_1L: return new NMinusCR1LSelection(cfg);

    case reg::Selection::CR_Z:	// fallthrough
    case reg::Selection::CR_SF: return new NMinusCRZSelection(cfg);

    case reg::Selection::CR_T:	// fallthrough
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

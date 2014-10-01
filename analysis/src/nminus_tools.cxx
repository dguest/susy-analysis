#include "nminus_tools.hh"
#include "RegionConfig.hh"

#include "EventObjects.hh"

// selection stuff (maybe should break off?)
#include "SignalSelection.hh"
#include "CR1LSelection.hh"
#include "CR1ESelection.hh"
#include "CR1MSelection.hh"
#include "OSDFSelection.hh"
#include "OSSFSelection.hh"
#include "CRZSelection.hh"
#include "VRMCTSelection.hh"
#include "VRMCCSelection.hh"
#include "QualityEventSelection.hh"
#include "MetSelection.hh"

#include "Window.hh"
#include "constants_tagging.hh"
#include "constants_scharmcuts.hh"

#include <stdexcept>
#include <cmath>


// ________________________________________________________________________
// local utility functions

namespace {
  using namespace nminus;

  // _______________________________________________________________________
  // cut adding fucntions (shouldn't call each other)

  // forward for the next function to use
  void add_tagging_cuts(std::map<std::string, nminus::Window>& sel,
			int n_tagged = 2);

  std::map<std::string, Window> get_common_selection(
    const RegionConfig& cfg) {

    // basic kinematics (jets don't have to exist to pass this cut, but
    // the events will fail the n_jet selection)
    std::map<std::string, Window> sel = {
      {jpt(0), {cfg.leading_jet_pt, INFINITY, Window::Missing::ACCEPT} },
      {jpt(1), {cfg.second_jet_pt, INFINITY, Window::Missing::ACCEPT} },
    };
    if (cfg.met > 0.0) sel[MET] = {cfg.met, INFINITY};
    if (cfg.mct > 0.0) sel[MCT] = {cfg.mct, INFINITY};
    if (cfg.first_lepton_pt > 0.0) sel[lpt(0)] = {
	cfg.first_lepton_pt, INFINITY};

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

    // add tagging cuts
    if (cfg.tagger == btag::Tagger::JFC_LEADING_JET) {
      add_tagging_cuts(sel, 1);
    } else if (cfg.tagger == btag::Tagger::JFC ||
	       cfg.tagger == btag::Tagger::JFC_NO_SF) {
      add_tagging_cuts(sel, 2);
    } else if (cfg.tagger != btag::Tagger::NONE) {
      throw std::invalid_argument("tagger isn't going to work");
    }
    return sel;
  }

  void add_tagging_cuts(std::map<std::string, Window>& sel,
			int n_tagged) {
    // accept events where these jets are missing
    // (they will be rejected by the njet cut anyway)
    auto miss = Window::Missing::ACCEPT;
    // add tagging cuts (use of `insert` prevents overwrite)
    for (auto jn = 0; jn < n_tagged; jn++) {
      const auto& antib = btag::JFC_MEDIUM_ANTI_B_CUT;
      const auto& antiu = btag::JFC_MEDIUM_ANTI_U_CUT;
      sel.insert({jeta(jn), {-btag::TAG_ETA, btag::TAG_ETA, miss}});
      sel.insert({jantib(jn), {antib, INFINITY, miss} });
      sel.insert({jantiu(jn), {antiu, INFINITY, miss} });
    }
  }

  // ACHTUNG:
  // These should be merged into the Selection classes.
  // Migration shouldn't be terribly difficult, the only added dependency
  // in the classes is Window.
  void add_1l_cuts(std::map<std::string, Window>& sel) {
    using namespace cr1l;
    sel.insert(
      {
	{MT, {M_T_MIN, M_T_MAX} },
	  });
  }
  void add_sf_cuts(std::map<std::string, Window>& sel) {
    using namespace crsf;
    sel.insert(
      {
	// {LLPT, {LEPTON_PT_MIN, INFINITY} },
	{MLL, {M_LL_MIN, M_LL_MAX} },
	  });
  }
  void add_df_cuts(std::map<std::string, Window>& sel) {
    using namespace crdf;
    sel.insert(
      {
	{MLL, {M_LL_MIN, INFINITY} },
	  });
  }
  void add_sr_cuts(std::map<std::string, Window>& sel) {
    sel.insert( {
	{MET_EFF, {MET_EFF_MIN, INFINITY} },
	{MCC, {M_CC_MIN, INFINITY} },
	{DPHI, {MIN_DPHI_JET_MET, INFINITY} },
	  });
  }
  void add_vr_mcc_cuts(std::map<std::string, Window>& sel) {
    sel.insert( {
	{MET_EFF, {MET_EFF_MIN, INFINITY} },
	{DPHI, {MIN_DPHI_JET_MET, INFINITY} },
	{MCC, {-INFINITY, vr::VR_MCC_MAX} },
	  });
  }
  void add_vr_mct_cuts(std::map<std::string, Window>& sel) {
    sel.insert( {
	{MET_EFF, {MET_EFF_MIN, INFINITY} },
	{DPHI, {MIN_DPHI_JET_MET, INFINITY} },
	{MCT, {-INFINITY, vr::VR_MCT_MAX} },
	  });
  }
  void add_presel_cuts(std::map<std::string, Window>& sel) {
    // this case has phantom cuts to indicate where SR cuts _would_ be
    const auto PHANTOM = Window::Missing::PHANTOM;
    sel.insert({
	{DPHI, {MIN_DPHI_JET_MET, INFINITY} },
	{MCT, {SR_MCT_MIN, INFINITY, PHANTOM} },
	{MET_EFF, {MET_EFF_MIN, INFINITY, PHANTOM} },
	{MCC, {M_CC_MIN, INFINITY, PHANTOM} },
	{DPHI, {MIN_DPHI_JET_MET, INFINITY, PHANTOM} },
	  });
  }
}

// ______________________________________________________________________
// top level selection builder (calls the above functions)

namespace nminus {
  std::map<std::string, Window> get_windows(const RegionConfig& cfg)
  {
    auto sel = get_common_selection(cfg);
    switch (cfg.selection) {
    case reg::Selection::SIGNAL: { add_sr_cuts(sel); return sel; }
      // control / validation regions
    case reg::Selection::CR_1E:	// fallthrough
    case reg::Selection::CR_1M:	// fallthrough
    case reg::Selection::CR_W: { add_1l_cuts(sel); return sel; }
    case reg::Selection::CR_Z_1L: // fallthrough
    case reg::Selection::CR_Z_2L: // fallthrough
    case reg::Selection::CR_Z_AL: { add_sf_cuts(sel); return sel; }
    case reg::Selection::CR_T: { add_df_cuts(sel); return sel; }
    case reg::Selection::VR_MET: // fallthrough
    case reg::Selection::QUALITY_EVENT: { add_presel_cuts(sel); return sel; }
    case reg::Selection::VR_MCT: { add_vr_mct_cuts(sel); return sel; }
    case reg::Selection::VR_MCC: { add_vr_mcc_cuts(sel); return sel; }

    default: throw std::invalid_argument("unknown selection in " __FILE__);
    }
  }
}

namespace {
  std::string jpfx(int jn) {
    return "j" + std::to_string(jn);
  }
  std::string lpfx(int ln) {
    // hack for backward compatibility
    if (ln == 0 || ln == 1) {
      return (ln == 0) ? "leading_lepton" : "second_lepton";
    }
    return "l" + std::to_string(ln);
  }
}

namespace nminus {
  // _______________________________________________________________________
  // naming functions
  std::string jeta(int jn) {
    return jpfx(jn) + "_eta";
  }
  std::string jpt(int jn) {
    return jpfx(jn) + "_pt";
  }
  std::string jantib(int jn) {
    return jpfx(jn) + "_antib";
  }
  std::string jantiu(int jn) {
    return jpfx(jn) + "_antiu";
  }
  std::string jftl(int jn) {
    return jpfx(jn) + "_" + FTL;
  }
  std::string jmetdphi(int jn) {
    return jpfx(jn) + "_" + MET_DPHI;
  }

  std::string lpt(int ln) {
    return lpfx(ln) + "_pt";
  }
  std::string lmetdphi(int ln) {
    return lpfx(ln) + "_" + MET_DPHI;
  }

  // _______________________________________________________________________
  // selection factory
  ISelection* selection_factory(const RegionConfig& cfg) {
    switch (cfg.selection) {
    case reg::Selection::SIGNAL: return new NMinusSignalSelection(cfg);
    case reg::Selection::CR_W: return new NMinusCR1LSelection(cfg);
    case reg::Selection::CR_1E: return new NMinusCR1ESelection(cfg);
    case reg::Selection::CR_1M: return new NMinusCR1MSelection(cfg);
    case reg::Selection::CR_Z_AL: return new NMinusCRZSelection(cfg);
    case reg::Selection::CR_Z_2L: return new NMinusZ2LSelection(cfg);
    case reg::Selection::CR_Z_1L: return new NMinusZ1LSelection(cfg);
    case reg::Selection::CR_T: return new NMinusOSDFSelection(cfg);
    case reg::Selection::QUALITY_EVENT: return new QualityEventSelection(cfg);
    case reg::Selection::VR_MET: return new MetSelection(cfg);
    case reg::Selection::VR_MCT: return new NMinusVRMCTSelection(cfg);
    case reg::Selection::VR_MCC: return new NMinusVRMCCSelection(cfg);

    case reg::Selection::ERROR: throw std::invalid_argument(
      "region selection is ERROR");
    default: throw std::invalid_argument("unknown selection in " __FILE__);
    }
  }

  // ________________________________________________________________________
  // variable adding functions

  std::map<std::string, double> get_reco_map(
    const EventRecoParameters& reco, const TVector2& met){
    return {
      {NSJET, reco.n_signal_jets},
      {MET, met.Mod()},
      {DPHI, reco.min_jetmet_dphi},
      {LEP_DPHI, reco.lepmet_dphi},
      {MCT, reco.mct},
      {MCT_UNCORR, reco.mct_uncorr},
      {MET_EFF, reco.met_eff},
      {MCC, reco.mcc},
	};
  }

  void insert_jets(const std::vector<Jet>& jets, const TVector2& met,
		   std::map<std::string, double>& values) {
    int jn = 0;
    for (const auto& jet: jets) {
      double pb = jet.flavor_weight(Flavor::BOTTOM);
      double pc = jet.flavor_weight(Flavor::CHARM);
      double pu = jet.flavor_weight(Flavor::LIGHT);
      double met_dphi = std::abs(jet.Vect().XYvector().DeltaPhi(met));
      values.insert( {
	  {jeta(jn), jet.Eta()},
	  {jpt(jn), jet.Pt()},
	  {jantib(jn), log(pc/pb)},
	  {jantiu(jn), log(pc/pu)},
	  {jmetdphi(jn), met_dphi} } );

      jn++;
    }
    if (jn >= 2) {
      double jj_dphi = std::abs(jets.at(0).DeltaPhi(jets.at(1)));
      double jj_dr = std::abs(jets.at(0).DeltaR(jets.at(1)));
      values.insert( { {DPHI_CC, jj_dphi}, {DR_CC, jj_dr} });
    }
  } // end insert_jets

  int condensed_ftl(Flavor flavor) {
    switch(flavor) {
    case Flavor::LIGHT: return 0;
    case Flavor::CHARM: return 1;
    case Flavor::BOTTOM: return 2;
    case Flavor::TAU: return 3;
    default: throw std::invalid_argument("bad ftl");
    }
  }
  void insert_jet_ftl(const std::vector<Jet>& jets,
		      std::map<std::string, double>& values) {
    int jn = 0;
    for (const auto& jet: jets) {
      Flavor flavor = jet.flavor_truth_label();
      int flav_bin = condensed_ftl(flavor);
      values[jftl(jn)] = flav_bin;

      jn++;
    }
  }

  void insert_leptons(
    const std::vector<Lepton>& leptons, const TVector2& met,
    std::map<std::string, double>& values){
    int ln = 0;
    for (const auto& lep: leptons) {
      double met_dphi = std::abs(lep.Vect().XYvector().DeltaPhi(met));
      values.insert( {
	  {lpt(ln), lep.Pt() },
	  {lmetdphi(ln), met_dphi} });
      ln++;
    }
    if (ln >= 2) {
      values[DPHI_LL] = std::abs(leptons.at(0).DeltaPhi(leptons.at(1)));
    }
  }

  double get_dphi_any(const std::vector<Jet>& jets,
		      const std::vector<Lepton>& leptons,
		      const TVector2& met){
    double min_dphi = 100;
    for (const auto& jet: jets) {
      double dphi = std::abs(jet.Vect().XYvector().DeltaPhi(met));
      min_dphi = std::min(min_dphi, dphi);
    }
    for (const auto& lep: leptons) {
      double dphi = std::abs(lep.Vect().XYvector().DeltaPhi(met));
      min_dphi = std::min(min_dphi, dphi);
    }
    return min_dphi;
  }

  // ____________________________________________________________________
  // misc stuff

  void throw_if_nan(const std::map<std::string, double>& values) {
    for (const auto itr: values) {
      if (std::isnan(itr.second)) {
	throw std::invalid_argument(itr.first + " is nan");
      }
    }
  }


}

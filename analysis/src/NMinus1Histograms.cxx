#include "NMinus1Histograms.hh"
#include "HistBuilderFlags.hh"
#include "ISelection.hh"
#include "typedefs.hh"
#include "Flavor.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "EventScalefactors.hh"
#include "region_logic.hh"
#include "nminus_tools.hh"

#include "Histogram.hh"

#include "H5Cpp.h"
#include "TVector2.h"

#include <stdexcept>
#include <cassert>
#include <set>

namespace nminus {
}

NMinus1Histograms
::NMinus1Histograms(const RegionConfig& config,
		    const unsigned flags) :
  m_region_config(new RegionConfig(config)),
  m_selection(nminus::selection_factory(config)),
  m_build_flags(flags),
  m_make_lepton_plots(false),
  m_make_dilep_plots(false),
  m_weight_hist(0)
{
  if (reg::lepton_region(config.selection)) m_make_lepton_plots = true;
  if (reg::twolep_region(config.selection)) m_make_dilep_plots = true;

  using namespace nminus;
  const auto sel = get_windows(config);
  // save wt^2 for _some_ hists (those with no systematic applied)
  const auto hf = config.save_wt2 ? hist::wt2 : 0;
  bool is_sim = (config.stream == reg::Stream::SIMULATED);
  m_hists.emplace_back(Axis{MET, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel, hf);
  m_hists.emplace_back(
    Axis{NSJET, SJET_RANGE, -0.5, SJET_RANGE - 0.5}, sel, hf);
  m_hists.emplace_back(Axis{DPHI, 80, 0.0, 3.2}, sel, hf);
  m_hists.emplace_back(Axis{DPHI_CC, 80, 0.0, 3.2}, sel, hf);
  m_hists.emplace_back(Axis{DR_CC, 200, 0.0, 8.0}, sel, hf);
  m_hists.emplace_back(Axis{DPHI_ANY, 80, 0.0, 3.2}, sel, hf);
  m_hists.emplace_back(Axis{MCT, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel, hf);
  m_hists.emplace_back(
    Axis{MCT_UNCORR, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel, hf);
  m_hists.emplace_back(Axis{MET_EFF, N_BINS, 0, 1.0}, sel, hf);
  m_hists.emplace_back(Axis{MCC, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel, hf);
  for (int jn: {0,1,2}) {
    m_hists.emplace_back(Axis{jeta(jn), 112, -2.8, 2.8}, sel, hf);
    m_hists.emplace_back(Axis{jpt(jn), N_BINS, 0, MAX_ENERGY, EUNIT},
			 sel, hf);
    m_hists.emplace_back(Axis{jantib(jn), 300, -7.5, 7.5}, sel, hf);
    m_hists.emplace_back(Axis{jantiu(jn), 300, -7.5, 7.5}, sel, hf);
    m_hists.emplace_back(Axis{jmetdphi(jn), 80, 0.0, 3.2}, sel, hf);
    if (is_sim) m_hists.emplace_back(Axis{jftl(jn), 4, -0.5, 3.5}, sel, hf);
  }

  if (m_make_lepton_plots) {
    m_hists.emplace_back(Axis{lpt(0), N_BINS, 0.0, 500_GeV, EUNIT}, sel, hf);
    m_hists.emplace_back(Axis{lmetdphi(0), 80, 0.0, 3.2}, sel, hf);
    m_hists.emplace_back(Axis{LEP_DPHI, 80, 0.0, 3.2}, sel, hf);
    m_hists.emplace_back(Axis{DPHI_LL, 80, 0.0, 3.2}, sel, hf);
    if (!m_make_dilep_plots) {
      m_hists.emplace_back(Axis{MT, N_BINS, 0.0, 500_GeV, EUNIT}, sel, hf);
    }
  }
  if (m_make_dilep_plots) {
    m_hists.emplace_back(Axis{lpt(1), N_BINS, 0.0, 400_GeV, EUNIT}, sel, hf);
    m_hists.emplace_back(Axis{MLL, 200, 0.0, 200_GeV, EUNIT}, sel, hf);
    m_hists.emplace_back(Axis{lmetdphi(1), 80, 0.0, 3.2}, sel, hf);
  }
  if (is_sim) {
    m_weight_hist = new NMinusHist(Axis{EVT_WT, 200, 0.0, 2.0}, sel);
  }
}

NMinus1Histograms::~NMinus1Histograms() {
  delete m_selection;
  delete m_region_config;
  delete m_weight_hist;
}


namespace nminus {
  std::map<std::string, double> get_reco_map(
    const EventRecoParameters& reco, const TVector2& met);
  void insert_jets(const std::vector<Jet>&, const TVector2& met,
		   std::map<std::string, double>& values);
  void insert_leptons(
    const std::vector<Lepton>&, const TVector2& met,
    std::map<std::string, double>& values);
  double get_dphi_any(const std::vector<Jet>&, const std::vector<Lepton>&,
		      const TVector2& met);
  void insert_jet_ftl(const std::vector<Jet>&,
		      std::map<std::string, double>& values);
  void throw_if_nan(const std::map<std::string, double>& values);
}

namespace {
  // weighting, only apply to mc
  double get_mc_weight(const RegionConfig&, const EventObjects& obj);
}


void NMinus1Histograms::fill(const EventObjects& obj) {

  const EventRecoParameters& reco = obj.reco;
  if (!reco.pass_event_quality) return;
  if (!m_selection->pass(obj)) return;

  // TODO: use the stream and remove build_flags
  double weight = 1.0;
  bool is_mc = !(m_build_flags & buildflag::is_data);
  if (is_mc) {
    weight = get_mc_weight(*m_region_config, obj);
  }

  const TVector2& met = obj.met;

  using namespace nminus;
  auto values = get_reco_map(reco, met);
  insert_jets(obj.jets, met, values);
  if (is_mc) insert_jet_ftl(obj.jets, values);
  if (m_make_lepton_plots) {
    values.insert( { {MT, reco.mt} } );
  }
  if (m_make_dilep_plots) {
    values.insert({ {MLL, reco.mll} });
  }
  insert_leptons(obj.leptons, met, values);
  values[DPHI_ANY] = get_dphi_any(obj.jets, obj.leptons, met);

  // check for nan values
  throw_if_nan(values);
  for (auto& hist: m_hists) {
    hist.fill(values, weight);
  }
  if (m_weight_hist) {
    values[EVT_WT] = weight;
    m_weight_hist->fill(values, 1.0);
  }
}

namespace nminus {
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
}

namespace {
  // weighting, only apply to mc
  double get_mc_weight(const RegionConfig& config, const EventObjects& obj){

    double weight = obj.weight;
    auto syst = config.systematic;

    // apply the tagging SF
    size_t n_jets_tagged = 0;
    if (config.tagger == btag::Tagger::JFC) {
      n_jets_tagged = 2;
    } else if (config.tagger == btag::Tagger::JFC_LEADING_JET) {
      n_jets_tagged = 1;
    }
    size_t n_jets = std::min(n_jets_tagged, obj.jets.size());
    for (size_t jn = 0; jn < n_jets; jn++) {
      auto jet_wt = obj.jets.at(jn).get_scalefactor(syst);
      // hack to deal with jets outside |eta| > 2.5
      if (std::isinf(jet_wt)) jet_wt = 1.0;
      weight *= jet_wt;
    }

    // apply the lepton SF, etc
    weight *= obj.event_scalefactors->get_sf(syst);
    if (config.boson_pt_correction == reg::MARKS) {
      weight *= obj.marks_boson_pt_weight;
    }
    return weight;
  } // end get_weight
}

namespace nminus {

  // __________________________________________________________________
  // map insereting functions

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

void NMinus1Histograms::write_to(H5::CommonFG& file) const {
  using namespace H5;
  Group region(file.createGroup(m_region_config->name));

  for (const auto& hist: m_hists) {
    hist.write_to(region);
  }
  if (m_weight_hist) m_weight_hist->write_to(region);

}


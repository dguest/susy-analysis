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
  m_make_dilep_plots(false)
{
  if (reg::lepton_region(config.selection)) m_make_lepton_plots = true;
  if (reg::twolep_region(config.selection)) m_make_dilep_plots = true;

  using namespace nminus;
  const auto sel = get_windows(config);
  // save wt^2 for _some_ hists (those with no systematic applied)
  const auto hf = config.save_wt2 ? hist::wt2 : 0;
  m_hists.emplace_back(Axis{MET, N_BINS, 0.0, MAX_ENERGY, EUNIT}, sel, hf);
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
    if (config.stream == reg::Stream::SIMULATED) {
      m_hists.emplace_back(Axis{jftl(jn), 4, -0.5, 3.5}, sel, hf);
    }
  }

  if (m_make_lepton_plots) {
    m_hists.emplace_back(Axis{LLPT, N_BINS, 0.0, 500_GeV, EUNIT}, sel, hf);
    m_hists.emplace_back(Axis{MT, N_BINS, 0.0, 500_GeV, EUNIT}, sel, hf);
  }
  if (m_make_dilep_plots) {
    m_hists.emplace_back(Axis{SLPT, N_BINS, 0.0, 400_GeV, EUNIT}, sel, hf);
    m_hists.emplace_back(Axis{MLL, 200, 0.0, 200_GeV, EUNIT}, sel, hf);
  }
}

NMinus1Histograms::~NMinus1Histograms() {
  delete m_selection;
  delete m_region_config;
}


namespace nminus {
  std::map<std::string, double> get_reco_map(
    const EventRecoParameters& reco, const TVector2& met);
  void insert_jets(const std::vector<Jet>&,
		   std::map<std::string, double>& values);
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
  insert_jets(obj.jets, values);
  if (is_mc) insert_jet_ftl(obj.jets, values);
  if (m_make_lepton_plots) {
    values.insert( { {LLPT, reco.first_lepton_pt}, {MT, reco.mt} } );
  }
  if (m_make_dilep_plots) {
    values.insert({ {MLL, reco.mll}, {SLPT, reco.second_lepton_pt}});
  }

  // check for nan values
  throw_if_nan(values);
  for (auto& hist: m_hists) {
    hist.fill(values, weight);
  }
}

namespace nminus {
  std::map<std::string, double> get_reco_map(
    const EventRecoParameters& reco, const TVector2& met){
    return {
      {NSJET, reco.n_signal_jets},
      {MET, met.Mod()},
      {DPHI, reco.min_jetmet_dphi},
      {MCT, reco.mct},
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
    weight *= obj.event_scalefactors->get_sf(EventSyst::ELECTRON, syst);
    weight *= obj.event_scalefactors->get_sf(EventSyst::MUON, syst);
    weight *= obj.event_scalefactors->get_sf(EventSyst::LEPTRIG, syst);
    if (config.boson_pt_correction == reg::MARKS) {
      weight *= obj.marks_boson_pt_weight;
    }
    return weight;
  } // end get_weight
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


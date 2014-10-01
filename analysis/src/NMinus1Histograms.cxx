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

  auto add_hist = [this, &sel, &hf](Axis ax) {
    this->m_hists.emplace_back(ax, sel, hf);
  };
  add_hist(Axis{MET, N_BINS, 0.0, MAX_ENERGY, EUNIT});
  add_hist(Axis{NSJET, SJET_RANGE, -0.5, SJET_RANGE - 0.5});
  add_hist(Axis{DPHI, 80, 0.0, 3.2});
  add_hist(Axis{DPHI_CC, 80, 0.0, 3.2});
  add_hist(Axis{DR_CC, 200, 0.0, 8.0});
  add_hist(Axis{DPHI_ANY, 80, 0.0, 3.2});
  add_hist(Axis{MCT, N_BINS, 0.0, MAX_ENERGY, EUNIT});
  add_hist(Axis{MCT_UNCORR, N_BINS, 0.0, MAX_ENERGY, EUNIT});
  add_hist(Axis{MET_EFF, N_BINS, 0, 1.0});
  add_hist(Axis{MCC, N_BINS, 0.0, MAX_ENERGY, EUNIT});
  for (int jn: {0,1,2}) {
    add_hist(Axis{jeta(jn), 112, -2.8, 2.8});
    add_hist(Axis{jpt(jn), N_BINS, 0, MAX_ENERGY, EUNIT});
    add_hist(Axis{jantib(jn), 300, -7.5, 7.5});
    add_hist(Axis{jantiu(jn), 300, -7.5, 7.5});
    add_hist(Axis{jmetdphi(jn), 80, 0.0, 3.2});
    if (is_sim) add_hist(Axis{jftl(jn), 4, -0.5, 3.5});
  }

  if (m_make_lepton_plots) {
    add_hist(Axis{lpt(0), N_BINS, 0.0, 500_GeV, EUNIT});
    add_hist(Axis{lmetdphi(0), 80, 0.0, 3.2});
    add_hist(Axis{LEP_DPHI, 80, 0.0, 3.2});
    add_hist(Axis{DPHI_LL, 80, 0.0, 3.2});
    if (!m_make_dilep_plots) {
      add_hist(Axis{MT, N_BINS, 0.0, 500_GeV, EUNIT});
    }
  }
  if (m_make_dilep_plots) {
    add_hist(Axis{lpt(1), N_BINS, 0.0, 400_GeV, EUNIT});
    add_hist(Axis{MLL, 200, 0.0, 200_GeV, EUNIT});
    add_hist(Axis{lmetdphi(1), 80, 0.0, 3.2});
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

  // TODO: move these to the insert_leptons function
  if (m_make_lepton_plots) values[MT] = reco.mt;
  if (m_make_dilep_plots) values[MLL] = reco.mll;

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

}

void NMinus1Histograms::write_to(H5::CommonFG& file) const {
  using namespace H5;
  Group region(file.createGroup(m_region_config->name));

  for (const auto& hist: m_hists) {
    hist.write_to(region);
  }
  if (m_weight_hist) m_weight_hist->write_to(region);

}


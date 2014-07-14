#include "copy_functions.hh"
#include "OutTree.hh"
#include "EventObjects.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "ObjectComposites.hh"
#include "btag_defs.hh"
#include "cutflag.hh"
#include "TriggerSF.hh"

#include "TVector2.h"

#include <cassert>
#include <algorithm>

namespace {
  void copy_lepton_info(const std::vector<Muon*>&,
			const std::vector<Electron*>&,
			const TVector2& met,
			outtree::EvtParameters&);
  void copy_trigger_sf(const TriggerSF&, outtree::SFBox&);
}

// ______________________________________________________________
// master copy for event
void copy_event(const EventObjects& obj, JetRep rep,
		const ObjectComposites& par,
		const TVector2& met, outtree::OutTree& out_tree) {
  out_tree.par.n_preselected_jets = obj.preselected_jets.size();
  out_tree.par.n_signal_jets = obj.signal_jets(rep).size();
  out_tree.par.n_baseline_electrons = obj.after_overlap_electrons.size();
  out_tree.par.n_baseline_muons = obj.after_overlap_muons.size();
  out_tree.par.n_signal_electrons = obj.control_electrons.size();
  out_tree.par.n_signal_muons = obj.control_muons.size();

  out_tree.par.min_jetmet_dphi = par.min_jetmet_dphi;
  out_tree.par.met_eff = par.met_eff;
  out_tree.par.mct = par.mass_ct;
  out_tree.par.mcc = par.mass_cc;
  out_tree.par.mt = par.mass_t;
  out_tree.par.mll = par.mass_ll;
  out_tree.par.htx = par.htx;

  copy_met(out_tree, met);
  copy_leading_jet_info(obj.signal_jets(rep), out_tree);
  copy_lepton_info(obj.control_muons, obj.control_electrons,
		   met, out_tree.par);

  copy_id_vec_to_box(obj.control_electrons, out_tree.el_sf);
  copy_id_vec_to_box(obj.control_muons, out_tree.mu_sf);
  if (obj.get_trigger_sf()) {
    copy_trigger_sf(*obj.get_trigger_sf(), out_tree.lepton_trig_sf);
  }
}

//_________________________________________________________________
// copy functions
void copy_jet_info(const SelectedJet* in, outtree::Jet& jet)
{

  assert(in->Pt());
  jet.pt = in->Pt();
  jet.eta = in->Eta();
  jet.phi = in->Phi();
  jet.jfc_b = in->flavor_weight(btag::B, btag::JFC);
  jet.jfc_c = in->flavor_weight(btag::C, btag::JFC);
  jet.jfc_u = in->flavor_weight(btag::U, btag::JFC);
  if (in->has_truth()) {
    jet.flavor_truth_label = in->flavor_truth_label();
    copy_scale_factor(in, jet.jfc_medium, btag::JFC_MEDIUM);
    copy_scale_factor(in, jet.jfc_loose,  btag::JFC_LOOSE);
  }
  jet.jet_bits = in->bits();
}

void copy_scale_factor(const SelectedJet* in, outtree::ScaleFactor& factor,
		       btag::OperatingPoint tagger) {
  SelectedJet::CalResult scale_factor = in->scale_factor(tagger);
  factor.scale_factor = scale_factor.first;
  factor.scale_factor_err = scale_factor.second;
}

void copy_leading_jet_info(const std::vector<SelectedJet*>& signal_jets,
			   outtree::OutTree& out_tree)
{
  size_t maxjets = std::min(signal_jets.size(), out_tree.jets.size());
  for (size_t jet_n = 0; jet_n < maxjets; jet_n++) {
    copy_jet_info(signal_jets.at(jet_n), *out_tree.jets.at(jet_n));
  }
  out_tree.par.n_signal_jets = signal_jets.size();
}


void copy_met(outtree::OutTree& out, const TVector2& met) {
  out.met.set_met(met);
}

void copy_cjet_truth(outtree::OutTree& out_tree,
		     const std::vector<SelectedJet*>& jets)
{
  int leading_pos = -1;
  int subleading_pos = -1;
  int n_cjet = 0;
  for (unsigned jet_pos = 0; jet_pos < jets.size(); jet_pos++) {
    const SelectedJet& jet = *jets.at(jet_pos);
    if (jet.flavor_truth_label() == 4) {
      if (leading_pos == -1) {
	leading_pos = jet_pos;
      }
      else if (subleading_pos == -1){
	subleading_pos = jet_pos;
      }
      n_cjet++;
    } // end charm check
  } // end jet loop
  out_tree.truth_leading_cjet_pos = leading_pos;
  out_tree.truth_subleading_cjet_pos = subleading_pos;
  out_tree.truth_n_cjet = n_cjet;
}

//________________________________________________________________
// helpers

namespace {
  typedef std::pair<TLorentzVector*, int> PartId;
  bool lower_pt(const PartId& p1, const PartId& p2){
    // check to make sure everything is a real pdgid
    assert(std::abs(p1.second) > 0);
    return p1.first->Pt() < p2.first->Pt();
  }
  void copy_lepton_info(const std::vector<Muon*>& mus,
			const std::vector<Electron*>& els,
			const TVector2& met,
			outtree::EvtParameters& par) {
    std::vector<PartId> particles;
    for (auto el: els) particles.emplace_back(el, -11*el->charge());
    for (auto mu: mus) particles.emplace_back(mu, -13*mu->charge());
    if (particles.size() == 0) return;

    std::sort(particles.begin(), particles.end(), lower_pt);

    auto part_riter = particles.rbegin();
    const auto lpart = *part_riter->first;
    par.first_lepton_pt = lpart.Pt();
    par.first_lepton_pdgid = part_riter->second;
    par.lepmet_dphi = std::abs(lpart.Vect().XYvector().DeltaPhi(met));
    part_riter++;

    if (part_riter == particles.rend()) return;
    par.second_lepton_pt = part_riter->first->Pt();
    par.second_lepton_pdgid = part_riter->second;

  }

  void copy_trigger_sf(const TriggerSF& sf, outtree::SFBox& sfb) {
    sfb.nominal = sf.nominal;
    sfb.up = sf.lepton.up;
    sfb.down = sf.lepton.down;
  }

}

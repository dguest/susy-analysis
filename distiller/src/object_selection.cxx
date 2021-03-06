#include "object_selection.hh"
#include "systematic_defs.hh"
#include "constants_distiller.hh"
#include "constants_jet.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "constants_lepton.hh"
#include "exceptions.hh"

#include <cassert>
#include <vector>
#include <cmath>

// TODO: move leptons into leptons, jets into jets?

namespace object {
  Electrons control_electrons(const Electrons& electrons) {
    Electrons out;
    for (auto el: electrons) {
      // bool tight_pp = el->tightpp();
      // bool control_pt = el->Pt() > CONTROL_ELECTRON_PT;
      // bool rel_iso = el->rel_isolation() < CONTROL_ELECTRON_REL_ISOLATION;
      bool pass_signal_exp = el->is_signal();
      if (pass_signal_exp) {
	out.push_back(el);
      }
    }
    return out;
  }

  Muons control_muons(const Muons& muons) {
    Muons out;
    for (auto mu: muons) {
      // bool control_pt = mu->Pt() > CONTROL_MUON_PT;
      // bool iso = mu->isolation() < CONTROL_MUON_ISOLATION;
      if ( mu->is_signal() ) {
	out.push_back(mu);
      }
    }
    return out;
  }

  Jets preselection_jets(const Jets& jets){
    Jets out;
    for (auto jet_itr: jets){
      auto& jet = *jet_itr;
      bool is_low_pt = jet.Pt() < jet::PRESELECTION_PT;
      // bool is_good_eta = std::abs(jet.Eta()) < jet::PRESELECTION_ETA;
      if (!is_low_pt) {
	out.push_back(jet_itr);
      }
    }
    return out;
  }

  Jets remove_bad_jets(const Jets& jets) {
    Jets out;
    for (auto itr: jets){
      auto& jet = *itr;
      if (jet.bits() & jetbit::pass_susy_def) {
	out.push_back(itr);
      }
    }
    return out;
  }

  Jets bad_jets(const Jets& jets) {
    Jets out;
    for (auto jet_itr: jets){
      auto& jet = *jet_itr;
      bool pass_susy = (jet.bits() & jetbit::pass_susy_def);
      if (!pass_susy) {
	out.push_back(jet_itr);
      }
    }
    return out;
  }
  Jets signal_jets(const Jets& jets, systematic::Systematic syst) {
    Jets out;
    float jvf_cut = jet::JVF_CUT;
    if (syst == systematic::JVFDOWN) {
      jvf_cut -= jet::JVF_SYSTEMATIC_VARIATION;
    } else if (syst == systematic::JVFUP) {
      jvf_cut += jet::JVF_SYSTEMATIC_VARIATION;
    }
    for (auto jet_itr: jets){
      const SelectedJet& jet = *jet_itr;
      const float abs_eta = std::abs(jet.Eta());
      bool signal_pt = jet.Pt() > jet::SIGNAL_PT_CUT;
      bool ok_eta = abs_eta < jet::SIGNAL_ETA_CUT;

      bool no_tracks = jet.jvf() < -0.5;
      bool ok_jvf_frac = jet.jvf() > jvf_cut;
      bool ignore_jvf = (jet.Pt() > jet::PT_IGNORE_JVF ||
			 abs_eta > jet::ETA_IGNORE_JFV);

      bool ok_jvf = (ok_jvf_frac || ignore_jvf || no_tracks);
      if (signal_pt && ok_eta && ok_jvf ) {
	out.push_back(jet_itr);
      }
      // bool tag_eta = fabs(jet.Eta()) < jet::TAGGING_ETA_LIM;
    }
    return out;
  }

  SelectedJet* get_leptojet(const Jets& jets, const TLorentzVector& lepton){
    if (jets.size() == 0) return 0;
    SelectedJet* nearest_jet = 0;
    float min_delta_r = INFINITY;
    for (auto j_itr: jets) {
      float delta_r = lepton.DeltaR(*j_itr);
      if (delta_r < min_delta_r) {
	nearest_jet = j_itr;
	min_delta_r = delta_r;
      }
    }
    assert(nearest_jet);
    return nearest_jet;
  }

  bool has_lower_pt(const TLorentzVector* v1, const TLorentzVector* v2) {
    return v1->Pt() < v2->Pt();
  }
  bool has_higher_pt(const TLorentzVector* v1, const TLorentzVector* v2) {
    return v1->Pt() > v2->Pt();
  }


}

#include "distill_tools.hh"
#include "CutflowConstants.hh"
#include "SusyBuffer.h"
#include "OutTree.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "Met.hh"
#include "constants.hh"
#include "RunInfo.hh"
#include "SkimReport.hh"

#include "cutflag.hh"
#include "EventBits.hh"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string> 
#include <streambuf>
#include <cassert>
#include <cstdlib> // getenv, others
#include <cstdio>
#include <map>
#include "TVector2.h"
#include <math.h>
#include <stdexcept> 

TVector2 get_boson_child_pt(const std::vector<Electron*>& el, 
			    const std::vector<Muon*>& mu) { 
  const size_t n_muons = mu.size(); 
  for (size_t mu_n = 0; mu_n < n_muons; mu_n++) { 
    for (size_t mu_n2 = mu_n + 1; mu_n2 < n_muons; mu_n2++) { 
      const auto& mu1 = *mu.at(mu_n); 
      const auto& mu2 = *mu.at(mu_n2); 
      if (are_os_zmass(mu1, mu2)) { 
	return (mu1 + mu2).Vect().XYvector(); 
      }
    }
  }
  if (mu.size() == 1) { 
    return mu.at(0)->Vect().XYvector(); 
  }
  if (el.size() == 1) { 
    return el.at(0)->Vect().XYvector(); 
  }
  return TVector2(); 
}


//_________________________________________________________________
// jet utility 
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

//_________________________________________________________________
// run the cutflow
 

void copy_leading_jet_info(const std::vector<SelectedJet*>& signal_jets, 
			   outtree::OutTree& out_tree)
{
  size_t maxjets = std::min(signal_jets.size(), out_tree.jets.size()); 
  for (size_t jet_n = 0; jet_n < maxjets; jet_n++) { 
    copy_jet_info(signal_jets.at(jet_n), *out_tree.jets.at(jet_n)); 
  }
  out_tree.counts.n_signal_jets = signal_jets.size(); 
}

ull_t control_lepton_bits(const std::vector<Electron*>& el, 
			  const std::vector<Muon*>& mu) { 
  ull_t pass_bits = 0; 
  size_t n_el = el.size(); 
  size_t n_mu = mu.size(); 
  if (n_el == 1) pass_bits |= pass::control_electron; 
  if (n_mu == 1) pass_bits |= pass::control_muon; 
  return pass_bits; 
}

ull_t z_control_bits(const std::vector<Electron*>& el, 
		     const std::vector<Muon*>& mu) { 
  ull_t pass_bits = 0; 
  // both of these true if _any_ leptons are in the z mass window
  // (windown defined as a constant)
  if (has_os_zmass_pair(el)) pass_bits |= pass::os_zmass_el_pair; 
  if (has_os_zmass_pair(mu)) pass_bits |= pass::os_zmass_mu_pair; 
  return pass_bits; 
}

ull_t met_bits(const Mets& mets){ 
  ull_t pass_bits = 0; 
  if (mets.nominal.Mod() > FILTER_MET) { 
    pass_bits |= pass::met; 
  }
  if (mets.nominal.Mod() > CUTFLOW_MET) { 
    pass_bits |= pass::cutflow_met; 
  }
  if (mets.muon.Mod() > FILTER_MET) pass_bits |= pass::mu_met; 

  return pass_bits; 
}


void fill_event_truth(outtree::OutTree& out_tree, const SusyBuffer& buffer, 
		      unsigned flags) { 
  out_tree.hfor_type = buffer.hfor_type; 
  if (flags & cutflag::spartid) { 
    out_tree.spart1_pdgid = buffer.spart1_pdgid; 
    out_tree.spart2_pdgid = buffer.spart2_pdgid; 
  } 
  // currently there is a bug in mc_event_weight, we have to use 
  // mcevt_weight[0][0] instead
  //out_tree.mc_event_weight = buffer.mc_event_weight; 
  out_tree.mc_event_weight = buffer.mcevt_weight->at(0).at(0); 
}

void fill_met(outtree::OutTree& out, const Mets& mets) { 
  out.met_nom.set_met(mets.nominal); 
  out.met_mu.set_met(mets.muon); 
  out.met_nom_up.set_met(mets.nominal_up); 
  out.met_mu_up.set_met(mets.muon_up); 
  out.met_nom_down.set_met(mets.nominal_down); 
  out.met_mu_down.set_met(mets.muon_down); 
  out.met_nom_res.set_met(mets.nominal_res); 
  out.met_mu_res.set_met(mets.muon_res); 
}

void fill_cjet_truth(outtree::OutTree& out_tree, 
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

//___________________________________________________________
// signal jet bits

ull_t signal_jet_bits(const std::vector<SelectedJet*>& jets) { 
  ull_t pass_bits = 0; 
  if (jets.size() > 0) { 
    float leading_jet_pt = jets.at(0)->Pt(); 
    if (leading_jet_pt > FILTER_LEADING_JET_PT) { 
      pass_bits |= pass::leading_jet; 
    }
    if (leading_jet_pt > CUTFLOW_LEADING_JET_PT) { 
      pass_bits |= pass::cutflow_leading; 
    }
  }

  if (jets.size() >= 2) { 
    const SelectedJet* jet = jets.at(1); 
    if (pass_mainz_ctag(jet))       pass_bits |= pass::ctag_mainz; 
  }
  
  const unsigned medium = jetbit::cnn_medium_anti_u | jetbit::cnn_anti_b; 
  if (jets.size() >= 3) { 
    const SelectedJet* jet = jets.at(2); 
    if (pass_mainz_ctag(jet))       pass_bits |= pass::ctag_mainz; 
    if ( (jets.at(1)->bits() & jets.at(2)->bits() & medium) == medium) { 
      pass_bits |= pass::cutflow_tag_2; 
    }
  }
  const unsigned loose = jetbit::cnn_loose_anti_u | jetbit::cnn_anti_b; 
  if (jets.size() >= 4) { 
    if ( (jets.at(1)->bits() & jets.at(2)->bits() & loose) == loose) { 
      if ( (jets.at(3)->bits() & medium) == medium) {
	pass_bits |= pass::cutflow_tag_1; 
      }
    }
  }
  return pass_bits; 
} 

bool pass_mainz_ctag(const SelectedJet* jet){ 
  bool pass = (jet->combNN_btag() > -2.55 && 
	       jet->combNN_btag() < 1.0); 
  return pass; 
}


//____________________________________________________________
// utility functions 



float get_min_jetmet_dphi(const std::vector<SelectedJet*>& jets, 
			  const TVector2& met){ 
  typedef std::vector<SelectedJet*> JVec; 
  typedef JVec::const_iterator JItr; 
  TLorentzVector mector; 
  mector.SetPtEtaPhiE(1,0,met.Phi(),1); 
  float min_dphi = M_PI; 
  for(JItr itr = jets.begin(); itr != jets.end(); itr++){
    float deltaphi = fabs(mector.DeltaPhi(**itr)); 
    min_dphi = std::min(deltaphi, min_dphi); 
  }
  return min_dphi; 
}

TVector2 sum_muon_pt(const std::vector<Muon*>& muons) { 
  TLorentzVector sum; 
  for (auto mu: muons) { 
    sum += *mu; 
  }
  return sum.Vect().XYvector(); 
}

double get_htx(const std::vector<SelectedJet*>& jets, const size_t x){ 
  double htx = 0; 
  const unsigned required_bits = jetbit::pass_susy_def; 
  size_t n_jets = jets.size(); 
  if (n_jets <= x) return 0.0; 
  for (auto itr = jets.cbegin() + x; itr != jets.cend(); itr++) { 
    unsigned jet_bits = (*itr)->bits(); 
    bool has_required = ( (jet_bits & required_bits) == required_bits); 
    if (has_required) { 
      htx += (*itr)->Pt(); 
    }
  }
  return htx; 
}

bool has_lower_pt(const TLorentzVector* v1, const TLorentzVector* v2) { 
  return v1->Pt() < v2->Pt(); 
}
bool has_higher_pt(const TLorentzVector* v1, const TLorentzVector* v2) { 
  return v1->Pt() > v2->Pt(); 
}

void set_bit(std::vector<SelectedJet*>& jets, unsigned bit) { 
  for (std::vector<SelectedJet*>::iterator itr = jets.begin(); 
       itr != jets.end(); itr++) { 
    (*itr)->set_bit(bit); 
  }
}

void calibrate_jets(std::vector<SelectedJet*> jets, 
		    const BtagCalibration* cal) { 
  for (auto jitr: jets) {
    jitr->set_flavor_tag(cal); 
  }
}

void add_skim_report(const SkimReport& report, outtree::OutTree& tree) { 
  tree.add_ll_parameter("total_events", report.total_entries()); 
  if (!report.is_data()) {
    tree.add_double_parameter(
      "total_event_weight", report.sum_evt_weight()); 
  }

}

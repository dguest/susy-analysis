#include "distill_tools.hh"
#include "CutflowConstants.hh"
#include "SusyBuffer.h"
#include "OutTree.hh"
#include "Jets.hh"
#include "JetConstants.hh"
#include "Leptons.hh"
#include "constants.hh"
#include "RunInfo.hh"

#include "RunBits.hh"
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
#include "SUSYTools/SUSYObjDef.h"

#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>

//_________________________________________________________________
// jet utility 
void copy_jet_info(const SelectedJet* in, outtree::Jet& jet)
{

  assert(in->Pt()); 
  jet.pt = in->Pt(); 
  jet.eta = in->Eta(); 
  jet.phi = in->Phi(); 
  jet.cnn_b = in->pb(); 
  jet.cnn_c = in->pc(); 
  jet.cnn_u = in->pu(); 
  if (in->has_truth()) { 
    jet.flavor_truth_label = in->flavor_truth_label(); 
    copy_scale_factor(in, jet.cnn_tight, btag::CNN_TIGHT); 
    copy_scale_factor(in, jet.cnn_medium, btag::CNN_MEDIUM); 
    copy_scale_factor(in, jet.cnn_loose, btag::CNN_LOOSE); 
  }
  jet.cnn_log_cu = log( in->pc() / in->pu() ) ; 
  jet.cnn_log_cb = log( in->pc() / in->pb() ) ; 
  jet.jet_bits = in->bits(); 
}

void copy_scale_factor(const SelectedJet* in, outtree::ScaleFactor& factor, 
		       btag::Tagger tagger) { 
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
  out_tree.n_good_jets = signal_jets.size(); 
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
  if (has_os_zmass_pair(el)) pass_bits |= pass::os_zmass_el_pair; 
  if (has_os_zmass_pair(mu)) pass_bits |= pass::os_zmass_mu_pair; 
  return pass_bits; 
}

ull_t met_bits(const TVector2& met){ 
  ull_t pass_bits = 0; 
  if (met.Mod() > FILTER_MET) { 
    pass_bits |= pass::met; 
  }
  if (met.Mod() > CUTFLOW_MET) { 
    pass_bits |= pass::cutflow_met; 
  }
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
  out_tree.leading_cjet_pos = leading_pos; 
  out_tree.subleading_cjet_pos = subleading_pos; 
  out_tree.n_cjet = n_cjet; 
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

void set_bit(std::vector<SelectedJet*>& jets, unsigned bit) { 
  for (std::vector<SelectedJet*>::iterator itr = jets.begin(); 
       itr != jets.end(); itr++) { 
    (*itr)->set_bit(bit); 
  }
}

void calibrate_jets(std::vector<SelectedJet*> jets, 
		    const BtagCalibration* cal) { 
  typedef std::vector<SelectedJet*> Jets; 
  for (Jets::iterator jitr = jets.begin(); jitr != jets.end(); jitr++) { 
    (*jitr)->set_flavor_tag(cal); 
  }
}

//____________________________________________________________________
// io functions 



//______________________________________________________________________
// object check functions 
  
  
TVector2 get_met(const SusyBuffer& buffer, 
		 SUSYObjDef& def, 
		 const RunInfo&, 
		 const std::vector<int>& mu_mystery_index) 
{ 
  const int n_el = buffer.el_n; 
  std::vector<int> el_mystery_index; 
  for (int el_n = 0; el_n < n_el; el_n++) { 
    float mystery_float = buffer.el_MET_Egamma10NoTau_wet->at(el_n).at(0); 
    if (mystery_float != 0.0) { 
      el_mystery_index.push_back(el_n); 
    }
  }
  
  TVector2 met = def.GetMET
    (buffer.jet_pt, 
     buffer.jet_MET_Egamma10NoTau_wet,
     buffer.jet_MET_Egamma10NoTau_wpx,
     buffer.jet_MET_Egamma10NoTau_wpy,
     buffer.jet_MET_Egamma10NoTau_statusWord,
     el_mystery_index,
     buffer.el_MET_Egamma10NoTau_wet,
     buffer.el_MET_Egamma10NoTau_wpx,
     buffer.el_MET_Egamma10NoTau_wpy,
     buffer.el_MET_Egamma10NoTau_statusWord,
     buffer.MET_Egamma10NoTau_CellOut_etx, //CellOut
     buffer.MET_Egamma10NoTau_CellOut_ety, //CellOut
     buffer.MET_Egamma10NoTau_CellOut_sumet, //CellOut
     buffer.MET_CellOut_Eflow_STVF_etx, 
     buffer.MET_CellOut_Eflow_STVF_ety,
     buffer.MET_CellOut_Eflow_STVF_sumet,		  
     buffer.MET_Egamma10NoTau_RefGamma_etx,
     buffer.MET_Egamma10NoTau_RefGamma_ety,
     buffer.MET_Egamma10NoTau_RefGamma_sumet,
     mu_mystery_index, 
     buffer.mu_staco_ms_qoverp, 
     buffer.mu_staco_ms_theta, 
     buffer.mu_staco_ms_phi, 
     buffer.mu_staco_charge,
     buffer.averageIntPerXing); 
     
  return met; 
}

TVector2 get_mumet(const TVector2& met, const std::vector<Muon*> muons) { 
  TLorentzVector met4(met.Px(), met.Py(), 0, 0); 
  for (auto muon = muons.begin(); muon != muons.end(); muon++) { 
    met4 += **muon; 
  }
  return met4.Vect().XYvector(); 
}


//___________________________________________________________________
// selected jet  
  

//________________________________________________________________
// main function (for quick checks)



//______________________________________________________________
// smart chain


//_____________________________________________________________________
//            cut counter



/////////////////////////////////////////////////////////////////////
//////////////           obsolete?         /////////////////////////
/////////////////////////////////////////////////////////////////////




#include "distill_tools.hh"
#include "SusyBuffer.h"
#include "OutTree.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "constants.hh"
#include "RunInfo.hh"
#include "BitmapCutflow.hh"
#include "SmartChain.hh"
#include "CollectionTreeReport.hh"
#include "EventPreselector.hh"
#include "BtagCalibration.hh"

#include "RunBits.hh"
#include "EventBits.hh"
#include "EventConstants.hh"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string> 
#include <streambuf>
#include <cassert>
#include <cstdlib> // getenv, others
#include <cstdio>
#include <map>
#include "TChain.h"
#include "TVector2.h"
#include "TError.h"
#include <math.h>
#include <stdexcept> 
#include "SUSYTools/SUSYObjDef.h"
#include "SUSYTools/FakeMetEstimator.h"

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
    copy_scale_factor(in, jet.cnn_medium, btag::CNN_MEDIUM); 
    copy_scale_factor(in, jet.cnn_loose, btag::CNN_LOOSE); 
  }
  jet.cnn_log_cu = log( in->pc() / in->pu() ) ; 
  jet.cnn_log_cb = log( in->pc() / in->pb() ) ; 
}

void copy_scale_factor(const SelectedJet* in, outtree::ScaleFactor& factor, 
		       btag::Tagger tagger) { 
  SelectedJet::CalResult scale_factor = in->scale_factor(tagger); 
  SelectedJet::CalResult fail_factor = in->fail_factor(tagger); 
  factor.scale_factor = scale_factor.first; 
  factor.scale_factor_err = scale_factor.second; 
  factor.fail_factor = fail_factor.first; 
  factor.fail_factor_err = fail_factor.second; 
}

//_________________________________________________________________
// run the cutflow
 

void copy_leading_jet_info(const std::vector<SelectedJet*>& signal_jets, 
			   outtree::OutTree& out_tree)
{
  if (signal_jets.size() >= 1) { 
    copy_jet_info(signal_jets.at(0), out_tree.isr_jet); 
  }
  if (signal_jets.size() >= 2) { 
    copy_jet_info(signal_jets.at(1), out_tree.leading_jet);
  }
  if (signal_jets.size() >= 3) { 
    copy_jet_info(signal_jets.at(2), out_tree.subleading_jet); 
  }
  out_tree.n_good_jets = signal_jets.size(); 

}

unsigned jet_cleaning_bit(const std::vector<SelectedJet*>& preselection_jets)
{
  typedef std::vector<SelectedJet*> Jets; 
  unsigned pass_bits = pass::jet_clean; 
  for (Jets::const_iterator jet_itr = preselection_jets.begin(); 
       jet_itr != preselection_jets.end(); 
       jet_itr++) { 
    bool clean_jet = ((*jet_itr)->bits() & jetbit::susy); 
    assert( !( (*jet_itr)->bits() & jetbit::low_pt)); 
    assert( !( (*jet_itr)->bits() & jetbit::high_eta)); 
    if (!clean_jet) pass_bits &=~ pass::jet_clean; 
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

  if (fabs(buffer.mc_event_weight) < 2.0) { 
    out_tree.mc_event_weight = buffer.mc_event_weight; 
  }
  else { 
    out_tree.mc_event_weight = 1.0; 
  }
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
// ctag bits 

bool pass_mainz_ctag(const SelectedJet* jet){ 
  bool pass = (jet->combNN_btag() > -2.55 && 
	       jet->combNN_btag() < 1.0); 
  return pass; 
}
bool pass_anti_b_ctag(const SelectedJet* jet) { 
  return log(jet->pc() / jet->pb() ) > COMBNN_LOG_CB_CUT; 
}
bool pass_anti_u_ctag(const SelectedJet* jet, float cut) { 
  return log(jet->pc() / jet->pu() ) > cut; 
}
unsigned get_ctag_bits(const std::vector<SelectedJet*>& signal_jets) 
{ 
  const float MED = COMBNN_MED_LOG_CU_CUT; 
  const float TGT = COMBNN_TIGHT_LOG_CU_CUT; 
  unsigned pass_bits = 0; 
  if (signal_jets.size() >= 1) { 
    const SelectedJet* jet = signal_jets.at(0); 
    if (pass_anti_b_ctag(jet))      pass_bits |= pass::jet1_anti_b; 
    if (pass_anti_u_ctag(jet, MED)) pass_bits |= pass::jet1_anti_u_medium; 
    if (pass_anti_u_ctag(jet, TGT)) pass_bits |= pass::jet1_anti_u_tight; 
  }
  if (signal_jets.size() >= 2) { 
    const SelectedJet* jet = signal_jets.at(1); 
    if (pass_anti_b_ctag(jet))      pass_bits |= pass::jet2_anti_b; 
    if (pass_anti_u_ctag(jet, MED)) pass_bits |= pass::jet2_anti_u_medium; 
    if (pass_anti_u_ctag(jet, TGT)) pass_bits |= pass::jet2_anti_u_tight; 
    if (pass_mainz_ctag(jet))       pass_bits |= pass::ctag_mainz; 
  }
  if (signal_jets.size() >= 3) { 
    const SelectedJet* jet = signal_jets.at(2); 
    if (pass_anti_b_ctag(jet))      pass_bits |= pass::jet3_anti_b; 
    if (pass_anti_u_ctag(jet, MED)) pass_bits |= pass::jet3_anti_u_medium; 
    if (pass_anti_u_ctag(jet, TGT)) pass_bits |= pass::jet3_anti_u_tight; 
    if (pass_mainz_ctag(jet))       pass_bits |= pass::ctag_mainz; 
  }
  return pass_bits; 
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


double get_htx(const std::vector<SelectedJet*>& jets){ 
  double htx = 0; 
  const unsigned required_bits = jetbit::susy; 
  const unsigned veto_bits = jetbit::leading; 
  for (std::vector<SelectedJet*>::const_iterator itr = jets.begin(); 
       itr != jets.end(); itr++) { 
    unsigned jet_bits = (*itr)->bits(); 
    bool has_required = ( (jet_bits & required_bits) == required_bits); 
    bool has_veto = ( (jet_bits & veto_bits) != 0); 
    if (!has_veto && has_required) { 
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
    (*jitr)->set_scale_factors(cal); 
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
     buffer.MET_Egamma10NoTau_SoftJets_etx,
     buffer.MET_Egamma10NoTau_SoftJets_ety,
     buffer.MET_Egamma10NoTau_SoftJets_sumet,                                
     buffer.MET_Egamma10NoTau_CellOut_etx, //CellOut
     buffer.MET_Egamma10NoTau_CellOut_ety, //CellOut
     buffer.MET_Egamma10NoTau_CellOut_sumet, //CellOut
     buffer.MET_Egamma10NoTau_CellOut_etx, //CellOut Eflow
     buffer.MET_Egamma10NoTau_CellOut_ety, //CellOut Eflow
     buffer.MET_Egamma10NoTau_CellOut_sumet, //CellOut Eflow
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




#include "BosonTruthFilter.hh"
#include "SusyBuffer.h"
#include <stdexcept> 
#include <cassert> 

#include "TLorentzVector.h"

BosonTruthFilter::BosonTruthFilter(double threshold_mev): 
  m_threshold_mev(threshold_mev), 
  m_lt_2_lepton_events(0), 
  m_2_lepton_events(0)
{
}
bool BosonTruthFilter::is_over_threshold(const SusyBuffer* buffer) { 
  assert(buffer->mc_status); 
  int mc_n = buffer->mc_n; 
  TLorentzVector l1;
  TLorentzVector l2;
    
  bool foundFirst = false;
  bool foundSecond = false; 
  for ( int j = 0; j < mc_n; ++j){
    int mc_status = buffer->mc_status->at(j); 
    int abs_pdgid = std::fabs(buffer->mc_status->at(j)); 
    if (mc_status == 3 && abs_pdgid >= 11 && abs_pdgid <= 16){
      float pt = buffer->mc_pt->at(j); 
      float eta = buffer->mc_pt->at(j); 
      float phi = buffer->mc_phi->at(j); 
      float m = buffer->mc_m->at(j); 

      if (!foundFirst){
	l1.SetPtEtaPhiM(pt,eta,phi,m);
	foundFirst = true;
      } else if(!foundSecond){
	l2.SetPtEtaPhiM(pt,eta, phi, m);
	foundSecond = true;
      } else {
	throw std::runtime_error("found more than 2 leptons (in "__FILE__")");
      }
    }
  }
  if (!foundSecond) { 
    m_lt_2_lepton_events++; 
    // a boson decaying to less than two leptons is probably under threshold
    return false; 
  }
  TLorentzVector v = l1 + l2; 
  if (v.Pt() > m_threshold_mev) { 
    return true; 
  }
  else { 
    return false; 
  }
}

int BosonTruthFilter::get_n_lt_2_lepton_events() const { 
  return m_lt_2_lepton_events; 
}

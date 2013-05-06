#include "BosonTruthFilter.hh"
#include "SusyBuffer.h"
#include <stdexcept> 

#include "TLorentzVector.h"

BosonTruthFilter::BosonTruthFilter(double threshold_mev): 
  m_threshold_mev(threshold_mev)
{
}
bool BosonTruthFilter::is_over_threshold(const SusyBuffer* buffer) { 
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
    throw std::runtime_error("couldn't find 2 leptons (in "__FILE__")"); 
  }
  TLorentzVector v = l1 + l2; 
  if (v.Pt() > m_threshold_mev) { 
    return true; 
  }
  else { 
    return false; 
  }
}

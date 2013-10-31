#include "BosonTruthFilter.hh"
#include "SusyBuffer.h"
#include "boson_truth_tools.hh"
#include <cassert> 

#include "TLorentzVector.h"

BosonTruthFilter::BosonTruthFilter(double threshold_mev): 
  m_threshold_mev(threshold_mev), 
  m_lt_2_lepton_events(0), 
  m_2_lepton_events(0)
{
}
bool BosonTruthFilter::is_over_threshold(const SusyBuffer* buffer) { 
  float pt = get_boson_truth_pt(buffer); 
  if (pt > m_threshold_mev) { 
    return true; 
  }
  else { 
    return false; 
  }
}

int BosonTruthFilter::get_n_lt_2_lepton_events() const { 
  return m_lt_2_lepton_events; 
}

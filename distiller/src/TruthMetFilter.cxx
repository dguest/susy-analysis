#include "TruthMetFilter.hh"
#include "SusyBuffer.h"
#include <cmath>

TruthMetFilter::TruthMetFilter(double max_met_mev): 
  m_threshold(max_met_mev) 
{
}
bool TruthMetFilter::is_over_threshold(const SusyBuffer* buffer) { 
  double met_x = buffer->MET_Truth_NonInt_etx; 
  double met_y = buffer->MET_Truth_NonInt_ety; 
  double met = std::sqrt(std::pow(met_x, 2) + std::pow(met_y,2)); 
  return met > m_threshold; 
}

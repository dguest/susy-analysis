#include "OrderedJetTagFilter.hh"
#include "btag_enums.hh"
#include "Jet.hh"
#include <vector> 

OrderedJetTagFilter::
OrderedJetTagFilter(const std::vector<btag::OperatingPoint>& op_points) { 
  for (auto itr = op_points.begin(); itr != op_points.end(); itr++){
    if ( (*itr == btag::NOTAG) && (m_ordered_tags.size() == 0) ) {
      m_n_jets_skipped++; 
      continue; 
    }
    
  }
}

OrderedJetTagFilter::~OrderedJetTagFilter() {}

bool OrderedJetTagFilter::pass(const std::vector<Jet>& jets) { 
  return false; 
}

double OrderedJetTagFilter::jet_scalefactor(const std::vector<Jet>& jets) {
  return 0; 
}

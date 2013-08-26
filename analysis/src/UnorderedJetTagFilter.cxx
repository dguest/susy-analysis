#include "UnorderedJetTagFilter.hh"
#include "btag_enums.hh"
#include "Jet.hh"
#include <vector> 

UnorderedJetTagFilter::
UnorderedJetTagFilter(const std::vector<btag::OperatingPoint>& op_points, 
		      syst::Systematic systematic): 
  m_systematic(systematic)
{ 
  for (auto itr = op_points.begin(); itr != op_points.end(); itr++){
    if ( (*itr == btag::NOTAG) && (m_ordered_tags.size() == 0) ) {
      m_n_jets_skipped++; 
      continue; 
    }
    
  }
}

UnorderedJetTagFilter::~UnorderedJetTagFilter() {}

bool UnorderedJetTagFilter::pass(const std::vector<Jet>& jets) const { 
  return false; 
}

double UnorderedJetTagFilter
::jet_scalefactor(const std::vector<Jet>& jets) const {
  return 0; 
}

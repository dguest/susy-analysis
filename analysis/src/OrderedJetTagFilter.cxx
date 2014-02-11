#include "OrderedJetTagFilter.hh"
#include "btag_enums.hh"
#include "Jet.hh"
#include <vector> 

OrderedJetTagFilter::
OrderedJetTagFilter(const std::vector<btag::OperatingPoint>& op_points, 
		    syst::Systematic systematic) : 
  m_ordered_tags(op_points), 
  m_systematic(systematic)
{ 
}

OrderedJetTagFilter::~OrderedJetTagFilter() {}

std::vector<Jet> OrderedJetTagFilter::tagged_jets(
  const std::vector<Jet>& jets) const { 
  std::vector<Jet> tagged; 
  const auto& jet_req = m_ordered_tags; 
  if (jet_req.size() > jets.size()) return tagged; 
  for (unsigned jet_n = 0; jet_n < jet_req.size(); jet_n++) {
    const auto jet = jets.at(jet_n); 
    const auto requested_tag = jet_req.at(jet_n); 
    bool pass = jet.pass_tag(requested_tag); 
    if (pass) { 
      auto tagged_jet = jet; 
      tagged.push_back(tagged_jet); 
    } else { 
      // return empty vector if tagging fails
      return std::vector<Jet>(); 
    }
  }
  return tagged; 
}


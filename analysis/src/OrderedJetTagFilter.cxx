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

bool OrderedJetTagFilter::pass(const std::vector<Jet>& jets) const { 
  const auto& jet_req = m_ordered_tags; 
  for (unsigned jet_n = 0; jet_n < jet_req.size(); jet_n++) {
    const auto jet = jets.at(jet_n); 
    const auto requested_tag = jet_req.at(jet_n); 
    bool pass = jet.pass_tag(requested_tag); 
    if (!pass) return false; 
  }

  return true; 
}

double OrderedJetTagFilter
::jet_scalefactor(const std::vector<Jet>& jets) const {
  typedef std::vector<Jet> Jets; 
  double weight = 1; 
  const auto& jet_req = m_ordered_tags; 
  for (unsigned jet_n = 0; jet_n < jet_req.size(); jet_n++) {
    const auto jet = jets.at(jet_n); 
    const auto requested_tag = jet_req.at(jet_n); 
    weight *= jet.get_scalefactor(requested_tag, m_systematic); 

  }
  return weight; 
}

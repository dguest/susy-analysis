#include "UnorderedJetTagFilter.hh"
#include "btag_enums.hh"
#include "Jet.hh"
#include <vector> 
#include <list>
#include <stdexcept> 

UnorderedJetTagFilter::
UnorderedJetTagFilter(const std::vector<btag::OperatingPoint>& op_points, 
		      syst::Systematic systematic): 
  m_systematic(systematic), 
  m_n_jets_skipped(0)
{ 
  for (auto itr = op_points.begin(); itr != op_points.end(); itr++){
    if ( *itr == btag::NOTAG ){
      if (m_tag_counts.size() == 0) m_n_jets_skipped++; 
      continue; 
    }
    m_tag_counts[*itr]++; 
  }
}

UnorderedJetTagFilter::~UnorderedJetTagFilter() {}

std::vector<Jet> UnorderedJetTagFilter::tagged_jets(
  const std::vector<Jet>& jets) const { 
  std::vector<Jet> tagged; 
  if (m_n_jets_skipped > jets.size()) return tagged; 
  std::list<Jet> untagged_jets(jets.begin(), jets.end()); 
  for (size_t jn = 0; jn < m_n_jets_skipped; jn++) { 
    untagged_jets.front().set_tag(btag::NOTAG); 
    tagged.push_back(untagged_jets.front()); 
    untagged_jets.pop_front(); 
  }
  for (auto tag_req: m_tag_counts) { 
    btag::OperatingPoint op = tag_req.first; 
    const size_t n_tags_needed = tag_req.second; 
    size_t n_tags_found = 0; 
    auto jitr = untagged_jets.begin(); 
    while (jitr != untagged_jets.end()){
      if (jitr->pass_tag(op)) {
	n_tags_found++; 
	auto tagged_jet = *jitr; 
	tagged_jet.set_tag(op); 
	tagged.push_back(tagged_jet); 
	jitr = untagged_jets.erase(jitr); 
      }
      else { 
	jitr++; 
      }
      if (n_tags_found == n_tags_needed) break; 
    }
    // return empty vector if required tags aren't found 
    if (n_tags_found < n_tags_needed) return std::vector<Jet>(); 
  }
  return tagged; 

}

namespace jetfilter { 
  OpFilter::OpFilter(btag::OperatingPoint op) : 
    m_op(op)
  { 
  }
  bool OpFilter::operator()(const Jet& jet) { 
    return jet.pass_tag(m_op); 
  }
}

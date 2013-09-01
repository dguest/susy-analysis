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

bool UnorderedJetTagFilter::pass(const std::vector<Jet>& jets) const { 
  if (m_n_jets_skipped > jets.size()) return false; 
  std::list<Jet> untagged_jets(jets.begin() + m_n_jets_skipped, jets.end()); 
  for (auto tag_req = m_tag_counts.cbegin(); 
       tag_req != m_tag_counts.cend(); tag_req++) { 
    btag::OperatingPoint op = tag_req->first; 
    const size_t n_tags_needed = tag_req->second; 
    unsigned n_tags_found = 0; 
    for (auto jitr = untagged_jets.begin(); jitr != untagged_jets.end(); ){
      if (jitr->pass_tag(op)) {
	n_tags_found++; 
	jitr = untagged_jets.erase(jitr); 
      }
      else { 
	jitr++; 
      }
      if (n_tags_found == n_tags_needed) break; 
    }
    if (n_tags_found < n_tags_needed) return false; 
  }
  return true; 
}

double UnorderedJetTagFilter
::jet_scalefactor(const std::vector<Jet>& jets) const {
  double scale_factor = 1.0; 
  if (m_n_jets_skipped > jets.size()) return scale_factor; 
  std::list<Jet> untagged_jets(jets.begin() + m_n_jets_skipped, jets.end()); 
  for (auto tag_req = m_tag_counts.cbegin(); 
       tag_req != m_tag_counts.cend(); tag_req++) { 
    btag::OperatingPoint op = tag_req->first; 
    const unsigned n_tags_needed = tag_req->second; 
    unsigned n_tags_found = 0; 
    for (auto jitr = untagged_jets.begin(); jitr != untagged_jets.end(); ) { 
      if (jitr->pass_tag(op)) {
	n_tags_found++; 
	scale_factor *= jitr->get_scalefactor(op, m_systematic); 
	jitr = untagged_jets.erase(jitr); 
      }
      else { 
	jitr++; 
      }
      if (n_tags_found == n_tags_needed) break; 
    }

    // apply the remaining scale factors, start with highest pt
    unsigned remaining_tags = std::min(n_tags_needed - n_tags_found, 
				     untagged_jets.size()); 
    for (auto jitr = untagged_jets.begin(); jitr != untagged_jets.end(); 
	 jitr++) { 
      if (remaining_tags > 0) { 
	remaining_tags--; 
	scale_factor *= jitr->get_scalefactor(op, m_systematic); 
	jitr = untagged_jets.erase(jitr); 
      }
      else break; 
    }
  }
  return scale_factor; 
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

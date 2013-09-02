#ifndef UNORDERED_JET_TAG_FILTER_HH
#define UNORDERED_JET_TAG_FILTER_HH

#include <cstddef>
#include <vector> 
#include <map>
#include "btag_enums.hh"
#include "systematic_defs.hh"
#include "IJetTagFilter.hh"
class Jet; 
class UnorderedJetTagFilter : public IJetTagFilter
{
public: 
  UnorderedJetTagFilter(const std::vector<btag::OperatingPoint>&, 
			syst::Systematic); 
  UnorderedJetTagFilter(const UnorderedJetTagFilter&) = delete; 
  UnorderedJetTagFilter& operator=(const UnorderedJetTagFilter&) = delete; 
  ~UnorderedJetTagFilter(); 
  virtual bool pass(const std::vector<Jet>& jets) const; 
  virtual double jet_scalefactor(const std::vector<Jet>& jets) const; 
private: 
  syst::Systematic m_systematic; 
  size_t m_n_jets_skipped; 
  std::map<btag::OperatingPoint,size_t> m_tag_counts; 
};

namespace jetfilter { 
  // for use with std::list::remove_if
  class OpFilter { 
  public: 
    OpFilter(btag::OperatingPoint); 
    bool operator()(const Jet&); 
  private: 
    btag::OperatingPoint m_op; 
  }; 
}


#endif 

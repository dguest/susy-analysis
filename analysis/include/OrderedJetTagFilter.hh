#ifndef ORDERED_JET_TAG_FILTER_HH
#define ORDERED_JET_TAG_FILTER_HH

#include <vector>
#include "btag_enums.hh"
#include "systematic_defs.hh"
#include "IJetTagFilter.hh"
class Jet;
class OrderedJetTagFilter : public IJetTagFilter
{
public:
  OrderedJetTagFilter(const std::vector<btag::OperatingPoint>&,
		      syst::Systematic);
  OrderedJetTagFilter(const OrderedJetTagFilter&) = delete;
  OrderedJetTagFilter& operator=(const OrderedJetTagFilter&) = delete;
  ~OrderedJetTagFilter();
  virtual std::vector<Jet> tagged_jets(const std::vector<Jet>& jets) const;
private:
  std::vector<btag::OperatingPoint> m_ordered_tags;
  syst::Systematic m_systematic;
};

#endif

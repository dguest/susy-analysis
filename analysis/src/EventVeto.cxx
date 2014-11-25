#include "EventVeto.hh"

EventVeto::EventVeto(const std::vector<std::pair<long, long> >& vetos):
  m_veto_ranges(vetos)
{
}
bool EventVeto::veto(long event_number) const {
  // dumbest possible implementation...
  for (const auto& rng : m_veto_ranges) {
    long low = rng.first;
    long high = rng.second;
    if (low <= event_number && event_number <= high){
      return true;
    }
  }
  return false;
}


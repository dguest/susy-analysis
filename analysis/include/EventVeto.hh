#ifndef EVENT_VETO_HH
#define EVENT_VETO_HH

#include <vector>
#include <utility>

class EventVeto {
public:
  EventVeto(const std::vector<std::pair<long, long> >&);
  bool veto(long event_number) const;
private:
  std::vector<std::pair<long, long> > m_veto_ranges;
};

#endif

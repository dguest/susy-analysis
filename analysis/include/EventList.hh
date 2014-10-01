#ifndef EVENT_LIST_HH
#define EVENT_LIST_HH

#include "IRegionHistograms.hh"
#include "Window.hh"

#include <vector>
#include <string>

class RegionConfig;
class ISelection;

struct EventIndex {
  unsigned run;
  unsigned event;
  bool has_run;
};

class EventList: public IRegionHistograms
{
public:
  EventList(const RegionConfig& config, const unsigned build = 0);
  EventList(EventList&) = delete;
  ~EventList();
  EventList& operator=(EventList&) = delete;
  virtual void fill(const EventObjects& objects);
  virtual void write_to(H5::CommonFG&) const;
private:
  const RegionConfig* m_region_config;
  const ISelection* m_selection;
  std::vector<std::pair<std::string, nminus::Window> > m_windows;
  std::vector<EventIndex> m_events;
};

#endif

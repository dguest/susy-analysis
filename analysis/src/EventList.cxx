#include "EventList.hh"

#include "HistBuilderFlags.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "ISelection.hh"

#include "nminus_tools.hh"

#include "TVector2.h"

EventList::EventList(const RegionConfig& config, const unsigned flags):
  m_region_config(new RegionConfig(config)),
  m_selection(nminus::selection_factory(config))
{
  if (flags & buildflag::is_data) {
    throw std::invalid_argument(__FILE__" not work with data");
  }

  const auto windows = nminus::get_windows(config);
  for (const auto& window: windows) {
    if (window.second.missing != nminus::Window::Missing::PHANTOM){
      m_windows.push_back(window);
    }
  }
}
EventList::~EventList() {
  delete m_region_config;
  delete m_selection;
}

void EventList::fill(const EventObjects& obj) {
  const EventRecoParameters& reco = obj.reco;
  if (!reco.pass_event_quality) return;
  if (!m_selection->pass(obj)) return;

  const TVector2& met = obj.met;
  auto values = nminus::get_reco_map(reco, met);
  nminus::insert_leptons(obj.leptons, met, values);
  nminus::throw_if_nan(values);

  for (const auto& named_window: m_windows) {
    if (! values.count(named_window.first) ) {
      // if (named_window.second.accept_missing()) continue;
      // else return; // reject event if the window doesn't accept missing
    }
  }

}

void EventList::write_to(H5::CommonFG& file) const {

}

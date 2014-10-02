#include "EventList.hh"

#include "HistBuilderFlags.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "ISelection.hh"

#include "nminus_tools.hh"

#include "H5Cpp.h"
#include "TVector2.h"

EventList::EventList(const RegionConfig& config, const unsigned flags):
  m_region_config(new RegionConfig(config)),
  m_selection(nminus::selection_factory(config))
{
  if (!(flags & buildflag::is_data)) {
    throw std::invalid_argument(__FILE__" not work with simulation");
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

  // get value map and add variables
  auto values = nminus::get_reco_map(reco, met);
  nminus::insert_jets(obj.jets, met, values);
  nminus::insert_leptons(obj.leptons, met, values);
  values[nminus::MT] = reco.mt;
  values[nminus::MLL] = reco.mll;
  nminus::throw_if_nan(values);

  // check all cuts
  for (const auto& named_window: m_windows) {
    const auto& window_name = named_window.first;
    const auto& window = named_window.second;
    if (! values.count(window_name) ) {
      if (nminus::accept_missing(window)) continue;
      else return; // reject event if the window doesn't accept missing
    }
    const auto value = values.at(window_name);
    if (window.min > value || value > window.max) return;
  }
  m_events.push_back(EventIndex{0, reco.event_number, false});
}

namespace {
  void write_ds(H5::CommonFG& file, const std::vector<long>& vec,
		const std::string& name) {
    auto type = H5::PredType::NATIVE_LONG;
    hsize_t size = vec.size();
    H5::DataSpace dsp(1, {&size});
    H5::DSetCreatPropList params;
    params.setChunk(1, {&size});
    params.setDeflate(7);
    H5::DataSet ds = file.createDataSet(name, type, dsp, params);
    ds.write(vec.data(), type);
  }
}

void EventList::write_to(H5::CommonFG& file) const {
  using namespace H5;
  const auto& regname = m_region_config->name;
  if (H5Lexists(file.getLocId(), regname.c_str(), H5P_DEFAULT)) {
    throw std::runtime_error("tried to overwrite '" + regname + "'");
  }
  Group region(file.createGroup(regname));
  std::vector<long> evt;
  std::vector<long> run;
  for (auto event: m_events) {
    long signed_evt = static_cast<long>(event.event);
    if (signed_evt < 0){
      throw std::runtime_error("Event counter too big to store as signed");
    }
    evt.push_back(signed_evt);
    run.push_back(event.has_run ? static_cast<long>(event.run) : -1);
  }
  write_ds(file, evt, "event");
  write_ds(file, run, "run");
}

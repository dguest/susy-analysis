#ifndef TRIGGER_LOGIC_HH
#define TRIGGER_LOGIC_HH

#include "region_enums.hh"

struct EventRecoParameters;

namespace trig { 
  void throw_if_not_jetstream(reg::Stream);
  void throw_if_not_lepstream(reg::Stream);
  bool pass_single_lepton_trigger(const EventRecoParameters&, reg::Stream);
  bool pass_two_lepton_trigger(const EventRecoParameters&, reg::Stream);
  bool pass_any_lepton_trigger(const EventRecoParameters&, reg::Stream);
}
#endif

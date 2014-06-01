#include "ObjectComposites.hh"
#include "constants_distiller.hh"
#include "EventObjects.hh"
#include "distill_tools.hh"
#include "Jets.hh"

#include "TVector2.h"

ObjectComposites::ObjectComposites(const EventObjects& obj,
				   const TVector2& met,
				   const TVector2& alt_met) {
  energy_weighted_time = get_energy_weighted_time(
    obj.signal_jets(), ENERGY_WEIGHTED_TIME_NJET);
  min_jetmet_dphi = get_min_jetmet_dphi(
    obj.signal_jets(), alt_met, DPHI_JET_MET_NJET);
  mass_eff = alt_met.Mod() + scalar_sum_pt(obj.leading_jets());
  met_eff = alt_met.Mod() / mass_eff;
  mass_ct = obj.signal_jets().size() >= 2 ?
    get_mctcorr(*obj.signal_jets().at(0), *obj.signal_jets().at(1),
		obj.control_electrons, obj.control_muons, alt_met) : -1;
  mass_cc = obj.signal_jets().size() >= 2 ?
    (*obj.signal_jets().at(0) + *obj.signal_jets().at(1)).M() : -1;
  // mt should use the "normal" met, but the others all use "alt met"
  // whatever that may be.
  mass_t = get_mt(obj.control_electrons, obj.control_muons, met);
  mass_ll = get_mll(obj.control_electrons, obj.control_muons);
  htx = get_htx(obj.signal_jets(), N_SR_JETS);

}


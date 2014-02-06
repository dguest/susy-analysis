#include "ObjectComposites.hh"
#include "constants_distiller.hh"
#include "EventObjects.hh"
#include "distill_tools.hh"
#include "Jets.hh"

#include "TVector2.h"

ObjectComposites::ObjectComposites(const EventObjects& obj, 
				   const TVector2& met) { 
  energy_weighted_time = get_energy_weighted_time(
    obj.signal_jets, ENERGY_WEIGHTED_TIME_NJET); 
  min_jetmet_dphi = get_min_jetmet_dphi(
    obj.signal_jets, met, DPHI_JET_MET_NJET);
  mass_eff = met.Mod() + scalar_sum_pt(obj.leading_jets); 
  met_eff = met.Mod() / mass_eff; 
  mass_ct = obj.signal_jets.size() >= 2 ? 
    get_mctcorr(*obj.signal_jets.at(0), *obj.signal_jets.at(1), met) : -1; 
  mass_cc = obj.signal_jets.size() >= 2 ? 
    (*obj.signal_jets.at(0) + *obj.signal_jets.at(1)).M() : -1; 
  mass_t = get_mt(obj.control_electrons, 
		      obj.control_muons, met); 
  mass_ll = get_mll(obj.control_electrons, obj.control_muons); 
  htx = get_htx(obj.signal_jets, N_SR_JETS); 

}


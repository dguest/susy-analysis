#ifndef OBJECTCOMPOSITES_HH
#define OBJECTCOMPOSITES_HH

class TVector2;
class EventObjects;

struct ObjectComposites {
  ObjectComposites(const EventObjects&,
		   const TVector2& met, const TVector2& alt_met);
  double energy_weighted_time;
  double min_jetmet_dphi;
  double mass_eff;
  double met_eff;
  double mass_ct;
  double mass_cc;
  double mass_t;
  double mass_ll;
  double htx;
};

#endif

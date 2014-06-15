#ifndef EVENT_SCALEFACTORS_HH
#define EVENT_SCALEFACTORS_HH

#include "systematic_defs.hh"
#include "EventScaleEnums.hh"
#include <string>

class TTree;

class SFBox
{
public:
  SFBox(TTree* tree, const std::string& prefix);
  SFBox(const SFBox&) = delete;
  SFBox& operator=(const SFBox&) = delete;
  float get_sf(SystVariation) const;
private:
  float m_nominal;
  float m_up;
  float m_down;
  bool m_has_variations;
};


class EventScalefactors
{
public:
  EventScalefactors(TTree* tree);
  ~EventScalefactors();
  EventScalefactors(const EventScalefactors&) = delete;
  EventScalefactors& operator=(const EventScalefactors&) = delete;
  const SFBox* get_box(EventSyst) const;

  // convenience method for single systematic
  float get_sf(syst::Systematic syst) const;
private:
  SFBox* m_el_sf;
  SFBox* m_mu_sf;
  SFBox* m_lepton_trig_sf;
};

// ________________________________________________________________________
// functions to grab info about variations
SystVariation sf_direction(syst::Systematic);
EventSyst sf_type(syst::Systematic);
bool is_sf_systematic(syst::Systematic);

#endif

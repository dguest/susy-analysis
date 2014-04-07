#include "EventScalefactors.hh"
#include "EventScaleEnums.hh"
#include "common_functions.hh"	// set_branch
#include "StackerExceptions.hh"

#include <string>
#include <stdexcept>

#include "TTree.h"

SFBox::SFBox(TTree* tree, const std::string& prefix):
  m_has_variations(true)
{
  std::string nom_name = prefix;
  set_branch(tree, nom_name, &m_nominal);
  try {
    std::string up_name = prefix + "_up";
    std::string down_name = prefix + "_down";
    set_branch(tree, up_name, &m_up);
    set_branch(tree, down_name, &m_down);
  } catch (MissingBranch& err) {
    m_has_variations = false;
  }
}
float SFBox::get_sf(SystVariation systematic) const{
  if (!m_has_variations && systematic != SystVariation::NONE){
    throw std::logic_error(
      "asked for systematic variation where there was none");
  }
  switch (systematic) {
  case SystVariation::NONE: return m_nominal;
  case SystVariation::UP: return m_up;
  case SystVariation::DOWN: return m_down;
  default:
    throw std::logic_error("unknown systematic in " __FILE__);
  }
}

EventScalefactors::EventScalefactors(TTree* tree):
  m_el_sf(new SFBox(tree, "el_sf")),
  m_mu_sf(new SFBox(tree, "mu_sf"))
{
}

EventScalefactors::~EventScalefactors() {
  delete m_el_sf;
  delete m_mu_sf;
  m_el_sf = 0;
  m_mu_sf = 0;
}

float EventScalefactors::get_sf(EventSyst lept, syst::Systematic sys) const
{
  SystVariation box_syst;
  switch (sys) {
  case syst::ELUP:
  case syst::MUUP:
    box_syst = SystVariation::UP;
    break;
  case syst::ELDOWN:
  case syst::MUDOWN:
    box_syst = SystVariation::DOWN;
  default: box_syst = SystVariation::NONE;
  }
  switch (lept){
  case EventSyst::ELECTRON: return m_el_sf->get_sf(box_syst);
  case EventSyst::MUON: return m_mu_sf->get_sf(box_syst);
  default:
    throw std::logic_error("unknown lepton in " __FILE__);
  }
}

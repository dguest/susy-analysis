#include "EventScalefactors.hh"
#include "EventScaleEnums.hh"
#include "common_functions.hh"	// set_branch
#include "StackerExceptions.hh"
#include "TTBarReweighting.hh"

#include <string>
#include <set>
#include <stdexcept>

#include "TTree.h"

// ________________________________________________________________________
// functions to grab info about variations
namespace {
  SystVariation sf_direction(syst::Systematic);
  EventSyst sf_type(syst::Systematic);
  bool is_sf_systematic(syst::Systematic);
}

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
  m_mu_sf(new SFBox(tree, "mu_sf")),
  m_lepton_trig_sf(new SFBox(tree, "lepton_trig_sf")),
  m_pileup_sf(new SFBox(tree, "pileup_sf"))
{
  try {
    set_branch(tree, "truth_ttbar_pt", &m_truth_ttbar_pt.value);
    m_truth_ttbar_pt.defined = true;
  } catch (MissingBranch& err) {
    m_truth_ttbar_pt.defined = false;
  }
}

EventScalefactors::~EventScalefactors() {
  delete m_el_sf;
  delete m_mu_sf;
  delete m_lepton_trig_sf;
  delete m_pileup_sf;
  m_el_sf = 0;
  m_mu_sf = 0;
  m_lepton_trig_sf = 0;
  m_pileup_sf = 0;
}

const SFBox* EventScalefactors::get_box(EventSyst lept) const {
  switch (lept){
  case EventSyst::ELECTRON: return m_el_sf;
  case EventSyst::MUON: return m_mu_sf;
  case EventSyst::LEPTRIG: return m_lepton_trig_sf;
  case EventSyst::PU: return m_pileup_sf;
  default: return 0;
  }
}

float EventScalefactors::get_sf(syst::Systematic syst) const {
  float sf = 1.0;

  if (syst == syst::TTBAR_PT_RW && m_truth_ttbar_pt.defined) {
    sf *= get_ttbar_reweighting(m_truth_ttbar_pt.value);
  }

  const SFBox* varied = 0;
  if (is_sf_systematic(syst)) varied = get_box(sf_type(syst));
  // ACHTUNG: hack!
  // 14 Jul 2014: this was commented out to agree with Oxford
  for (SFBox* box: {m_el_sf, m_mu_sf, m_pileup_sf /*, m_lepton_trig_sf */} ) {
    if (box == varied) {
      sf *= box->get_sf(sf_direction(syst));
    } else {
      sf *= box->get_sf(SystVariation::NONE);
    }
  }
  return sf;
}

// ________________________________________________________________________
// functions to grab info about variations

namespace {
  const std::set<syst::Systematic> up_type_syst = {
    syst::ELUP, syst::MUUP, syst::LEPTRIGUP, syst::PUUP};
  const std::set<syst::Systematic> down_type_syst = {
    syst::ELDOWN, syst::MUDOWN, syst::LEPTRIGDOWN, syst::PUDOWN};

  const std::set<syst::Systematic> el_syst = {
    syst::ELUP, syst::ELDOWN};
  const std::set<syst::Systematic> mu_syst = {
    syst::MUUP, syst::MUDOWN};
  const std::set<syst::Systematic> trig_syst = {
    syst::LEPTRIGUP, syst::LEPTRIGDOWN};
  const std::set<syst::Systematic> pu_syst = {
    syst::PUUP, syst::PUDOWN};


  SystVariation sf_direction(syst::Systematic sys)
  {
    if (up_type_syst.count(sys)) return SystVariation::UP;
    else if (down_type_syst.count(sys)) return SystVariation::DOWN;
    else return SystVariation::NONE;
  }

  EventSyst sf_type(syst::Systematic sys) {
    if (el_syst.count(sys)) return EventSyst::ELECTRON;
    if (mu_syst.count(sys)) return EventSyst::MUON;
    if (trig_syst.count(sys)) return EventSyst::LEPTRIG;
    if (pu_syst.count(sys)) return EventSyst::PU;
    throw std::logic_error("non-sf systematic");
  }

  bool is_sf_systematic(syst::Systematic sys) {
    return up_type_syst.count(sys) || down_type_syst.count(sys);
  }

}


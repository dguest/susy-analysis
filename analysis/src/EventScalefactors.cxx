#include "EventScalefactors.hh"
#include "EventScaleEnums.hh"

#include <string> 
#include <stdexcept>

#include "TTree.h"

SFBox::SFBox(TTree* tree, const std::string& prefix) 
{ 
  std::string nom_name = prefix;
  std::string up_name = prefix + "_up"; 
  std::string down_name = prefix + "_down"; 
  size_t errors = 0; 
  errors += std::abs(tree->SetBranchAddress(nom_name.c_str(), &m_nominal)); 
  errors += std::abs(tree->SetBranchAddress(up_name.c_str(), &m_up)); 
  errors += std::abs(tree->SetBranchAddress(down_name.c_str(), &m_down)); 
  if (errors) throw std::logic_error("missing branches in " __FILE__); 
}
float SFBox::get_sf(SystVariation systematic) const{ 
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

float EventScalefactors
::lepton_sf(EventSyst lept, syst::Systematic sys) const 
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

#include "CutAugmenter.hh"
#include "CutBits.hh"
#include "JetFactory.hh"
#include "common_functions.hh"

#include "TVector2.h"

#include <vector> 
#include <stdexcept> 

CutAugmenter::CutAugmenter() : 
  m_leading_jet_pt_cut(-1), 
  m_met_cut(-1), 
  m_j2_anti_b_cut(-999), 
  m_j2_anti_u_cut(-999), 
  m_j3_anti_b_cut(-999), 
  m_j3_anti_u_cut(-999), 
  m_mttop_cut(-999)
{ 
}

void CutAugmenter::set_float(std::string name, double value) { 
  if (name == "leading_jet") { 
    m_leading_jet_pt_cut = value; 
  }
  else if (name == "met") { 
    m_met_cut = value; 
  }
  else if (name == "j2_anti_b") { 
    m_j2_anti_b_cut = value; 
  }
  else if (name == "j2_anti_u") { 
    m_j2_anti_u_cut = value; 
  }
  else if (name == "j3_anti_b") { 
    m_j3_anti_b_cut = value; 
  }
  else if (name == "j3_anti_u") { 
    m_j3_anti_u_cut = value; 
  }
  else if (name == "mttop") { 
    m_mttop_cut = value; 
  }
  else { 
    throw std::runtime_error(name + " isn't defined in CutAugmenter"); 
  }
}

unsigned CutAugmenter::get_added_cuts(const std::vector<Jet>& jets, 
				      const TVector2& evt_met) const { 
  using namespace optcut; 
  unsigned added = 0; 
  if (evt_met.Mod() > m_met_cut) { 
    added |= met; 
  }

  if (jets.size() <= 0) { 
    return added; 
  }
  if (jets.at(0).Pt() > m_leading_jet_pt_cut) { 
    added |= leading_jet; 
  }

  if (jets.size() <= 1) { 
    return added; 
  }
  const Jet& j2 = jets.at(1); 
  if (log(j2.pc() / j2.pb()) > m_j2_anti_b_cut) { 
    added |= j2_anti_b; 
  }
  if (log(j2.pc() / j2.pu()) > m_j2_anti_u_cut) { 
    added |= j2_anti_u; 
  }

  if (jets.size() <= 2) { 
    return added; 
  }
  const Jet& j3 = jets.at(2); 
  if (log(j3.pc() / j3.pb()) > m_j3_anti_b_cut) { 
    added |= j3_anti_b; 
  }
  if (log(j3.pc() / j3.pu()) > m_j3_anti_u_cut) { 
    added |= j3_anti_u; 
  }
  if (get_mttop(jets, evt_met) > m_mttop_cut) { 
    added |= mttop; 
  }
  return added; 

}

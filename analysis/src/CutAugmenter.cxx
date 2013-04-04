#include "CutAugmenter.hh"
#include "JetFactory.hh"
#include "common_functions.hh"

#include <vector> 
#include <stdexcept> 

#include "distiller/EventBits.hh"

#include "TVector2.h"

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

void CutAugmenter::set_cutmask(ull_t& added, 
			       const std::vector<Jet>& jets, 
			       const TVector2& evt_met) const { 
  if (m_met_cut > 0) { 
    if (evt_met.Mod() > m_met_cut) { 
      added |= pass::met; 
    }
    else { 
      added &=~ pass::met; 
    }
  }

  if (jets.size() <= 0) { 
    return; 
  }
  if (m_leading_jet_pt_cut > 0) { 
    if (jets.at(0).Pt() > m_leading_jet_pt_cut) { 
      added |= pass::leading_jet; 
    }
    else { 
      added &=~ pass::leading_jet; 
    }
  }
  if (jets.size() <= 1) { 
    return; 
  }
  // const Jet& j2 = jets.at(1); 
  // if (log(j2.pc() / j2.pb()) > m_j2_anti_b_cut) { 
  //   added |= pass::j2_anti_b_opt; 
  // }
  // if (log(j2.pc() / j2.pu()) > m_j2_anti_u_cut) { 
  //   added |= pass::j2_anti_u_opt; 
  // }

  // if (jets.size() <= 2) { 
  //   return; 
  // }
  // const Jet& j3 = jets.at(2); 
  // if (log(j3.pc() / j3.pb()) > m_j3_anti_b_cut) { 
  //   added |= pass::j3_anti_b_opt; 
  // }
  // if (log(j3.pc() / j3.pu()) > m_j3_anti_u_cut) { 
  //   added |= pass::j3_anti_u_opt; 
  // }
  if (get_mttop(jets, evt_met) > m_mttop_cut) { 
    added |= pass::mttop; 
  }

}

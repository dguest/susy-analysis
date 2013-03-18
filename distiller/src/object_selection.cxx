#include "object_selection.hh"
#include "DistillerConstants.hh"
#include "Leptons.hh"
#include <vector> 

namespace object { 
  Electrons veto_electrons(const Electrons& preselected_electrons) { 
    Electrons out; 
    for (auto el = preselected_electrons.begin(); 
	 el != preselected_electrons.end(); el++) { 
      if ((*el)->rel_isolation() < VETO_ELECTRON_REL_ISOLATION) { 
	out.push_back(*el); 
      }
    }
    return out; 
  }
  Electrons control_electrons(const Electrons& electrons) { 
    Electrons out; 
    for (auto el = electrons.begin(); el != electrons.end(); el++) { 
      if ((*el)->Pt() > CONTROL_ELECTRON_PT) { 
	out.push_back(*el); 
      }
    }
    return out; 
  }

  Muons veto_muons(const Muons& preselected_muons){ 
    Muons out; 
    for (auto mu = preselected_muons.begin(); mu != preselected_muons.end(); 
	 mu++) { 
      if ( (*mu)->isolation() < VETO_MUON_ISOLATION) { 
	out.push_back(*mu); 
      }
    }
    return out; 
  }
  Muons control_muons(const Muons& muons) { 
    Muons out; 
    for (auto mu = muons.begin(); mu != muons.end(); mu++) { 
      if ( (*mu)->Pt() > CONTROL_MUON_PT) { 
	out.push_back(*mu); 
      }
    }
    return out; 
  }
  
}

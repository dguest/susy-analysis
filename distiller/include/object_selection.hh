#ifndef OBJECT_SELECTION_HH
#define OBJECT_SELECTION_HH

#include <vector> 
#include <cassert>
class Electron; 
class Muon; 
class SelectedJet; 
class TLorentzVector; 

namespace object { 
  typedef std::vector<Electron*> Electrons; 
  typedef std::vector<Muon*> Muons; 
  typedef std::vector<SelectedJet*> Jets; 
  Electrons veto_electrons(const Electrons&); 
  Electrons control_electrons(const Electrons&); 
  Muons veto_muons(const Muons&); 
  Muons control_muons(const Muons&); 
  Jets preselection_jets(const Jets&); 
  Jets bad_jets(const Jets&); 
  Jets signal_jets(const Jets&); 
  Jets remove_bad_jets(const Jets&); 

  bool has_lower_pt(const TLorentzVector*, const TLorentzVector*); 
  bool has_higher_pt(const TLorentzVector*, const TLorentzVector*); 

  template<typename T>
  std::vector<int> get_indices(const T&); 

  template<typename T>
  std::vector<T*> filter_susy(const std::vector<T*>&); 

  template<typename M, typename A>
  A remove_overlaping(const M& mask, A altered, const float delta_r); 

  // returns jet with lowest delta R to the TLorentzVector
  SelectedJet* get_leptojet(const Jets&, const TLorentzVector&); 

  // implementation 

  template<typename T>
  std::vector<int> get_indices(const T& cont) 
  { 
    std::vector<int> indices; 
    for (typename T::const_iterator itr = cont.begin(); 
	 itr != cont.end(); itr++ ) { 
      indices.push_back((*itr)->index()); 
    }
    return indices; 
  }; 

  template<typename T>
  std::vector<T*> filter_susy(const std::vector<T*>& in) 
  { 
    std::vector<T*> out; 
    for (typename std::vector<T*>::const_iterator itr = in.begin(); 
	 itr!= in.end(); itr++) { 
      if ( (*itr)->pass_susy() ) { 
	out.push_back(*itr); 
      }
    }
    return out; 
  }

  template<typename M, typename A>
  A remove_overlaping(const M& mask, A altered, const float delta_r) { 
    for (auto mitr = mask.begin(); mitr != mask.end(); mitr++) { 
      A new_container; 
      for (auto vic = altered.begin(); vic != altered.end(); vic++) { 
	assert((*mitr)->Pt() > 0); 
	double delr = (*mitr)->DeltaR(**vic); 
	if (delr > delta_r) { 
	  new_container.push_back(*vic); 
	}
      }
      altered = new_container; 
    }
    return altered; 
  } 

}

#endif 

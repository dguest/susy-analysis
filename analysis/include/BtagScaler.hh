#ifndef BTAG_SCALER_HH
#define BTAG_SCALER_HH

#include <string> 
#include <set> 
#include <boost/noncopyable.hpp>
#include "systematic_defs.hh"
#include "typedefs.hh"
#include "BtagConfig.hh"

class TTree; 


// Class is set on a tree with a mask and antimask to determine whether 
// a given btagger was passed. 
// For each event, user calls get_scalefactor() with the event bitmask.
// The mask is checked and the scalefactor is returned. 
// Optionally can apply a systematic. 
class BtagScaler: boost::noncopyable
{
public: 
  BtagScaler(TTree*, std::string branch_name, btag::JetTag); 
  double get_scalefactor(unsigned jet_mask, int flavor, 
			 syst::Systematic = syst::NONE) const; 
private: 
  void safeset(TTree*, std::string branch, void* address); 
  std::string joiner(btag::JetTag); 
  unsigned m_veto; 
  unsigned m_required; 
  std::set<std::string> m_set_branches; 

  // these guys are pointed to by the tree, don't try to reset them
  double m_scale_factor; 
  double m_scale_factor_err; 
}; 

#endif 

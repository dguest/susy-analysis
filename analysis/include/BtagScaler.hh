#ifndef BTAG_SCALER_HH
#define BTAG_SCALER_HH

#include <string> 
#include <boost/noncopyable.hpp>
#include "systematic_defs.hh"
#include "typedefs.hh"

class TTree; 


// Class is set on a tree with a mask and antimask to determine whether 
// a given btagger was passed. 
// For each event, user calls get_scalefactor() with the event bitmask.
// The mask is checked and the scalefactor is returned. 
// Optionally can apply a systematic. 
class BtagScaler: boost::noncopyable
{
public: 
  BtagScaler(TTree*, std::string prefix, 
	     ull_t required, ull_t veto = 0); 
  double get_scalefactor(ull_t evt_mask, int flavor, 
			 syst::Systematic = syst::NONE); 
private: 
  void safeset(TTree*, std::string branch, void* address); 
  ull_t m_required; 
  ull_t m_veto; 

  // these guys are pointed to by the tree, don't try to reset them
  double m_scale_factor; 
  double m_scale_factor_err; 
}; 

#endif 

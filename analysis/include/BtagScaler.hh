#ifndef BTAG_SCALER_HH
#define BTAG_SCALER_HH

#include <string> 
#include <boost/noncopyable.hpp>

class TTree; 

namespace btag 
{ 
  enum Systematic { NONE, BUP, BDOWN, CUP, CDOWN, UUP, UDOWN, TUP, TDOWN}; 
}; 

// Class is set on a tree with a mask and antimask to determine whether 
// a given btagger was passed. 
// For each event, user calls get_scalefactor() with the event bitmask.
// The mask is checked and the scalefactor is returned. 
// Optionally can apply a systematic. 
class BtagScaler: boost::noncopyable
{
public: 
  BtagScaler(TTree*, std::string prefix, 
	     unsigned required, unsigned veto = 0); 
  double get_scalefactor(unsigned evt_mask, int flavor, 
			 btag::Systematic = btag::NONE); 
private: 
  void safeset(TTree*, std::string branch, void* address); 
  unsigned m_required; 
  unsigned m_veto; 

  // these guys are pointed to by the tree, don't try to reset them
  double m_scale_factor; 
  double m_fail_factor; 
  double m_scale_factor_err; 
  double m_fail_factor_err; 
}; 

#endif 

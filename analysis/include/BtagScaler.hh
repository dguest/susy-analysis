#ifndef BTAG_SCALER_HH
#define BTAG_SCALER_HH

#include <string> 
#include <map>
#include <boost/noncopyable.hpp>
#include "systematic_defs.hh"
#include "typedefs.hh"
#include "btag_enums.hh"
#include "Flavor.hh"

class BtagBuffer; 

// Class is set on a tree with a mask and antimask to determine whether 
// a given btagger was passed. 
// For each event, user calls get_scalefactor() with the event bitmask.
// The mask is checked and the scalefactor is returned. 
// Optionally can apply a systematic. 
class BtagScaler: boost::noncopyable
{
public: 
  BtagScaler(const BtagBuffer* buffer, btag::OperatingPoint); 
  double get_scalefactor(unsigned jet_mask, Flavor flavor, 
			 syst::Systematic = syst::NONE) const; 
private: 
  const BtagBuffer* m_buffer; 
  unsigned m_veto; 
  unsigned m_required; 
  bool m_inverted; 
}; 

#endif 

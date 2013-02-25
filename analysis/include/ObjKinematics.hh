#ifndef OBJ_KINEMATICS
#define OBJ_KINEMATICS

class Jet; 
namespace H5 { 
  class CommonFG; 
}
class MaskedHistArray; 

#include <string> 
#include <boost/noncopyable.hpp>
#include "typedefs.hh"

class Jet1DHists: public boost::noncopyable
{
public: 
  Jet1DHists(double max_pt, const unsigned flags, std::string tag = ""); 
  ~Jet1DHists(); 
  void add_mask(ull_t bitmask, std::string name = "", 
		ull_t antimask = 0); 
  void write_to(H5::CommonFG&, std::string stub = "", 
		std::string postfix = "_cut_"); 
  void fill(const Jet&, ull_t = 0, double weight = 1.0); 
private: 
  MaskedHistArray* m_pt; 
  MaskedHistArray* m_eta; 
  MaskedHistArray* m_cnnLogCb; 
  MaskedHistArray* m_cnnLogCu; 
  MaskedHistArray* m_cnnLogBu; 
  MaskedHistArray* m_met_dphi; 

  void write_truth_info(H5::CommonFG&); 
  MaskedHistArray* m_truth_label; 
  std::string m_tag; 
}; 

#endif

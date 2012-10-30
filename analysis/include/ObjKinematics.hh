#ifndef OBJ_KINEMATICS
#define OBJ_KINEMATICS

class Jet; 
namespace H5 { 
  class CommonFG; 
}
class MaskedHistArray; 

#include <string> 
#include <boost/noncopyable.hpp>

class Jet1DHists: public boost::noncopyable
{
public: 
  Jet1DHists(double max_pt); 
  ~Jet1DHists(); 
  void add_mask(unsigned bitmask, std::string name = ""); 
  void write_to(H5::CommonFG&, std::string stub = "", 
		std::string postfix = "_cut_"); 
  void fill(const Jet&, unsigned = 0); 
private: 
  MaskedHistArray* m_pt; 
  MaskedHistArray* m_eta; 
  MaskedHistArray* m_cnnLogCb; 
  MaskedHistArray* m_cnnLogCu; 
  MaskedHistArray* m_cnnLogBu; 
}; 

#endif

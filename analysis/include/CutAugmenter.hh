#ifndef CUT_AUGMENTER_HH
#define CUT_AUGMENTER_HH

#include <string> 
// #include <map>
#include <vector> 
#include <boost/noncopyable.hpp>

class Jet; 
class TVector2; 

class CutAugmenter: public boost::noncopyable
{
public: 
  CutAugmenter(); 
  void set_float(std::string, double);
  void set_cutmask(unsigned& mask, const std::vector<Jet>&, 
		   const TVector2& ) const; 
private: 
  double m_leading_jet_pt_cut; 
  double m_met_cut; 
  double m_j2_anti_b_cut; 
  double m_j2_anti_u_cut; 
  double m_j3_anti_b_cut; 
  double m_j3_anti_u_cut; 
  double m_mttop_cut; 
}; 

#endif 

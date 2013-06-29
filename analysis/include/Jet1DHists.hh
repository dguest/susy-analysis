#ifndef JET_1D_HISTS
#define JET_1D_HISTS

class Jet; 
namespace H5 { 
  class CommonFG; 
}
class Histogram; 

#include <string> 
#include <boost/noncopyable.hpp>
#include "typedefs.hh"

class Jet1DHists: public boost::noncopyable
{
public: 
  Jet1DHists(double max_pt, const unsigned flags); 
  ~Jet1DHists(); 
  void write_to(H5::CommonFG&); 
  void fill(const Jet&, double weight = 1.0); 
private: 
  Histogram* m_pt; 
  Histogram* m_eta; 
  Histogram* m_cnnLogCb; 
  Histogram* m_cnnLogCu; 
  Histogram* m_cnnLogBu; 
  Histogram* m_met_dphi; 
  Histogram* m_mu_met_dphi; 

  void write_truth_info(H5::CommonFG&); 
  Histogram* m_truth_label; 
}; 

#endif

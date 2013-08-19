#ifndef TRUTH_JET_HISTS_H
#define TRUTH_JET_HISTS_H

class Jet; 
namespace H5 { 
  class CommonFG; 
}
class Jet1DHists;

#include <string> 
#include <boost/noncopyable.hpp>
#include "typedefs.hh"
#include "btag_enums.hh"

class TruthJetHists: public boost::noncopyable
{
public: 
  TruthJetHists(double max_pt, const unsigned flags, btag::Tagger); 
  ~TruthJetHists(); 
  void write_to(H5::CommonFG&); 
  void fill(const Jet&, double weight = 1.0); 
private: 
  Jet1DHists* m_bottom; 
  Jet1DHists* m_charm; 
  Jet1DHists* m_light; 
  Jet1DHists* m_tau; 
  Jet1DHists* m_other; 
}; 

#endif 

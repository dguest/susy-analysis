#ifndef HIST_BUILDER_HH
#define HIST_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 
#include <boost/noncopyable.hpp>

class JetFactory; 
class Histogram; 
class MaskedHistArray; 
class Axis; 
class Jet1DHists; 
class Jet; 
class TVector2; 


class HistBuilder : public boost::noncopyable
{
public: 
  HistBuilder(std::string input, const unsigned flags = 0); 
  ~HistBuilder(); 
  void add_cut_mask(std::string name, unsigned bits); 
  void build(); 
  void save(std::string output = ""); 
private: 
  double get_mttop(const std::vector<Jet>& jets, TVector2 met); 
  double get_mttop(const Jet& jet, TVector2 met); 
  void fill_truth_hists(const std::vector<Jet>& jets, const unsigned mask); 
  typedef std::map<std::string, unsigned> CutMasks; 

  const unsigned m_flags; 

  JetFactory* m_factory; 
  
  CutMasks m_cut_masks; 

  Jet1DHists* m_jet1_hists; 
  Jet1DHists* m_jet2_hists; 
  Jet1DHists* m_jet3_hists; 

  MaskedHistArray* m_met; 
  MaskedHistArray* m_min_dphi; 
  MaskedHistArray* m_j2_met_dphi; 
  MaskedHistArray* m_mttop; 
  MaskedHistArray* m_n_good_jets; 

  MaskedHistArray* m_leading_cjet_rank; 
  MaskedHistArray* m_subleading_cjet_rank; 

};

#endif // HIST_BUILDER_HH

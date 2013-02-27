#ifndef HIST_BUILDER_HH
#define HIST_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 
#include <boost/noncopyable.hpp>
#include "typedefs.hh"

class JetFactory; 
class Histogram; 
class MaskedHistArray; 
class TruthJetHists; 
struct Axis; 
class Jet1DHists; 
class Jet; 
class TVector2; 
class CutAugmenter; 

class HistBuilder : public boost::noncopyable
{
public: 
  HistBuilder(std::string input, const unsigned flags = 0); 
  ~HistBuilder(); 
  void add_cut_mask(std::string name, ull_t bits, ull_t antibits = 0); 
  int build(); 
  void save(std::string output = ""); 
  void set_float(std::string, double);
private: 
  void fill_truth_hists(const std::vector<Jet>& jets, 
			const ull_t mask, double weight); 
  typedef std::map<std::string, ull_t> CutMasks; 

  const unsigned m_flags; 

  JetFactory* m_factory; 
  
  CutMasks m_cut_masks; 

  Jet1DHists* m_jet1_hists; 
  Jet1DHists* m_jet2_hists; 
  Jet1DHists* m_jet3_hists; 

  MaskedHistArray* m_met; 
  MaskedHistArray* m_min_dphi; 
  MaskedHistArray* m_mttop; 
  MaskedHistArray* m_n_good_jets; 

  MaskedHistArray* m_htx; 

  MaskedHistArray* m_leading_cjet_rank; 
  MaskedHistArray* m_subleading_cjet_rank; 

  TruthJetHists* m_jet1_truth; 
  TruthJetHists* m_jet2_truth; 
  TruthJetHists* m_jet3_truth; 

  CutAugmenter* m_cut_augmenter; 
};

#endif // HIST_BUILDER_HH

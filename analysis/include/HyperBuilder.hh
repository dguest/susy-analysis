#ifndef HYPER_BUILDER_HH
#define HYPER_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 
#include <boost/noncopyable.hpp>
#include "typedefs.hh"

class JetFactory; 
class Histogram; 
class MaskedHistArray; 
class Axis; 
class Jet; 
class CutAugmenter; 


class HyperBuilder : public boost::noncopyable
{
public: 
  HyperBuilder(std::string input, const unsigned flags = 0); 
  ~HyperBuilder(); 
  void add_cut_mask(std::string name, ull_t bits, ull_t antibits = 0); 
  int build(); 
  void save(std::string output = ""); 
  void set_float(std::string, double); 
private: 
  void init(std::string input, const unsigned flags = 0); 
  std::map<std::string, double> get_jet_vars(const std::vector<Jet>&) const; 
  int int_from_double(double) const; 
  typedef std::vector<std::pair<std::string, ull_t> > CutMasks; 
  const unsigned m_flags; 
  JetFactory* m_factory;
  CutAugmenter* m_augmenter; 
  CutMasks m_cut_masks; 

  MaskedHistArray* m_hists; 
  std::string m_input_file; 

  double m_max_pt; 
  double m_min_pt; 

  bool m_do_cu; 
  bool m_do_cb; 

  int m_n_met_bins; 
  int m_n_pt_bins; 

  double m_min_cu; 
  double m_max_cu; 
  int m_n_cu_bins; 

  double m_min_cb; 
  double m_max_cb; 
  int m_n_cb_bins; 

};


#endif 

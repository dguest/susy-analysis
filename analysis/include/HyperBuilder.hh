#ifndef HYPER_BUILDER_HH
#define HYPER_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 
#include <boost/noncopyable.hpp>

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
  void add_cut_mask(std::string name, unsigned bits); 
  int build(); 
  void save(std::string output = ""); 
  void set_float(std::string, double); 
private: 
  void init(std::string input, const unsigned flags = 0); 
  std::map<std::string, double> get_jet_vars(const std::vector<Jet>&); 
  typedef std::vector<std::pair<std::string, unsigned> > CutMasks; 
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

};


#endif 

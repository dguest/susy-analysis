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


class HyperBuilder : public boost::noncopyable
{
public: 
  HyperBuilder(std::string input, const unsigned flags = 0); 
  ~HyperBuilder(); 
  void add_cut_mask(std::string name, unsigned bits); 
  void build(); 
  void save(std::string output = ""); 
private: 
  typedef std::map<std::string, unsigned> CutMasks; 
  const unsigned m_flags; 
  JetFactory* m_factory; 
  CutMasks m_cut_masks; 

  MaskedHistArray* m_hists; 

};


#endif 

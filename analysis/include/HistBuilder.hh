#ifndef HIST_BUILDER_HH
#define HIST_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 
#include <boost/noncopyable.hpp>
#include "typedefs.hh"

class JetFactory; 
class CutAugmenter;
class EventHistograms; 
struct HistConfig; 

class HistBuilder : public boost::noncopyable
{
public: 
  HistBuilder(std::string input, const HistConfig& config, 
	      const unsigned flags = 0); 
  ~HistBuilder(); 
  void add_cut_mask(std::string name, ull_t bits, ull_t antibits = 0); 
  int build(); 
  void save(std::string output = ""); 
private: 
  void set_float(std::string, double);
  typedef std::map<std::string, ull_t> CutMasks; 

  const unsigned m_flags; 
  ull_t m_cutmask; 
  ull_t m_antimask; 
  std::string m_cutmask_name; 

  JetFactory* m_factory; 
  EventHistograms* m_histograms; 
  
  CutMasks m_cut_masks; 

  CutAugmenter* m_cut_augmenter; 
};

#endif // HIST_BUILDER_HH

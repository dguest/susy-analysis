#ifndef HIST_BUILDER_HH
#define HIST_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 
#include <utility>		// pair? 
#include <boost/noncopyable.hpp>
#include "typedefs.hh"

class JetFactory; 
class CutAugmenter;
class EventHistograms; 
struct HistConfig; 
struct RegionConfig; 

class HistBuilder : public boost::noncopyable
{
public: 
  HistBuilder(std::string input, const HistConfig& config, 
	      const unsigned flags = 0); 
  ~HistBuilder(); 
  void add_cut_mask(std::string name, ull_t bits, ull_t antibits = 0); 
  void add_region(const RegionConfig& region); 
  int build(); 
  void save(std::string output = ""); 
private: 
  void set_float(std::string, double);

  const unsigned m_flags; 

  JetFactory* m_factory; 
  std::vector<std::pair<std::string, EventHistograms*> > m_histograms; 

  CutAugmenter* m_cut_augmenter; 
};

#endif // HIST_BUILDER_HH

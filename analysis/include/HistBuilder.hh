#ifndef HIST_BUILDER_HH
#define HIST_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 
#include <utility>		// pair? 
#include <boost/noncopyable.hpp>
#include "typedefs.hh"
#include "systematic_defs.hh"

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
  void add_region(const RegionConfig& region); 
  int build(); 
  void save(std::string output = ""); 
private: 
  void set_float(std::string, double);

  const unsigned m_flags; 
  const syst::Systematic m_systematic; 

  JetFactory* m_factory; 
  std::vector<std::pair<std::string, EventHistograms*> > m_histograms; 

  CutAugmenter* m_cut_augmenter; 
};

#endif // HIST_BUILDER_HH

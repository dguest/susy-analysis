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
class RegionHistograms; 
struct RegionConfig; 

class HistBuilder : public boost::noncopyable
{
public: 
  HistBuilder(std::string input, const unsigned flags = 0); 
  ~HistBuilder(); 
  void add_region(const RegionConfig& region); 
  int build(); 
  void save(); 
private: 
  const std::string m_input_file; 
  const unsigned m_flags; 
  
  std::map<std::string, std::vector<RegionHistograms*> > m_out_file_map; 

  JetFactory* m_factory; 
  std::vector<std::pair<std::string, RegionHistograms*> > m_histograms; 

};

#endif // HIST_BUILDER_HH

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
class IRegionHistograms; 
struct RegionConfig; 

class HistBuilder : public boost::noncopyable
{
public: 
  HistBuilder(std::string input, const unsigned flags = 0); 
  ~HistBuilder(); 
  void add_region(const RegionConfig& region); 
  int build(); 
  void save() const; 
private: 
  const std::string m_input_file; 
  const unsigned m_build_flags; 
  
  std::map<std::string, 
	   std::vector<const IRegionHistograms*> > m_out_file_map; 

  JetFactory* m_factory; 
  std::vector<std::pair<std::string, IRegionHistograms*> > m_histograms; 

};

#endif // HIST_BUILDER_HH

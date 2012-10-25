#ifndef HIST_BUILDER_HH
#define HIST_BUILDER_HH

#include <string> 
#include <map>

class JetFactory; 
class Histogram; 

class HistBuilder
{
public: 
  HistBuilder(std::string input); 
  ~HistBuilder(); 
  void add_cut_mask(std::string name, unsigned bits); 
  void build(); 
  void save(std::string output = ""); 
private: 
  void make_histograms(); 
  typedef std::map<std::string, unsigned> CutMasks; 
  typedef std::map<std::string, unsigned> Histograms; 
  JetFactory* m_factory; 
  CutMasks m_cut_masks; 
  Histograms m_histograms; 
};

#endif // HIST_BUILDER_HH

#ifndef HIST_BUILDER_HH
#define HIST_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 

class JetFactory; 
class Histogram; 
class Axis; 

class HistBuilder
{
public: 
  HistBuilder(std::string input); 
  ~HistBuilder(); 
  void add_cut_mask(std::string name, unsigned bits); 
  void build(); 
  void save(std::string output = ""); 
private: 
  void book_ptmet_histograms(); 
  void book_tobster_histograms(); 
  void book_one_var_hist(Axis); 
  void fill(std::string name, double); 
  typedef std::map<std::string, unsigned> CutMasks; 
  typedef std::map<std::string, Histogram> HistByCut; 
  JetFactory* m_factory; 
  CutMasks m_cut_masks; 
  HistByCut m_histograms; 
};

#endif // HIST_BUILDER_HH

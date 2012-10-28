#ifndef HIST_BUILDER_HH
#define HIST_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 
#include <boost/noncopyable.hpp>

class JetFactory; 
class Histogram; 
class MaskedHistArray; 
class Axis; 
class Jet1DHists; 

const double MeV = 1.0; 
const double GeV = 1e3; 

class HistBuilder : public boost::noncopyable
{
public: 
  HistBuilder(std::string input); 
  ~HistBuilder(); 
  void add_cut_mask(std::string name, unsigned bits); 
  void build(); 
  void save(std::string output = ""); 
private: 
  HistBuilder(const HistBuilder&); 
  HistBuilder& operator=(const HistBuilder&); 
  void book_ptmet_histograms(); 
  void book_tobster_histograms(); 
  void book_hist(std::string name, int bins, 
		 double low, double high); 

  void fill(std::string name, double); 
  typedef std::map<std::string, unsigned> CutMasks; 
  typedef std::map<std::string, Histogram> HistByCut; 
  JetFactory* m_factory; 
  
  CutMasks m_cut_masks; 
  HistByCut m_histograms; 
  HistByCut m_h1; 

  Jet1DHists* m_jet1_hists; 
  Jet1DHists* m_jet2_hists; 
  Jet1DHists* m_jet3_hists; 

};

#endif // HIST_BUILDER_HH

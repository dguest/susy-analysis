#ifndef SKIM_COUNTS_HH
#define SKIM_COUNTS_HH

class TFile; 
namespace H5 { 
  class CommonFG; 
}


class SkimCounts { 
public: 
  SkimCounts(TFile& file); 
  void write_to(H5::CommonFG&) const; 
private: 
  long long m_total_events; 
  double m_total_event_weight; 
  bool m_has_total_event_weight; 
}; 


#endif 

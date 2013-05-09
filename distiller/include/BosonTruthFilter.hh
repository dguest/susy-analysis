#ifndef BOSON_TRUTH_FILTER_HH
#define BOSON_TRUTH_FILTER_HH

class SusyBuffer; 

class BosonTruthFilter 
{
public: 
  BosonTruthFilter(double threshold_mev); 
  bool is_over_threshold(const SusyBuffer* ); 
  int get_n_lt_2_lepton_events() const; 
private: 
  double m_threshold_mev; 
  int m_lt_2_lepton_events; 
  int m_2_lepton_events; 
}; 

#endif 

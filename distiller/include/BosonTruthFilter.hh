#ifndef BOSON_TRUTH_FILTER_HH
#define BOSON_TRUTH_FILTER_HH

class SusyBuffer; 

class BosonTruthFilter 
{
public: 
  BosonTruthFilter(double threshold_mev); 
  bool is_over_threshold(const SusyBuffer* ); 
private: 
  double m_threshold_mev; 
}; 

#endif 

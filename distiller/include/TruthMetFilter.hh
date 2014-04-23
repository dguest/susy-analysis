#ifndef TRUTH_MET_FILTER_HH
#define TRUTH_MET_FILTER_HH

class SusyBuffer;

class TruthMetFilter
{
public:
  TruthMetFilter(double max_met_mev);
  bool is_over_threshold(const SusyBuffer* buffer);
private:
  double m_threshold;
};

#endif

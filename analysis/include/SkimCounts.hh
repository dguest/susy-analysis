#ifndef SKIM_COUNTS_HH
#define SKIM_COUNTS_HH

class TFile;
namespace H5 {
  class H5Location;
}


class SkimCounts {
public:
  SkimCounts(TFile& file);
  void write_to(H5::H5Location&) const;
private:
  long long m_total_events;
  long long m_total_ct;
  double m_total_event_weight;
  bool m_has_total_event_weight;
};


#endif

#ifndef REGION_BOSON_PT_HISTOGRAMS_HH
#define REGION_BOSON_PT_HISTOGRAMS_HH

struct EventObjects;
struct RegionConfig;
namespace H5 {
  class CommonFG;
}
class Histogram;
class RegionEventFilter;

#include "IRegionHistograms.hh"


class RegionBosonPtHistograms: public IRegionHistograms
{
public:
  RegionBosonPtHistograms(const RegionConfig&, unsigned);
  RegionBosonPtHistograms(const RegionBosonPtHistograms&) = delete;
  RegionBosonPtHistograms& operator=(const RegionBosonPtHistograms&) = delete;
  virtual ~RegionBosonPtHistograms();
  virtual void fill(const EventObjects& objects);
  virtual void write_to(H5::CommonFG&) const;
private:
  const RegionConfig* m_region_config;
  const RegionEventFilter* m_event_filter;
  const unsigned m_build_flags;

  Histogram* m_boson_pt;
  Histogram* m_boson_truth_pt;
  Histogram* m_reco_vs_truth_pt;
};

#endif

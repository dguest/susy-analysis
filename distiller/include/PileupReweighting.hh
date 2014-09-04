#ifndef PILEUPREWEIGHTING_HH
#define PILEUPREWEIGHTING_HH

class SusyBuffer;
class RunInfo;
class ScaleFactor;
namespace Root {
  class TPileupReweighting;
}

// mu offset used to calculate pileup error
// from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/InDetTrackingPerformanceGuidelines#Pile_up_rescaling
static const float PU_MU_BASE = 1.09;
static const float PU_MU_ERR = 0.04;

#include <string>

class PileupReweighting
{
public:
  PileupReweighting(const RunInfo&, unsigned run_flags);
  PileupReweighting(PileupReweighting&) = delete;
  PileupReweighting& operator=(PileupReweighting&) = delete;
  ~PileupReweighting();
  ScaleFactor get_pileup_weight(const SusyBuffer&);
  unsigned random_run_number(const SusyBuffer&) const;
  void write_to(const std::string& file_name);
private:
  Root::TPileupReweighting* m_prw;
  Root::TPileupReweighting* m_prw_down;
  Root::TPileupReweighting* m_prw_up;
  bool m_generate;
};


#endif

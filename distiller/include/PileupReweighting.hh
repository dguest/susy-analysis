#ifndef PILEUPREWEIGHTING_HH
#define PILEUPREWEIGHTING_HH

class SusyBuffer;
class RunInfo;
class ScaleFactor;
namespace Root {
  class TPileupReweighting;
}

// mu offset used to calculate pileup error
static const float PU_MU_ERR = 0.1;

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
  bool m_generate;
};


#endif

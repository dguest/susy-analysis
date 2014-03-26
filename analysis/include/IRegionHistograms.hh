#ifndef I_REGION_HISTOGRAMS_HH
#define I_REGION_HISTOGRAMS_HH

struct EventObjects;
namespace H5 {
  class CommonFG;
}


class IRegionHistograms
{
public:
  virtual void fill(const EventObjects& objects) = 0;
  virtual void write_to(H5::CommonFG&) const = 0;
  virtual ~IRegionHistograms(){};
};

#endif

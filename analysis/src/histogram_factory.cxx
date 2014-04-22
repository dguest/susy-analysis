#include "histogram_factory.hh"

#include "RegionConfig.hh"
#include "IRegionHistograms.hh"
#include "NMinus1Histograms.hh"

#include <stdexcept>

IRegionHistograms* histogram_factory(const RegionConfig& region,
				     const unsigned build_flags){
  using namespace reg;
  switch (region.hists) {
  case NMINUS:
    return new NMinus1Histograms(region, build_flags);
  default:
    throw std::logic_error("unknown hist type for region " + region.name);
  }
}

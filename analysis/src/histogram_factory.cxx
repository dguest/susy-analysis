#include "histogram_factory.hh"

#include "RegionConfig.hh"
#include "IRegionHistograms.hh"
#include "RegionHistograms.hh"
#include "Region2dKinematicHistograms.hh"
#include "RegionJetEfficiencyHistograms.hh"
#include "RegionBosonPtHistograms.hh"
#include "NMinus1Histograms.hh"

#include <stdexcept>

IRegionHistograms* histogram_factory(const RegionConfig& region, 
				     const unsigned build_flags){ 
  using namespace reg; 
  switch (region.hists) { 
  case HISTMILL: return new RegionHistograms(region, build_flags);
  case KINEMATIC_STAT: 
    return new Region2dKinematicHistograms(region, build_flags); 
  case TAG_EFFICIENCY:
    return new RegionJetEfficiencyHistograms(region, build_flags);
  case BOSON_PT: 
    return new RegionBosonPtHistograms(region, build_flags); 
  case NMINUS: 
    return new NMinus1Histograms(region, build_flags);
  default: 
    throw std::logic_error("unknown hist type for region " + region.name); 
  }
}

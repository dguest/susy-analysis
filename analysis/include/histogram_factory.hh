#ifndef HISTOGRAM_FACTORY_HH
#define HISTOGRAM_FACTORY_HH

class IRegionHistograms;
class RegionConfig;

IRegionHistograms* histogram_factory(const RegionConfig&,
				     const unsigned build_flags);

#endif

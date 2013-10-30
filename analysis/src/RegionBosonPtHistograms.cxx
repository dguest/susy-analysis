#include "RegionBosonPtHistograms.hh"
#include "EventObjects.hh"
#include "RegionConfig.hh"
#include "H5Cpp.h"

RegionBosonPtHistograms::RegionBosonPtHistograms(
  const RegionConfig& objects, unsigned) { }
RegionBosonPtHistograms::~RegionBosonPtHistograms() {}
void RegionBosonPtHistograms::fill(const EventObjects&) {}
void RegionBosonPtHistograms::write_to(H5::CommonFG&) const {}




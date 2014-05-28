#include "region_logic.hh"
#include <set>

namespace {
  std::set<reg::Selection> onelep_regions {
    reg::Selection::CR_W, reg::Selection::CR_1E, reg::Selection::CR_1M};
  std::set<reg::Selection> dilep_regions {
    reg::Selection::CR_Z, reg::Selection::CR_Z_1L, reg::Selection::CR_Z_2L,
      reg::Selection::CR_T,
      reg::Selection::QUALITY_EVENT};
}

namespace reg {
  bool onelep_region(Selection sel) {
    return onelep_regions.count(sel);
  }
  bool twolep_region(Selection sel) {
    return dilep_regions.count(sel);
  }
  bool lepton_region(Selection sel) {
    return onelep_region(sel) || twolep_region(sel);
  }

}

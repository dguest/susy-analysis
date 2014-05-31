#include "nminus_tools.hh"
#include "RegionConfig.hh"

#include "SignalSelection.hh"
#include "CR1LSelection.hh"
#include "CR1ESelection.hh"
#include "CR1MSelection.hh"
#include "OSDFSelection.hh"
#include "OSSFSelection.hh"
#include "CRZSelection.hh"
#include "QualityEventSelection.hh"
#include "MetSelection.hh"

#include <stdexcept>

namespace nminus {
  ISelection* selection_factory(const RegionConfig& cfg) {
    switch (cfg.selection) {
    case reg::Selection::SIGNAL: return new NMinusSignalSelection(cfg);
    case reg::Selection::CR_W: return new NMinusCR1LSelection(cfg);
    case reg::Selection::CR_1E: return new NMinusCR1ESelection(cfg);
    case reg::Selection::CR_1M: return new NMinusCR1MSelection(cfg);
    case reg::Selection::CR_Z: return new NMinusCRZSelection(cfg);
    case reg::Selection::CR_Z_2L: return new NMinusZ2LSelection(cfg);
    case reg::Selection::CR_Z_1L: return new NMinusZ1LSelection(cfg);
    case reg::Selection::CR_T: return new NMinusOSDFSelection(cfg);
    case reg::Selection::QUALITY_EVENT: return new QualityEventSelection(cfg);
    case reg::Selection::VR_MET: return new MetSelection(cfg);

    case reg::Selection::ERROR: throw std::invalid_argument(
      "region selection is ERROR");
    default: throw std::invalid_argument("unknown selection in " __FILE__);
    }
  }
}

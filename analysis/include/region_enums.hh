#ifndef REGION_ENUMS
#define REGION_ENUMS

namespace reg {
  enum Type { CONTROL, SIGNAL, VALIDATION };
  enum Hists {
    HISTMILL, KINEMATIC_STAT, TAG_EFFICIENCY, BOSON_PT, NMINUS, EVENT_LIST};
  enum BosonPtCorrection {NO_PT_CORRECTION, MARKS};
  enum class Selection {
    SIGNAL, CR_Z_AL, CR_Z_2L, CR_Z_1L, CR_1E, CR_1M, CR_W, CR_T,
      ERROR, QUALITY_EVENT, VR_MET, VR_MCT, VR_MCC};
  // streams used to prevent double-counting
  enum class Stream { ELECTRON, MUON, JET, SIMULATED, ERROR};
}

#endif

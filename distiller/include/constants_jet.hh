#ifndef JET_CONSTANTS_HH
#define JET_CONSTANTS_HH

#include "constants.hh"

namespace jet {
  static const float SUSYTOOLS_PT_CUT = 20*GeV;
  static const float SUSYTOOLS_ETA_CUT = 10;
  static const float PRESELECTION_PT = 20*GeV;
  static const float PRESELECTION_ETA = 2.8;
  static const float SIGNAL_PT_CUT = 20*GeV;
  static const float SIGNAL_ETA_CUT = 2.8;

  // jvf cuts: require a jvf above these values unless one of the below
  // "ignore" thresholds is met. These are used in the signal jet selection.
  static const float JVF_CUT = 0.5;
  static const float PT_IGNORE_JVF = 50*GeV;
  static const float ETA_IGNORE_JFV = 2.4;

  static const float TAGGING_ETA_LIM = 2.5;

  // bad tile veto parameters (jets must meet all to count as bad)
  static const float BAD_TILE_JET_PT_MIN = 40*GeV;
  static const float BAD_TILE_BCH_CORR_MIN = 0.05;
  static const float BAD_TILE_MET_DPHI_MAX = 0.3;

  // charge fraction (jet must meet all to count as bad)
  static const float FAIL_CHF_PT_MIN = 100*GeV;
  static const float FAIL_CHF_ETA_MAX = 2.0;
  static const float FAIL_CHF_MAX_NOEM = 0.02;
  // second category of chf max when emfrac is high
  static const float FAIL_CHF_MAX_EM = 0.05;
  static const float FAIL_CHF_EM_FRAC_MIN = 0.9;

  // static const float UNCALIBRATED_PT_MIN = 2.0*GeV;
}
#endif

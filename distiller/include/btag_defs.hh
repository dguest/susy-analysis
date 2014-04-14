#ifndef BTAG_DEFS_HH
#define BTAG_DEFS_HH
namespace btag {
  enum Uncertainty { None, Total, Statistical, Systematic };
  enum Tagger { CNN, JFC };
  enum OperatingPoint { CNN_LOOSE, CNN_MEDIUM, CNN_TIGHT,
			JFC_LOOSE, JFC_MEDIUM, JFC_TIGHT };
  enum Flavor { B, C, U, T, DATA};
  const float TAG_ETA_MAX = 2.5;
}


#endif

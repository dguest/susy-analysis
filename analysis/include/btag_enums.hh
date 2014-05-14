#ifndef BTAG_ENUMS_HH
#define BTAG_ENUMS_HH
namespace btag {
  enum class Tagger {NONE, JFC, JFC_LEADING_JET, ERROR};

  // ordering is important here: the UnorderedJetTagFilter
  // will apply the lower tags first, so tight should be lower
  enum OperatingPoint {
    NOTAG,
    TIGHT,
    MEDIUM,
    LOOSE,
    ANTILOOSE,
    JFC_TIGHT,
    JFC_MEDIUM,
    JFC_LOOSE,
    JFC_ANTILOOSE};
}

#endif

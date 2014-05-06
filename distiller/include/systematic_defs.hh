#ifndef SYSTEMATICS_HH
#define SYSTEMATICS_HH
namespace systematic {
  enum Systematic {
    NONE,
    JESUP, JESDOWN, JER,
    METUP, METDOWN, METRES,
    // electron systematics
    EGZEEUP, EGZEEDOWN, EGMATUP, EGMATDOWN, EGPSUP, EGPSDOWN,
    EGLOWUP, EGLOWDOWN, EGRESUP, EGRESDOWN,
    // muon systematics
    MMSDOWN, MMSUP, MIDDOWN, MIDUP, MSCALEDOWN, MSCALEUP
  };
}
#endif

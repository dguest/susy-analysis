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
    MMSDOWN, MMSUP, MIDDOWN, MIDUP, MSCALEDOWN, MSCALEUP,

    // === JES breakdown ===
    // Effective NP
    JENP1DOWN,JENP1UP,
    JENP2DOWN,JENP2UP,
    JENP3DOWN,JENP3UP,
    JENP4DOWN,JENP4UP,
    JENP5DOWN,JENP5UP,
    JENP6DOWN,JENP6UP,
    // EtaIntercalibration (modelling, stat and method)
    JICALMDOWN,JICALMUP,
    JICALSDOWN,JICALSUP,
    // Single particle high pt
    JSPDOWN,JSPUP,
    // Relative non-closure, pythia 8
    JNCDOWN,JNCUP,
    // Pileup offset mu
    JPUMUDOWN,JPUMUUP,
    // Pileup offset NPV
    JPUNPVDOWN,JPUNPVUP,
    // Pileup offset pt term
    JPUPTDOWN,JPUPTUP,
    // Pileup offset rho topology
    JPURHODOWN,JPURHOUP,
    // Close by term
    JCBDOWN,JCBUP,
    // Flavor composition uncertainty
    JFLAVCOMPDOWN,JFLAVCOMPUP,
    // Flavor response uncertainty
    JFLAVRESPDOWN,JFLAVRESPUP,
    // BJes
    JBJESDOWN,JBJESUP,


  };
}
#endif

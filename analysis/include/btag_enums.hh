#ifndef BTAG_ENUMS_HH
#define BTAG_ENUMS_HH
namespace btag { 
  enum Tagger {CNN, JFC}; 
  enum OperatingPoint {
    NOTAG, LOOSE, MEDIUM, TIGHT, ANTILOOSE, 
    JFC_LOOSE, JFC_MEDIUM, JFC_TIGHT, JFC_ANTILOOSE}; 
}

#endif

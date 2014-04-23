#include "BtagConfig.hh"
#include "distiller/JetBits.hh"
#include <stdexcept>

namespace btag {
  unsigned required_from_tag(OperatingPoint tag)
  {
    using namespace jetbit;
    switch (tag){
    case LOOSE:  return cnn_loose_anti_u  | cnn_anti_b;
    case MEDIUM: return cnn_medium_anti_u | cnn_anti_b;
    case TIGHT:  return cnn_tight_anti_u  | cnn_anti_b;
    case ANTILOOSE: return 0;
    case JFC_LOOSE:  return jfc_loose_anti_u  | jfc_anti_b;
    case JFC_MEDIUM: return jfc_medium_anti_u | jfc_anti_b;
    case JFC_TIGHT:  return jfc_tight_anti_u  | jfc_anti_b;
    case JFC_ANTILOOSE: return 0;
    case NOTAG: return 0;
    default: throw std::logic_error("just... fuck");
    }
  }
  unsigned veto_from_tag(OperatingPoint tag)
  {
    using namespace jetbit;
    switch (tag){
    case LOOSE:  return 0;
    case MEDIUM: return 0;
    case TIGHT:  return 0;
    case ANTILOOSE: return cnn_anti_b;
    case JFC_LOOSE:  return 0;
    case JFC_MEDIUM: return 0;
    case JFC_TIGHT:  return 0;
    case JFC_ANTILOOSE: return jfc_anti_b;
    case NOTAG: return 0;
    default: throw std::logic_error("just... fuck");
    }
  }

  std::string joiner(btag::OperatingPoint tag) {
    switch (tag){
    case LOOSE:  return "_cnn_loose_";
    case MEDIUM: return "_cnn_medium_";
    case TIGHT:  return "_cnn_tight_";
    case ANTILOOSE: return joiner(LOOSE);
    case JFC_LOOSE:  return "_jfc_loose_";
    case JFC_MEDIUM: return "_jfc_medium_";
    case JFC_TIGHT:  return "_jfc_tight_";
    case JFC_ANTILOOSE: return joiner(JFC_LOOSE);
    case NOTAG: throw std::logic_error("we don't set no tag (in " __FILE__")");
    default: throw std::logic_error("asked for string for undefined tagger");
    }
  }

  bool is_inverted(btag::OperatingPoint tag) {
    if (tag == ANTILOOSE || tag == JFC_ANTILOOSE) return true;
    else return false;
  }

}

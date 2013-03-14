#include "BtagConfig.hh"
#include "distiller/JetBits.hh"
#include <stdexcept>

namespace btag { 
  unsigned required_from_tag(JetTag tag)
  { 
    using namespace jetbit; 
    switch (tag){ 
    case LOOSE: return cnn_loose_anti_u | cnn_anti_b; 
    case MEDIUM: return cnn_medium_anti_u | cnn_anti_b; 
    case TIGHT: return cnn_tight_anti_u | cnn_anti_b; 
    case ANTILOOSE: return 0; 
    case NOTAG: return 0; 
    default: throw std::logic_error("just... fuck"); 
    }
  }
  unsigned veto_from_tag(JetTag tag)
  { 
    using namespace jetbit; 
    switch (tag){ 
    case LOOSE: return 0; 
    case MEDIUM: return 0; 
    case TIGHT: return 0; 
    case ANTILOOSE: return cnn_anti_b; 
    case NOTAG: return 0; 
    default: throw std::logic_error("just... fuck"); 
    }
  }
}

#ifndef BTAG_CONFIG_HH
#define BTAG_CONFIG_HH
namespace btag { 
  enum EventConfig { NONE, LOOSE_TIGHT, MEDIUM_MEDIUM, MEDIUM_TIGHT}; 
  enum JetTag {NOTAG, LOOSE, MEDIUM, TIGHT, ANTILOOSE}; 
  unsigned required_from_tag(JetTag); 
  unsigned veto_from_tag(JetTag); 
} 
#endif 

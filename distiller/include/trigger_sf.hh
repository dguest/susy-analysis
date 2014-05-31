#ifndef TRIGGER_SF_HH
#define TRIGGER_SF_HH

class SUSYObjDef;
class EventObjects;

struct TrigSFVariation {
  float up;
  float down;
};

struct TriggerSF {
  float nominal;
  TrigSFVariation el;
  TrigSFVariation mu;
  TrigSFVariation lepton;
};

TriggerSF get_lepton_trigger_sf(SUSYObjDef& def, const EventObjects&);


#endif

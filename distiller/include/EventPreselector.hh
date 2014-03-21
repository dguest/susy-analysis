#ifndef EVENT_PRESELECTOR_HH
#define EVENT_PRESELECTOR_HH

#include <string>
#include "typedefs.hh"

class Grl; 
class SusyBuffer; 
class SUSYObjDef; 
class RunInfo;
class PileupReweighting;
class PreselectionInfo;

class EventPreselector
{ 
public: 
  EventPreselector(unsigned flags, const RunInfo& info); 
  ~EventPreselector(); 
  EventPreselector(EventPreselector&) = delete; 
  EventPreselector& operator=(EventPreselector&) = delete; 
  PreselectionInfo get_preselection_info(const SusyBuffer& buffer, 
					 SUSYObjDef& def) const; 
private: 
  ull_t get_presel_bits(const SusyBuffer&, SUSYObjDef&) const;
  const unsigned m_flags; 
  Grl* m_grl; 
  PileupReweighting* m_prw; 
}; 
#endif 

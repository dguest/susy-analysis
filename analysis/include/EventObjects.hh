#ifndef EVENT_OBJECTS_HH
#define EVENT_OBJECTS_HH

// Structure that can be built once per event and passed into all the 
// histograming routines. 
// Constructed in HistBuilder

#include "TVector2.h"
#include "Jet.hh"
#include <vector> 
// #include <unordered_map>
#include "typedefs.hh"
#include "systematic_defs.hh"

class ObjectFactory; 
class EventScalefactors; 

struct MetFlavors
{
  TVector2 bare; 
  TVector2 muon; 
  MetFlavors(const ObjectFactory*, syst::Systematic); 
}; 

// TODO: replace this thing with an unordered_map or some other simple thing
class MetSystematics
{
public: 
  MetSystematics(const ObjectFactory*, bool is_data); 
  ~MetSystematics(); 
  MetSystematics(const MetSystematics&) = delete; 
  MetSystematics& operator=(const MetSystematics&) = delete; 
  const MetFlavors& get_syst(syst::Systematic) const; 
private: 
  const MetFlavors m_nominal; 
  const MetFlavors* m_up; 
  const MetFlavors* m_down; 
  const MetFlavors* m_res; 
};

struct EventObjects
{
  const MetSystematics met; 
  double weight; 
  ull_t event_mask; 
  double htx; 
  int leading_cjet_pos; 
  int subleading_cjet_pos; 
  int n_signal_jets; 
  std::vector<Jet> jets; 
  std::vector<Jet> jets_with_eljet; 
  EventScalefactors* event_scalefactors; 
  float true_boson_pt; 
  float marks_boson_pt_weight; 
  TVector2 boson_child; 
  EventObjects(const ObjectFactory*, unsigned build_flags); 
}; 

#endif 

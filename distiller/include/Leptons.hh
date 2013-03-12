#ifndef LEPTONS_H
#define LEPTONS_H

#include "TLorentzVector.h"

class EventElectrons; 
class EventMuons; 
class SusyBuffer; 
class SUSYObjDef; 
struct RunInfo; 

class Electron: public TLorentzVector 
{ 
public: 
  Electron(const EventElectrons* container, int index); 
  bool pass_susy() const; 
  bool is_signal() const; 
  int index() const; 
private: 
  bool m_pass_susy; 
  bool m_is_signal; 
  int m_index; 
}; 

class EventElectrons: public std::vector<Electron*> 
{ 
public: 
  EventElectrons(const SusyBuffer& buffer, SUSYObjDef& def, 
		 unsigned flags, const RunInfo& info); 
  ~EventElectrons(); 
private: 
  EventElectrons() {}; 
  EventElectrons(const EventElectrons&) {}; 
  const SusyBuffer* m_buffer; 
  SUSYObjDef* m_def; 
  unsigned m_flags; 
  const RunInfo* m_info; 
  friend class Electron; 
}; 

class Muon: public TLorentzVector
{
public: 
  Muon(const EventMuons* container, int index); 
  bool pass_susy() const; 
  int index() const; 
private: 
  bool m_pass_susy; 
  int m_index; 
}; 

class EventMuons: public std::vector<Muon*>
{
public: 
  EventMuons(const SusyBuffer& buffer, SUSYObjDef& def, 
	     unsigned flags, const RunInfo& info); 
  ~EventMuons(); 
private: 
  EventMuons() {}; 
  EventMuons(const EventMuons&) {}; 
  const SusyBuffer* m_buffer; 
  SUSYObjDef* m_def; 
  unsigned m_flags; 
  const RunInfo* m_info; 
  friend class Muon; 
}; 


bool check_if_electron(int iEl,
		       const SusyBuffer& buffer, 
		       SUSYObjDef& def, 
		       const unsigned flags, 
		       const RunInfo& info);

bool el_size_check(const SusyBuffer& buffer); 

bool check_if_muon(int iMu,
		   const SusyBuffer& buffer, 
		   SUSYObjDef& def, 
		   const unsigned flags);


#endif // LEPTONS_H

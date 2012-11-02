#include "Leptons.hh"
#include "SusyBuffer.h"
#include "RunInfo.hh"
#include "CutBits.hh"

#include <cassert>


#include "TLorentzVector.h"
#include "SUSYTools/SUSYObjDef.h"



//______________________________________________________________
// lepton classes

// -- electron --
Electron::Electron(const EventElectrons* container, int index) { 
  const SusyBuffer* buffer = container->m_buffer; 
  
  // calls the susytools electron filler
  m_pass_susy = check_if_electron
    (index , 
     *buffer, 
     *container->m_def, 
     container->m_flags, 
     *container->m_info);

  SUSYObjDef* def = container->m_def; 
  const TLorentzVector& tlv = def->GetElecTLV(index); 
  SetPxPyPzE(tlv.Px(), tlv.Py(), tlv.Pz(), tlv.E()); 
  
}
bool Electron::pass_susy() const { 
  return m_pass_susy; 
}

EventElectrons::EventElectrons(const SusyBuffer& buffer, SUSYObjDef& def, 
			       unsigned flags, const RunInfo& info): 
  m_buffer(&buffer), 
  m_def(&def), 
  m_flags(flags), 
  m_info(&info)
{
  for (int iii = 0; iii < buffer.el_n; iii++) { 
    push_back(new Electron(this, iii)); 
  }
  
}

EventElectrons::~EventElectrons() { 
  for (iterator itr = begin(); itr != end(); itr++) { 
    delete *itr; 
  }
}

// -- muon --

Muon::Muon(const EventMuons* container, int index): 
  m_index(index)
{ 
  const SusyBuffer* buffer = container->m_buffer; 

  // calls the susytools muon filler
  m_pass_susy = check_if_muon(index, 
			      *buffer, 
			      *container->m_def, 
			      container->m_flags); 

  SUSYObjDef* def = container->m_def; 
  const TLorentzVector& tlv = def->GetMuonTLV(index); 
  assert(tlv.Pt() > 0); 
  SetPxPyPzE(tlv.Px(), tlv.Py(), tlv.Pz(), tlv.E()); 
}
bool Muon::pass_susy() const { 
  return m_pass_susy; 
}
int Muon::index() const { 
  return m_index; 
}

EventMuons::EventMuons(const SusyBuffer& buffer, SUSYObjDef& def, 
		       unsigned flags, const RunInfo& info):
  m_buffer(&buffer), 
  m_def(&def), 
  m_flags(flags), 
  m_info(&info)
{
  for (int iii = 0; iii < buffer.mu_staco_n; iii++) { 
    push_back(new Muon(this, iii)); 
  }
}
EventMuons::~EventMuons() { 
  for (iterator itr = begin(); itr != end() ; itr++) { 
    delete *itr; 
  }
}



bool check_if_electron(int iEl,
		       const SusyBuffer& buffer,
		       SUSYObjDef& def,
		       const unsigned flags, 
		       const RunInfo& info){
  
  bool pass_el = def.FillElectron
    (iEl,
     buffer.RunNumber,
     buffer.el_cl_eta                ->at(iEl),
     buffer.el_cl_phi                ->at(iEl),
     buffer.el_cl_E                  ->at(iEl),
     buffer.el_tracketa              ->at(iEl),
     buffer.el_trackphi              ->at(iEl),
     buffer.el_author                ->at(iEl),
     buffer.el_mediumPP              ->at(iEl),
     buffer.el_OQ                    ->at(iEl),
     buffer.el_nPixHits              ->at(iEl),
     buffer.el_nSCTHits              ->at(iEl),
     buffer.el_MET_Egamma10NoTau_wet->at(iEl).at(0), 
     // buffer.el_MET_Egamma10NoTau_wet ->at(iEl),
     flags & cutflag::is_data);

  return pass_el; 
  //info.isAF2
  //NOTE: el_wet set to zero, couldn't match variable in D3PD
}

bool check_if_muon(int iMu,
		   const SusyBuffer& buffer,
		   SUSYObjDef& def,
		   const unsigned flags){

  return def.FillMuon
    (iMu,
     buffer.mu_staco_pt                           ->at(iMu),
     buffer.mu_staco_eta                          ->at(iMu),
     buffer.mu_staco_phi                          ->at(iMu),
     buffer.mu_staco_E                            ->at(iMu),
     buffer.mu_staco_me_qoverp_exPV               ->at(iMu),
     buffer.mu_staco_id_qoverp_exPV               ->at(iMu),
     buffer.mu_staco_me_theta_exPV                ->at(iMu),
     buffer.mu_staco_id_theta_exPV                ->at(iMu),
     buffer.mu_staco_id_theta                     ->at(iMu),
     buffer.mu_staco_charge                       ->at(iMu), 
     buffer.mu_staco_isCombinedMuon               ->at(iMu),
     buffer.mu_staco_isSegmentTaggedMuon          ->at(iMu),
     buffer.mu_staco_loose                        ->at(iMu),
     buffer.mu_staco_expectBLayerHit              ->at(iMu),
     buffer.mu_staco_nBLHits                      ->at(iMu),
     buffer.mu_staco_nPixHits                     ->at(iMu),
     buffer.mu_staco_nPixelDeadSensors            ->at(iMu),
     buffer.mu_staco_nPixHoles                    ->at(iMu),
     buffer.mu_staco_nSCTHits                     ->at(iMu),
     buffer.mu_staco_nSCTDeadSensors              ->at(iMu),
     buffer.mu_staco_nSCTHoles                    ->at(iMu),
     buffer.mu_staco_nTRTHits                     ->at(iMu),
     buffer.mu_staco_nTRTOutliers                 ->at(iMu),
     flags & cutflag::is_data);


}


#include "Leptons.hh"
#include "SusyBuffer.h"
#include "RunInfo.hh"
#include "RunBits.hh"
#include "RunBits.hh"
#include "constants.hh"
#include "LeptonConstants.hh"
#include "CheckSize.hh"

#include <cassert>
#include <stdexcept>

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
  
  m_is_signal = def->IsSignalElectron
    (index, 
     buffer->el_tightPP  ->at(index), 
     buffer->el_ptcone20 ->at(index),
     buffer->el_trackd0pv->at(index), 
     buffer->el_trackz0pv->at(index), 
     SIGNAL_ELECTRON_ET_CUT, 
     SIGNAL_ELECTRON_ISO_CUT, 
     SIGNAL_ELECTRON_D0_CUT, 
     SIGNAL_ELECTRON_Z0_CUT); 

}
bool Electron::pass_susy() const { 
  return m_pass_susy; 
}
bool Electron::is_signal() const { 
  return m_is_signal; 
}

EventElectrons::EventElectrons(const SusyBuffer& buffer, SUSYObjDef& def, 
			       unsigned flags, const RunInfo& info): 
  m_buffer(&buffer), 
  m_def(&def), 
  m_flags(flags), 
  m_info(&info)
{
  assert(el_size_check(buffer)); 
  try { 
    for (int iii = 0; iii < buffer.el_n; iii++) { 
      push_back(new Electron(this, iii)); 
    }
  }
  catch (std::out_of_range& e) { 
    throw std::out_of_range("out of range in electron constructor"); 
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
  try { 
    for (int iii = 0; iii < buffer.mu_staco_n; iii++) { 
      push_back(new Muon(this, iii)); 
    }
  }
  catch (std::out_of_range& e) { 
    throw std::out_of_range("out of range in muon constructor"); 
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

  if (buffer.el_MET_Egamma10NoTau_wet->at(iEl).size() == 0) { 
    CHECK::throw_size("wet branch entry %i is empty, need %i entry", iEl, 1); 
  }

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
     flags & cutflag::is_data, 
     ELECTRON_ET_CUT,			// et cut
     ELECTRON_ETA_CUT
     );

  return pass_el; 
  //info.isAF2
  //NOTE: el_wet set to zero, couldn't match variable in D3PD
}

bool el_size_check(const SusyBuffer& buffer) { 
  CHECK_SIZE(buffer.el_cl_eta    , buffer.el_n);
  CHECK_SIZE(buffer.el_cl_phi    , buffer.el_n);
  CHECK_SIZE(buffer.el_cl_E      , buffer.el_n);
  CHECK_SIZE(buffer.el_tracketa  , buffer.el_n);
  CHECK_SIZE(buffer.el_trackphi  , buffer.el_n);
  CHECK_SIZE(buffer.el_author    , buffer.el_n);
  CHECK_SIZE(buffer.el_mediumPP  , buffer.el_n);
  CHECK_SIZE(buffer.el_OQ        , buffer.el_n);
  CHECK_SIZE(buffer.el_nPixHits  , buffer.el_n);
  CHECK_SIZE(buffer.el_nSCTHits  , buffer.el_n);
  CHECK_SIZE(buffer.el_MET_Egamma10NoTau_wet, buffer.el_n); 
  CHECK_SIZE(buffer.el_tightPP   , buffer.el_n);
  CHECK_SIZE(buffer.el_ptcone20  , buffer.el_n);
  CHECK_SIZE(buffer.el_trackd0pv , buffer.el_n); 
  CHECK_SIZE(buffer.el_trackz0pv , buffer.el_n); 
  return true; 
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
     flags & cutflag::is_data, 
     MUON_PT_CUT, 
     MUON_ETA_CUT);


}


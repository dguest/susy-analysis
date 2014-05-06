#include "Leptons.hh"
#include "SusyBuffer.h"
#include "RunInfo.hh"
#include "cutflag.hh"
#include "constants.hh"
#include "constants_lepton.hh"
#include "CheckSize.hh"

#include <cassert>
#include <stdexcept>

#include "TLorentzVector.h"
#include "SUSYTools/SUSYObjDef.h"
#include "egammaAnalysisUtils/egammaTriggerMatching.h"

// forward declare of smearing translator
SystErr::Syste get_susytools_syste(systematic::Systematic);

//______________________________________________________________
// lepton classes

// -- electron --
Electron::Electron(const EventElectrons* container, int index):
  m_trigger(false), m_dilep_trigger(false) {
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

  m_tight_pp = buffer->el_tightPP->at(index);

  IsSignalElectronExpCutArgs signal_el_exp_args;
  signal_el_exp_args.etcut(ELECTRON_ET_CUT); // 'baseline' pt cut
  m_is_signal = def->IsSignalElectronExp
    (index,
     m_tight_pp,
     buffer->vx_nTracks,
     buffer->el_ptcone30 ->at(index),
     buffer->el_topoEtcone30_corrected->at(index),
     buffer->el_trackIPEstimate_d0_unbiasedpvunbiased->at(index),
     buffer->el_trackIPEstimate_z0_unbiasedpvunbiased->at(index),
     buffer->el_trackIPEstimate_sigd0_unbiasedpvunbiased->at(index),
     SignalIsoExp::TightIso,
     signal_el_exp_args);

  // m_rel_isolation = buffer->el_ptcone20->at(index) / Pt();
  m_charge = buffer->el_charge->at(index);

  if (m_pass_susy) {
    m_id_sf = def->GetSignalElecSF(
      buffer->el_cl_eta->at(index),
      Pt(),
      true,			// recoSF
      false, 			// idSF
      false, 			// triggerSF
      buffer->RunNumber,
      SystErr::NONE);

    // bit of a hack to pull out the uncertainty
    m_id_sf_unct = def->GetSignalElecSF(
      buffer->el_cl_eta->at(index),
      Pt(),
      true,			// recoSF
      false, 			// idSF
      false, 			// triggerSF
      buffer->RunNumber,
      SystErr::EEFFUP) - m_id_sf;
  }

  // trigger matching
  int nothing;
  if (Pt() > SINGLE_EL_TRIGGER_PT_MIN) {
    if (PassedTriggerEF(
    	  Eta(), Phi(),
    	  buffer->trig_EF_el_EF_e24vhi_medium1,
    	  nothing, buffer->trig_EF_el_eta->size(),
    	  buffer->trig_EF_el_eta, buffer->trig_EF_el_phi)) m_trigger = true;
    if (PassedTriggerEF(
    	  Eta(), Phi(),
    	  buffer->trig_EF_el_EF_e60_medium1,
    	  nothing, buffer->trig_EF_el_eta->size(),
    	  buffer->trig_EF_el_eta, buffer->trig_EF_el_phi)) m_trigger = true;
  }
  if (Pt() > DOUBLE_EL_TRIGGER_PT_MIN) {
    if (PassedTriggerEF(
	  Eta(), Phi(),
	  buffer->trig_EF_el_EF_2e12Tvh_loose1,
	  nothing, buffer->trig_EF_el_eta->size(),
	  buffer->trig_EF_el_eta,
	  buffer->trig_EF_el_phi)) m_dilep_trigger = true;
  }
}
bool Electron::pass_susy() const {
  return m_pass_susy;
}
bool Electron::is_signal() const {
  return m_is_signal;
}
// double Electron::rel_isolation() const {
//   return m_rel_isolation;
// }
float Electron::charge() const {
  return m_charge;
}
float Electron::id_sf() const {
  return m_id_sf;
}
float Electron::id_sf_err() const {
  return m_id_sf_unct;
}
bool Electron::tightpp() const {
  return m_tight_pp;
}
bool Electron::trigger() const {
  return m_trigger;
}
bool Electron::dilep_trigger() const {
  return m_dilep_trigger;
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
  m_index(index), m_trigger(false), m_dilep_trigger(false)
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

  IsSignalMuonExpCutArgs muon_exp_args;
  muon_exp_args.ptcut(MUON_PT_CUT); // 'baseline' pt cut

  m_signal = def->IsSignalMuonExp(
    index,
    buffer->vx_nTracks,
    buffer->mu_staco_ptcone30_trkelstyle->at(index),
    buffer->mu_staco_etcone30->at(index),
    buffer->mu_staco_trackIPEstimate_d0_unbiasedpvunbiased->at(index),
    buffer->mu_staco_trackIPEstimate_z0_unbiasedpvunbiased->at(index),
    buffer->mu_staco_trackIPEstimate_sigd0_unbiasedpvunbiased->at(index),
    SignalIsoExp::TightIso,
    muon_exp_args);
  m_charge = buffer->mu_staco_charge->at(index);
  float z0 = buffer->mu_staco_z0_exPV->at(index);
  float d0 = buffer->mu_staco_d0_exPV->at(index);
  m_is_cosmic = def->IsCosmicMuon(z0, d0);

  float qoverp = buffer->mu_staco_qoverp_exPV->at(index);
  float cov_qoverp = buffer->mu_staco_cov_qoverp_exPV->at(index);
  m_is_bad = def->IsBadMuon(qoverp, cov_qoverp);

  if (m_pass_susy) {
    m_id_sf = def->GetSignalMuonSF(index, SystErr::NONE);
    m_id_sf_unct = def->GetSignalMuonSF(index, SystErr::MEFFUP) - m_id_sf;
  }
  int nothing;
  if (Pt() > SINGLE_MU_TRIGGER_PT_MIN) {
    if (def->MuonHasTriggerMatch(
    	  Eta(), Phi(),
    	  buffer->trig_EF_trigmuonef_EF_mu24i_tight,
    	  nothing, nothing,
    	  buffer->trig_EF_trigmuonef_track_CB_eta->size(),
    	  buffer->trig_EF_trigmuonef_track_CB_eta,
    	  buffer->trig_EF_trigmuonef_track_CB_phi,
    	  buffer->trig_EF_trigmuonef_track_CB_hasCB)) m_trigger = true;
    if (def->MuonHasTriggerMatch(
    	  Eta(), Phi(),
    	  buffer->trig_EF_trigmuonef_EF_mu36_tight,
    	  nothing, nothing,
    	  buffer->trig_EF_trigmuonef_track_CB_eta->size(),
    	  buffer->trig_EF_trigmuonef_track_CB_eta,
    	  buffer->trig_EF_trigmuonef_track_CB_phi,
    	  buffer->trig_EF_trigmuonef_track_CB_hasCB)) m_trigger = true;
  }
  if (Pt() > DOUBLE_MU_TRIGGER_PT_MIN) {
    if (def->MuonHasTriggerMatch(
	  Eta(), Phi(),
	  buffer->trig_EF_trigmuonef_EF_mu18_tight_mu8_EFFS,
	  nothing, nothing,
	  buffer->trig_EF_trigmuonef_track_CB_eta->size(),
	  buffer->trig_EF_trigmuonef_track_CB_eta,
	  buffer->trig_EF_trigmuonef_track_CB_phi,
	  buffer->trig_EF_trigmuonef_track_CB_hasCB)) m_dilep_trigger = true;
  }
}
bool Muon::pass_susy() const {
  return m_pass_susy;
}
bool Muon::is_signal() const {
  return m_signal;
}
int Muon::index() const {
  return m_index;
}
float Muon::charge() const {
  return m_charge;
}
float Muon::id_sf() const {
  return m_id_sf;
}
float Muon::id_sf_err() const {
  return m_id_sf_unct;
}
bool Muon::cosmic() const {
  return m_is_cosmic;
}
bool Muon::bad() const {
  return m_is_bad;
}
bool Muon::trigger() const {
  return m_trigger;
}
bool Muon::dilep_trigger() const {
  return m_dilep_trigger;
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
     // buffer.RunNumber,
     buffer.el_eta                   ->at(iEl),
     buffer.el_phi                   ->at(iEl),
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
  CHECK_SIZE(buffer.el_ptcone30  , buffer.el_n);
  // CHECK_SIZE(buffer.el_trackd0pv , buffer.el_n);
  // CHECK_SIZE(buffer.el_trackz0pv , buffer.el_n);
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
     buffer.mu_staco_me_qoverp_exPV               ->at(iMu),
     buffer.mu_staco_id_qoverp_exPV               ->at(iMu),
     buffer.mu_staco_me_theta_exPV                ->at(iMu),
     buffer.mu_staco_id_theta_exPV                ->at(iMu),
     buffer.mu_staco_id_theta                     ->at(iMu),
     buffer.mu_staco_charge                       ->at(iMu),
     buffer.mu_staco_isCombinedMuon               ->at(iMu),
     buffer.mu_staco_isSegmentTaggedMuon          ->at(iMu),
     buffer.mu_staco_loose                        ->at(iMu),
     buffer.mu_staco_nPixHits                     ->at(iMu),
     buffer.mu_staco_nPixelDeadSensors            ->at(iMu),
     buffer.mu_staco_nPixHoles                    ->at(iMu),
     buffer.mu_staco_nSCTHits                     ->at(iMu),
     buffer.mu_staco_nSCTDeadSensors              ->at(iMu),
     buffer.mu_staco_nSCTHoles                    ->at(iMu),
     buffer.mu_staco_nTRTHits                     ->at(iMu),
     buffer.mu_staco_nTRTOutliers                 ->at(iMu),
     MUON_PT_CUT,
     MUON_ETA_CUT);
  // NOTE: no muon systematics can be applied right now

}

// smearing translator
SystErr::Syste get_susytools_syste(systematic::Systematic syst){
  switch (syst) {

#define STANDARD(SYSNAME) \
  case systematic::SYSNAME ## UP: return SystErr::SYSNAME ## UP;	\
  case systematic::SYSNAME ## DOWN: return SystErr::SYSNAME ## DOWN

#define RPLLOWDOWN(SYSNAME) \
  case systematic::SYSNAME ## UP: return SystErr::SYSNAME ## UP;	\
  case systematic::SYSNAME ## DOWN: return SystErr::SYSNAME ## LOW

    STANDARD(EGZEE);
    STANDARD(EGMAT);
    STANDARD(EGPS);
    STANDARD(EGLOW);
    STANDARD(EGRES);
    RPLLOWDOWN(MMS);
    RPLLOWDOWN(MID);
    RPLLOWDOWN(MSCALE);

  default: return SystErr::NONE;

  }
}

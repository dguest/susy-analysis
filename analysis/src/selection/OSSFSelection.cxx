#include "OSSFSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"	// trig namespace

#include <stdexcept>

// _______________________________________________________________________
// 2 lepton trigger version

NMinusZ2LSelection::NMinusZ2LSelection(const RegionConfig& reg):
  m_stream(reg.stream)
{
  trig::throw_if_not_lepstream(m_stream);
}

NMinusZ2LSelection::~NMinusZ2LSelection() {

}

bool NMinusZ2LSelection::pass(const EventObjects& obj) const {
  const EventRecoParameters& reco = obj.reco;

  // check trigger
  if (!trig::pass_two_lepton_trigger(reco, m_stream)) return false;

  if (!reco.pass_ossf) return false;

  // check object counts
  if (reco.n_baseline_electrons + reco.n_baseline_muons != 2) return false;

  return true;
}

// _______________________________________________________________________
// 1 lepton trigger version

NMinusZ1LSelection::NMinusZ1LSelection(const RegionConfig& reg):
  m_stream(reg.stream)
{
  trig::throw_if_not_lepstream(m_stream);
}

NMinusZ1LSelection::~NMinusZ1LSelection() {

}

bool NMinusZ1LSelection::pass(const EventObjects& obj) const {
  const EventRecoParameters& reco = obj.reco;

  // check trigger
  if (!trig::pass_single_lepton_trigger(reco, m_stream)) return false;

  if (!reco.pass_ossf) return false;

  // check object counts
  if (reco.n_baseline_electrons + reco.n_baseline_muons != 2) return false;

  return true;
}

// _______________________________________________________________________
// tight

OSSFSelection::OSSFSelection(const RegionConfig& cfg):
  m_nminus(cfg)
{

}

OSSFSelection::~OSSFSelection() {

}

bool OSSFSelection::pass(const EventObjects& obj) const {

  throw std::logic_error("deprecated in " __FILE__);
  if (!m_nminus.pass(obj)) return false;
  const EventRecoParameters& reco = obj.reco;

  // other parameters
  if (obj.jets.size() > 2) {
    if (obj.jets.at(2).Pt() > SIGNAL_JET_3_MAX_PT) return false;
  }

  using namespace crsf;

  if (reco.min_jetmet_dphi < MIN_DPHI_JET_MET) return false;

  if (reco.mll > M_LL_MAX) return false;
  if (reco.mll < M_LL_MIN) return false;

  if (obj.leptons.at(0).Pt() < LEPTON_PT_MIN) return false;
  if (reco.mcc < M_CC_MIN) return false;


  return true;
}


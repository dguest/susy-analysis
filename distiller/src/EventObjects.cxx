#include "EventObjects.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "CutCounter.hh"
#include "RunInfo.hh"
#include "constants_distiller.hh"
#include "object_selection.hh"
#include "EventPreselector.hh"
#include "trigger_sf.hh"

EventObjects::EventObjects(
  const SusyBuffer& buf, SUSYObjDef& def,
  unsigned flags, const RunInfo& info, const EventPreselector& presel):

  m_all_jets(0),
  m_all_electrons(0),
  m_all_muons(0),

  m_trigger_sf(0)
{
  prec = presel.get_preselection_info(buf, def);

  m_all_jets = new EventJets(buf, def, flags, info);
  m_all_electrons = new EventElectrons(
    buf, def, flags, info, prec.random_run);
  m_all_muons = new EventMuons(buf, def, flags, info);
}

EventObjects::~EventObjects() {
  delete m_all_jets;
  delete m_all_electrons;
  delete m_all_muons;
  delete m_trigger_sf;
  m_all_jets = 0;
  m_all_electrons = 0;
  m_all_muons = 0;
  m_trigger_sf = 0;
}

void EventObjects::do_overlap_removal(CutCounter& ob_counts) {
  std::sort(m_all_jets->begin(),m_all_jets->end(),object::has_higher_pt);

  preselected_jets = object::preselection_jets(*m_all_jets);
  preselected_electrons = object::filter_susy(*m_all_electrons);
  preselected_muons = object::filter_susy(*m_all_muons);

  ob_counts["preselected_jets"] += preselected_jets.size();
  ob_counts["preselected_el"] += preselected_electrons.size();
  ob_counts["preselected_mu"] += preselected_muons.size();

  // need to get susy muon indices before overlap
  susy_muon_idx = object::get_indices(preselected_muons);

  // --- overlap removal ---
  after_overlap_jets = object::remove_overlaping(
    preselected_electrons, preselected_jets, REMOVE_JET_CONE);
  after_overlap_electrons = object::remove_overlaping(
    after_overlap_jets, preselected_electrons, REMOVE_EL_CONE);
  after_overlap_muons = object::remove_overlaping(
    after_overlap_jets, preselected_muons, REMOVE_MU_CONE);

  ob_counts["after_overlap_jets"] += after_overlap_jets.size();
  ob_counts["after_overlap_el"] += after_overlap_electrons.size();
  ob_counts["after_overlap_mu"] += after_overlap_muons.size();

  veto_jets = object::bad_jets(after_overlap_jets);

  ob_counts["veto_jets"] += veto_jets.size();

  good_jets = object::remove_bad_jets(after_overlap_jets);
  ob_counts["good_jets"] += good_jets.size();
  m_signal_jets = object::signal_jets(good_jets);
  control_electrons = object::control_electrons(after_overlap_electrons);
  control_muons = object::control_muons(after_overlap_muons);

  ob_counts["signal_jets"] += m_signal_jets.size();
  ob_counts["control_el"] += control_electrons.size();
  ob_counts["control_mu"] += control_muons.size();

  const int n_leading = std::min(m_signal_jets.size(), N_SR_JETS);
  m_leading_jets.assign(
    m_signal_jets.begin(), m_signal_jets.begin() + n_leading);
}

void EventObjects::make_electron_jet_collection(const BtagCalibration* cal){
  m_signal_jets_eljet = m_signal_jets;
  auto* eljet = electron_jet();
  if (eljet) {
    eljet->set_flavor_tag(cal);
    m_signal_jets_eljet.push_back(eljet);
    std::sort(m_signal_jets_eljet.begin(), m_signal_jets_eljet.end(),
	      object::has_higher_pt);
  }
  const int n_leading = std::min(m_signal_jets_eljet.size(), N_SR_JETS);
  m_leading_jets_eljet.assign(
    m_signal_jets_eljet.begin(), m_signal_jets_eljet.begin() + n_leading);
}

void EventObjects::compute_trigger_sf(SUSYObjDef& def) {
  assert(!m_trigger_sf);
  m_trigger_sf = new TriggerSF(get_lepton_trigger_sf(def, *this));
}

TriggerSF* EventObjects::get_trigger_sf() const {
  return m_trigger_sf;
}

const std::vector<SelectedJet*>&
EventObjects::signal_jets(JetRep jr)
  const {
  if (jr == JetRep::ELJET) return m_signal_jets_eljet;
  return m_signal_jets;
}
const std::vector<SelectedJet*>&
EventObjects::leading_jets(JetRep jr)
  const {
  if (jr == JetRep::ELJET) return m_leading_jets_eljet;
  return m_leading_jets;
}

// _______________________________________________________________________
// private

SelectedJet* EventObjects::electron_jet() const {
  if (control_electrons.size() == 1) {
    auto control_el = control_electrons.at(0);
    return object::get_leptojet(*m_all_jets, *control_el);
  } else {
    return 0;
  }

}

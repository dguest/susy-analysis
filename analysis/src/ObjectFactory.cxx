#include "ObjectFactory.hh"
#include "Jet.hh"
#include "JetBuffer.hh"
#include "Lepton.hh"
#include "LeptonBuffer.hh"
#include "BtagBuffer.hh"
#include "BtagConfig.hh"
#include "EventScalefactors.hh"
#include "EventRecoParameters.hh"
#include "SkimCounts.hh"
#include "common_functions.hh"
#include "StackerExceptions.hh"

#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <algorithm>

#include "TVector2.h"
#include "TFile.h"
#include "TTree.h"

namespace {
  void set_reco_parameters(TTree*, EventRecoParameters&);
}

MetBuffer::MetBuffer(TTree* tree, const std::string& prefix) {
  int errors = 0;
  errors += std::abs(
    tree->SetBranchAddress((prefix + "met").c_str(), &met));
  errors += std::abs(
    tree->SetBranchAddress((prefix + "met_phi").c_str(), &met_phi));
  if (errors) throw std::logic_error("unknown met prefix: " + prefix);
}


ObjectFactory::ObjectFactory(const std::string& root_file, int n_jets,
			     unsigned buildflags) :
  m_tree(0),
  m_file(0),
  m_event_reco_parameters(0),
  m_hfor_type(-1),
  m_leading_cjet_pos(-1),
  m_subleading_cjet_pos(-1),
  m_mc_event_weight(0.0/0.0),
  m_boson_pt_weight(0.0/0.0),
  m_boson_pt(0.0/0.0),
  m_ioflags(0),
  m_evt_sf(0),
  m_skim_counts(0)

{
  m_file = new TFile(root_file.c_str());
  if (!m_file) {
    throw std::runtime_error(root_file + " could not be found");
  }
  m_skim_counts = new SkimCounts(*m_file);
  m_tree = dynamic_cast<TTree*>(m_file->Get("evt_tree"));
  if (!m_tree) {
    throw std::runtime_error("evt_tree could not be found");
  }
  m_met.emplace(syst::NONE, new MetBuffer(m_tree, ""));

  m_event_reco_parameters = new EventRecoParameters;
  set_reco_parameters(m_tree, *m_event_reco_parameters);

  try {
    set_branch(m_tree,"hfor_type", &m_hfor_type);
    set_branch(m_tree,"truth_leading_cjet_pos", &m_leading_cjet_pos);
    set_branch(m_tree,"truth_subleading_cjet_pos", &m_subleading_cjet_pos);
    set_branch(m_tree,"mc_event_weight", &m_mc_event_weight);
    m_evt_sf = new EventScalefactors(m_tree, buildflags);
  } catch (MissingBranch& err) {
    m_ioflags |= ioflag::no_truth;
  }

  for (int ji = 0; ji < n_jets; ji++) {
    std::string base_name = "jet" + std::to_string(ji) + "_";
    m_jet_buffers.push_back(new JetBuffer);
    m_jet_buffers.back()->set_buffer(m_tree, base_name);
  }
  for (auto lepbase: {"first_lepton_", "second_lepton_"}) {
    m_lepton_buffers.push_back(new LeptonBuffer(m_tree, lepbase));
  }
  if (m_tree->GetBranch("truth_boson_pt_weight")) {
    m_tree->SetBranchAddress("truth_boson_pt_weight", &m_boson_pt_weight);
    m_tree->SetBranchAddress("truth_boson_pt", &m_boson_pt);
    m_ioflags |= ioflag::has_boson_pt_weight;
  }
  set_branch(m_tree, "boson_child_pt", &m_boson_child_pt);
  set_branch(m_tree, "boson_child_phi", &m_boson_child_phi);
}

ObjectFactory::~ObjectFactory()
{
  for (auto itr: m_jet_buffers) {
    delete itr;
    itr = 0;
  }
  for (auto itr: m_lepton_buffers) {
    delete itr;
    itr = 0;
  }
  delete m_tree;
  if (m_file) {
    m_file->Close("R");
  }
  delete m_file;
  delete m_event_reco_parameters;
  delete m_evt_sf;
  m_file = 0;
  m_evt_sf = 0;
  for (auto itr: m_met) {
    delete itr.second;
    itr.second = 0;
  }
  delete m_skim_counts;
}

int ObjectFactory::entries() const {
  return m_tree->GetEntries();
}

bool ObjectFactory::is_data() const {
  return ioflag::no_truth & m_ioflags;
}

void ObjectFactory::entry(int n) {
  m_tree->GetEntry(n);
}

std::vector<Jet> ObjectFactory::jets() const {

  std::vector<Jet> jets_out;
  int jrank = 0;
  for (auto jet_buffer: m_jet_buffers) {
    if (!jet_buffer->has_jet()) {
      return jets_out;
    }
    jets_out.push_back(Jet(jet_buffer,m_ioflags));
    Jet& jet = *jets_out.rbegin();
    jet.set_rank(jrank);
    jet.set_event_met(met());
    jrank++;
  }

  return jets_out;
}

std::vector<Lepton> ObjectFactory::leptons() const {
  std::vector<Lepton> leptons;
  for (auto lep_buffer: m_lepton_buffers) {
    if (!lep_buffer->has_lepton()) {
      return leptons;
    }
    leptons.emplace_back(lep_buffer->lepton());
  }
  return leptons;
}

TVector2 ObjectFactory::met(syst::Systematic sy) const  {
  if (!m_met.count(sy)) sy = syst::NONE;
  const MetBuffer& buf = *m_met.at(sy);
  TVector2 met;
  met.SetMagPhi(buf.met, buf.met_phi);
  return met;
}
EventRecoParameters ObjectFactory::event_reco_parameters() const {
  return *m_event_reco_parameters;
}
int ObjectFactory::leading_cjet_pos() const {return m_leading_cjet_pos;}
int ObjectFactory::subleading_cjet_pos() const {return m_subleading_cjet_pos;}
double ObjectFactory::event_weight() const
{
  if (m_ioflags & ioflag::no_truth) {
    return 1.0;
  }
  float base = m_mc_event_weight;
  return base;
}
float ObjectFactory::marks_boson_pt_weight() const {
  if (!(m_ioflags & ioflag::has_boson_pt_weight) ) {
    return 1.0;
  }
  return m_boson_pt_weight;
}
float ObjectFactory::boson_pt() const {
  if (!(m_ioflags & ioflag::has_boson_pt_weight) ) {
    return -1.0;
  }
  return m_boson_pt;
}
TVector2 ObjectFactory::boson_child() const {
  if (m_boson_child_pt < 0.0) {
    return TVector2();
  }
  TVector2 vec;
  vec.SetMagPhi(m_boson_child_pt, m_boson_child_phi);
  return vec;
}

EventScalefactors* ObjectFactory::event_scalefactors() const {
  return m_evt_sf;
}

const SkimCounts* ObjectFactory::skim_counts() const {
  return m_skim_counts;
}

hfor::JetType ObjectFactory::hfor_type() const {
  if (m_ioflags & ioflag::no_truth) {
    throw std::runtime_error("no truth info found, can't get hfor type");
  }
  using namespace hfor;
  switch (m_hfor_type) {
  case -1: return NONE;
  case 0: return BB;
  case 1: return CC;
  case 2: return C;
  case 3: return LIGHT;
  case 4: return KILL;
  default:
    throw std::runtime_error("given undefined hfor type");
  }
}

bool has_higher_pt(const Jet& v1, const Jet& v2) {
  return v1.Pt() > v2.Pt();
}


namespace {
  void set_reco_parameters(TTree* tree, EventRecoParameters& re) {

#define SET_PARAMETER(tree, branch)		\
  set_branch(tree, #branch, &re.branch)

    SET_PARAMETER(tree, pass_met_trigger);
    SET_PARAMETER(tree, pass_single_el_trigger);
    SET_PARAMETER(tree, pass_single_mu_trigger);
    SET_PARAMETER(tree, pass_two_el_trigger);
    SET_PARAMETER(tree, pass_two_mu_trigger);

    SET_PARAMETER(tree, pass_event_quality);

    SET_PARAMETER(tree, pass_osdf);
    SET_PARAMETER(tree, pass_ossf);

    SET_PARAMETER(tree, n_preselected_jets);
    SET_PARAMETER(tree, n_signal_jets);
    SET_PARAMETER(tree, n_baseline_electrons);
    SET_PARAMETER(tree, n_baseline_muons);
    SET_PARAMETER(tree, n_signal_electrons);
    SET_PARAMETER(tree, n_signal_muons);

    SET_PARAMETER(tree, min_jetmet_dphi);
    SET_PARAMETER(tree, lepmet_dphi);
    SET_PARAMETER(tree, met_eff);
    SET_PARAMETER(tree, mct);
    SET_PARAMETER(tree, mct_uncorr);
    SET_PARAMETER(tree, mcc);
    SET_PARAMETER(tree, mt);
    SET_PARAMETER(tree, mll);
    SET_PARAMETER(tree, htx);

    SET_PARAMETER(tree, event_number);

    // SET_PARAMETER(tree, first_lepton_pt);
    // SET_PARAMETER(tree, first_lepton_pdgid);

    // SET_PARAMETER(tree, second_lepton_pt);
    // SET_PARAMETER(tree, second_lepton_pdgid);

#undef SET_PARAMETER

  }
}


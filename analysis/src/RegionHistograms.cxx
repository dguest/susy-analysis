#include "RegionHistograms.hh"
#include "HistBuilderFlags.hh"
#include "ObjKinematics.hh"
#include "TruthJetHists.hh"
#include "PhysicalConstants.hh"
#include "Histogram.hh"
#include "typedefs.hh"
#include "RegionConfig.hh"
#include "JetFactory.hh"
#include "common_functions.hh"
#include "H5Cpp.h"
#include <stdexcept>

#include "TVector2.h"

RegionHistograms::RegionHistograms(const RegionConfig& config, 
				   const unsigned flags) : 
  m_region_config(config), 

  m_leading_cjet_rank(0), 
  m_subleading_cjet_rank(0), 
  m_jet1_truth(0), 
  m_jet2_truth(0), 
  m_jet3_truth(0)
{ 
  const double max_pt = 1e3*GeV; 

  if (config.output_name.size() == 0) { 
    throw std::runtime_error("output histograms not named, quitting"); 
  }

  m_jet1_hists = new Jet1DHists(max_pt, flags); 
  m_jet2_hists = new Jet1DHists(max_pt, flags); 
  m_jet3_hists = new Jet1DHists(max_pt, flags); 

  m_met = new Histogram(100, 0.0, max_pt, "MeV"); 
  m_min_dphi = new Histogram(100, 0.0, 3.2); 
  m_mttop = new Histogram(100, 0.0, max_pt, "MeV"); 
  m_n_good_jets = new Histogram(11, -0.5, 10.5); 

  m_htx = new Histogram(100, 0.0, max_pt, "MeV"); 

  if (flags & buildflag::fill_truth) { 
    m_leading_cjet_rank = new Histogram(6, -0.5, 5.5); 
    m_subleading_cjet_rank = new Histogram(6, -0.5, 5.5); 
    m_jet1_truth = new TruthJetHists(max_pt, flags); 
    m_jet2_truth = new TruthJetHists(max_pt, flags); 
    m_jet3_truth = new TruthJetHists(max_pt, flags); 
  }

}

RegionHistograms::~RegionHistograms() { 
  delete m_jet1_hists; 
  delete m_jet2_hists; 
  delete m_jet3_hists; 

  delete m_met; 
  delete m_min_dphi; 
  delete m_mttop; 
  delete m_n_good_jets; 

  delete m_htx; 

  delete m_leading_cjet_rank; 
  delete m_subleading_cjet_rank; 
  delete m_jet1_truth; 
  delete m_jet2_truth; 
  delete m_jet3_truth; 
}

void RegionHistograms::fill(const JetFactory* factory) { 
  typedef std::vector<Jet> Jets; 

  const ull_t evt_mask = factory->bits(); 
  double weight = factory->event_weight(m_region_config.systematic); 
  if (evt_mask & m_region_config.veto_bits) return; 
  const ull_t req_bits = m_region_config.required_bits; 
  if ( (evt_mask & req_bits) != req_bits){
    return; 
  }

  const Jets jets = factory->jets(); 
  unsigned n_required_jets = m_region_config.jet_tag_requirements.size(); 
  if (jets.size() < n_required_jets) { 
    return; 
  }
  const auto& jet_req = m_region_config.jet_tag_requirements; 
  for (unsigned jet_n = 0; jet_n < jet_req.size(); jet_n++) {
    bool pass = jets.at(jet_n).pass_tag(jet_req.at(jet_n)); 
    if (!pass) return; 
  }
  if (jets.at(0).Pt() < m_region_config.leading_jet_pt) { 
    return; 
  }
  const TVector2 met = factory->met(); 
  if (met.Mod() < m_region_config.met) { 
    return; 
  }


  const TLorentzVector met4(met.Px(), met.Py(), 0, 0); 

  m_met->fill(met.Mod(),  weight); 

  if (jets.size() >= 1) { 
    const Jet& jet = jets.at(0); 
    m_jet1_hists->fill(jet, weight); 
  }
  if (jets.size() >= 2) { 
    const Jet& jet = jets.at(1); 
    m_jet2_hists->fill(jet, weight); 
  }
  if (jets.size() >= 3) { 
    const Jet& jet = jets.at(2); 
    m_jet3_hists->fill(jet,  weight); 
    double mttop = get_mttop(std::vector<Jet>(jets.begin(), 
					      jets.begin() + 3), met); 
    m_mttop->fill(mttop,  weight); 
  }

  double min_jetmet_dphi = 10; 
  for (Jets::const_iterator itr = jets.begin(); itr != jets.end(); itr++) { 
    min_jetmet_dphi = std::min(min_jetmet_dphi, 
			       fabs(met4.DeltaPhi(*itr))); 
  }
  m_min_dphi->fill(min_jetmet_dphi,  weight); 

  m_htx->fill(factory->htx(),  weight); 
    
  m_n_good_jets->fill(factory->n_good(),  weight); 
  if (m_leading_cjet_rank) { 
    m_leading_cjet_rank->fill(factory->leading_cjet_pos(),  weight); 
    m_subleading_cjet_rank->fill(factory->subleading_cjet_pos(), 
				 weight); 

    unsigned n_jets = jets.size(); 
    if (n_jets >= 1) m_jet1_truth->fill(jets.at(0),  weight); 
    if (n_jets >= 2) m_jet2_truth->fill(jets.at(1),  weight); 
    if (n_jets >= 3) m_jet3_truth->fill(jets.at(2),  weight); 

  }

}

void RegionHistograms::write_to(H5::CommonFG& file) const { 
  using namespace H5; 
  Group region(file.createGroup(m_region_config.name)); 
  Group jet1(region.createGroup("jet1")); 
  m_jet1_hists->write_to(jet1); 
  Group jet2(region.createGroup("jet2")); 
  m_jet2_hists->write_to(jet2); 
  Group jet3(region.createGroup("jet3")); 
  m_jet3_hists->write_to(jet3); 

  m_met->write_to(region, "met");
  m_min_dphi->write_to(region, "minDphi"); 
  m_mttop->write_to(region, "mttop"); 
  m_n_good_jets->write_to(region, "nGoodJets"); 
  m_htx->write_to(region, "htx"); 

  if (m_leading_cjet_rank) { 
    Group truth(region.createGroup("truth")); 
    m_leading_cjet_rank->write_to(truth, "leadingCJet"); 
    m_subleading_cjet_rank->write_to(truth, "subleadingCJet"); 

    Group j1_truth(truth.createGroup("jet1")); 
    Group j2_truth(truth.createGroup("jet2")); 
    Group j3_truth(truth.createGroup("jet3")); 
    m_jet1_truth->write_to(j1_truth); 
    m_jet2_truth->write_to(j2_truth); 
    m_jet3_truth->write_to(j3_truth); 
  }

}

void RegionHistograms::write_to(std::string file_name) const { 
  if (file_name.size() == 0) { 
    file_name = m_region_config.output_name; 
  }
  using namespace H5; 
  H5File file(file_name, H5F_ACC_TRUNC); 
  Group region(file.createGroup(m_region_config.name)); 
  write_to(region); 
}

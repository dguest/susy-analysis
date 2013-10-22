#include "RegionHistograms.hh"
#include "HistBuilderFlags.hh"
#include "HistBuilderConstants.hh"
#include "Jet1DHists.hh"
#include "Jet2DHists.hh"
#include "TruthJetHists.hh"
#include "PhysicalConstants.hh"
#include "Histogram.hh"
#include "typedefs.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "common_functions.hh"
#include "H5Cpp.h"
#include <stdexcept>
#include <boost/format.hpp>

#include "TVector2.h"

RegionHistograms::RegionHistograms(const RegionConfig& config, 
				   const unsigned flags) : 
  m_region_config(config), 
  m_event_filter(config), 
  m_build_flags(flags), 

  m_cjet_rank(0), 
  m_jet_scalefactor(0), 
  m_lepton_scalefactor(0)
{ 
  const double max_pt = 1e3*GeV; 
  const size_t n_jets = N_JETS_TO_READ; 

  if (config.output_name.size() == 0) { 
    throw std::runtime_error("output histograms not named, quitting"); 
  }
  if (config.name.size() == 0) { 
    throw std::runtime_error("no name for region, quitting"); 
  }

  m_jet1_no_jet_scalefactor = new Jet1DHists(max_pt, flags, config); 
  for (size_t jet_n = 0; jet_n < n_jets; jet_n++) { 
    m_jet_hists.push_back(new Jet1DHists(max_pt, flags, config)); 
    m_jet_2hists.push_back(new Jet2DHists(flags, config.tagger)); 
  }
  m_met = new Histogram(N_BINS, 0.0, max_pt, "MeV"); 
  m_alt_met = new Histogram(N_BINS, 0.0, max_pt, "MeV"); 
  m_min_dphi = new Histogram(N_BINS, 0.0, 3.2); 
  m_mttop = new Histogram(N_BINS, 0.0, max_pt, "MeV"); 
  m_n_good_jets = new Histogram(11, -0.5, 10.5); 

  m_htx = new Histogram(N_BINS, 0.0, max_pt, "MeV"); 

  if (!(flags & buildflag::is_data)) { 
    add_cjet_rank(); 
    m_jet_scalefactor = new Histogram(N_BINS, 0.0, 2.0); 
    m_lepton_scalefactor = new Histogram(N_BINS, 0.9, 1.1); 
  }

  if (flags & buildflag::fill_truth) { 
    for (size_t jet_n = 0; jet_n < n_jets; jet_n++) { 
      m_jet_truth_hists.push_back
	(new TruthJetHists(max_pt, flags, config)); 
    }
  }

  for (size_t tag_n = 0; tag_n < config.jet_tag_requirements.size(); tag_n++){
    m_tagged_jet_hists.push_back(
      new Jet1DHists(max_pt, flags, config));
    if (! (flags & buildflag::is_data) ) { 
      m_tagged_jet_truth_hists.push_back(
	new TruthJetHists(max_pt, flags, config)); 
    }
  }

}

RegionHistograms::~RegionHistograms() { 
  delete m_jet1_no_jet_scalefactor; 
  for (auto hitr: m_jet_hists) delete hitr; 
  for (auto hitr: m_jet_2hists) delete hitr; 

  delete m_met; 
  delete m_alt_met; 
  delete m_min_dphi; 
  delete m_mttop; 
  delete m_n_good_jets; 

  delete m_htx; 

  delete m_cjet_rank; 
  for (auto hitr: m_jet_truth_hists) delete hitr; 
  delete m_jet_scalefactor; 
  delete m_lepton_scalefactor; 
  for (auto hitr: m_tagged_jet_hists) delete hitr; 
  for (auto hitr: m_tagged_jet_truth_hists) delete hitr; 
}

void RegionHistograms::fill(const EventObjects& obj) { 
  typedef std::vector<Jet> Jets; 

  double base_weight = obj.weight; 
  bool use_electron_jet = m_region_config.region_bits & reg::electron_jet; 
  const Jets jets = use_electron_jet ? obj.jets_with_eljet : obj.jets; 

  if (!m_event_filter.pass(obj)) return; 
  const std::vector<Jet> tagged_jets = m_event_filter.tagged_jets(jets); 
  bool needs_tags = m_region_config.jet_tag_requirements.size() > 0; 
  if (tagged_jets.size() == 0 && needs_tags) return; 

  if (jets.size() > 1){ 
    m_jet1_no_jet_scalefactor->fill(jets.at(1)); 
  }

  if (! (m_build_flags & buildflag::is_data)) { 
    double jet_sf = m_event_filter.jet_scalefactor(tagged_jets); 
    float lepton_sf = m_event_filter.lepton_scalefactor(obj); 
    m_jet_scalefactor->fill(jet_sf, base_weight); 
    m_lepton_scalefactor->fill(lepton_sf, base_weight); 
    base_weight *= jet_sf * lepton_sf; 
  }

  const auto weight = base_weight; 

  // --- filling hists from here (no more weight calc or filtering) ---

  fill_tagged_hists(tagged_jets, weight); 

  const bool do_mu_met = m_region_config.region_bits & reg::mu_met; 
  const MetFlavors& mets = obj.met.get_syst(m_region_config.systematic); 
  const TVector2 met = do_mu_met ? mets.muon: mets.bare; 
  const TVector2 alt_met = do_mu_met ? mets.bare: mets.muon; 
  const TLorentzVector met4(met.Px(), met.Py(), 0, 0); 

  m_met->fill(met.Mod(),  weight); 
  m_alt_met->fill(alt_met.Mod(), weight); 

  unsigned n_jets = std::min(jets.size(), m_jet_hists.size()); 
  for (size_t jet_n = 0; jet_n < n_jets; jet_n++) { 
    const Jet& jet = jets.at(jet_n); 
    m_jet_hists.at(jet_n)->fill(jet, weight); 
    m_jet_2hists.at(jet_n)->fill(jet, weight); 
  }
  if (jets.size() >= 3) { 
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

  m_htx->fill(obj.htx,  weight); 
    
  m_n_good_jets->fill(obj.n_signal_jets,  weight); 
  if (m_cjet_rank) fill_cjet_rank(obj, weight); 

  unsigned n_jets_truth = std::min(jets.size(), m_jet_truth_hists.size()); 
  for (size_t jet_n = 0; jet_n < n_jets_truth; jet_n++) { 
    m_jet_truth_hists.at(jet_n)->fill(jets.at(jet_n),  weight); 
  }

}

void RegionHistograms::write_to(H5::CommonFG& file) const { 
  using namespace H5; 
  Group region(file.createGroup(m_region_config.name)); 
  
  Group jet1_unscaled(region.createGroup("jet1NoJetSf")); 
  m_jet1_no_jet_scalefactor->write_to(jet1_unscaled); 
  for (size_t iii = 0; iii < m_jet_hists.size(); iii++) { 
    std::string jet_name = (boost::format("jet%i") % iii).str(); 
    Group jet(region.createGroup(jet_name)); 
    m_jet_hists.at(iii)->write_to(jet); 
    m_jet_2hists.at(iii)->write_to(jet); 
  }

  m_met->write_to(region, "met");
  m_alt_met->write_to(region, "altMet");
  m_min_dphi->write_to(region, "minDphi"); 
  m_mttop->write_to(region, "mttop"); 
  m_n_good_jets->write_to(region, "nGoodJets"); 
  m_htx->write_to(region, "htx"); 

  if (!(m_build_flags & buildflag::is_data)) { 
    m_cjet_rank->write_to(region, "cjetRank"); 
    m_jet_scalefactor->write_to(region, "jetScalefactor"); 
    m_lepton_scalefactor->write_to(region, "leptonScalefactor"); 
  }

  if (m_jet_truth_hists.size() || m_tagged_jet_truth_hists.size()) { 
    Group truth(region.createGroup("truth")); 
    for (size_t iii = 0; iii < m_jet_truth_hists.size(); iii++) { 
      std::string jet_name = (boost::format("jet%i") % iii).str(); 
      Group jet(truth.createGroup(jet_name)); 
      m_jet_truth_hists.at(iii)->write_to(jet); 
    }
    for (size_t jn = 0; jn < m_tagged_jet_truth_hists.size(); jn++) { 
      std::string jet_name = (boost::format("taggedJet%i") % jn).str(); 
      Group jet(truth.createGroup(jet_name)); 
      m_tagged_jet_truth_hists.at(jn)->write_to(jet); 
    }
  }
  for (size_t jn = 0; jn < m_tagged_jet_hists.size(); jn++) { 
    std::string jet_name = (boost::format("taggedJet%i") % jn).str(); 
    Group jet(region.createGroup(jet_name)); 
    m_tagged_jet_hists.at(jn)->write_to(jet); 
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

void RegionHistograms::add_cjet_rank() { 
  Axis leading; 
  leading.name = "leading"; 
  leading.n_bins = 7; 
  leading.low = -1.5; 
  leading.high = 5.5; 
  Axis subleading; 
  subleading.name = "subleading"; 
  subleading.n_bins = 7; 
  subleading.low = -1.5; 
  subleading.high = 5.5; 
  std::vector<Axis> axes = {leading, subleading}; 
  m_cjet_rank = new Histogram(axes); 
}

void RegionHistograms::fill_cjet_rank(const EventObjects& obj, double w) { 
  std::vector<int> ranks = {
    obj.leading_cjet_pos, obj.subleading_cjet_pos}; 
  std::vector<double> franks(ranks.begin(), ranks.end()); 
  m_cjet_rank->fill(franks, w); 
}

void RegionHistograms::fill_tagged_hists(
  const std::vector<Jet>& jets, double weight) { 
  size_t n_jets = std::min(jets.size(), m_tagged_jet_hists.size()); 
  for (size_t jn = 0; jn < n_jets; jn++) { 
    const Jet& jet = jets.at(jn); 
    m_tagged_jet_hists.at(jn)->fill(jet, weight); 
    if (! (m_build_flags & buildflag::is_data) ) { 
      m_tagged_jet_truth_hists.at(jn)->fill(jet, weight); 
    }
  }
}

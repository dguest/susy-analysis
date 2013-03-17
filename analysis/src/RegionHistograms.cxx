#include "RegionHistograms.hh"
#include "HistBuilderFlags.hh"
#include "ObjKinematics.hh"
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

  m_cjet_rank(0)
{ 
  const double max_pt = 1e3*GeV; 
  const size_t n_jets = 3; 

  if (config.output_name.size() == 0) { 
    throw std::runtime_error("output histograms not named, quitting"); 
  }
  if (config.name.size() == 0) { 
    throw std::runtime_error("no name for region, quitting"); 
  }

  for (size_t jet_n = 0; jet_n < n_jets; jet_n++) { 
    m_jet_hists.push_back(new Jet1DHists(max_pt, flags)); 
  }
  m_met = new Histogram(N_BINS, 0.0, max_pt, "MeV"); 
  m_min_dphi = new Histogram(N_BINS, 0.0, 3.2); 
  m_mttop = new Histogram(N_BINS, 0.0, max_pt, "MeV"); 
  m_n_good_jets = new Histogram(11, -0.5, 10.5); 

  m_htx = new Histogram(N_BINS, 0.0, max_pt, "MeV"); 

  if (!(flags & buildflag::is_data)) { 
    add_cjet_rank(); 
  }

  if (flags & buildflag::fill_truth) { 
    for (size_t jet_n = 0; jet_n < n_jets; jet_n++) { 
      m_jet_truth_hists.push_back(new TruthJetHists(max_pt, flags)); 
    }
  }

}

RegionHistograms::~RegionHistograms() { 

  for (size_t iii = 0; iii < m_jet_hists.size(); iii++) { 
    delete m_jet_hists.at(iii); 
    m_jet_hists.at(iii) = 0; 
  }

  delete m_met; 
  delete m_min_dphi; 
  delete m_mttop; 
  delete m_n_good_jets; 

  delete m_htx; 

  delete m_cjet_rank; 
  for (size_t iii = 0; iii < m_jet_truth_hists.size(); iii++) { 
    delete m_jet_truth_hists.at(iii); 
    m_jet_truth_hists.at(iii) = 0; 
  }
}

void RegionHistograms::fill(const EventObjects& obj) { 
  typedef std::vector<Jet> Jets; 

  double weight = obj.weight; 
  const Jets jets = obj.jets; 

  if (!m_event_filter.pass(obj)) return; 

  if (! (m_build_flags & buildflag::is_data)) { 
    weight *= m_event_filter.jet_scalefactor(obj); 
  }
  const TVector2 met = obj.met; 
  const TLorentzVector met4(met.Px(), met.Py(), 0, 0); 

  m_met->fill(obj.met.Mod(),  weight); 

  unsigned n_jets = std::min(jets.size(), m_jet_hists.size()); 
  for (size_t jet_n = 0; jet_n < n_jets; jet_n++) { 
    const Jet& jet = jets.at(jet_n); 
    m_jet_hists.at(jet_n)->fill(jet, weight); 
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
  if (m_cjet_rank) { 
    std::vector<int> ranks = {
      obj.leading_cjet_pos, obj.subleading_cjet_pos}; 
    std::vector<double> franks(ranks.begin(), ranks.end()); 
    m_cjet_rank->fill(franks,  weight); 
  }
  unsigned n_jets_truth = std::min(jets.size(), m_jet_truth_hists.size()); 
  for (size_t jet_n = 0; jet_n < n_jets_truth; jet_n++) { 
    m_jet_truth_hists.at(jet_n)->fill(jets.at(jet_n),  weight); 
  }

}

void RegionHistograms::write_to(H5::CommonFG& file) const { 
  using namespace H5; 
  Group region(file.createGroup(m_region_config.name)); 
  for (size_t iii = 0; iii < m_jet_hists.size(); iii++) { 
    std::string jet_name = (boost::format("jet%i") % iii).str(); 
    Group jet(region.createGroup(jet_name)); 
    m_jet_hists.at(iii)->write_to(jet); 
  }

  m_met->write_to(region, "met");
  m_min_dphi->write_to(region, "minDphi"); 
  m_mttop->write_to(region, "mttop"); 
  m_n_good_jets->write_to(region, "nGoodJets"); 
  m_htx->write_to(region, "htx"); 

  if (m_cjet_rank) { 
    m_cjet_rank->write_to(region, "cjetRank"); 
  }

  if (m_jet_truth_hists.size()) { 
    Group truth(region.createGroup("truth")); 
    for (size_t iii = 0; iii < m_jet_truth_hists.size(); iii++) { 
      std::string jet_name = (boost::format("jet%i") % iii).str(); 
      Group jet(truth.createGroup(jet_name)); 
      m_jet_truth_hists.at(iii)->write_to(jet); 
    }
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
  leading.n_bins = 6; 
  leading.low = -0.5; 
  leading.high = 5.5; 
  Axis subleading; 
  subleading.name = "subleading"; 
  subleading.n_bins = 6; 
  subleading.low = -0.5; 
  subleading.high = 5.5; 
  std::vector<Axis> axes = {leading, subleading}; 
  m_cjet_rank = new Histogram(6, -0.5, 5.5); 
}

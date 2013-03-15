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
  m_build_flags(flags), 

  m_leading_cjet_rank(0), 
  m_subleading_cjet_rank(0)
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
  m_met = new Histogram(100, 0.0, max_pt, "MeV"); 
  m_min_dphi = new Histogram(100, 0.0, 3.2); 
  m_mttop = new Histogram(100, 0.0, max_pt, "MeV"); 
  m_n_good_jets = new Histogram(11, -0.5, 10.5); 

  m_htx = new Histogram(100, 0.0, max_pt, "MeV"); 

  if (flags & buildflag::fill_truth) { 
    m_leading_cjet_rank = new Histogram(6, -0.5, 5.5); 
    m_subleading_cjet_rank = new Histogram(6, -0.5, 5.5); 

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

  delete m_leading_cjet_rank; 
  delete m_subleading_cjet_rank; 
  for (size_t iii = 0; iii < m_jet_truth_hists.size(); iii++) { 
    delete m_jet_truth_hists.at(iii); 
    m_jet_truth_hists.at(iii) = 0; 
  }
}

void RegionHistograms::fill(const EventObjects& obj) { 
  typedef std::vector<Jet> Jets; 

  double weight = obj.weight; 
  if (obj.event_mask & m_region_config.veto_bits) return; 
  const ull_t req_bits = m_region_config.required_bits; 
  if ( (obj.event_mask & req_bits) != req_bits){
    return; 
  }

  const Jets jets = obj.jets; 
  unsigned n_required_jets = m_region_config.jet_tag_requirements.size(); 
  if (jets.size() < n_required_jets) { 
    return; 
  }
  const auto& jet_req = m_region_config.jet_tag_requirements; 
  for (unsigned jet_n = 0; jet_n < jet_req.size(); jet_n++) {
    const auto jet = jets.at(jet_n); 
    const auto requested_tag = jet_req.at(jet_n); 
    bool pass = jet.pass_tag(requested_tag); 
    if (!pass) return; 
    if (! (m_build_flags & buildflag::is_data)) { 
      weight *= jet.get_scalefactor(requested_tag, 
				    m_region_config.systematic);
    }
  }

  if (jets.at(0).Pt() < m_region_config.leading_jet_pt) { 
    return; 
  }
  const TVector2& met = obj.met; 
  if (met.Mod() < m_region_config.met) { 
    return; 
  }

  const TLorentzVector met4(met.Px(), met.Py(), 0, 0); 

  m_met->fill(met.Mod(),  weight); 

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
  if (m_leading_cjet_rank) { 
    m_leading_cjet_rank->fill(obj.leading_cjet_pos,  weight); 
    m_subleading_cjet_rank->fill(obj.subleading_cjet_pos, weight); 

    unsigned n_jets_truth = std::min(jets.size(), m_jet_truth_hists.size()); 
    for (size_t jet_n = 0; jet_n < n_jets_truth; jet_n++) { 
      m_jet_truth_hists.at(jet_n)->fill(jets.at(jet_n),  weight); 
    }

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

  if (m_leading_cjet_rank) { 
    Group truth(region.createGroup("truth")); 
    m_leading_cjet_rank->write_to(truth, "leadingCJet"); 
    m_subleading_cjet_rank->write_to(truth, "subleadingCJet"); 
    
    for (size_t iii = 0; iii < m_jet_truth_hists.size(); iii++) { 
      std::string jet_name = (boost::format("jet%i") % iii).str(); 
      Group jet(region.createGroup(jet_name)); 
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

#include "HistBuilder.hh" 
#include "HistBuilderFlags.hh"
#include "JetFactory.hh"
#include "Histogram.hh"
#include "ObjKinematics.hh"
#include "MaskedHistArray.hh"
#include "PhysicalConstants.hh"
#include "TruthJetHists.hh"
#include "CutAugmenter.hh"
#include "common_functions.hh"
#include "H5Cpp.h"
#include "BtagScaler.hh"
#include "typedefs.hh"
#include "HistConfig.hh"

#include <string> 
#include <stdexcept>
#include <boost/format.hpp>
#include <algorithm>
#include <iostream> 
#include <ostream>
#include <fstream>

#include "distiller/EventBits.hh"

#include "TVector2.h"

HistBuilder::HistBuilder(std::string input, const HistConfig& config, 
			 const unsigned flags): 
  m_flags(flags), 
  m_leading_cjet_rank(0), 
  m_subleading_cjet_rank(0), 
  m_jet1_truth(0), 
  m_jet2_truth(0), 
  m_jet3_truth(0), 
  m_cut_augmenter(0)
{ 
  const double max_pt = 1e3*GeV; 
  for (auto itr = config.floats.begin(); itr != config.floats.end(); itr++) { 
    set_float(itr->first, itr->second); 
  }
  m_factory = new JetFactory(input); 
  if (! (flags & buildflag::is_data) ) { 
    try { 
      m_factory->set_btagging(config.btag_config); 
    }
    catch (std::runtime_error& err) { 
      std::string app = " file: " + input; 
      throw std::runtime_error(err.what() + app); 
    }
  }

  m_jet1_hists = new Jet1DHists(max_pt, flags); 
  m_jet2_hists = new Jet1DHists(max_pt, flags); 
  m_jet3_hists = new Jet1DHists(max_pt, flags); 

  m_met = new MaskedHistArray(Histogram(100, 0.0, max_pt, "MeV")); 
  m_min_dphi = new MaskedHistArray(Histogram(100, 0.0, M_PI)); 
  m_mttop = new MaskedHistArray(Histogram(100, 0.0, max_pt, "MeV")); 
  m_n_good_jets = new MaskedHistArray(Histogram(11, -0.5, 10.5)); 

  m_htx = new MaskedHistArray(Histogram(100, 0.0, max_pt, "MeV")); 

  if (flags & buildflag::fill_truth) { 
    m_leading_cjet_rank = new MaskedHistArray(Histogram(6, -0.5, 5.5)); 
    m_subleading_cjet_rank = new MaskedHistArray(Histogram(6, -0.5, 5.5)); 
    m_jet1_truth = new TruthJetHists(max_pt, flags); 
    m_jet2_truth = new TruthJetHists(max_pt, flags); 
    m_jet3_truth = new TruthJetHists(max_pt, flags); 
  }

}

HistBuilder::~HistBuilder() { 
  delete m_factory; 
  m_factory = 0; 

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

  delete m_cut_augmenter; 
}

void HistBuilder::add_cut_mask(std::string name, ull_t bits, 
			       ull_t antibits)
{
  CutMasks::const_iterator pos = m_cut_masks.find(name); 
  if (pos != m_cut_masks.end()) { 
    throw std::runtime_error("tried to overwrite cut mask \"" + name + "\""); 
  }
  m_cut_masks[name] = bits; 
  m_jet1_hists->add_mask(bits, name, antibits); 
  m_jet2_hists->add_mask(bits, name, antibits); 
  m_jet3_hists->add_mask(bits, name, antibits); 

  m_met->add_mask(bits, name, antibits); 
  m_min_dphi->add_mask(bits, name, antibits); 
  m_mttop->add_mask(bits, name, antibits); 
  m_n_good_jets->add_mask(bits, name, antibits); 

  m_htx->add_mask(bits, name, antibits); 

  if (m_leading_cjet_rank) { 
    m_leading_cjet_rank->add_mask(bits, name, antibits); 
    m_subleading_cjet_rank->add_mask(bits, name, antibits); 
    m_jet1_truth->add_mask(bits, name, antibits); 
    m_jet2_truth->add_mask(bits, name, antibits); 
    m_jet3_truth->add_mask(bits, name, antibits); 
  }  
}

int HistBuilder::build() { 

  typedef std::vector<Jet> Jets; 
  const int n_entries = m_factory->entries(); 
  const int one_percent = n_entries / 100; 

  std::ofstream nullstream("/dev/null"); 
  std::streambuf* out_buffer = nullstream.rdbuf(); 
  if (m_flags & buildflag::verbose) { 
    out_buffer = std::cout.rdbuf(); 
  }
  std::ostream outstream(out_buffer); 

  for (int entry = 0; entry < n_entries; entry++) { 

    if (one_percent && (entry % one_percent == 0 || entry == n_entries - 1)){ 
      outstream << boost::format("\r%i of %i (%.1f%%) processed") % 
	entry % n_entries % ( entry / one_percent); 
      outstream.flush(); 
    }

    m_factory->entry(entry); 
    
    if (m_flags & buildflag::fill_truth) { 
      if (m_factory->hfor_type() == hfor::KILL) continue; 
    }

    const Jets jets = m_factory->jets(); 
    ull_t mask = m_factory->bits(); 
    const TVector2 met = m_factory->met(); 
    const TLorentzVector met4(met.Px(), met.Py(), 0, 0); 
    double weight = m_factory->event_weight(); 

    if (m_cut_augmenter) { 
      m_cut_augmenter->set_cutmask(mask, jets, met); 
    }

    m_met->fill(met.Mod(), mask, weight); 

    if (jets.size() >= 1) { 
      const Jet& jet = jets.at(0); 
      m_jet1_hists->fill(jet,mask, weight); 
    }
    if (jets.size() >= 2) { 
      const Jet& jet = jets.at(1); 
      m_jet2_hists->fill(jet,mask, weight); 
    }
    if (jets.size() >= 3) { 
      const Jet& jet = jets.at(2); 
      m_jet3_hists->fill(jet, mask, weight); 
      double mttop = get_mttop(std::vector<Jet>(jets.begin(), 
						jets.begin() + 3), met); 
      m_mttop->fill(mttop, mask, weight); 
    }

    double min_jetmet_dphi = 10; 
    for (Jets::const_iterator itr = jets.begin(); itr != jets.end(); itr++) { 
      min_jetmet_dphi = std::min(min_jetmet_dphi, 
				 fabs(met4.DeltaPhi(*itr))); 
    }
    m_min_dphi->fill(min_jetmet_dphi, mask, weight); 

    m_htx->fill(m_factory->htx(), mask, weight); 
    
    m_n_good_jets->fill(m_factory->n_good(), mask, weight); 
    if (m_leading_cjet_rank) { 
      m_leading_cjet_rank->fill(m_factory->leading_cjet_pos(), mask, weight); 
      m_subleading_cjet_rank->fill(m_factory->subleading_cjet_pos(), 
				   mask, weight); 
      fill_truth_hists(jets, mask, weight); 
    }

  }
  outstream << "\n";
  outstream.flush(); 

  return 0; 
}

void HistBuilder::fill_truth_hists(const std::vector<Jet>& jets, 
				   const ull_t mask, double weight) { 
  unsigned n_jets = jets.size(); 
  if (n_jets >= 1) m_jet1_truth->fill(jets.at(0), mask, weight); 
  if (n_jets >= 2) m_jet2_truth->fill(jets.at(1), mask, weight); 
  if (n_jets >= 3) m_jet3_truth->fill(jets.at(2), mask, weight); 
}

void HistBuilder::save(std::string output) { 
  if (output.size() == 0) { 
    return; 
  }
  using namespace H5; 
  H5File file(output, H5F_ACC_TRUNC); 

  Group jet1(file.createGroup("/jet1")); 
  m_jet1_hists->write_to(jet1); 
  Group jet2(file.createGroup("/jet2")); 
  m_jet2_hists->write_to(jet2); 
  Group jet3(file.createGroup("/jet3")); 
  m_jet3_hists->write_to(jet3); 

  Group met(file.createGroup("/met")); 
  m_met->write_to(met);
  Group min_dphi(file.createGroup("/minDphi")); 
  m_min_dphi->write_to(min_dphi); 

  Group mttop(file.createGroup("/mttop")); 
  m_mttop->write_to(mttop); 

  Group nGoodJets(file.createGroup("/nGoodJets")); 
  m_n_good_jets->write_to(nGoodJets); 

  Group htx(file.createGroup("/htx"));
  m_htx->write_to(htx); 

  if (m_leading_cjet_rank) { 
    Group truth(file.createGroup("/truth")); 
    Group leadingCJet(truth.createGroup("leadingCJet")); 
    m_leading_cjet_rank->write_to(leadingCJet); 
    Group subleadingCJet(truth.createGroup("subleadingCJet")); 
    m_subleading_cjet_rank->write_to(subleadingCJet); 

    Group j1_truth(truth.createGroup("jet1")); 
    Group j2_truth(truth.createGroup("jet2")); 
    Group j3_truth(truth.createGroup("jet3")); 
    m_jet1_truth->write_to(j1_truth); 
    m_jet2_truth->write_to(j2_truth); 
    m_jet3_truth->write_to(j3_truth); 
  }

}


void HistBuilder::set_float(std::string name, double value) { 
  if (!m_cut_augmenter) { 
    m_cut_augmenter = new CutAugmenter; 
  }
  m_cut_augmenter->set_float(name, value); 
}



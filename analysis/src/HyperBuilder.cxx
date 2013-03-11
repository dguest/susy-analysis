#include "HyperBuilder.hh"
#include "HistBuilderFlags.hh"

#include "JetFactory.hh"
#include "Histogram.hh"
#include "MaskedHistArray.hh"
#include "PhysicalConstants.hh"
#include "CutAugmenter.hh"
#include "common_functions.hh"
#include "H5Cpp.h"
#include "HistConfig.hh"

#include <string> 
#include <stdexcept>
#include <boost/format.hpp>
#include <algorithm>
#include <iostream> 
#include <ostream>
#include <fstream>
#include <cmath>

#include "TVector2.h"


HyperBuilder::HyperBuilder(std::string input, const HistConfig& config, 
			   const unsigned flags) : 
  m_flags(flags), 
  m_factory(0), 
  m_augmenter(0), 
  m_hists(0), 
  m_input_file(input), 
  m_max_pt(250*GeV), 
  m_min_pt(50*GeV), 
  m_do_cu(true), 
  m_do_cb(true), 
  m_n_met_bins(10), 
  m_n_pt_bins(10), 
  m_min_cu(-3), 
  m_max_cu(7), 
  m_n_cu_bins(10), 
  m_min_cb(-6), 
  m_max_cb(4), 
  m_n_cb_bins(10)
{ 
  for (auto itr = config.floats.begin(); itr != config.floats.end(); itr++) { 
    set_float(itr->first, itr->second); 
  }
}

void HyperBuilder::init(std::string input, const unsigned flags) { 
  const double max_pt = m_max_pt; 
  const double min_pt = m_min_pt;  
  m_factory = new JetFactory(input, 3); 

  std::vector<Axis> axes; 
  Axis leading_pt = {"leadingPt", m_n_pt_bins, min_pt, max_pt}; 
  Axis met = {"met", m_n_met_bins, min_pt, max_pt}; 
  Axis j2Cu = {"j2Cu", m_n_cu_bins, m_min_cu, m_max_cu}; 
  Axis j2Cb = {"j2Cb", m_n_cb_bins, m_min_cb, m_max_cb}; 
  Axis j3Cu = {"j3Cu", m_n_cu_bins, m_min_cu, m_max_cu}; 
  Axis j3Cb = {"j3Cb", m_n_cb_bins, m_min_cb, m_max_cb}; 
  Axis j1Bu = {"j1Bu", 10, -8, 8}; 
  Axis mttop = {"mttop", 2, 0, 600*GeV}; 
  axes.push_back(leading_pt); 
  axes.push_back(met); 
  if (m_flags & buildflag::leading_jet_btag) { 
    axes.push_back(j1Bu); 
  }

  bool do_c_tagging = !(m_flags & buildflag::disable_c_tags); 
  if (m_do_cu && do_c_tagging) { 
    axes.push_back(j2Cu); 
  }
  if (m_do_cb && do_c_tagging) { 
    axes.push_back(j2Cb); 
  }
  if (m_do_cu && do_c_tagging) { 
    axes.push_back(j3Cu); 
  }
  if (m_do_cb && do_c_tagging) { 
    axes.push_back(j3Cb); 
  }
  if (m_flags & buildflag::mttop) { 
    axes.push_back(mttop); 
  }


  Histogram base_hist(axes); 
  m_hists = new MaskedHistArray(base_hist); 

  for (CutMasks::const_iterator itr = m_cut_masks.begin(); 
       itr != m_cut_masks.end(); itr++) { 
    m_hists->add_mask(itr->second, itr->first);     
  }

}

HyperBuilder::~HyperBuilder() { 
  delete m_factory; 
  delete m_hists; 
  delete m_augmenter; 
}

void HyperBuilder::add_cut_mask(std::string name, ull_t bits, ull_t antibits) 
{
  if (antibits) throw std::logic_error("antibits not defined in " __FILE__); 
  m_cut_masks.push_back(std::make_pair(name, bits)); 
}

int HyperBuilder::build() { 

  init(m_input_file, m_flags); 

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
    if (entry % one_percent == 0 || entry == n_entries - 1 ) { 
      outstream << boost::format("\r%i of %i (%.1f%%) processed") % 
	entry % n_entries % ( entry / one_percent); 
      outstream.flush(); 
    }

    m_factory->entry(entry); 

    if (m_factory->hfor_type() == hfor::KILL) continue; 

    const Jets jets = m_factory->jets(); 
    if (jets.size() < 3) continue; 

    TVector2 met = m_factory->met(); 

    ull_t dist_bits = m_factory->bits(); 
    ull_t aug_bits = 0; 
    if (m_augmenter) m_augmenter->set_cutmask(aug_bits, jets, met); 

    const ull_t mask = (dist_bits | aug_bits); 

    std::map<std::string, double> vars = get_jet_vars(jets); 
    vars["met"] = met.Mod(); 

    if (m_flags & buildflag::mttop) { 
      vars["mttop"] = get_mttop(jets, met); 
    }
    
    m_hists->fill(vars, mask); 

  }

  outstream << "\n";
  outstream.flush(); 

  return 0; 
}

std::map<std::string, double> 
HyperBuilder::get_jet_vars(const std::vector<Jet>& jets) const { 

  std::map<std::string, double> vars;

  const Jet& jet1 = jets.at(0); 
  vars["leadingPt"] = jet1.Pt(); 
  vars["j1Bu"] = log(jet1.pb() / jet1.pu()); 

  const Jet& jet2 = jets.at(1); 
  vars["j2Cu"] = log(jet2.pc() / jet2.pu()); 
  vars["j2Cb"] = log(jet2.pc() / jet2.pb()); 

  const Jet& jet3 = jets.at(2); 
  vars["j3Cu"] = log(jet3.pc() / jet3.pu()); 
  vars["j3Cb"] = log(jet3.pc() / jet3.pb()); 

  return vars; 
}

void HyperBuilder::save(std::string output) { 
  if (output.size() == 0) { 
    return; 
  }

  H5::H5File file(output, H5F_ACC_TRUNC); 
  m_hists->write_to(file); 

}

void HyperBuilder::set_float(std::string name, double value) { 

  if (name == "max_pt") {  
    m_max_pt = value; 
    return; 
  }
  else if (name == "min_pt") { 
    m_min_pt = value; 
    return; 
  }
  else if (name == "hard_cu") { 
    set_float("j2_anti_u", value); 
    set_float("j3_anti_u", value); 
    m_do_cu = false; 
  }
  else if (name == "hard_cb") { 
    set_float("j2_anti_b", value); 
    set_float("j3_anti_b", value); 
    m_do_cb = false; 
  }
  else if (name == "n_met_bins") { 
    m_n_met_bins = int_from_double(value); 
  }
  else if (name == "n_pt_bins") { 
    m_n_pt_bins = int_from_double(value); 
  }

  else if (name == "min_cu") { 
    m_min_cu = value; 
  }
  else if (name == "max_cu") { 
    m_max_cu = value; 
  }
  else if (name == "n_cu_bins") { 
    m_n_cu_bins = int_from_double(value); 
  }

  else if (name == "min_cb") { 
    m_min_cb = value; 
  }
  else if (name == "max_cb") { 
    m_max_cb = value; 
  }
  else if (name == "n_cb_bins") { 
    m_n_cb_bins = int_from_double(value); 
  }


  else { 
    if (m_augmenter == 0) { 
      m_augmenter = new CutAugmenter(); 
    }
    m_augmenter->set_float(name, value); 
  }
}

int HyperBuilder::int_from_double(double input) const { 
  int rounded = int(floor(input + 0.5)); 
  if (fabs(rounded - input) > 1e-4) { 
    throw std::runtime_error
      ((boost::format("rounded %f to %f for int conversion") % 
	input % rounded).str()); 
  }
  return rounded; 
}

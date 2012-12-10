#include "HyperBuilder.hh"
#include "HistBuilderFlags.hh"

#include "JetFactory.hh"
#include "Histogram.hh"
#include "MaskedHistArray.hh"
#include "PhysicalConstants.hh"
#include "CutAugmenter.hh"
#include "common_functions.hh"
#include "H5Cpp.h"

#include <string> 
#include <stdexcept>
#include <boost/format.hpp>
#include <algorithm>
#include <iostream> 
#include <ostream>
#include <fstream>

#include "TVector2.h"


HyperBuilder::HyperBuilder(std::string input, const unsigned flags) : 
  m_flags(flags), 
  m_factory(0), 
  m_augmenter(0), 
  m_hists(0), 
  m_input_file(input), 
  m_max_pt(250*GeV), 
  m_min_pt(50*GeV), 
  m_do_cu(true), 
  m_do_cb(true)
{ 
}

void HyperBuilder::init(std::string input, const unsigned flags) { 
  const double max_pt = m_max_pt; 
  const double min_pt = m_min_pt;  
  m_factory = new JetFactory(input); 

  std::vector<Axis> axes; 
  Axis leading_pt = {"leadingPt", 10, min_pt, max_pt}; 
  Axis met = {"met", 10, min_pt, max_pt}; 
  Axis j2Cu = {"j2Cu", 10, -3, 7}; 
  Axis j2Cb = {"j2Cb", 10, -6, 4}; 
  Axis j3Cu = {"j3Cu", 10, -3, 7}; 
  Axis j3Cb = {"j3Cb", 10, -6, 4}; 
  Axis j1Bu = {"j1Bu", 10, -8, 8}; 
  Axis mttop = {"mttop", 2, 0, 600*GeV}; 
  axes.push_back(leading_pt); 
  axes.push_back(met); 
  if (m_flags & buildflag::leading_jet_btag) { 
    axes.push_back(j1Bu); 
  }
  if (m_do_cu) { 
    axes.push_back(j2Cu); 
  }
  if (m_do_cb) { 
    axes.push_back(j2Cb); 
  }
  if (m_do_cu) { 
    axes.push_back(j3Cu); 
  }
  if (m_do_cb) { 
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

void HyperBuilder::add_cut_mask(std::string name, unsigned bits) 
{
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

    unsigned dist_bits = m_factory->bits(); 
    unsigned aug_bits = m_augmenter->get_added_cuts(jets, met); 
    assert( (dist_bits & aug_bits) == 0); 
    const unsigned mask = (dist_bits | aug_bits); 

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
HyperBuilder::get_jet_vars(const std::vector<Jet>& jets) { 

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
  else { 
    if (m_augmenter == 0) { 
      m_augmenter = new CutAugmenter(); 
    }
    m_augmenter->set_float(name, value); 
  }
}
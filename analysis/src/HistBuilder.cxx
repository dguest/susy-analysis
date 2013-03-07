#include "HistBuilder.hh" 
#include "HistBuilderFlags.hh"
#include "JetFactory.hh"
#include "EventHistograms.hh"
#include "PhysicalConstants.hh"
#include "CutAugmenter.hh"
#include "common_functions.hh"
#include "H5Cpp.h"
#include "BtagScaler.hh"
#include "typedefs.hh"
#include "HistConfig.hh"
#include "RegionConfig.hh"

#include <string> 
#include <stdexcept>
#include <boost/format.hpp>
#include <algorithm>
#include <iostream> 
#include <ostream>
#include <fstream>

#include "distiller/EventBits.hh"


HistBuilder::HistBuilder(std::string input, const HistConfig& config, 
			 const unsigned flags): 
  m_flags(flags), 
  m_cut_augmenter(0)
{ 
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

}

HistBuilder::~HistBuilder() { 
  delete m_factory; 
  m_factory = 0; 

  delete m_cut_augmenter; 

  for (auto itr = m_histograms.begin(); itr != m_histograms.end(); itr++) { 
    delete itr->second; 
    itr->second = 0; 
  }
}

void HistBuilder::add_cut_mask(std::string name, ull_t mask, 
			       ull_t antimask){ 
  using namespace std; 
  m_histograms.push_back
    (make_pair(name,new EventHistograms(mask, antimask, m_flags))); 
}
void HistBuilder::add_region(const RegionConfig& region){ 
  using namespace std; 
  m_histograms.push_back
    (make_pair
     (region.name,new EventHistograms
      (region.required_bits, region.veto_bits, m_flags)));
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

    ull_t mask = m_factory->bits(); 
    const Jets jets = m_factory->jets(); 
    const TVector2 met = m_factory->met(); 
    const TLorentzVector met4(met.Px(), met.Py(), 0, 0); 
    double weight = m_factory->event_weight(); 

    if (m_cut_augmenter) { 
      m_cut_augmenter->set_cutmask(mask, jets, met); 
    }
    for (auto itr = m_histograms.begin(); itr != m_histograms.end(); itr++){
      itr->second->fill(m_factory, weight); 
    }

  }
  outstream << "\n";
  outstream.flush(); 

  return 0; 
}


void HistBuilder::save(std::string output) { 
  if (output.size() == 0) { 
    return; 
  }
  using namespace H5; 
  H5File file(output, H5F_ACC_TRUNC); 

  for (auto itr = m_histograms.begin(); itr != m_histograms.end(); itr++) { 
    Group newgroup(file.createGroup(itr->first)); 
    itr->second->write_to(newgroup); 
  }
}


void HistBuilder::set_float(std::string name, double value) { 
  if (!m_cut_augmenter) { 
    m_cut_augmenter = new CutAugmenter; 
  }
  m_cut_augmenter->set_float(name, value); 
}



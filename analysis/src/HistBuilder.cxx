#include "HistBuilder.hh" 
#include "HistBuilderFlags.hh"
#include "HistBuilderConstants.hh"
#include "JetFactory.hh"
#include "EventObjects.hh"
#include "RegionHistograms.hh"
#include "IRegionHistograms.hh"
#include "Region2dKinematicHistograms.hh"
#include "PhysicalConstants.hh"
#include "CutAugmenter.hh"
#include "common_functions.hh"
#include "H5Cpp.h"
#include "typedefs.hh"
#include "RegionConfig.hh"

#include <string> 
#include <stdexcept>
#include <boost/format.hpp>
#include <algorithm>
#include <iostream> 
#include <ostream>
#include <fstream>

#include "distiller/EventBits.hh"


HistBuilder::HistBuilder(std::string input, const unsigned flags): 
  m_input_file(input), 
  m_build_flags(flags)
{ 
  m_factory = new JetFactory(input, N_JETS_TO_READ); 
}

HistBuilder::~HistBuilder() { 
  delete m_factory; 
  m_factory = 0; 

  for (auto itr = m_histograms.begin(); itr != m_histograms.end(); itr++) { 
    delete itr->second; 
    itr->second = 0; 
  }
}

void HistBuilder::add_region(const RegionConfig& region){ 
  using namespace std; 

  // do blinding here
  if (region.type == reg::SIGNAL && 
      (m_build_flags & buildflag::is_data)) { 
    return; 
  }

  if (region.hists == reg::HISTMILL) { 
    m_histograms.push_back
      (make_pair(region.name,new RegionHistograms(region, m_build_flags)));
  }
  else if (region.hists == reg::KINEMATIC_STAT) { 
    m_histograms.push_back
      (make_pair(region.name, 
		 new Region2dKinematicHistograms(region, m_build_flags))); 
  }
  else { 
    throw std::logic_error("unknown hist type for region " + region.name); 
  }
  
  if (! (m_build_flags & buildflag::is_data) ) { 
    try { 
      m_factory->set_btagging(region.jet_tag_requirements); 
    }
    catch (std::runtime_error& err) { 
      std::string app = " file: " + m_input_file; 
      throw std::runtime_error(err.what() + app); 
    }
  }
  const IRegionHistograms* hists = m_histograms.rbegin()->second; 
  m_out_file_map[region.output_name].push_back(hists); 

}


int HistBuilder::build() { 

  typedef std::vector<Jet> Jets; 
  const int n_entries = m_factory->entries(); 
  const int one_percent = n_entries / 100; 

  std::ofstream nullstream("/dev/null"); 
  std::streambuf* out_buffer = nullstream.rdbuf(); 
  if (m_build_flags & buildflag::verbose) { 
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
    
    if (m_build_flags & buildflag::fill_truth) { 
      if (m_factory->hfor_type() == hfor::KILL) continue; 
    }
    EventObjects objects(m_factory, m_build_flags); 
    for (auto itr = m_histograms.begin(); 
	 itr != m_histograms.end(); itr++){
      itr->second->fill(objects); 
    }

  }
  outstream << "\n";
  outstream.flush(); 

  return 0; 
}


void HistBuilder::save() const { 

  for (auto itr = m_out_file_map.begin(); 
       itr != m_out_file_map.end(); itr++) { 
    H5::H5File file(itr->first, H5F_ACC_TRUNC); 
    for (auto hist_itr = itr->second.begin(); hist_itr != itr->second.end(); 
	 hist_itr++) { 
      (*hist_itr)->write_to(file); 
    }
  }
}




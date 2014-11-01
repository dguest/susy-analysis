#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include "constants_schema.hh"
#include "ObjectFactory.hh"
#include "EventObjects.hh"
#include "IRegionHistograms.hh"
#include "histogram_factory.hh"
#include "SkimCounts.hh"
#include "H5Cpp.h"
#include "typedefs.hh"
#include "RegionConfig.hh"

#include <string>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <fstream>

HistBuilder::HistBuilder(std::string input, const unsigned buildflags):
  m_input_file(input),
  m_build_flags(buildflags)
{
  m_factory = new ObjectFactory(input, N_JETS_TO_READ, buildflags);
  if (m_factory->is_data()) m_build_flags |= buildflag::is_data;
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

  // do blinding here (starts with false because we're unblinded)
  if (false && region.type == reg::SIGNAL &&
      (m_build_flags & buildflag::is_data)) {
    return;
  }

  add_histogram(region);

  const IRegionHistograms* hists = m_histograms.back().second;
  m_out_file_map[region.output_name].push_back(hists);

}


int HistBuilder::build() {

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
      outstream << "\r" << entry << " of " << n_entries << " (" <<
	std::setprecision(0) << entry / one_percent << "%) processed";
      outstream.flush();
      if (m_build_flags & buildflag::short_run && entry) break;
    }
    m_factory->entry(entry);

    if (! (m_build_flags & buildflag::is_data)) {
      if (m_factory->hfor_type() == hfor::KILL) continue;
    }
    const EventObjects objects(m_factory, m_build_flags);
    for (auto itr: m_histograms){
      itr.second->fill(objects);
    }
  }
  outstream << "\n";
  outstream.flush();

  return 0;
}


void HistBuilder::save() const {

  for (auto itr: m_out_file_map) {
    H5::H5File file(itr.first, H5F_ACC_TRUNC);
    for (auto hist_itr: itr.second){
      hist_itr->write_to(file);
    }
    m_factory->skim_counts()->write_to(file);
  }
}

// ==================== private =============================

void HistBuilder::add_histogram(const RegionConfig& region) {
  using namespace reg;
  m_histograms.push_back(make_pair(region.name, histogram_factory(
				     region, m_build_flags)));
}



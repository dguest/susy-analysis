#include "HyperBuilder.hh"
#include "HistBuilderFlags.hh"

#include "JetFactory.hh"
#include "Histogram.hh"
#include "MaskedHistArray.hh"
#include "PhysicalConstants.hh"
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
  m_hists(0)
{ 
  const double max_pt = 180*GeV; 
  const double min_pt = 80*GeV; 
  m_factory = new JetFactory(input); 

  std::vector<Axis> axes; 
  Axis leading_pt = {"leadingPt", 10, min_pt, max_pt}; 
  Axis met = {"met", 10, min_pt, max_pt}; 
  Axis j2Cu = {"j2Cu", 10, -8, 7}; 
  Axis j2Cb = {"j2Cb", 10, -10, 4}; 
  Axis j3Cu = {"j3Cu", 10, -8, 7}; 
  Axis j3Cb = {"j3Cb", 10, -10, 4}; 
  Axis j1Bu = {"j1Bu", 10, -8, 8}; 
  axes.push_back(leading_pt); 
  axes.push_back(met); 
  axes.push_back(j1Bu); 
  axes.push_back(j2Cu); 
  axes.push_back(j2Cb); 
  axes.push_back(j3Cu); 
  axes.push_back(j3Cb); 

  Histogram base_hist(axes); 
  m_hists = new MaskedHistArray(base_hist); 
}

HyperBuilder::~HyperBuilder() { 
  delete m_factory; 
  delete m_hists; 
}

void HyperBuilder::add_cut_mask(std::string name, unsigned bits) 
{
  m_hists->add_mask(bits, name); 
}

void HyperBuilder::build() { 
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

    const unsigned mask = m_factory->bits(); 
    std::map<std::string, double> vars; 
    vars["met"] = m_factory->met().Mod(); 

    {
      const Jet& jet1 = jets.at(0); 
      vars["leadingPt"] = jet1.Pt(); 
      vars["j1Bu"] = log(jet1.pb() / jet1.pu()); 
    }
    {
      const Jet& jet2 = jets.at(1); 
      vars["j2Cu"] = log(jet2.pc() / jet2.pu()); 
      vars["j2Cb"] = log(jet2.pc() / jet2.pb()); 
    }
    {
      const Jet& jet3 = jets.at(2); 
      vars["j3Cu"] = log(jet3.pc() / jet3.pu()); 
      vars["j3Cb"] = log(jet3.pc() / jet3.pb()); 
    }

    m_hists->fill(vars, mask); 

  }

  outstream << "\n";
  outstream.flush(); 

}

void HyperBuilder::save(std::string output) { 
  if (output.size() == 0) { 
    return; 
  }

  H5::H5File file(output, H5F_ACC_TRUNC); 
  m_hists->write_to(file); 

}



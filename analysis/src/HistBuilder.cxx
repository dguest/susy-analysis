#include "HistBuilder.hh" 
#include "JetFactory.hh"
#include "Histogram.hh"

#include "HdfFromHist.hh"
#include "H5Cpp.h"


#include <string> 
#include <stdexcept>

#include <iostream> 

HistBuilder::HistBuilder(std::string input)
{ 
  m_factory = new JetFactory(input); 
}

HistBuilder::~HistBuilder() { 
  delete m_factory; 
  m_factory = 0; 
}

void HistBuilder::add_cut_mask(std::string name, unsigned bits)
{
  CutMasks::const_iterator pos = m_cut_masks.find(name); 
  if (pos != m_cut_masks.end()) { 
    throw std::runtime_error("tried to overwrite " + name); 
  }
  m_cut_masks[name] = bits; 
}

void HistBuilder::build() { 
  int n_entries = m_factory->entries(); 
  for (int entry = 0; entry < n_entries; entry++) { 
    m_factory->entry(entry); 
    Jet leading_jet = m_factory->jet(1); 
    std::cout << leading_jet.Pt() << std::endl; 
    std::cout << leading_jet.Eta() << std::endl; 
    std::cout << leading_jet.pu() << std::endl; 
  }

}

void HistBuilder::save(std::string output) { 

}

void HistBuilder::make_histograms() { 
  Axis a; 
  a.name = "bob"; 
  a.n_bins = 30; 
  a.low = 0; 
  a.high = 10; 
  std::vector<Axis> avec; 
  avec.push_back(a); 
  Histogram hist(avec); 
}

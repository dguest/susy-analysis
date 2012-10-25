#include "HistBuilder.hh" 
#include "JetFactory.hh"
#include "Histogram.hh"

#include "HdfFromHist.hh"
#include "H5Cpp.h"


#include <string> 
#include <stdexcept>
#include <boost/format.hpp>

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
  if (m_histograms.size() == 0) { 
    book_tobster_histograms(); 
    // book_ptmet_histograms(); 
  }
  typedef std::vector<Jet> Jets; 
  int n_entries = m_factory->entries(); 
  for (int entry = 0; entry < n_entries; entry++) { 
    m_factory->entry(entry); 
    Jets jets = m_factory->jets(); 

    if (jets.size() >= 2) { 
      fill("dphiC1Met",m_factory->met()/1e6); 
    }
    if (entry > 100) { 
      break; 
    }

  }

}

void HistBuilder::save(std::string output) { 
  if (output.size() == 0) { 
    return; 
  }
  H5::H5File file(output, H5F_ACC_TRUNC); 

  for (HistByCut::const_iterator itr = m_histograms.begin(); 
       itr != m_histograms.end(); itr++) { 
    histToFile(itr->second, file, itr->first); 
  }
  for (HistByCut::const_iterator itr = m_h1.begin(); 
       itr != m_h1.end(); itr++) { 
    histToFile(itr->second, file, itr->first); 
  }
}

void HistBuilder::book_ptmet_histograms() { 
  Axis jet_pt; 
  jet_pt.n_bins = 30; 
  jet_pt.low = 0.0; 
  jet_pt.high = 1e6; 
  std::vector<Axis> pt_met_axes; 
  for (int jet_n = 0; jet_n < 3; jet_n++) { 
    std::string cutname = (boost::format("jet%iPt") % jet_n).str(); 
    jet_pt.name = cutname; 
    pt_met_axes.push_back(jet_pt); 
  }
  jet_pt.name = "met"; 
  pt_met_axes.push_back(jet_pt); 
  for (CutMasks::const_iterator itr = m_cut_masks.begin(); 
       itr != m_cut_masks.end(); itr++) {
    std::string name = itr->first + "_rawjetmet"; 
    m_histograms.insert(std::make_pair(name,Histogram(pt_met_axes))); 
  }

}
void HistBuilder::book_hist(std::string name, int bins, 
			    double low, double high)
{ 
  Histogram hist(bins, low, high);
  m_h1.insert(std::make_pair(name,hist)); 
}
void HistBuilder::book_tobster_histograms() { 
  book_hist("dphiC1Met",100,0.0,M_PI); 
  book_hist("mTTop",100,0.0,1e6); 
}
void HistBuilder::fill(std::string hist_name, double value){ 
  HistByCut::iterator itr = m_h1.find(hist_name); 
  if (itr == m_h1.end()){ 
    throw std::range_error("hist " + hist_name + " doesn't exist"); 
  }
  itr->second.fill(std::vector<double>(1,value)); 
}; 

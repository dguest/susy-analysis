#include "Jet2DHists.hh"
#include "Jet.hh"
#include "Histogram.hh"
#include "HistBuilderFlags.hh"
#include "Flavor.hh"
#include "H5Cpp.h"

#include <string> 
#include <cmath>

Jet2DHists::Jet2DHists(const unsigned flags): 
  m_anti_b_vs_anti_light(0)
{
  Axis anti_light_wt; 
  anti_light_wt.name = "logCu"; 
  anti_light_wt.n_bins = 100; 
  anti_light_wt.low = -5.0; 
  anti_light_wt.high = 8.0; 
  
  Axis anti_b_wt; 
  anti_b_wt.name = "logCb"; 
  anti_b_wt.n_bins = 100; 
  anti_b_wt.low = -8.0; 
  anti_b_wt.high = 4.0; 
  
  std::vector<Axis> tag_wt_axes = {anti_light_wt, anti_b_wt}; 
  m_anti_b_vs_anti_light = new Histogram(tag_wt_axes); 
}

Jet2DHists::~Jet2DHists() { 
  delete m_anti_b_vs_anti_light; 
}

void Jet2DHists::fill(const Jet& jet, double weight) { 
  std::vector<double> wt_vector = {
    log(jet.pc() / jet.pu() ), log(jet.pc() / jet.pb())}; 
  m_anti_b_vs_anti_light->fill(wt_vector); 
}

void Jet2DHists::write_to(H5::CommonFG& file) { 
  m_anti_b_vs_anti_light->write_to(file, "taggerWeights"); 
}
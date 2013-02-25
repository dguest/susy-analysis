#include "ObjKinematics.hh"
#include "JetFactory.hh"
#include "Histogram.hh"
#include "MaskedHistArray.hh"
#include "HistBuilderFlags.hh"
#include "PhysicalConstants.hh"
#include "H5Cpp.h"

#include <string> 
#include <cmath>


Jet1DHists::Jet1DHists(double max_pt, const unsigned flags, std::string tag): 
  m_truth_label(0), 
  m_tag(tag)
{ 
  m_pt = new MaskedHistArray(Histogram(100, 0, max_pt), tag); 
  m_eta = new MaskedHistArray(Histogram(100, -2.7, 2.7), tag); 
  m_cnnLogCb = new MaskedHistArray(Histogram(100, -10, 10), tag); 
  m_cnnLogCu = new MaskedHistArray(Histogram(100, -10, 10), tag); 
  m_cnnLogBu = new MaskedHistArray(Histogram(100, -10, 10), tag); 
  m_met_dphi = new MaskedHistArray(Histogram(100, -3.2, 3.2), tag); 

  if (flags & buildflag::fill_truth) { 
    m_truth_label = new MaskedHistArray(Histogram(21, -0.5, 20.5), tag); 
  }
}
Jet1DHists::~Jet1DHists() 
{
  delete m_pt; 
  delete m_eta; 
  delete m_cnnLogCb; 
  delete m_cnnLogCu; 
  delete m_cnnLogBu;
  delete m_met_dphi; 

  delete m_truth_label; 

}

void Jet1DHists::add_mask(ull_t bitmask, std::string name, 
			  ull_t antimask) { 
  m_pt->add_mask(bitmask,  name, antimask);       
  m_eta->add_mask(bitmask, name, antimask);      
  m_cnnLogCb->add_mask(bitmask, name, antimask); 
  m_cnnLogCu->add_mask(bitmask, name, antimask);
  m_cnnLogBu->add_mask(bitmask, name, antimask); 
  m_met_dphi->add_mask(bitmask, name, antimask); 

  if (m_truth_label) { 
    m_truth_label->add_mask(bitmask, name, antimask); 
  }
} 

void Jet1DHists::write_to(H5::CommonFG& file, std::string stub, 
			  std::string postfix) { 
  using namespace H5;
  Group pt(file.createGroup("pt")); 
  m_pt->write_to(pt);       
  Group eta(file.createGroup("eta")); 
  m_eta->write_to(eta); 
  Group logCb(file.createGroup("logCb")); 
  m_cnnLogCb->write_to(logCb); 
  Group logCu(file.createGroup("logCu")); 
  m_cnnLogCu->write_to(logCu); 
  Group logBu(file.createGroup("logBu")); 
  m_cnnLogBu->write_to(logBu); 
  Group metDphi(file.createGroup("metDphi")); 
  m_met_dphi->write_to(metDphi); 

  if (m_truth_label) { 
    Group truth(file.createGroup("truth")); 
    write_truth_info(truth); 
  }

}
void Jet1DHists::write_truth_info(H5::CommonFG& truth){ 
  
  using namespace H5; 
  Group label(truth.createGroup("label")); 
  m_truth_label->write_to(label); 
}

void Jet1DHists::fill(const Jet& jet, const ull_t mask, double w) { 
  m_pt->fill(jet.Pt(), mask, w); 
  m_eta->fill(jet.Eta(), mask, w); 
  m_cnnLogCu->fill(log(jet.pc() / jet.pu()), mask, w); 
  m_cnnLogCb->fill(log(jet.pc() / jet.pb()), mask, w); 
  m_cnnLogBu->fill(log(jet.pb() / jet.pu()), mask, w); 
  m_met_dphi->fill(jet.met_dphi(), mask, w); 

  if (m_truth_label) { 
    int label = jet.flavor_truth_label(); 
    m_truth_label->fill(label, mask, w); 
  }

}

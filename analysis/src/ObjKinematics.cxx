#include "ObjKinematics.hh"
#include "JetFactory.hh"
#include "Histogram.hh"
#include "MaskedHistArray.hh"
#include "HistBuilderFlags.hh"
#include "H5Cpp.h"

#include <string> 
#include <cmath>


Jet1DHists::Jet1DHists(double max_pt, const unsigned flags) : 
  m_truth_label(0), 
  m_bottom_pt(0), 
  m_charm_pt(0), 
  m_light_pt(0), 
  m_other_pt(0)
{ 
  m_pt = new MaskedHistArray(Histogram(100, 0, max_pt)); 
  m_eta = new MaskedHistArray(Histogram(100, -2.7, 2.7)); 
  m_cnnLogCb = new MaskedHistArray(Histogram(100, -10, 10)); 
  m_cnnLogCu = new MaskedHistArray(Histogram(100, -10, 10)); 
  m_cnnLogBu = new MaskedHistArray(Histogram(100, -10, 10)); 

  if (flags & buildflag::fill_truth) { 
    m_truth_label = new MaskedHistArray(Histogram(7, -0.5, 6.5)); 
    m_bottom_pt = new MaskedHistArray(Histogram(100, 0, max_pt)); 
    m_charm_pt = new MaskedHistArray(Histogram(100, 0, max_pt)); 
    m_light_pt = new MaskedHistArray(Histogram(100, 0, max_pt)); 
    m_other_pt = new MaskedHistArray(Histogram(100, 0, max_pt)); 
  }
}
Jet1DHists::~Jet1DHists() 
{
  delete m_pt; 
  delete m_eta; 
  delete m_cnnLogCb; 
  delete m_cnnLogCu; 
  delete m_cnnLogBu;
}

void Jet1DHists::add_mask(unsigned bitmask, std::string name) { 
  m_pt->add_mask(bitmask, name);       
  m_eta->add_mask(bitmask, name);      
  m_cnnLogCb->add_mask(bitmask, name); 
  m_cnnLogCu->add_mask(bitmask, name);
  m_cnnLogBu->add_mask(bitmask, name); 

  if (m_truth_label) { 
    m_truth_label->add_mask(bitmask, name); 
    m_bottom_pt->add_mask(bitmask, name); 
    m_charm_pt->add_mask(bitmask, name); 
    m_light_pt->add_mask(bitmask, name); 
    m_other_pt->add_mask(bitmask, name); 
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

  if (m_truth_label) { 
    Group truth(file.createGroup("truth")); 
    write_truth_info(truth); 
  }

}
void Jet1DHists::write_truth_info(H5::CommonFG& truth){ 
  
  using namespace H5; 

  Group label(truth.createGroup("label")); 
  m_truth_label->write_to(label); 
    
  Group pt(truth.createGroup("pt")); 
  Group bottom(pt.createGroup("bottom")); 
  m_bottom_pt->write_to(bottom); 
  Group charm(pt.createGroup("charm")); 
  m_charm_pt->write_to(charm); 
  Group light(pt.createGroup("light")); 
  m_light_pt->write_to(light); 
  Group other(pt.createGroup("other")); 
}

void Jet1DHists::fill(const Jet& jet, const unsigned mask) { 
  m_pt->fill(jet.Pt(), mask); 
  m_eta->fill(jet.Eta(), mask); 
  m_cnnLogCu->fill(log(jet.pc() / jet.pu()), mask); 
  m_cnnLogCb->fill(log(jet.pc() / jet.pb()), mask); 
  m_cnnLogBu->fill(log(jet.pb() / jet.pu()), mask); 

  if (m_truth_label) { 
    int label = jet.flavor_truth_label(); 
    double pt = jet.Pt(); 
    switch (label) { 
    case 4:
      m_charm_pt->fill(pt, mask); 
      break; 
    case 5: 
      m_bottom_pt->fill(pt, mask); 
      break; 
    case 1: 
      m_light_pt->fill(pt, mask); 
      break; 
    default: 
      m_other_pt->fill(pt, mask); 
      break; 
    }
  }

}

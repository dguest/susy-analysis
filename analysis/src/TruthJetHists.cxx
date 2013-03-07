#include "TruthJetHists.hh"
#include "ObjKinematics.hh"
#include "H5Cpp.h"
#include "JetFactory.hh"
#include "PhysicalConstants.hh"

TruthJetHists::TruthJetHists(double max_pt, const unsigned flags) { 
  m_bottom = new Jet1DHists(max_pt, flags); 
  m_charm = new Jet1DHists(max_pt, flags); 
  m_light = new Jet1DHists(max_pt, flags); 
  m_tau = new Jet1DHists(max_pt, flags); 
  m_other = new Jet1DHists(max_pt, flags); 
}

TruthJetHists::~TruthJetHists() { 
  delete m_bottom; 
  delete m_charm; 
  delete m_light; 
  delete m_tau; 
  delete m_other; 
}


void TruthJetHists::write_to(H5::CommonFG& file) { 
  using namespace H5; 
  Group bottom(file.createGroup("bottom")); 
  Group charm(file.createGroup("charm")); 
  Group light(file.createGroup("light")); 
  Group tau(file.createGroup("tau")); 
  Group other(file.createGroup("other")); 
  m_bottom->write_to(bottom); 
  m_charm->write_to(charm); 
  m_light->write_to(light); 
  m_tau->write_to(tau); 
  m_other->write_to(other); 
}

void TruthJetHists::fill(const Jet& jet, double w) { 
  int label = jet.flavor_truth_label(); 
  switch (label) { 
  case BOTTOM: 
    m_bottom->fill(jet,  w); 
    return; 
  case CHARM: 
    m_charm->fill(jet,  w); 
    return; 
  case LIGHT: 
    m_light->fill(jet,  w); 
    return; 
  case TAU:
    m_tau->fill(jet,  w); 
    return; 
  default: 
    m_other->fill(jet,  w); 
    return; 
  }
}

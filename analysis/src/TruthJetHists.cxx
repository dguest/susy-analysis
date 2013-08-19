#include "TruthJetHists.hh"
#include "Jet1DHists.hh"
#include "H5Cpp.h"
#include "Jet.hh"
#include "Flavor.hh"

TruthJetHists::TruthJetHists(double max_pt, const unsigned flags, 
			     btag::Tagger tag) { 
  m_bottom = new Jet1DHists(max_pt, flags, tag); 
  m_charm  = new Jet1DHists(max_pt, flags, tag); 
  m_light  = new Jet1DHists(max_pt, flags, tag); 
  m_tau    = new Jet1DHists(max_pt, flags, tag); 
  m_other  = new Jet1DHists(max_pt, flags, tag); 
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
  Flavor label = jet.flavor_truth_label(); 
  switch (label) { 
  case Flavor::BOTTOM: 
    m_bottom->fill(jet,  w); 
    return; 
  case Flavor::CHARM: 
    m_charm->fill(jet,  w); 
    return; 
  case Flavor::LIGHT: 
    m_light->fill(jet,  w); 
    return; 
  case Flavor::TAU:
    m_tau->fill(jet,  w); 
    return; 
  default: 
    m_other->fill(jet,  w); 
    return; 
  }
}

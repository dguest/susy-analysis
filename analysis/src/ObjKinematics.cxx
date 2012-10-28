#include "ObjKinematics.hh"
#include "JetFactory.hh"
#include "Histogram.hh"
#include "MaskedHistArray.hh"
#include "H5Cpp.h"

#include <string> 
#include <cmath>


Jet1DHists::Jet1DHists(double max_pt) 
{ 
  m_pt = new MaskedHistArray(Histogram(100, 0, max_pt)); 
  m_eta = new MaskedHistArray(Histogram(100, -2.7, 2.7)); 
  m_phi = new MaskedHistArray(Histogram(100, -M_PI, M_PI)); 
  m_cnnLogCb = new MaskedHistArray(Histogram(100, -10, 10)); 
  m_cnnLogCu = new MaskedHistArray(Histogram(100, -10, 10)); 
}
Jet1DHists::~Jet1DHists() 
{
  delete m_pt; 
  delete m_eta; 
  delete m_phi; 
  delete m_cnnLogCb; 
  delete m_cnnLogCu; 
}

void Jet1DHists::add_mask(unsigned bitmask, std::string name) { 
  m_pt->add_mask(bitmask, name);       
  m_eta->add_mask(bitmask, name);      
  m_phi->add_mask(bitmask, name);      
  m_cnnLogCb->add_mask(bitmask, name); 
  m_cnnLogCu->add_mask(bitmask, name);
} 

void Jet1DHists::write_to(H5::CommonFG& file, std::string stub, 
			  std::string postfix) { 
  m_pt->write_to(file, stub + "pt" + postfix);       
  m_eta->write_to(file, stub + "eta" + postfix);      
  m_phi->write_to(file, stub + "phi" + postfix);      
  m_cnnLogCb->write_to(file, stub + "logCb" + postfix); 
  m_cnnLogCu->write_to(file, stub + "logCu" + postfix);
}
void Jet1DHists::fill(const Jet& jet, const unsigned mask) { 
  m_pt->fill(jet.Pt(), mask); 
  m_eta->fill(jet.Eta(), mask); 
  m_phi->fill(jet.Phi(), mask); 
  m_cnnLogCu->fill(log(jet.pc() / jet.pu()), mask); 
  m_cnnLogCb->fill(log(jet.pc() / jet.pb()), mask); 
}

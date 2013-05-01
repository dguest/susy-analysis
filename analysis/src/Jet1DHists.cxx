#include "Jet1DHists.hh"
#include "Jet.hh"
#include "Histogram.hh"
#include "HistBuilderFlags.hh"
#include "Flavor.hh"
#include "H5Cpp.h"

#include <string> 
#include <cmath>


Jet1DHists::Jet1DHists(double max_pt, const unsigned flags): 
  m_truth_label(0)
{ 
  m_pt = new Histogram(100, 0, max_pt, "MeV"); 
  m_eta = new Histogram(100, -2.7, 2.7); 
  m_cnnLogCb = new Histogram(100, -10, 10);
  m_cnnLogCu = new Histogram(100, -10, 10);
  m_cnnLogBu = new Histogram(100, -10, 10);
  m_met_dphi = new Histogram(100, 0, 3.2);

  if ( !(flags & buildflag::is_data)) { 
    m_truth_label = new Histogram(21, -0.5, 20.5); 
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

void Jet1DHists::write_to(H5::CommonFG& file) { 
  using namespace H5;
  m_pt->write_to(file,"pt");       
  m_eta->write_to(file,"eta"); 
  m_cnnLogCb->write_to(file,"logCb"); 
  m_cnnLogCu->write_to(file,"logCu"); 
  m_cnnLogBu->write_to(file,"logBu"); 
  m_met_dphi->write_to(file,"metDphi"); 
  if (m_truth_label) { 
    write_truth_info(file); 
  }

}
void Jet1DHists::write_truth_info(H5::CommonFG& file){ 
  using namespace H5; 
  m_truth_label->write_to(file,"truthLabel"); 
}

void Jet1DHists::fill(const Jet& jet, double w) { 
  m_pt->fill(jet.Pt(),  w); 
  m_eta->fill(jet.Eta(),  w); 
  m_cnnLogCu->fill(log(jet.pc() / jet.pu()),  w); 
  m_cnnLogCb->fill(log(jet.pc() / jet.pb()),  w); 
  m_cnnLogBu->fill(log(jet.pb() / jet.pu()),  w); 
  m_met_dphi->fill(fabs(jet.met_dphi()),  w); 

  if (m_truth_label) { 
    int label = static_cast<int>(jet.flavor_truth_label()); 
    m_truth_label->fill(label,  w); 
  }

}

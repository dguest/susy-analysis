#include "Jet1DHists.hh"
#include "Jet.hh"
#include "Histogram.hh"
#include "HistBuilderFlags.hh"
#include "Flavor.hh"
#include "H5Cpp.h"

#include <string> 
#include <cmath>
#include <stdexcept>
#include <sstream>

Jet1DHists::Jet1DHists(double max_pt, const unsigned flags, btag::Tagger tag): 
  m_tagger(tag), 
  m_truth_label(0)
{ 
  m_pt = new Histogram(100, 0, max_pt, "MeV"); 
  m_eta = new Histogram(100, -2.7, 2.7); 
  m_cnnLogCb = new Histogram(100, -10, 10);
  m_cnnLogCu = new Histogram(100, -10, 10);
  m_cnnLogBu = new Histogram(100, -10, 10);
  m_met_dphi = new Histogram(100, 0, 3.2);
  m_mu_met_dphi = new Histogram(100, 0, 3.2);

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
  delete m_mu_met_dphi; 

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
  m_mu_met_dphi->write_to(file,"muMetDphi"); 
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
  double pb = jet.flavor_weight(Flavor::BOTTOM, m_tagger); 
  double pc = jet.flavor_weight(Flavor::CHARM, m_tagger); 
  double pu = jet.flavor_weight(Flavor::LIGHT, m_tagger); 

  try { 
    m_cnnLogCu->fill(log(pc / pu),  w); 
    m_cnnLogCb->fill(log(pc / pb),  w); 
    m_cnnLogBu->fill(log(pb / pu),  w); 
  }
  catch (std::range_error& r) {
    std::stringstream str; 
    str << r.what() << " in 1d filling, (b,c,u) = " << 
      pb << " " << pc << " " << pu; 
    throw std::range_error(str.str()); 
  }
  m_met_dphi->fill(std::abs(jet.met_dphi()),  w);
  m_mu_met_dphi->fill(std::abs(jet.mu_met_dphi()), w); 

  if (m_truth_label) { 
    int label = static_cast<int>(jet.flavor_truth_label()); 
    m_truth_label->fill(label,  w); 
  }

}

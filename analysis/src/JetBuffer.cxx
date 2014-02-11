#include "JetBuffer.hh"
#include "BtagScaler.hh"
#include "BtagBuffer.hh"
#include "Jet.hh"
#include "common_functions.hh"

#include "TTree.h"

JetBuffer::JetBuffer(): 
  flavor_truth_label(-1), 
  m_medium_sf(0), 
  m_loose_sf(0), 
  m_has_truth(false), 
  m_is_electron_jet(false)
{ 
}

bool JetBuffer::has_jet() const { 
  return pt > 0; 
}

void JetBuffer::set_buffer(TTree* tree, const std::string& base_name) 
{
  using namespace std; 
  string pt = base_name + "pt"; 
  string eta = base_name + "eta"; 
  string phi = base_name + "phi"; 
  // not setting E because not saved yet...
  string pb_jfc = base_name + "jfc_b"; 
  string pc_jfc = base_name + "jfc_c"; 
  string pu_jfc = base_name + "jfc_u"; 
  string flavor_truth = base_name + "flavor_truth_label"; 
  
  set_branch(tree, pt.c_str(), &pt); 
  set_branch(tree, eta.c_str(), &eta); 
  set_branch(tree, phi.c_str(), &phi); 

  set_branch(tree, pb_jfc.c_str(), &jfc_b); 
  set_branch(tree, pc_jfc.c_str(), &jfc_c); 
  set_branch(tree, pu_jfc.c_str(), &jfc_u); 

  if (tree->GetBranch(flavor_truth.c_str())) { 
    set_branch(tree, flavor_truth.c_str(), &flavor_truth_label); 
    m_medium_sf = new BtagBuffer(tree, "medium_scale_factor"); 
    m_loose_sf = new BtagBuffer(tree, "loose_scale_factor"); 
    m_has_truth = true; 
  }

}

Jet JetBuffer::jet() const { 
  return Jet(this, m_has_truth); 
}

JetBuffer::~JetBuffer() { 
  delete m_medium_sf; 
  delete m_loose_sf; 
}

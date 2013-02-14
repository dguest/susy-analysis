#include "BtagScaler.hh"
#include <stdexcept> 
#include "TTree.h"
#include <boost/format.hpp>

BtagScaler::BtagScaler(TTree* tree, std::string prefix, unsigned required, 
		       unsigned veto): 
  m_required(required), 
  m_veto(veto)
{
  safeset(tree, prefix + "scale_factor", &m_scale_factor); 
  safeset(tree, prefix + "fail_factor", &m_fail_factor); 
  safeset(tree, prefix + "scale_factor_err", &m_scale_factor_err); 
  safeset(tree, prefix + "fail_factor_err", &m_fail_factor_err); 
}

double BtagScaler::get_scalefactor(unsigned evt_mask, int flavor, 
				   btag::Systematic syst)
{
  using namespace btag; 
  bool pass = (evt_mask & m_required) && !(evt_mask | m_veto); 
  double base = pass ? m_scale_factor: m_fail_factor; 
  double err_up = pass ? m_scale_factor_err: -m_fail_factor_err;
  double err_down = -err_up; 
  switch (syst) { 
  case NONE: 
    return base; 
  case BUP: 
    return flavor == 5 ? base + err_up: base; 
  case BDOWN: 
    return flavor == 5 ? base + err_down: base; 
  case CUP: 
    return flavor == 4 ? base + err_up: base; 
  case CDOWN: 
    return flavor == 4 ? base + err_down: base; 
  case UUP: 
    return flavor == 0 ? base + err_up: base; 
  case UDOWN: 
    return flavor == 0 ? base + err_down: base; 
  case TUP: 
    return flavor == 15 ? base + err_up: base; 
  case TDOWN: 
    return flavor == 15 ? base + err_down: base; 
  default: 
    std::string bstring = "btagging scaler: got unknown pdgid %i"; 
    std::string error((boost::format(bstring) % flavor).str());
    throw std::runtime_error(error); 
  }
}

void BtagScaler::safeset(TTree* tree, std::string branch, void* address) { 
  unsigned ret_code; 
  tree->SetBranchStatus(branch.c_str(), 1, &ret_code); 
  if (ret_code != 1) { 
    throw std::runtime_error("branch: " + branch + ", where the fuck is it?"); 
  }
  tree->SetBranchAddress(branch.c_str(), address); 
}

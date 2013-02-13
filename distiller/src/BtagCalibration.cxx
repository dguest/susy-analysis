#include "BtagCalibration.hh"
#include "CalibrationDataInterface/CalibrationDataInterfaceROOT.h"
#include <stdexcept> 
#include <cassert> 

Analysis::Uncertainty get_unct(btag::Uncertainty); 

BtagCalibration::BtagCalibration(std::string clb_file, 
				 std::string file_path)
{
  using namespace btag; 
  m_cnn = new CDI("JetFitterCOMBCharm", clb_file, file_path); 
  m_ops[CNN_LOOSE] = "-1.0_0.0"; 
  m_ops[CNN_MEDIUM] = "-1.0_-0.82"; 
  // m_ops[CNN_TIGHT] = "XXX"; 
  m_interfaces[CNN_LOOSE] = m_cnn; 
  m_interfaces[CNN_MEDIUM] = m_cnn; 
  // m_interfaces[CNN_TIGHT] = m_cnn; 
}

BtagCalibration::~BtagCalibration() { 
  delete m_cnn; 
  m_cnn = 0; 
}

BtagCalibration::CalResult 
BtagCalibration::scale_factor(double pt, double eta, 
			      btag::Flavor flavor, 
			      btag::Tagger tagger, 
			      btag::Uncertainty uncert) const { 
  CalVars vars = get_vars(pt, eta); 
  std::string op = get_op(tagger); 
  std::string label = get_label(flavor); 
  Analysis::Uncertainty unct = get_unct(uncert); 
  return get_cdi(tagger)->getScaleFactor(vars, label, op, unct);
}
BtagCalibration::CalResult 
BtagCalibration::fail_factor(double pt, double eta, 
			     btag::Flavor flavor, 
			     btag::Tagger tagger, 
			     btag::Uncertainty uncert) const { 
  CalVars vars = get_vars(pt, eta); 
  std::string op = get_op(tagger); 
  std::string label = get_label(flavor); 
  Analysis::Uncertainty unct = get_unct(uncert); 
  return get_cdi(tagger)->getInefficiencyScaleFactor(vars, label, op, unct);
}

// private stuff

BtagCalibration::CDI* BtagCalibration::get_cdi(btag::Tagger t)
  const { 
  Interfaces::const_iterator cdi = m_interfaces.find(t); 
  if (cdi == m_interfaces.end()) { 
    throw std::logic_error("didn't find cdi in " __FILE__); 
  }
  return cdi->second; 
}

std::string BtagCalibration::get_op(btag::Tagger t) const { 
  Names::const_iterator oper = m_ops.find(t); 
  if (oper == m_ops.end()) { 
    throw std::logic_error("didn't find OP in " __FILE__); 
  }
  return oper->second; 
}

Analysis::CalibrationDataVariables BtagCalibration::get_vars(double pt, 
							     double eta) 
  const { 
  CalVars vars; 
  vars.jetAuthor = "AntiKt4TopoLCJVF"; 
  vars.jetPt = pt; 
  vars.jetEta = eta; 
  return vars; 
}

std::string BtagCalibration::get_label(btag::Flavor flavor) const { 
  switch (flavor) { 
  case btag::B: return std::string("B"); 
  case btag::C: return std::string("C"); 
  case btag::U: return std::string("Light"); 
  case btag::T: return std::string("T"); 
  default: 
    assert(false); 
  }
}

Analysis::Uncertainty get_unct(btag::Uncertainty u) { 
  using namespace Analysis; 
  switch (u) { 
  case btag::None: return None; 
  case btag::Total: return Total; 
  case btag::Statistical: return Statistical; 
  case btag::Systematic: return Systematic; 
  default: 
    assert(false); 
  }
}

#ifndef BTAG_CALIBRATION_HH
#define BTAG_CALIBRATION_HH

#include <boost/noncopyable.hpp>
#include <string> 
#include <map>
#include "btag_defs.hh"

class BaselineJet; 

namespace Analysis { 
  class CalibrationDataInterfaceROOT; 
  class CalibrationDataVariables; 
}

struct JetTagFactorInputs { 
  double pt; 
  double eta; 
  double anti_b; 
  double anti_u; 
  btag::Flavor flavor; 
};

class BtagCalibration : boost::noncopyable 
{
public: 
  typedef std::pair<double, double> CalResult; 
  BtagCalibration(std::string calibration_file, std::string file_path); 
  ~BtagCalibration(); 
  CalResult applied_factor(const JetTagFactorInputs& jet_tf_inputs,  
			   btag::Tagger tagger, 
			   btag::Uncertainty = btag::Total) const; 
  bool pass_anti_u(const JetTagFactorInputs& jet_tf_inputs, 
		   btag::Tagger) const; 
  bool pass_anti_b(const JetTagFactorInputs& jet_tf_inputs, 
		   btag::Tagger) const; 
private: 
  CalResult pass_factor(double pt, double eta, 
			btag::Flavor flavor, 
			btag::Tagger tagger, 
			btag::Uncertainty = btag::Total) const;
  CalResult fail_factor(double pt, double eta, 
			btag::Flavor flavor,
			btag::Tagger tagger, 
			btag::Uncertainty = btag::Total) const;
  typedef Analysis::CalibrationDataInterfaceROOT CDI; 
  typedef Analysis::CalibrationDataVariables CalVars; 
  typedef std::map<btag::Tagger, CDI*> Interfaces;
  typedef std::map<btag::Tagger, std::string> Names; 
  typedef std::map<btag::Tagger, double> CutValue; 
  std::string get_op(btag::Tagger) const; 
  CDI* get_cdi(btag::Tagger) const; 
  CalVars get_vars(double pt, double eta) const; 
  std::string get_label(btag::Flavor) const; 
  CDI* m_cnn; 
  Interfaces m_interfaces; 
  Names m_ops; 
  std::string m_jet_author; 
  CutValue m_anti_u_cuts; 
  CutValue m_anti_b_cuts; 
}; 

#endif 

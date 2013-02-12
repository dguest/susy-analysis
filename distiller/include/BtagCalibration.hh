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


class BtagCalibration : boost::noncopyable 
{
public: 
  typedef std::pair<double, double> CalResult; 
  BtagCalibration(std::string calibration_file, std::string file_path); 
  ~BtagCalibration(); 
  CalResult scale_factor(double pt, double eta, 
			 btag::Flavor flavor, 
			 btag::Tagger tagger, 
			 btag::Uncertainty = btag::Total) const;
  CalResult ineff_scale_factor(double pt, double eta, 
			       btag::Flavor flavor,
			       btag::Tagger tagger, 
			       btag::Uncertainty = btag::Total) const;
private: 
  typedef Analysis::CalibrationDataInterfaceROOT CDI; 
  typedef Analysis::CalibrationDataVariables CalVars; 
  typedef std::map<btag::Tagger, CDI*> Interfaces;
  typedef std::map<btag::Tagger, std::string> Names; 
  std::string get_op(btag::Tagger) const; 
  CDI* get_cdi(btag::Tagger) const; 
  CalVars get_vars(double pt, double eta) const; 
  std::string get_label(btag::Flavor) const; 
  CDI* m_cnn; 
  Interfaces m_interfaces; 
  Names m_ops; 
}; 

#endif 

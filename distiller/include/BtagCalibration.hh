#ifndef BTAG_CALIBRATION_HH
#define BTAG_CALIBRATION_HH

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

class BtagCalibration
{
public:
  typedef std::pair<double, double> CalResult;
  BtagCalibration(std::string calibration_file, std::string file_path);
  ~BtagCalibration();
  BtagCalibration(BtagCalibration&) = delete;
  BtagCalibration& operator=(BtagCalibration&) = delete;
  CalResult applied_factor(const JetTagFactorInputs& jet_tf_inputs,
			   btag::OperatingPoint tagger,
			   btag::Uncertainty = btag::Total) const;
  bool pass_anti_u(const JetTagFactorInputs& jet_tf_inputs,
		   btag::OperatingPoint) const;
  bool pass_anti_b(const JetTagFactorInputs& jet_tf_inputs,
		   btag::OperatingPoint) const;
private:
  CalResult pass_factor(double pt, double eta,
			btag::Flavor flavor,
			btag::OperatingPoint tagger,
			btag::Uncertainty = btag::Total) const;
  CalResult fail_factor(double pt, double eta,
			btag::Flavor flavor,
			btag::OperatingPoint tagger,
			btag::Uncertainty = btag::Total) const;
  typedef Analysis::CalibrationDataInterfaceROOT CDI;
  typedef Analysis::CalibrationDataVariables CalVars;
  typedef std::map<btag::OperatingPoint, CDI*> Interfaces;
  typedef std::map<btag::OperatingPoint, std::string> Names;
  typedef std::map<btag::OperatingPoint, double> CutValue;
  void check_cdi() const;
  std::string get_op(btag::OperatingPoint) const;
  CDI* get_cdi(btag::OperatingPoint) const;
  CalVars get_vars(double pt, double eta) const;
  std::string get_label(btag::Flavor) const;
  void set_indices(btag::Flavor, btag::OperatingPoint);
  CDI* m_cnn;
  Interfaces m_interfaces;
  Names m_ops;
  std::string m_jet_author;
  CutValue m_anti_u_cuts;
  CutValue m_anti_b_cuts;
  typedef std::pair<btag::Flavor,btag::OperatingPoint> FOPIndex;
  std::map<FOPIndex, unsigned> m_flav_op_eff_index;
  std::map<FOPIndex, unsigned> m_flav_op_sf_index;
};

namespace btag {
  Tagger tagger_from_op(OperatingPoint);
}

#endif

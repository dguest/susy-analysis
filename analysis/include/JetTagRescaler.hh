#ifndef JET_TAG_RESCALER_HH
#define JET_TAG_RESCALER_HH

#include <istream> 
#include <map> 
#include <string> 

// ----- Class to rescale b-tagging weights across MC. ----

namespace jettag { 
  enum TaggingPoint {LOOSE, MEDIUM, TIGHT, ANTILOOSE, NOTAG}; 
}

class JetTagRescaler { 
public: 
  // Constructed from a textstream. The columns must be: 
  //  - numerator mc name
  //  - denominator mc name
  //  - flavor {tau, charm, light, bottom} for sf
  //  - operating point (TaggingPoint)
  //  - lower pt bound for correction (in GeV)
  //  - upper pt bound for correction (in GeV)
  //  - correction factor
  //  - correction factor error
  JetTagRescaler(std::istream& in_file); 

  // you can also construct from the file path (calls above)
  JetTagRescaler(const std::string& file_path); 
  
  // if the flavor / operating point can't be found return this number
  // with no error. By default we'll just throw an exception. 
  void set_dummy(double, double error=0.0); 

  // Convenience method, since we probably won't use errors. 
  // Expects pt in MeV. 
  double get_sf(double pt, int flavor_truth_label, 
		jettag::TaggingPoint) const; 
  
  // get the sf and error as a std::pair<double,double>, 
  // expects pt in MeV. 
  // NOTE: Will return the lower bin for pt below the lowest bin, 
  //       likewise will return the upper bin for pt above the highest. 
  std::pair<double,double> get_sf_err(double pt, int flavor_truth_label, 
				      jettag::TaggingPoint) const; 

private: 
  void init(std::istream& in_file); 
  typedef std::pair<double, double> SfAndError; 
  typedef std::map<double, SfAndError > PtMap; 
  typedef std::map<int, PtMap> TruthMap; 
  typedef std::map<jettag::TaggingPoint, TruthMap> OPMap; 
  std::pair<double,double> m_dummy_value; 
  bool m_return_dummy; 
  jettag::TaggingPoint tp_from_str(std::string); 
  int truth_label_from_str(std::string); 
  OPMap m_op_map; 
}; 

#endif 

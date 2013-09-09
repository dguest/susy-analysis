#include "JetTagRescaler.hh"
#include <sstream>
#include <stdexcept> 
#include <fstream>

JetTagRescaler::JetTagRescaler(std::istream& in_file): 
  m_return_dummy(false)
{
  init(in_file); 
}

JetTagRescaler::JetTagRescaler(const std::string& in_path): 
  m_return_dummy(false)
{ 
  std::ifstream in_file(in_path); 
  init(in_file); 
}
void JetTagRescaler::init(std::istream& in_file)
{ 
  std::string line; 
  double pt_low;       // not used, but could be for error checks
  double pt_high;      // locate the pt bin with this number
  double correction;   
  double error; 		
  std::string flavor; 
  std::string op; 

  // these aren't used, but they may be if we decide to use multiple 
  // scalefactors across different samples, so the columns exist in the 
  // textfile. 
  std::string numerator; 	// the MC the b-tagging group calibrated for
  std::string denominator; 	// the MC we're using 


  while (std::getline(in_file, line)) { 
    if (line.at(0) == '#') continue; 
    std::stringstream stream(line); 
    if (! ( stream >> numerator >> denominator >> flavor >> op >> pt_low >> 
	    pt_high >> correction >> error)) { 
      throw std::runtime_error
	("couldn't parse line in flavor calibrator: \n" + line); 
    }
    jettag::TaggingPoint tagging_pt = tp_from_str(op);
    int flavor_truth_label = truth_label_from_str(flavor); 
    std::pair<double, double> cor_err(correction, error); 
    if (m_op_map[tagging_pt][flavor_truth_label][pt_high * 1e3].first != 0.0){
      throw std::runtime_error("tried to redefine scalefactor "
			       "(in " __FILE__")"); 
    }
      
    m_op_map[tagging_pt][flavor_truth_label][pt_high * 1e3] = cor_err; 
  }
  if (m_op_map.size() == 0) { 
    throw std::runtime_error("no jet tag rescaler info parsed"); 
  }
}

void JetTagRescaler::set_dummy(double value, double error) { 
  m_return_dummy = true; 
  m_dummy_value = std::make_pair(value, error);  
}

double JetTagRescaler::get_sf(double pt, int flavor_truth_label, 
			      jettag::TaggingPoint tp) const { 
  return get_sf_err(pt, flavor_truth_label, tp).first; 
}

std::pair<double, double> JetTagRescaler
::get_sf_err(double pt, int truth_label, jettag::TaggingPoint tp) const 
{ 
  if (tp == jettag::NOTAG) return std::make_pair(1.0,0); 
  OPMap::const_iterator flavor_map =  m_op_map.find(tp); 
  if (flavor_map == m_op_map.end()) { 
    if (m_return_dummy) { 
      return m_dummy_value; 
    }
    throw std::logic_error("can't find operating point in " __FILE__); 
  }
  TruthMap::const_iterator pt_map = flavor_map->second.find(truth_label); 
  if (pt_map == flavor_map->second.end() ) {
    if (m_return_dummy) { 
      return m_dummy_value; 
    }
    throw std::runtime_error("can't find flavor in " __FILE__); 
  }

  double min_pt = pt_map->second.begin()->first; 
  double max_pt = pt_map->second.rbegin()->first; 

  if (pt < min_pt) { 
    return pt_map->second.begin()->second; 
  }
  else if (pt >= max_pt) { 
    return pt_map->second.rbegin()->second; 
  }

  PtMap::const_iterator sf_err = pt_map->second.upper_bound(pt); 
  if (sf_err == pt_map->second.end()) { 
    if (m_return_dummy) { 
      return m_dummy_value; 
    }
    throw std::runtime_error("can't find pt bin in " __FILE__); 
  }
  return sf_err->second; 
}

int JetTagRescaler::truth_label_from_str(std::string flavor) { 
  if (flavor == "bottom") { 
    return 5; 
  } else if (flavor == "charm") { 
    return 4; 
  } else if (flavor == "light") { 
    return 0; 
  } else if (flavor == "tau") { 
    return 15; 
  } else { 
    throw std::runtime_error("got unknown flavor string while"
			     " parsing calibration file: " + flavor); 
  }
}

jettag::TaggingPoint JetTagRescaler::tp_from_str(std::string op) { 
  using namespace jettag; 
  if (op == "loose") { 
    return LOOSE; 
  }
  else if (op == "medium") { 
    return MEDIUM; 
  }
  else if (op == "tight") { 
    return TIGHT; 
  }
  else if (op == "antiloose") { 
    return ANTILOOSE; 
  }
  else { 
    throw std::runtime_error("not sure what to do with OP " + op + 
			     " in " __FILE__); 
  }
}

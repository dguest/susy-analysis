#ifndef JET_BUFFER_HH
#define JET_BUFFER_HH

#include <string>
#include <map>
#include <vector>

class BtagScaler; 
class BtagBuffer; 
class TTree; 
class Jet; 

class JetBuffer
{ 
public: 
  JetBuffer(); 
  ~JetBuffer(); 
  void set_buffer(TTree* tree, const std::string& base_name); 
  bool has_jet() const; 
  Jet jet() const; 
private: 
  friend class Jet; 
  double pt; 
  double eta; 
  double phi; 
  double E; 
  double jfc_b; 
  double jfc_c; 
  double jfc_u; 
  int flavor_truth_label; 
  BtagBuffer* m_medium_sf; 
  BtagBuffer* m_loose_sf; 
  bool m_has_truth; 
  bool m_is_electron_jet; 
};

#endif	// JET_BUFFER_HH

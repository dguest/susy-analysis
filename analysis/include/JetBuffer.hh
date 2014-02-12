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
  double m_pt; 
  double m_eta; 
  double m_phi; 
  double m_E; 
  double m_jfc_b; 
  double m_jfc_c; 
  double m_jfc_u; 
  int m_flavor_truth_label; 
  BtagBuffer* m_medium_sf; 
  BtagBuffer* m_loose_sf; 
  bool m_has_truth; 
  bool m_is_electron_jet; 
};

#endif	// JET_BUFFER_HH

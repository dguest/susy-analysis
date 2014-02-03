#ifndef MC_PARTICLE_BUFFER_HH
#define MC_PARTICLE_BUFFER_HH

class SmartChain; 
// needed for boson filter

#include <vector>

struct McParticleBuffer { 
  McParticleBuffer(); 
  void set_branches(SmartChain*); 
  bool has_skimmed_info() const; 

  // may be calculated in the skim
  float skimmed_boson_pt; 

  // if not, fall back to these
  int mc_n; 
  std::vector<float>* mc_pt; 
  std::vector<float>* mc_eta; 
  std::vector<float>* mc_phi; 
  std::vector<float>* mc_m; 
  std::vector<int>* mc_status; 
  std::vector<int>* mc_pdgId; 
private: 
  bool m_has_skimmed_info; 
};

#endif 

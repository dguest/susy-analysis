#ifndef JET_BUFFER_HH
#define JET_BUFFER_HH

#include <string>
#include <map>
#include <vector>

class BtagScaler; 
class BtagBuffer; 

struct JetBuffer
{ 
  double pt; 
  double eta; 
  double phi; 
  double E; 
  double jfc_b; 
  double jfc_c; 
  double jfc_u; 
  int flavor_truth_label; 
  std::map<std::string, BtagBuffer*> btag_buffers; 
  // TODO: the btag scalers  shoudldn't be owned by the buffer. 
  std::vector<BtagScaler*> btag_scalers; 
  bool is_electron_jet; 
  JetBuffer(); 
  ~JetBuffer(); 
};

#endif	// JET_BUFFER_HH

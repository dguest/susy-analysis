#include "JetBuffer.hh"
#include "BtagScaler.hh"
#include "BtagBuffer.hh"

JetBuffer::JetBuffer(): 
  flavor_truth_label(-1), 
  is_electron_jet(false)
{ 
}
JetBuffer::~JetBuffer() { 
  for (auto itr = btag_scalers.begin(); itr != btag_scalers.end(); itr++) { 
    delete *itr; 
    *itr = 0; 
  }
  for (auto itr = btag_buffers.begin(); itr != btag_buffers.end(); itr++) { 
    delete itr->second; 
    itr->second = 0; 
  }
}

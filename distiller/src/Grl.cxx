#include "Grl.hh"

#include <cassert> 

#include "GoodRunsLists/TGRLCollection.h"
#include "GoodRunsLists/TGoodRunsListReader.h"

Grl::Grl(std::string grl_location): 
  m_grl(0)
{ 
  if (grl_location.size() == 0) { 
    return; 
  }
  Root::TGoodRunsListReader reader(grl_location.c_str(), true); 
  reader.AddXMLFile(grl_location.c_str()); 
  reader.Interpret(); 
  m_grl = new Root::TGRLCollection(reader.GetMergedGRLCollection());
}

Grl::~Grl() { 
  delete m_grl; 
  m_grl = 0; 
}

bool Grl::has_lb(int run_number, int lbn){ 
  assert(m_grl); 
  return m_grl->HasRunLumiBlock(run_number, lbn); 
}

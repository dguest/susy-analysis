#include "Grl.hh"

#include <cassert>

#include "GoodRunsLists/TGRLCollection.h"
#include "GoodRunsLists/TGoodRunsListReader.h"

Grl::Grl(std::string grl_location):
  m_grl(0)
{
  assert(grl_location.size());

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
  return m_grl->HasRunLumiBlock(run_number, lbn);
}

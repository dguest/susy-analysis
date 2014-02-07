#include "SkimCounts.hh"
#include "hdf_helpers.hh"

#include "H5Cpp.h"

#include "TFile.h"
#include "TParameter.h"

namespace { 
  const std::string TOTAL_EVT = "total_events"; 
  const std::string TOTAL_WT = "total_event_weight"; 
  const std::string META = "meta"; 
}

SkimCounts::SkimCounts(TFile& file): 
  m_has_total_event_weight(false)
{ 
  auto total_evt = dynamic_cast<TParameter<long long>* >(
    file.Get(TOTAL_EVT.c_str())); 
  if (total_evt) { 
    m_total_events = total_evt->GetVal(); 
  } else { 
    m_total_events = -1; 
  }
  auto total_wt = dynamic_cast<TParameter<double>* > (
    file.Get(TOTAL_WT.c_str())); 
  if (total_wt) { 
    m_total_event_weight = total_wt->GetVal(); 
    m_has_total_event_weight = true; 
  } else { 
    m_total_event_weight = 0; 
  }
}




void SkimCounts::write_to(H5::CommonFG& fg) const { 
  using namespace H5; 
  Group meta(fg.createGroup(META)); 
  h5::write_attr(meta, TOTAL_EVT, &m_total_events); 
  if (m_has_total_event_weight) { 
    h5::write_attr(meta, TOTAL_WT, &m_total_event_weight); 
  }
}

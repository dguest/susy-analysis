#include "Skimmer.hh"
#include "SusyBuffer.h"

#include "TChain.h"
#include "TFile.h"
#include "TError.h"
#include "TParameter.h"

#include <set>

Skimmer::Skimmer(const std::vector<std::string>& vars): 
  m_chain(0), 
  m_skimmed_var_prefix("skimmed_")
{ 
  m_chain = new TChain("susy","susy"); 
  
  std::set<std::string> ps_vars; 
  ps_vars.insert("mcevt_weight"); 
  
  for (std::vector<std::string>::const_iterator itr = vars.begin(); 
       itr != vars.end(); itr++) {
    if (ps_vars.count(*itr)) { 
      printf("%s is used in skim computation, won't include in output\n", 
	     itr->c_str()); 
    } else { 
      m_variables.push_back(*itr); 
    }
  }
}

Skimmer::~Skimmer() { 
  // dont' really know the proper cleanup for root anything...
  delete m_chain; 
}

void Skimmer::addFile(const std::string& file_name) { 
  
  m_chain->Add(file_name.c_str()); 
}

void Skimmer::makeSkim(const std::string& out_file_name) { 

  TFile output_file(out_file_name.c_str(), "recreate"); 
  printf("adding variables\n"); 
  TTree* out_tree = copyChain(output_file); 
  printf("computing varaibles\n"); 
  output_file.cd(); 
  addMCVairablesTo(out_tree, &output_file); 
  out_tree->Write(); 
}

// ----- private ----

const char* Skimmer::pfx(const std::string& word) { 
  return (m_skimmed_var_prefix + word).c_str(); 
}

// copy all the m_variables to the output_file
TTree* Skimmer::copyChain(TFile& output_file) { 
  m_chain->SetBranchStatus("*", 0);
  for (std::vector<std::string>::const_iterator itr = m_variables.begin(); 
       itr != m_variables.end(); itr++) { 
    unsigned branches_found = 0; 
    m_chain->SetBranchStatus(itr->c_str(), 1, &branches_found);
    if (branches_found != 1) { 
      printf("%s not found...\n", itr->c_str()); 
    }
  }

  int old_error_level = gErrorIgnoreLevel; 
  gErrorIgnoreLevel = kFatal; 
  TTree* out_tree = m_chain->CloneTree(); 
  gErrorIgnoreLevel = old_error_level; 
  return out_tree; 
}


void Skimmer::addMCVairablesTo(TTree* output_tree, TFile* file) { 
  m_chain->SetBranchStatus("*", 0);
  SusyBuffer buffer(*m_chain, std::vector<std::string>()); 
  double event_wt = 0; 
  TBranch* wt_branch = output_tree->Branch(pfx("mcevt_weight"), &event_wt); 

  double total_event_weight = 0; 
  const int n_entries = m_chain->GetEntries(); 
  for (int entry_n = 0; entry_n < n_entries; entry_n++) { 
    m_chain->GetEntry(entry_n); 
    event_wt = buffer.mcevt_weight->at(0).at(0); 
    total_event_weight += event_wt; 
    wt_branch->Fill(); 
  }
  if (file) { 
    TParameter<double> skim_total(pfx("total_event_weight"), 
				  total_event_weight); 
    file->WriteTObject(&skim_total); 
  }

}

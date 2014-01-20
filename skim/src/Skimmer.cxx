#include "Skimmer.hh"
#include "SusyBuffer.hh"

#include "TChain.h"
#include "TFile.h"
#include "TError.h"
#include "TROOT.h"  
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
  TTree* out_tree = new TTree("susy","susy"); 
  printf("computing varaibles\n"); 
  output_file.cd(); 
  copyVariablesTo(out_tree, &output_file); 
  out_tree->Write(); 
}

// ----- private ----

const char* Skimmer::pfx(const std::string& word) { 
  return (m_skimmed_var_prefix + word).c_str(); 
}

void Skimmer::copyVariablesTo(TTree* output_tree, TFile* file) { 
  m_chain->SetBranchStatus("*", 0);
  SusyBuffer buffer(*m_chain, m_variables); 
  buffer.setPassThrough(*output_tree); 
  double event_wt = 0; 
  output_tree->Branch(pfx("mcevt_weight"), &event_wt); 

  double total_event_weight = 0; 
  const int n_entries = m_chain->GetEntries(); 
  for (int entry_n = 0; entry_n < n_entries; entry_n++) { 
    m_chain->GetEntry(entry_n); 
    // buffer.printSizes(); 
    event_wt = buffer.mcevt_weight->at(0).at(0); 
    total_event_weight += event_wt; 
    output_tree->Fill(); 
  }
  if (file) { 
    TParameter<double> skim_total(pfx("total_event_weight"), 
				  total_event_weight); 
    file->WriteTObject(&skim_total); 
  }

}


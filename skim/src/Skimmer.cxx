#include "Skimmer.hh"
#include "SusyBuffer.hh"
#include "boson_truth_tools.hh"
#include "constants_skim.hh"

#include "TChain.h"
#include "TFile.h"
#include "TError.h"
#include "TROOT.h"  
#include "TParameter.h"

#include <set>
#include <cmath>

namespace { 
  bool hasTriggerRequirements(const SusyBuffer&); 
  void dumpMissing(const SusyBuffer& buffer); 
}

Skimmer::Skimmer(const std::vector<std::string>& vars): 
  m_chain(0), 
  m_variables(vars), 
  m_skimmed_var_prefix("skimmed_")
{ 
  m_chain = new TChain("susy","susy"); 
}

Skimmer::~Skimmer() { 
  // dont' really know the proper cleanup for root anything...
  delete m_chain; 
}

void Skimmer::addFile(const std::string& file_name) 
{ 
  m_chain->Add(file_name.c_str()); 
}

void Skimmer::makeSkim(const std::string& out_file_name) { 

  TFile output_file(out_file_name.c_str(), "recreate"); 
  TTree* out_tree = new TTree("susy","susy"); 
  printf("computing varaibles\n"); 
  output_file.cd(); 

  // this block to shut root up before running the copy 
  int old_error_level = gErrorIgnoreLevel; 
  gErrorIgnoreLevel = kFatal; 
  copyVariablesTo(out_tree, &output_file); 
  gErrorIgnoreLevel = old_error_level; 

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
  float boson_pt = -1.0; 
  if (buffer.hasMc()) {
    output_tree->Branch(pfx("mcevt_weight"), &event_wt); 
    output_tree->Branch(pfx("boson_pt"), &boson_pt); 
  }
  
  double total_event_weight = 0; 
  bool has_bosons = buffer.hasMc(); 
  
  const int n_entries = m_chain->GetEntries(); 
  for (int entry_n = 0; entry_n < n_entries; entry_n++) { 
    m_chain->GetEntry(entry_n); 

    if (buffer.hasMc()) { 
      if (has_bosons) { 
	try {
	  boson_pt = get_boson_truth_pt(buffer); 
	} catch (BosonError& err) { 
	  has_bosons = false; 
	}
      }
      event_wt = buffer.mcevt_weight->at(0).at(0); 
      total_event_weight += event_wt; 
    }
    if (hasTriggerRequirements(buffer)) { 
      output_tree->Fill(); 
    }
  }
  if (file) { 
    TParameter<double> skim_total("total_event_weight", total_event_weight); 
    TParameter<bool> bosons_found("bosons_found", has_bosons);
    if (buffer.hasMc()) { 
      file->WriteTObject(&skim_total); 
      file->WriteTObject(&bosons_found); 
    }
  }
  dumpMissing(buffer); 

}

namespace { 
  bool hasTriggerRequirements(const SusyBuffer& buf) { 
    if (buf.mu18_tight_mu8_EFFS || buf.mu24i_tight || buf.mu36_tight) { 
      return true; 
    }
    if (buf.xe80_tclcw_tight || 
	buf.xe80T_tclcw_loose || 
	buf.xe80_tclcw_loose) { 
      float mx2 = std::pow(buf.met_etx - buf.met_muon_etx,2); 
      float my2 = std::pow(buf.met_ety - buf.met_muon_ety,2); 
      float threshold = skim::MET_REQUIREMENT; 
      if (mx2 + my2 > threshold*threshold) { 
	return true; 
      }
    }
    return false; 
  }
  void dumpMissing(const SusyBuffer& buffer) { 

    std::set<std::string> missing = buffer.getMissingBranches(); 
    if (missing.size() > 0) puts("======= missing branches ======="); 
    for (std::set<std::string>::const_iterator itr = missing.begin(); 
	 itr != missing.end(); itr++ ) { 
      puts(itr->c_str()); 
    }
  }
}

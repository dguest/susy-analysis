#include "Skimmer.hh"
#include "SusyBuffer.hh"
#include "boson_truth_tools.hh"
#include "constants_skim.hh"
#include "misc_func.hh" // TolerableDataError

#include "TChain.h"
#include "TFile.h"
#include "TError.h"
#include "TROOT.h"  
#include "TParameter.h"
#include "TH1.h"

#include <set>
#include <cmath>
#include <memory>

namespace { 
  bool hasTriggerRequirements(const Triggers&, const Met&); 
  bool hasWillsTriggers(const Triggers& tr, const Met& met); 
  void dumpMissing(const SusyBuffer& buffer); 
  long long entriesInTree(const std::string& file, 
			  const std::string& tree_name); 
}

Skimmer::Skimmer(const std::vector<std::string>& vars): 
  m_chain(0), 
  m_variables(vars), 
  m_skimmed_var_prefix("skimmed_"), 
  m_chain_name("susy"), 
  m_collection_tree_name("CollectionTree"), 
  m_collection_tree_events(0), 
  m_fast(false)
{ 
  m_chain = new TChain(m_chain_name.c_str(), m_chain_name.c_str()); 
}

Skimmer::~Skimmer() { 
  // dont' really know the proper cleanup for root anything...
  delete m_chain; 
}

void Skimmer::runFast() { 
  m_fast = true; 
}

void Skimmer::addFile(const std::string& file_name) 
{ 
  if (!entriesInTree(file_name, m_chain_name)) { 
    printf("empty tree in %s, skipping...\n", file_name.c_str()); 
    return; 
  }
  m_chain->Add(file_name.c_str(), -1); 
  m_collection_tree_events += entriesInTree(file_name, 
					    m_collection_tree_name); 
}

void Skimmer::makeSkim(const std::string& out_file_name) { 

  TFile output_file(out_file_name.c_str(), "recreate"); 
  TTree* out_tree = new TTree("susy","susy"); 
  printf("making skim %s\n", out_file_name.c_str()); 
  output_file.cd(); 

  // this block to shut root up before running the copy 
  int old_error_level = gErrorIgnoreLevel; 
  // gErrorIgnoreLevel = kFatal; 
  try { 
    copyVariablesTo(out_tree, &output_file); 
  } catch (TolerableDataError& err) { 
    printf("Tolerable error: %s\n",err.what()); 
  }
  gErrorIgnoreLevel = old_error_level; 

  out_tree->Write(); 
}

// ----- private ----

std::string Skimmer::pfx(const std::string& word) { 
  return m_skimmed_var_prefix + word; 
}

void Skimmer::copyVariablesTo(TTree* output_tree, TFile* file) { 
  m_chain->SetBranchStatus("*", 0);
  SusyBuffer buffer(*m_chain, m_variables); 
  buffer.setPassThrough(*output_tree); 

  float event_wt = 0; 
  float boson_pt = -1.0; 
  if (buffer.hasMc()) {
    output_tree->Branch(pfx("mcevt_weight").c_str(), &event_wt); 
    output_tree->Branch(pfx("boson_pt").c_str(), &boson_pt); 
  }

  SummaryParameters summary(buffer.hasMc()); 
  
  const int n_entries = m_chain->GetEntries(); 
  summary.total_events = n_entries; 
  summary.collection_tree_events = m_collection_tree_events; 
  for (int entry_n = 0; entry_n < n_entries; entry_n++) { 
    m_chain->GetEntry(entry_n); 

    // buffer.dump(); 

    if (summary.has_mc) { 
      if (summary.has_bosons) { 
	try {
	  boson_pt = get_boson_truth_pt(buffer); 
	} catch (BosonError& err) { 
	  summary.has_bosons = false; 
	}
      }
      event_wt = buffer.mcevt_weight->at(0).at(0); 
      summary.total_event_weight += event_wt; 
    }
    bool trigger = hasWillsTriggers(buffer.triggers, buffer.met); 
    // bool trigger = hasTriggerRequirements(buffer.triggers, buffer.met); 
    if (trigger) { 
      summary.skimmed_events++; 
      output_tree->Fill(); 
    }
    if (m_fast) break; 
  }
  if (file) { 
    summary.writeTo(*file); 
  }
  dumpMissing(buffer); 
  output_tree->ResetBranchAddresses(); 

}

namespace { 
  long long entriesInTree(const std::string& fname, const std::string& tree){ 
    std::unique_ptr<TFile> file(TFile::Open(fname.c_str())); 
    if (!file) throw std::runtime_error("no file: " + fname);
    if (!file->IsOpen() || file->IsZombie()) { 
      throw std::runtime_error("bad file: " + fname); 
    }
    TTree* the_tree = dynamic_cast<TTree*>(file->Get(tree.c_str())); 
    if (!the_tree) { 
      throw std::runtime_error("missing tree " + tree + " in " + fname); 
    } 
    return the_tree->GetEntries(); 
  }

  bool hasTriggerRequirements(const Triggers& tr, const Met& met) { 
    // ACHTUNG: should update these with some thresholds on reco quantities
    if (tr.EF_mu18_tight_mu8_EFFS || 
    	tr.EF_mu24i_tight || 
    	tr.EF_mu36_tight ||
    	tr.EF_e24vhi_medium1 ||
    	tr.EF_e60_medium1 ||
    	tr.EF_2e12Tvh_loose1) { 
      return true; 
    }
    constexpr float threshold2 = std::pow(skim::MET_REQUIREMENT,2); 
    float met_x2 = std::pow(met.etx - met.muon_etx,2); 
    float met_y2 = std::pow(met.ety - met.muon_ety,2); 
    if (
      tr.EF_xe80_tclcw_tight || 
      tr.EF_xe80T_tclcw_loose || 
      tr.EF_xe80_tclcw_loose ||
      false) { 
      if (met_x2 + met_y2 > threshold2) { 
    	return true; 
      }
    }
    return false; 
  }
  void dumpMissing(const SusyBuffer& buffer) { 

    std::set<std::string> missing = buffer.getMissingBranches(); 
    if (missing.size() > 0) puts("======= missing branches ======="); 
    for (const auto itr: missing) { 
      puts(itr.c_str()); 
    }
  }
  // these should work fine, if I remember to set the fucking things...
  bool hasWillsTriggers(const Triggers& tr, const Met& met) { 
    if (
      tr.EF_xe80T_tclcw_loose ||
      tr.EF_xe80_tclcw_loose || 
      tr.EF_xe80T_tclcw || // not sure if needed
      tr.EF_xe80_tclcw_tight || 
      tr.EF_e24vhi_medium1 || 
      tr.EF_2e12Tvh_loose1 || 
      tr.EF_mu18_tight_mu8_EFFS || 
      tr.EF_mu24i_tight || 
      tr.EF_e60_medium1 ||
      tr.EF_mu36_tight ||
      false) { 	// false here just to make above lines consistent 
      return true; 
    }
    return false; 
  }

}

SummaryParameters::SummaryParameters(bool has_mc) : 
  total_event_weight(0), 
  collection_tree_events(0),
  total_events(0), 
  skimmed_events(0), 
  has_bosons(has_mc), 
  has_mc(has_mc)
{ 
}

namespace {
  void setBin(int bin_n, double value, const std::string& name, TH1& hist) {
    hist.SetBinContent(bin_n, value);
    hist.GetXaxis()->SetBinLabel(bin_n, name.c_str());
  }
}

void SummaryParameters::writeTo(TFile& file) const { 
  TParameter<long long> events("total_events", total_events); 
  TParameter<long long> skimmed("skimmed_events", skimmed_events); 
  TParameter<long long> col_tree("collection_tree_events", 
				 collection_tree_events); 
  file.WriteTObject(&events); 
  file.WriteTObject(&skimmed); 
  file.WriteTObject(&col_tree); 

  TH1D entry_hist("skim_counts", "skim_counts", 3, 0, 3);
  setBin(1, total_events, "total_events", entry_hist);
  setBin(2, skimmed_events, "skimmed_events", entry_hist);
  setBin(3, collection_tree_events, "collection_tree_events", entry_hist);
  file.WriteTObject(&entry_hist);

  if (has_mc) { 
    TParameter<double> skim_total("total_event_weight", total_event_weight); 
    TParameter<bool> bosons_found("bosons_found", has_bosons);
    file.WriteTObject(&skim_total); 
    file.WriteTObject(&bosons_found); 

    TH1D mc_hist("mc_counts","mc_counts", 1, 0, 1);
    setBin(1, total_event_weight, "total_event_weight", mc_hist);
    file.WriteTObject(&mc_hist);
  }

}

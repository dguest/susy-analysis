#include "CutflowBuilder.hh"
#include "HistBuilderFlags.hh"
#include "CutAugmenter.hh"

#include <string> 
#include <stdexcept>
#include <boost/format.hpp>
#include <algorithm>
#include <iostream> 
#include <ostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"

CutflowBuilder::CutflowBuilder(std::string input, const unsigned flags) : 
  m_flags(flags), 
  m_file(0), 
  m_tree(0), 
  m_mask(0), 

  m_pass_bits(0), 
  m_hfor_type(0), 
  
  m_cut_augmenter(0)
{ 
  m_file = new TFile(input.c_str()); 
  if (!m_file) { 
    throw std::runtime_error(input + " could not be foune"); 
  }
  m_tree = dynamic_cast<TTree*>(m_file->Get("evt_tree")); 
  if (!m_tree) { 
    throw std::runtime_error("evt_tree could not be found"); 
  }
  int errors = 0; 
  m_tree->SetBranchStatus("*",0); 
  m_tree->SetBranchStatus("pass_bits",1); 
  errors += m_tree->SetBranchAddress("pass_bits",&m_pass_bits); 
  m_tree->SetBranchStatus("hfor_type",1); 
  errors += m_tree->SetBranchAddress("hfor_type",&m_hfor_type); 
  if (errors) { 
    throw std::runtime_error
      ((boost::format("%i branch setting errors") % errors).str()); 
  }

}

CutflowBuilder::~CutflowBuilder() { 
  delete m_file; 
}

void CutflowBuilder::add_cut_mask(std::string, unsigned bits) 
{
  m_mask |= bits; 
}

int CutflowBuilder::build() { 

  const int n_entries = m_tree->GetEntries(); 
  const int one_percent = n_entries / 100; 

  std::ofstream nullstream("/dev/null"); 
  std::streambuf* out_buffer = nullstream.rdbuf(); 
  if (m_flags & buildflag::verbose) { 
    out_buffer = std::cout.rdbuf(); 
  }
  std::ostream outstream(out_buffer); 

  int n_pass = 0; 

  for (int entry = 0; entry < n_entries; entry++) { 
    if (entry % one_percent == 0 || entry == n_entries - 1 ) { 
      outstream << boost::format("\r%i of %i (%.1f%%) processed") % 
	entry % n_entries % ( entry / one_percent); 
      outstream.flush(); 
    }
    m_tree->GetEntry(entry); 
    if (m_hfor_type == 4) continue; 
    if ( (m_pass_bits & m_mask) == m_mask) { 
      n_pass++; 
    }
  }

  outstream << "\n";
  outstream.flush(); 

  return n_pass; 
}

void CutflowBuilder::save(std::string output) { 
  if (output.size() == 0) { 
    return; 
  }

}

void CutflowBuilder::set_float(std::string name, double value) { 
  if (!m_cut_augmenter) { 
    m_cut_augmenter = new CutAugmenter; 
  }
  m_cut_augmenter->set_float(name, value); 
  assert(false); 
}; 


#include "SmartChain.hh"
#include "TChain.h"
#include "TFile.h"
#include <boost/format.hpp>


SmartChain::SmartChain(std::string tree_name): 
  TChain(tree_name.c_str()), 
  m_last_tree(-1)
{ 
}

int SmartChain::Add(std::string file_name, int nentries) { 
  m_files.push_back(file_name); 
  TFile file(file_name.c_str()); 
  if (!file.IsOpen() || file.IsZombie()) { 
    throw std::runtime_error("bad file: " + file_name); 
  }
  return TChain::Add(file_name.c_str(), nentries); 
}
int SmartChain::GetEntry(int entry_n) { 
  int return_val = TChain::GetEntry(entry_n); 
  int this_tree_n = GetTreeNumber(); 
  if (this_tree_n != m_last_tree) { 
    for (Strings::const_iterator br_itr = m_set_branches.begin(); 
	 br_itr != m_set_branches.end(); br_itr++) { 
      if (!GetBranch(br_itr->c_str())) { 
	throw_bad_branch(*br_itr); 
      }
    }
    m_last_tree = this_tree_n; 
  }
  return return_val; 
}

std::vector<std::string> SmartChain::get_all_branch_names() const { 
  return m_set_branches; 
}
std::string SmartChain::get_current_file() const { 
  assert(m_last_tree < int(m_files.size())); 
  return m_files.at(m_last_tree); 
}

void SmartChain::SetBranchAddressPrivate(std::string name, void* branch, 
					 bool turnon) { 

  if (turnon) { 
    unsigned branches_found = 0; 
    SetBranchStatus(name.c_str(), 1, &branches_found); 
    if (branches_found != 1) { 
      std::string prob = (boost::format("missing branch: %s") % name).str();
      throw std::runtime_error(prob);
    }
  }
  int return_code = TChain::SetBranchAddress(name.c_str(), branch); 
  if (return_code != 0 && return_code != 5 ){ 
    std::string issue = (boost::format("can not set %s , return code %i") % 
			 name % return_code).str(); 
    throw std::runtime_error(issue); 
  }
  if (!GetBranch(name.c_str())) { 
    throw_bad_branch(name); 
  }

  m_set_branches.push_back(name);

  if (m_set_branch_set.count(name)) { 
    std::string err = (boost::format("setting branch address: %s is already"
				     " set in %s") % name % GetName()).str(); 
    throw std::runtime_error(err); 
  }
  m_set_branch_set.insert(name); 
}


void SmartChain::throw_bad_branch(std::string name) const { 
  std::string issue = (boost::format("can't find branch %s, ") % name).str(); 
  std::string file = GetFile()->GetName(); 
  issue.append("bad file: " + file); 
  throw std::runtime_error(issue); 

}

std::string SmartChain::get_files_string() const { 
  std::string issue = " files: "; 
  for (Strings::const_iterator itr = m_files.begin(); 
       itr != m_files.end(); itr++) {
    issue.append(*itr); 
    if (*itr != *m_files.rbegin()) issue.append(", "); 
  }
  return issue; 
}

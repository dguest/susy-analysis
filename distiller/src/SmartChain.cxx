#include "SmartChain.hh"
#include "TChain.h"
#include <boost/format.hpp>

SmartChain::SmartChain(std::string tree_name): 
  TChain(tree_name.c_str())
{ 
}

int SmartChain::Add(std::string file_name, int nentries) { 
  m_files.push_back(file_name); 
  return TChain::Add(file_name.c_str(), nentries); 
}

std::vector<std::string> SmartChain::get_all_branch_names() const { 
  return m_set_branches; 
}

void SmartChain::SetBranchAddressPrivate(std::string name, void* branch, 
					 bool turnon) { 

  if (turnon) { 
    SetBranchStatus(name.c_str(), 1); 
  }
  int return_code = TChain::SetBranchAddress(name.c_str(), branch); 
  if (return_code != 0 && return_code != 5 ){ 
    std::string issue = (boost::format("can not set %s , return code %i") % 
			 name % return_code).str(); 
    throw std::runtime_error(issue); 
  }
  if (!GetBranch(name.c_str())) { 
    std::string issue = (boost::format("can't find branch %s") % name).str(); 
    Strings bad_files = get_bad_files(); 
    if (bad_files.size()) issue.append(" bad files: "); 
    for (Strings::const_iterator itr = bad_files.begin(); 
	 itr != bad_files.end(); itr++) {
      issue.append(*itr); 
      if (*itr != *bad_files.rbegin()) issue.append(", "); 
    }
	 
    throw std::runtime_error(issue); 
  }

  m_set_branches.push_back(name);

  if (m_set_branch_set.count(name)) { 
    std::string err = (boost::format("setting branch address: %s is already"
				     " set in %s") % name % GetName()).str(); 
    throw std::runtime_error(err); 
  }
  m_set_branch_set.insert(name); 
}

std::vector<std::string> SmartChain::get_bad_files() const { 
  Strings bad_files; 
  for (Strings::const_iterator itr = m_files.begin(); 
       itr != m_files.end(); itr++) { 
    bad_files.push_back(*itr); 
  }
  return bad_files; 
}

#include "CollectionTreeReport.hh"

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"


CollectionTreeReport::CollectionTreeReport(std::string tree_name): 
  m_tree_name(tree_name), 
  m_total_entries(0), 
  m_n_errors(0), 
  m_n_files(0)
{
}

void CollectionTreeReport::add_files(std::vector<std::string> files) 
{
  for (StrItr itr = files.begin(); itr != files.end(); itr++) { 
    m_n_files++; 
    m_total_entries += get_entries_in_file(*itr); 
  }
}

int CollectionTreeReport::total_entries() const {return m_total_entries; }
int CollectionTreeReport::total_errors() const {return m_n_errors; }
int CollectionTreeReport::total_files() const {return m_n_files;}

int CollectionTreeReport::get_entries_in_file(std::string file_name){
  TFile file(file_name.c_str()); 
  if (file.IsZombie() || !file.IsOpen()) {
    m_n_errors++; 
    return 0; 
  }
  TTree* tree = dynamic_cast<TTree*>(file.Get(m_tree_name.c_str())); 
  if (!tree) { 
    m_n_errors++; 
    return 0; 
  }
  return tree->GetEntries(); 

}

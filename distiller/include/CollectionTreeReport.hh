#ifndef COLLECTION_TREE_REPORT_HH
#define COLLECTION_TREE_REPORT_HH

#include <string>
#include <vector> 

class CollectionTreeReport
{
public: 
  CollectionTreeReport(std::string tree_name = "CollectionTree"); 
  void add_files(std::vector<std::string>); 
  int total_entries() const; 
  int total_errors() const; 
  int total_files() const; 
private: 
  typedef std::vector<std::string>::const_iterator StrItr; 
  int get_entries_in_file(std::string); 
  std::string m_tree_name; 
  int m_total_entries; 
  int m_n_errors; 
  int m_n_files; 
}; 

#endif 

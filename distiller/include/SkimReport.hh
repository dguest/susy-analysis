#ifndef COLLECTION_TREE_REPORT_HH
#define COLLECTION_TREE_REPORT_HH

#include <string>
#include <vector>

class SkimReport
{
public:
  SkimReport();
  void add_files(const std::vector<std::string>&);
  int total_entries() const;
  int total_errors() const;
  int total_files() const;
  int empty_files() const;
  bool is_data() const;
  double sum_evt_weight() const;
private:
  void increment_with_file(const std::string&);
  bool m_is_data;
  double m_sum_evt_weight;
  int m_total_entries;
  int m_n_errors;
  int m_n_files;
  int m_n_empty;
};

#endif

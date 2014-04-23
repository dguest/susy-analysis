#include "SkimReport.hh"

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TParameter.h"


SkimReport::SkimReport():
  m_is_data(false),
  m_sum_evt_weight(0),
  m_total_entries(0),
  m_sum_collection_tree(0),
  m_n_errors(0),
  m_n_files(0),
  m_n_empty(0)

{
}

void SkimReport::add_files(const std::vector<std::string>& files)
{
  for (auto itr: files) {
    m_n_files++;
    increment_with_file(itr);
  }
}

int SkimReport::total_entries() const {return m_total_entries; }
int SkimReport::total_collection_tree() const {return m_sum_collection_tree;}
int SkimReport::total_errors() const {return m_n_errors; }
int SkimReport::total_files() const {return m_n_files;}
int SkimReport::empty_files() const {return m_n_empty;}
bool SkimReport::is_data() const {return m_is_data;}
double SkimReport::sum_evt_weight() const {return m_sum_evt_weight;}

void SkimReport::increment_with_file(const std::string& file_name){
  TFile file(file_name.c_str());
  if (file.IsZombie() || !file.IsOpen()) {
    m_n_errors++;
    return;
  }
  auto* collection_tree = dynamic_cast<TParameter<long long>*>(
    file.Get("collection_tree_events"));
  if (collection_tree) {
    m_sum_collection_tree += collection_tree->GetVal();
  }

  auto* events = dynamic_cast<TParameter<long long>*>(
    file.Get("total_events"));
  auto* skimmed = dynamic_cast<TParameter<long long>*>(
    file.Get("skimmed_events"));
  if (!events || !skimmed) {
    m_n_empty++;
    return;
  }
  m_total_entries += events->GetVal();

  auto* event_weight = dynamic_cast<TParameter<double>*>(
    file.Get("total_event_weight"));
  if (event_weight) {
    m_sum_evt_weight += event_weight->GetVal();
  } else {
    m_is_data = true;
  }

}

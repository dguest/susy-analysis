#include <boost/noncopyable.hpp>
#include <vector> 
#include <string>
#include "RunInfo.hh"		// could we remove this struct?  
// #include <fstream>

class TChain; 
class CollectionTreeReport; 
class SusyBuffer; 
class SUSYObjDef; 
class EventPreselector; 
class OutTree; 
class BitmapCutflow; 

namespace std { 
  class streambuf; 
  class ofstream; 
  class ostream; 
}

class StopDistiller : public boost::noncopyable
{ 
public: 
  typedef std::vector<std::pair<std::string, int> > Cutflow; 
  StopDistiller(const std::vector<std::string>& in, 
		const RunInfo& info, unsigned flags, 
		std::string out); 
  ~StopDistiller(); 
  Cutflow run_cutflow();
private: 
  typedef std::vector<SelectedJet*> Jets; 
  void setup_chain(const std::vector<std::string>& in); 
  void setup_susytools(); 
  void setup_streams(); 
  void configure_flags(); 
  void setup_outputs(); 
  void print_progress(int entry_n, std::ostream&); 
  void process_event(int entry_n, std::ostream&); 
  RunInfo m_info; 
  unsigned m_flags; 
  std::string m_out_ntuple_name; 
  std::string m_susy_dbg_file; 
  int m_n_entries; 
  int m_one_percent; 
  std::ofstream* m_norm_dbg_file; 
  std::ofstream* m_null_file; 
  TChain* m_chain; 
  CollectionTreeReport* m_ct_report; 
  SusyBuffer* m_susy_buffer; 
  SUSYObjDef* m_def; 
  EventPreselector* m_event_preselector; 
  OutTree* m_out_tree; 
  BitmapCutflow* m_cutflow; 
}; 

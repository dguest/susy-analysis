#ifndef STOP_DISTILLER_HH
#define STOP_DISTILLER_HH
#include <boost/noncopyable.hpp>
#include <vector> 
#include <string>
#include "RunInfo.hh"		// could we remove this struct?  
#include "typedefs.hh"

class SmartChain; 
class CollectionTreeReport; 
class SusyBuffer; 
class SUSYObjDef; 
class EventPreselector; 
namespace outtree { 
  class OutTree; 
}
class BitmapCutflow; 
class SelectedJet; 
class BtagCalibration; 
class OutputFilter; 

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
  void process_event(int entry_n, std::ostream&); 
  void setup_flags(); 
  void setup_streams(); 
  void setup_chain(const std::vector<std::string>& in); 
  void setup_susytools(); 
  void setup_outputs(); 

  void print_progress(int entry_n, std::ostream&); 
  RunInfo m_info; 
  const unsigned m_flags; 
  std::string m_out_ntuple_name; 
  std::string m_susy_dbg_file; 
  int m_n_entries; 
  int m_one_percent; 
  OutputFilter* m_output_filter; 
  std::ofstream* m_norm_dbg_file; 
  std::ofstream* m_null_file; 
  std::streambuf* m_debug_buffer; 
  SmartChain* m_chain; 
  CollectionTreeReport* m_ct_report; 
  SusyBuffer* m_susy_buffer; 
  SUSYObjDef* m_def; 
  EventPreselector* m_event_preselector; 
  outtree::OutTree* m_out_tree; 
  BitmapCutflow* m_cutflow; 
  BtagCalibration* m_btag_calibration; 
}; 

#endif

#ifndef STOP_DISTILLER_HH
#define STOP_DISTILLER_HH
#include <vector>
#include <string>
#include "RunInfo.hh"		// could we remove this struct?
#include "typedefs.hh"

class SmartChain;
class SkimReport;
class SusyBuffer;
class SUSYObjDef;
class EventPreselector;
class EventObjects;
namespace outtree {
  class OutTree;
}
class BitmapCutflow;
class CutCounter;
class SelectedJet;
class BtagCalibration;
class OutputFilter;
class BosonTruthFilter;
class TruthMetFilter;
class BosonPtReweighter;
class TVector2;

class StopDistiller
{
public:
  typedef std::vector<std::pair<std::string, double> > Cutflow;
  StopDistiller(const std::vector<std::string>& in,
		const RunInfo& info, unsigned flags);
  ~StopDistiller();
  StopDistiller(StopDistiller&) = delete;
  StopDistiller& operator=(StopDistiller&) = delete;
  Cutflow run_cutflow();
private:
  typedef std::vector<SelectedJet*> Jets;
  // top level function called on each event
  void process_event(int entry_n, std::ostream&);

  // various setup
  void check_flags();
  void setup_streams();
  void setup_chain(const std::vector<std::string>& in);
  void setup_susytools();
  void setup_outputs();
  void setup_cutflow(CutflowType config);

  std::vector<std::pair<std::string, double> > get_cutflow_vec(int errs) const;
  // called within process_event
  void fill_event_output(const EventObjects&, const TVector2& met,
			 const TVector2& alt_met,
			 outtree::OutTree&, BitmapCutflow* = 0) const;

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
  SkimReport* m_skim_report;
  SusyBuffer* m_susy_buffer;
  SUSYObjDef* m_def;
  EventPreselector* m_event_preselector;
  outtree::OutTree* m_out_tree;
  outtree::OutTree* m_mumet_out_tree;
  outtree::OutTree* m_leptmet_out_tree;
  BitmapCutflow* m_cutflow;
  CutCounter* m_object_counter;
  BtagCalibration* m_btag_calibration;
  BosonTruthFilter* m_boson_truth_filter;
  TruthMetFilter* m_truth_met_filter;
  BosonPtReweighter* m_boson_pt_reweighter;
};

#endif

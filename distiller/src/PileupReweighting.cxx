#include "PileupReweighting.hh"
#include "SusyBuffer.h"
#include "RunInfo.hh"
#include "PileupReweighting/TPileupReweighting.h"
#include "cutflag.hh"
#include "ScaleFactor.hh"

namespace {
  void set_prw(Root::TPileupReweighting* prw, const RunInfo& info,
	       float sf = PU_MU_BASE);
}

PileupReweighting::PileupReweighting(const RunInfo& info, unsigned run_flags):
  m_prw(0),
  m_prw_down(0),
  m_prw_up(0)
{
  m_generate = run_flags & cutflag::generate_pileup;
  m_prw = new Root::TPileupReweighting("PileupReweighting");
  m_prw_down = new Root::TPileupReweighting("PileupReweightingDown");
  m_prw_up = new Root::TPileupReweighting("PileupReweightingUp");

  bool disable_warnings = !(run_flags & cutflag::debug_susy);
  m_prw->DisableWarnings(disable_warnings);
  m_prw_down->DisableWarnings(disable_warnings);
  m_prw_up->DisableWarnings(disable_warnings);

  if (m_generate) {
    m_prw->UsePeriodConfig("MC12a");
  } else {
    set_prw(m_prw, info);
    set_prw(m_prw_up, info, PU_MU_BASE + PU_MU_ERR);
    set_prw(m_prw_down, info, PU_MU_BASE - PU_MU_ERR);
    m_prw_up->Initialize();
    m_prw_down->Initialize();
  }
  m_prw->Initialize();

}

PileupReweighting::~PileupReweighting() {
  delete m_prw;
  delete m_prw_down;
  delete m_prw_up;
  m_prw = 0;
  m_prw_down = 0;
  m_prw_up = 0;
}

unsigned PileupReweighting::random_run_number(const SusyBuffer& buf) const {
  m_prw->SetRandomSeed(
    314159 + buf.mc_channel_number * 2718 + buf.EventNumber);
  return m_prw->GetRandomRunNumber(buf.RunNumber);
}

ScaleFactor PileupReweighting::get_pileup_weight(const SusyBuffer& buffer) {
  float avx = buffer.average_int_per_xing();

  if (m_generate) {
    m_prw->Fill(
      buffer.RunNumber, buffer.mc_channel_number,
      buffer.mc_event_weight, avx);
    return {1.0,1.0,1.0};
  } else {
    ScaleFactor sf;
    sf.nominal = m_prw->GetCombinedWeight(
      buffer.RunNumber, buffer.mc_channel_number,avx);
    sf.down = m_prw_down->GetCombinedWeight(
      buffer.RunNumber, buffer.mc_channel_number,avx);
    sf.up = m_prw_up->GetCombinedWeight(
      buffer.RunNumber, buffer.mc_channel_number,avx);
    return sf;
  }
}

void PileupReweighting::write_to(const std::string& file_name) {
  m_prw->WriteToFile(file_name);
}

// helpers

namespace {
  void set_prw(Root::TPileupReweighting* prw, const RunInfo& info, float sf){
    prw->SetDefaultChannel(0); // this is what Brett does
    prw->AddConfigFile(info.pu_config);

    // this is taken from:
    // twiki.cern.ch/twiki/bin/view/AtlasProtected/ExtendedPileupReweighting#Recipe_A_MC12a_Pileup_Reweightin
    // and confirmed by Alex.
    prw->SetDataScaleFactors(1.0 / sf);

    prw->AddLumiCalcFile(info.pu_lumicalc);
    prw->MergeMCRunNumbers(195847,195848);
    prw->SetUnrepresentedDataAction(2);

  }
}


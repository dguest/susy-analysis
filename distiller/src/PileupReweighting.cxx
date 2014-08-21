#include "PileupReweighting.hh"
#include "SusyBuffer.h"
#include "RunInfo.hh"
#include "PileupReweighting/TPileupReweighting.h"
#include "cutflag.hh"
#include "ScaleFactor.hh"

PileupReweighting::PileupReweighting(const RunInfo& info, unsigned run_flags):
  m_prw(0)
{
  m_generate = run_flags & cutflag::generate_pileup;
  m_prw = new Root::TPileupReweighting("PileupReweighting");
  m_prw->DisableWarnings( !(run_flags & cutflag::debug_susy));
  if (m_generate) {
    m_prw->UsePeriodConfig("MC12a");
  } else {
    m_prw->SetDefaultChannel(0); // this is what Brett does
    m_prw->AddConfigFile(info.pu_config);

    // this is taken from:
    // twiki.cern.ch/twiki/bin/view/AtlasProtected/ExtendedPileupReweighting#Recipe_A_MC12a_Pileup_Reweightin
    // and confirmed by Alex.
    m_prw->SetDataScaleFactors(1/1.09);

    m_prw->AddLumiCalcFile(info.pu_lumicalc);
    m_prw->MergeMCRunNumbers(195847,195848);
    m_prw->SetUnrepresentedDataAction(2);
  }
  m_prw->Initialize();

}

PileupReweighting::~PileupReweighting() {
  delete m_prw;
  m_prw = 0;
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
    sf.down = sf.nominal = m_prw->GetCombinedWeight(
      buffer.RunNumber, buffer.mc_channel_number,avx * (1 - PU_MU_ERR));
    sf.up = sf.nominal = m_prw->GetCombinedWeight(
      buffer.RunNumber, buffer.mc_channel_number,avx * (1 + PU_MU_ERR));
    return sf;
  }
}

void PileupReweighting::write_to(const std::string& file_name) {
  m_prw->WriteToFile(file_name);
}

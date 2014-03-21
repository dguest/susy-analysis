#include "PileupReweighting.hh"
#include "SusyBuffer.h"
#include "RunInfo.hh"
#include "PileupReweighting/TPileupReweighting.h"
#include "cutflag.hh"

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
    m_prw->SetDataScaleFactors(1/1.11); // What Brett, Jan do
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

unsigned PileupReweighting::random_run_number(unsigned mc_run) const { 
  return m_prw->GetRandomRunNumber(mc_run);
}

float PileupReweighting::get_pileup_weight(const SusyBuffer& buffer) { 
  // apply hack from Will Butt's twiki: 
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/ExtendedPileupReweighting#Recipe_A_MC12a_Pileup_Reweightin
  // sets some values from 1 to 0
  bool need_hack = buffer.lbn==1 && int(buffer.averageIntPerXing+0.5)==1; 
  float avx = need_hack ? 0.0 : buffer.averageIntPerXing;

  if (m_generate) { 
    m_prw->Fill(
      buffer.RunNumber, buffer.mc_channel_number, 
      buffer.mc_event_weight, avx); 
    return 1.0; 
  } else { 
    return m_prw->GetCombinedWeight(
      buffer.RunNumber, buffer.mc_channel_number,avx);
  }
}

void PileupReweighting::write_to(const std::string& file_name) { 
  m_prw->WriteToFile(file_name); 
}

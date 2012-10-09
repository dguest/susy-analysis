#include "Jets.hh"
#include "SusyBuffer.h"
#include "constants.hh"
#include "RunInfo.hh"
#include "CutBits.hh"

#include "SUSYTools/SUSYObjDef.h"

#include <cassert> 

SelectedJet::SelectedJet(const EventJets* parent, int jet_index): 
  m_bits(0)
{ 
  const SusyBuffer& buffer = *parent->m_buffer; 
  double pt = buffer.jet_pt  ->at(jet_index); 
  double eta = buffer.jet_eta ->at(jet_index); 
  double phi = buffer.jet_phi ->at(jet_index); 
  double e = buffer.jet_E   ->at(jet_index); 
  SetPtEtaPhiE(pt,eta,phi,e); 
  m_jet_index = jet_index;

  m_jvf = buffer.jet_jvtxf->at(jet_index); 

  m_cnn_b = buffer.jet_flavor_component_jfitcomb_pb->at(jet_index); 
  m_cnn_c = buffer.jet_flavor_component_jfitcomb_pc->at(jet_index); 
  m_cnn_u = buffer.jet_flavor_component_jfitcomb_pu->at(jet_index); 
  m_flavor_truth_label = buffer.jet_flavor_truth_label->at(jet_index); 


}
  
double SelectedJet::combNN_btag() const { 
  return log(m_cnn_b / m_cnn_u); 
}
  
double SelectedJet::jfitcomb_cu(const SusyBuffer& buffer, 
				int jet_index) const {
  return m_cnn_u;
}

double SelectedJet::jfitcomb_cb(const SusyBuffer& buffer, 
				int jet_index) const {
  return m_cnn_b; 
}
 
int SelectedJet::index() const{
  return m_jet_index;
}

double SelectedJet::jvf() const { 
  return m_jvf; 
}
double SelectedJet::pb() const { 
  return m_cnn_b; 
}
double SelectedJet::pc() const { 
  return m_cnn_c; 
}
double SelectedJet::pu() const { 
  return m_cnn_u; 
}
int SelectedJet::flavor_truth_label() const { 
  return m_flavor_truth_label; 
}

unsigned SelectedJet::bits() const { 
  return m_bits; 
}

void SelectedJet::set_bit(unsigned bit) { 
  m_bits |= bit; 
}
void SelectedJet::unset_bit(unsigned bit) { 
  m_bits &=~ bit; 
}

EventJets::EventJets(const SusyBuffer& buffer, SUSYObjDef& def, 
		     unsigned flags, const RunInfo& info): 
  m_buffer(&buffer)
{ 

  assert(buffer.jet_n); 
  assert(buffer.jet_pt); 
  assert(buffer.el_cl_pt); 
  assert(buffer.el_cl_eta); 
  assert(buffer.el_cl_phi); 
  assert(buffer.el_cl_E); 

  const int n_jets = buffer.jet_n; 
  for (int jet_n = 0; jet_n < n_jets; jet_n++){ 
    
      //add the "standard quality" cuts here ************************
      //JVF>0.75, pt>20GeV, isGoodJet (from SUSYTools), ...)

    // must initalize all jets
    bool is_jet = check_if_jet(jet_n, buffer, def, flags, info); 

    SelectedJet* the_jet = new SelectedJet(this, jet_n); 
    push_back(the_jet); 

    // susytools corrected tlv
    TLorentzVector tlv = def.GetJetTLV(); 
    the_jet->SetPtEtaPhiE(tlv.Pt(), tlv.Eta(), tlv.Phi(), tlv.E()); 

    if (is_jet) { 
      the_jet->set_bit(jetbit::good); 
    }
    
    bool low_pt_jet = the_jet->Pt() < 20*GeV;
    if (low_pt_jet) { 
      the_jet->set_bit(jetbit::low_pt); 
    }
    

    // overlap removal 
    double min_delta_r = 10; 
    for (int el_n = 0; el_n < buffer.el_n; el_n++) { 
      TLorentzVector el; 
      el.SetPtEtaPhiE(buffer.el_cl_pt->at(el_n), 
		      buffer.el_cl_eta->at(el_n), 
		      buffer.el_cl_phi->at(el_n), 
		      buffer.el_cl_E->at(el_n)); 
      float delta_r = el.DeltaR(*the_jet); 
      if (delta_r < 0.2) { 
	min_delta_r = delta_r; 
	break; 
      }
    }
    if (min_delta_r < 0.2) { 
      the_jet->set_bit(jetbit::el_overlap); 
    }

  }
}

EventJets::~EventJets() { 
  for (iterator itr = begin(); itr != end(); itr++) { 
    delete *itr; 
  }
}

bool check_if_jet(int iJet, 
		  const SusyBuffer& buffer, 
		  SUSYObjDef& def, 
		  const unsigned flags, 
		  const RunInfo& info){ 

  assert(buffer.jet_pt                 );
  assert(buffer.jet_eta                );
  assert(buffer.jet_phi                );
  assert(buffer.jet_E                  );
  assert(buffer.jet_emscale_eta        );
  assert(buffer.jet_emfrac             );
  assert(buffer.jet_hecf               );
  assert(buffer.jet_LArQuality         );
  assert(buffer.jet_HECQuality         );
  assert(buffer.jet_AverageLArQF       );
  assert(buffer.jet_Timing             );
  assert(buffer.jet_sumPtTrk           );
  assert(buffer.jet_fracSamplingMax    );
  assert(buffer.jet_SamplingMax        );
  assert(buffer.jet_NegativeE          );
  assert(buffer.jet_flavor_truth_label );
  assert(buffer.jet_emscale_E          );
  assert(buffer.jet_emscale_eta        );
  assert(buffer.jet_EtaOrigin          );
  assert(buffer.jet_PhiOrigin          );
  assert(buffer.jet_MOrigin            );
    
  return def.FillJet
    (iJet, 
     buffer.jet_pt                 ->at(iJet), 
     buffer.jet_eta                ->at(iJet), 
     buffer.jet_phi                ->at(iJet),
     buffer.jet_E                  ->at(iJet), 
     buffer.jet_emscale_eta        ->at(iJet), 
     buffer.jet_emfrac             ->at(iJet), 
     buffer.jet_hecf               ->at(iJet),
     buffer.jet_LArQuality         ->at(iJet), 
     buffer.jet_HECQuality         ->at(iJet), 
     buffer.jet_AverageLArQF       ->at(iJet), 
     buffer.jet_Timing             ->at(iJet), 
     buffer.jet_sumPtTrk           ->at(iJet), 
     buffer.jet_fracSamplingMax    ->at(iJet),
     buffer.jet_SamplingMax        ->at(iJet), 
     buffer.jet_NegativeE          ->at(iJet), 
     buffer.jet_flavor_truth_label ->at(iJet), 
     buffer.jet_emscale_E          ->at(iJet),
     buffer.jet_emscale_eta        ->at(iJet), 
     buffer.jet_EtaOrigin          ->at(iJet), 
     buffer.jet_PhiOrigin          ->at(iJet), 
     buffer.jet_MOrigin            ->at(iJet), 
     buffer.averageIntPerXing,
     buffer.vx_nTracks,             
     info.run_number, 
     flags & cutflag::is_data, 
     20000.0, 	// pt cut
     2.8,	// eta cut - changed from 2.8 for testing
     JetID::VeryLooseBad,
     SystErr::NONE);
    
}
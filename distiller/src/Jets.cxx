#include "Jets.hh"
#include "SusyBuffer.h"
#include "constants.hh"
#include "RunInfo.hh"
#include "RunBits.hh"
#include "JetConstants.hh"
#include "BtagCalibration.hh"
#include "CheckSize.hh"

#include "SUSYTools/SUSYObjDef.h"

#include <cassert> 
#include <stdexcept>
#include <boost/format.hpp>

PreselectionJets::PreselectionJets(const JetContainer& jets) { 
  for (auto jet_itr = jets.begin(); jet_itr != jets.end(); jet_itr++) {
    auto& jet = **jet_itr; 
    bool is_low_pt = jet.Pt() < JET_PT_CUT; 
    // bool is_high_eta = fabs(jet.Eta()) > JET_ETA_CUT; 
    bool pass_jvf = (jet.jvf() > JET_JVF_CUT || 
		     jet.Pt() > JET_PT_IGNORE_JVF); 
    if (!is_low_pt && pass_jvf) { 
      push_back(*jet_itr); 
    }
  }
}

SignalJets::SignalJets(const JetContainer& jets) { 
  for (auto jet_itr = jets.begin(); jet_itr != jets.end(); jet_itr++) { 
    const SelectedJet& jet = **jet_itr; 
    bool signal_pt = jet.Pt() > SIGNAL_JET_PT_CUT; 
    bool tag_eta = fabs(jet.Eta()) < JET_TAGGING_ETA_LIM; 
    // bool leading_jet = (*jet_itr == *jets.begin()); 
    if (signal_pt && (tag_eta )) { 
      push_back(*jet_itr); 
    }
  }
}

SelectedJet::SelectedJet(const EventJets* parent, int jet_index): 
  m_bits(0)
{ 
  const SusyBuffer& buffer = *parent->m_buffer; 
  double pt = buffer.jet_pt  ->at(jet_index); 
  double eta = buffer.jet_eta ->at(jet_index); 
  double phi = buffer.jet_phi ->at(jet_index); 
  double e = buffer.jet_E   ->at(jet_index); 

  if (pt){ 
    SetPtEtaPhiE(pt,eta,phi,e); 
  }
  else { 
    SetPxPyPzE(0,0,0,e); 
  }
  m_jet_index = jet_index;

  m_jvf = buffer.jet_jvtxf->at(jet_index); 

  m_cnn_b = buffer.jet_flavor_component_jfitcomb_pb->at(jet_index); 
  m_cnn_c = buffer.jet_flavor_component_jfitcomb_pc->at(jet_index); 
  m_cnn_u = buffer.jet_flavor_component_jfitcomb_pu->at(jet_index); 
  if ( parent->m_flags & cutflag::truth) { 
    m_flavor_truth_label = buffer.jet_flavor_truth_label->at(jet_index); 
  }
  else { 
    m_flavor_truth_label = -1; 
  }

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
  assert(m_flavor_truth_label != -1); 
  return m_flavor_truth_label; 
}

unsigned SelectedJet::bits() const { 
  return m_bits; 
}

void SelectedJet::set_bit(unsigned bit, bool set_to) { 
  if (set_to) { 
    m_bits |= bit; 
  }
  else { 
    unset_bit(bit); 
  }
}
void SelectedJet::unset_bit(unsigned bit) { 
  m_bits &=~ bit; 
}
bool SelectedJet::has_truth() const { 
  return (m_flavor_truth_label != -1); 
}
void SelectedJet::set_flavor_tag(const BtagCalibration* cal){ 
  btag::Flavor flavor;
  switch (m_flavor_truth_label) { 
  case -1: flavor = btag::DATA; break; 
  case 0: flavor = btag::U; break;
  case 4: flavor = btag::C; break; 
  case 5: flavor = btag::B; break; 
  case 15: flavor = btag::T; break;
  default: 
    std::string err = (boost::format("unknown pdgid: %i") % 
		       m_flavor_truth_label).str(); 
    throw std::runtime_error(err); 
  }
  set_flavor_tag(flavor, btag::CNN_TIGHT, cal); 
  set_flavor_tag(flavor, btag::CNN_MEDIUM, cal); 
  set_flavor_tag(flavor, btag::CNN_LOOSE, cal); 
}
void SelectedJet::set_flavor_tag(btag::Flavor flavor, 
				 btag::Tagger tagger, 
				 const BtagCalibration* cal) { 
  if (m_scale_factor.size() <= size_t(tagger)) { 
    size_t n_missing = size_t(tagger) - m_scale_factor.size() + 1; 
    m_scale_factor.insert(m_scale_factor.end(), n_missing,
			  std::make_pair(0,0) ); 
  }
  JetTagFactorInputs in; 
  in.pt = Pt(); 
  in.eta = Eta(); 
  in.anti_b = log(m_cnn_c / m_cnn_b); 
  in.anti_u = log(m_cnn_c / m_cnn_u); 
  in.flavor = flavor; 
  if (flavor != btag::DATA) { 
    m_scale_factor.at(tagger) = cal->applied_factor(in, tagger); 
  }
  set_bit(get_tagger_bit(btag::U,tagger),cal->pass_anti_u(in, tagger)); 
  set_bit(get_tagger_bit(btag::B,tagger),cal->pass_anti_b(in, tagger)); 
}
unsigned SelectedJet::get_tagger_bit(btag::Flavor flavor, 
				     btag::Tagger tagger) const{
  using namespace btag; 
  using namespace jetbit; 
  if (flavor == U) { 
    switch (tagger) { 
    case CNN_LOOSE: return cnn_loose_anti_u; 
    case CNN_MEDIUM: return cnn_medium_anti_u; 
    case CNN_TIGHT: return cnn_tight_anti_u; 
    }
  }
  else if (flavor == B) { 
    if (tagger == CNN_LOOSE || tagger == CNN_MEDIUM || tagger == CNN_TIGHT){
      return cnn_anti_b; 
    }
  }
  throw std::logic_error("tried to set unknown tagger bit"); 

}

std::pair<double, double> SelectedJet::scale_factor(btag::Tagger t) 
  const 
{
  if (m_scale_factor.size() <= size_t(t)) { 
    throw std::out_of_range("tried to access undefined scale factor"); 
  }
  return m_scale_factor.at(t); 
}
bool SelectedJet::pass_anti_u(btag::Tagger t) const { 
  return m_bits & get_tagger_bit(btag::U, t); 
}
bool SelectedJet::pass_anti_b(btag::Tagger t) const { 
  return m_bits & get_tagger_bit(btag::B, t); 
}

EventJets::EventJets(const SusyBuffer& buffer, SUSYObjDef& def, 
		     unsigned flags, const RunInfo& info): 
  m_buffer(&buffer), 
  m_flags(flags), 
  m_jets_under_uncalibrated_min(0)
{ 

  assert(buffer.jet_pt); 
  assert(buffer.el_cl_pt); 
  assert(buffer.el_cl_eta); 
  assert(buffer.el_cl_phi); 
  assert(buffer.el_cl_E); 

  fill(buffer, def, flags, info);


}
void EventJets::fill(const SusyBuffer& buffer, SUSYObjDef& def, 
		     unsigned flags, const RunInfo& info) { 
  const int n_jets = buffer.jet_n; 
  for (int jet_n = 0; jet_n < n_jets; jet_n++){ 

    if (!has_min_pt(jet_n, buffer, UNCALIBRATED_JET_PT_MIN)) {
      m_jets_under_uncalibrated_min++; 
    } 

    bool is_jet = fill_jet(jet_n, buffer, def, flags, info); 

    push_back(new SelectedJet(this, jet_n)); 
    SelectedJet* the_jet = *rbegin(); 

    // susytools corrected tlv
    TLorentzVector tlv = def.GetJetTLV(); 
    the_jet->SetPxPyPzE(tlv.Px(), tlv.Py(), tlv.Pz(), tlv.E()); 

    if (is_jet) { 
      the_jet->set_bit(jetbit::preselection); 
    }

  }
}

EventJets::~EventJets() { 
  for (iterator itr = begin(); itr != end(); itr++) { 
    delete *itr; 
  }
}

void EventJets::dump_debug(std::ostream& stream) { 
  stream << "n jets under " << UNCALIBRATED_JET_PT_MIN << ": " 
	 << m_jets_under_uncalibrated_min; 
}

namespace { 
  // forward dec for systematic translation
  SystErr::Syste get_susytools_systematic(systematic::Systematic syst); 

  bool has_min_pt(int jet_n, const SusyBuffer& buffer, float pt_cut) { 
    CHECK_SIZE(buffer.jet_pt, buffer.jet_n); 
    return buffer.jet_pt->at(jet_n) >= pt_cut; 
  } 

  bool fill_jet(int iJet, 
		const SusyBuffer& buffer, 
		SUSYObjDef& def, 
		const unsigned flags, 
		const RunInfo& info){ 

    assert(check_buffer(buffer)); 

    int flavor_truth_label = 0; 
    if ( flags & cutflag::truth ) { 
      CHECK_SIZE(buffer.jet_flavor_truth_label, buffer.jet_n); 
      flavor_truth_label = buffer.jet_flavor_truth_label->at(iJet); 
    }
    
    return def.FillJet
      (iJet, 
       buffer.jet_pt                 ->at(iJet), 
       buffer.jet_eta                ->at(iJet), 
       buffer.jet_phi                ->at(iJet),
       buffer.jet_E                  ->at(iJet), 
       buffer.jet_constscale_eta        ->at(iJet), 
       buffer.jet_constscale_phi        ->at(iJet), 
       buffer.jet_constscale_E        ->at(iJet), 
       buffer.jet_constscale_m        ->at(iJet),
       buffer.jet_ActiveAreaPx->at(iJet), 
       buffer.jet_ActiveAreaPy->at(iJet), 
       buffer.jet_ActiveAreaPz->at(iJet), 
       buffer.jet_ActiveAreaE->at(iJet), 
       buffer.Eventshape_rhoKt4LC, 
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
       flavor_truth_label, 
       buffer.averageIntPerXing,
       buffer.vx_nTracks,             
       buffer.RunNumber, 
       flags & cutflag::is_data, 
       JET_PT_CUT, 	
       JET_ETA_CUT,	
       JetID::VeryLooseBad,
       get_susytools_systematic(info.systematic));
    
  }



  bool check_buffer(const SusyBuffer& buffer) { 
    CHECK_SIZE(buffer.jet_pt                  , buffer.jet_n);
    CHECK_SIZE(buffer.jet_eta                 , buffer.jet_n);
    CHECK_SIZE(buffer.jet_phi                 , buffer.jet_n);
    CHECK_SIZE(buffer.jet_E                   , buffer.jet_n);
    CHECK_SIZE(buffer.jet_constscale_eta      , buffer.jet_n);
    CHECK_SIZE(buffer.jet_constscale_phi      , buffer.jet_n);
    CHECK_SIZE(buffer.jet_constscale_E        , buffer.jet_n);
    CHECK_SIZE(buffer.jet_constscale_m        , buffer.jet_n);
    CHECK_SIZE(buffer.jet_ActiveAreaPx        , buffer.jet_n);
    CHECK_SIZE(buffer.jet_ActiveAreaPy        , buffer.jet_n);
    CHECK_SIZE(buffer.jet_ActiveAreaPz        , buffer.jet_n);
    CHECK_SIZE(buffer.jet_ActiveAreaE         , buffer.jet_n);
    CHECK_SIZE(buffer.jet_emfrac              , buffer.jet_n);
    CHECK_SIZE(buffer.jet_hecf                , buffer.jet_n);
    CHECK_SIZE(buffer.jet_LArQuality          , buffer.jet_n);
    CHECK_SIZE(buffer.jet_HECQuality          , buffer.jet_n);
    CHECK_SIZE(buffer.jet_AverageLArQF        , buffer.jet_n);
    CHECK_SIZE(buffer.jet_Timing              , buffer.jet_n);
    CHECK_SIZE(buffer.jet_sumPtTrk            , buffer.jet_n);
    CHECK_SIZE(buffer.jet_fracSamplingMax     , buffer.jet_n);
    CHECK_SIZE(buffer.jet_SamplingMax         , buffer.jet_n);
    CHECK_SIZE(buffer.jet_NegativeE           , buffer.jet_n);
    return true; 
  }

  SystErr::Syste get_susytools_systematic(systematic::Systematic syst) { 
    using namespace systematic; 
    switch (syst) { 
    case NONE: return SystErr::NONE; 
    case JESUP: return SystErr::JESUP; 
    case JESDOWN: return SystErr::JESDOWN; 
    case JER: return SystErr::JER; 
    default: 
      throw std::logic_error("got undedined systematic in " __FILE__); 
    }
  }

}

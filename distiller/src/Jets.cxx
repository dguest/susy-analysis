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

void SelectedJet::set_bit(unsigned bit) { 
  m_bits |= bit; 
}
void SelectedJet::unset_bit(unsigned bit) { 
  m_bits &=~ bit; 
}
bool SelectedJet::has_truth() const { 
  return (m_flavor_truth_label != -1); 
}
void SelectedJet::set_scale_factors(const BtagCalibration* cal){ 
  if (!has_truth() || cal == 0) { 
    return;
  }
  btag::Flavor flavor;
  switch (m_flavor_truth_label) { 
  case 0: flavor = btag::U; break;
  case 4: flavor = btag::C; break; 
  case 5: flavor = btag::B; break; 
  case 15: flavor = btag::T; break;
  default: 
    std::string err = (boost::format("unknown pdgid: %i") % 
		       m_flavor_truth_label).str(); 
    throw std::runtime_error(err); 
  }
  set_scale_factors(flavor, btag::CNN_MEDIUM, cal); 
  set_scale_factors(flavor, btag::CNN_LOOSE, cal); 
}
void SelectedJet::set_scale_factors(btag::Flavor flavor, 
				    btag::Tagger tagger, 
				    const BtagCalibration* cal) { 
  if (m_scale_factor.size() <= size_t(tagger)) { 
    size_t n_missing = size_t(tagger) - m_scale_factor.size() + 1; 
    m_scale_factor.insert(m_scale_factor.end(), n_missing,
			  std::make_pair(0,0) ); 
  }
  if (m_fail_factor.size() <= size_t(tagger)) { 
    size_t n_missing = size_t(tagger) - m_fail_factor.size() + 1; 
    m_fail_factor.insert(m_fail_factor.end(), n_missing, 
			 std::make_pair(0,0) ); 
  }
  m_scale_factor.at(tagger) = cal->scale_factor(Pt(), Eta(), flavor, tagger); 
  m_fail_factor.at(tagger) = cal->fail_factor(Pt(), Eta(), flavor, tagger); 
}
std::pair<double, double> SelectedJet::scale_factor(btag::Tagger t) 
  const 
{
  if (m_scale_factor.size() <= size_t(t)) { 
    throw std::range_error("tried to access undefined scale factor"); 
  }
  return m_scale_factor.at(t); 
}
std::pair<double, double> SelectedJet::fail_factor(btag::Tagger t) 
  const 
{
  if (m_fail_factor.size() <= size_t(t)) { 
    throw std::range_error("tried to access undefined fail factor"); 
  }
  return m_fail_factor.at(t); 
}

EventJets::EventJets(const SusyBuffer& buffer, SUSYObjDef& def, 
		     unsigned flags, const RunInfo& info): 
  m_buffer(&buffer), 
  m_flags(flags)
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
    
      //add the "standard quality" cuts here ************************
      //JVF>0.75, pt>20GeV, isGoodJet (from SUSYTools), ...)

    // must initalize all jets

    bool is_jet = check_if_jet(jet_n, buffer, def, flags, info); 

    SelectedJet* the_jet = new SelectedJet(this, jet_n); 
    push_back(the_jet); 

    // susytools corrected tlv
    TLorentzVector tlv = def.GetJetTLV(); 
    the_jet->SetPxPyPzE(tlv.Px(), tlv.Py(), tlv.Pz(), tlv.E()); 

    if (is_jet) { 
      the_jet->set_bit(jetbit::susy); 
    }
    
    if (the_jet->Pt() < JET_PT_CUT) { 
      the_jet->set_bit(jetbit::low_pt); 
    }
    if (the_jet->Pt() > SIGNAL_JET_PT_CUT) { 
      the_jet->set_bit(jetbit::signal_pt); 
    }
    
    if (fabs(the_jet->Eta()) > JET_ETA_CUT) { 
      the_jet->set_bit(jetbit::high_eta); 
    }
    if (fabs(the_jet->Eta()) < JET_TAGGING_ETA_LIM) { 
      the_jet->set_bit(jetbit::taggable_eta); 
    }

  }
}

EventJets::~EventJets() { 
  for (iterator itr = begin(); itr != end(); itr++) { 
    delete *itr; 
  }
}

namespace { 
  // forward dec for systematic translation
  SystErr::Syste get_susytools_systematic(systematic::Systematic syst); 

  bool check_if_jet(int iJet, 
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
       info.run_number, 
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
    default: 
      throw std::logic_error("got undedined systematic in " __FILE__); 
    }
  }

}

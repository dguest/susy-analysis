#include "Jets.hh"
#include "SusyBuffer.h"
#include "constants.hh"
#include "RunInfo.hh"
#include "cutflag.hh"
#include "constants_jet.hh"
#include "BtagCalibration.hh"
#include "CheckSize.hh"

#include "SUSYTools/SUSYObjDef.h"

#include <cassert>
#include <stdexcept>
#include <csignal>
#include <string>

namespace {
  bool has_min_pt(int iJet, const SusyBuffer& buffer, float pt);
  bool fill_jet(int iJet,
		const SusyBuffer& buffer,
		SUSYObjDef& def,
		const unsigned flags,
		const RunInfo& info);

  bool check_buffer(const SusyBuffer& buffer);
}

SelectedJet::SelectedJet(const SusyBuffer& buffer,
			 SUSYObjDef& def,
			 int jet_index, bool good):
  m_bits(0)
{
  // susytools corrected tlv
  TLorentzVector tlv = def.GetJetTLV(jet_index);
  SetPxPyPzE(tlv.Px(), tlv.Py(), tlv.Pz(), tlv.E());

  m_jet_index = jet_index;

  m_jvf = buffer.jet_jvtxf->at(jet_index);

  m_cnn_b = buffer.jet_flavor_component_jfitcomb_pb->at(jet_index);
  m_cnn_c = buffer.jet_flavor_component_jfitcomb_pc->at(jet_index);
  m_cnn_u = buffer.jet_flavor_component_jfitcomb_pu->at(jet_index);
  m_jfc_b = buffer.jet_flavor_component_jfitc_pb->at(jet_index);
  m_jfc_c = buffer.jet_flavor_component_jfitc_pc->at(jet_index);
  m_jfc_u = buffer.jet_flavor_component_jfitc_pu->at(jet_index);

  m_bch_corr = buffer.jet_BCH_CORR_JET->at(jet_index);
  m_timing = buffer.jet_Timing->at(jet_index);

  double chf = buffer.jet_sumPtTrk->at(jet_index) / Pt();
  double em_frac = buffer.jet_emfrac->at(jet_index);
  bool fail_chf_pt = Pt() > jet::FAIL_CHF_PT_MIN;
  bool fail_chf_eta = std::abs(Eta()) < jet::FAIL_CHF_ETA_MAX;

  bool fail_chf_no_em = chf < jet::FAIL_CHF_MAX_NOEM;
  bool fail_chf_em = (chf < jet::FAIL_CHF_MAX_EM &&
		      em_frac > jet::FAIL_CHF_EM_FRAC_MIN);
  bool fail_chf = (fail_chf_em || fail_chf_no_em);
  m_pass_chf = !(fail_chf_pt && fail_chf_eta && fail_chf);

  if ( buffer.has_truth() ) {
    m_flavor_truth_label = buffer.jet_flavor_truth_label->at(jet_index);
  }
  else {
    m_flavor_truth_label = -1;
  }
  if (good) {
    set_bit(jetbit::pass_susy_def);
  }

}

int SelectedJet::index() const{
  return m_jet_index;
}

double SelectedJet::jvf() const {
  return m_jvf;
}
bool SelectedJet::bad_tile(const TVector2& met) const {
  float dphi = met.DeltaPhi(Vect().XYvector());
  if (Pt() > jet::BAD_TILE_JET_PT_MIN &&
      m_bch_corr > jet::BAD_TILE_BCH_CORR_MIN &&
      std::abs(dphi) < jet::BAD_TILE_MET_DPHI_MAX) {
    return true;
  }
  return false;
}

bool SelectedJet::pass_chf() const { return m_pass_chf; }

float SelectedJet::timing() const { return m_timing; }

double SelectedJet::flavor_weight(btag::Flavor flavor,
				  btag::Tagger tag) const {
  const std::string err = "flavor fallthrough in " __FILE__;
  if (tag == btag::CNN) {
    switch (flavor) {
    case btag::B: return m_cnn_b;
    case btag::C: return m_cnn_c;
    case btag::U: return m_cnn_u;
    default: {
      throw std::logic_error(err);
    }
    }
  }
  else if (tag == btag::JFC) {
    switch (flavor) {
    case btag::B: return m_jfc_b;
    case btag::C: return m_jfc_c;
    case btag::U: return m_jfc_u;
    default: {
      throw std::logic_error(err);
    }
    }
  }
  throw std::logic_error("tag fallthrough in " __FILE__);
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
    std::string err = "unknown pdgid: " + std::to_string(
      m_flavor_truth_label);
    throw std::runtime_error(err);
  }
  set_flavor_tag(flavor, btag::CNN_MEDIUM, cal);
  set_flavor_tag(flavor, btag::CNN_LOOSE, cal);
  set_flavor_tag(flavor, btag::JFC_MEDIUM, cal);
  set_flavor_tag(flavor, btag::JFC_LOOSE, cal);
}
void SelectedJet::set_flavor_tag(btag::Flavor truth_flavor,
				 btag::OperatingPoint op_point,
				 const BtagCalibration* cal) {
  if (m_scale_factor.size() <= size_t(op_point)) {
    size_t n_missing = size_t(op_point) - m_scale_factor.size() + 1;
    m_scale_factor.insert(m_scale_factor.end(), n_missing,
			  std::make_pair(0,0) );
  }
  JetTagFactorInputs in;
  in.pt = Pt();
  in.eta = Eta();
  btag::Tagger tagger = btag::tagger_from_op(op_point);
  double pb = flavor_weight(btag::B, tagger);
  double pc = flavor_weight(btag::C, tagger);
  double pu = flavor_weight(btag::U, tagger);
  in.anti_b = log(pc / pb);
  in.anti_u = log(pc / pu);
  in.flavor = truth_flavor;
  if (truth_flavor != btag::DATA) {
    m_scale_factor.at(op_point) = cal->applied_factor(in, op_point);
  }
  set_bit(get_tagger_bit(btag::U,op_point),cal->pass_anti_u(in, op_point));
  set_bit(get_tagger_bit(btag::B,op_point),cal->pass_anti_b(in, op_point));
}
unsigned SelectedJet::get_tagger_bit(btag::Flavor flavor,
				     btag::OperatingPoint tagger) const{
  using namespace btag;
  using namespace jetbit;
  if (flavor == U) {
    switch (tagger) {
    case CNN_LOOSE:  return cnn_loose_anti_u;
    case CNN_MEDIUM: return cnn_medium_anti_u;
    case CNN_TIGHT:  return cnn_tight_anti_u;
    case JFC_LOOSE:  return jfc_loose_anti_u;
    case JFC_MEDIUM: return jfc_medium_anti_u;
    case JFC_TIGHT:  return jfc_tight_anti_u;
    }
  }
  else if (flavor == B) {
    if (tagger == CNN_LOOSE || tagger == CNN_MEDIUM || tagger == CNN_TIGHT){
      return cnn_anti_b;
    }
    if (tagger == JFC_LOOSE || tagger == JFC_MEDIUM || tagger == JFC_TIGHT){
      return jfc_anti_b;
    }
  }
  throw std::logic_error("tried to set unknown tagger bit");

}

std::pair<double, double> SelectedJet::scale_factor(btag::OperatingPoint t)
  const
{
  if (m_scale_factor.size() <= size_t(t)) {
    throw std::out_of_range("tried to access undefined scale factor");
  }
  return m_scale_factor.at(t);
}
bool SelectedJet::pass_anti_u(btag::OperatingPoint t) const {
  return m_bits & get_tagger_bit(btag::U, t);
}
bool SelectedJet::pass_anti_b(btag::OperatingPoint t) const {
  return m_bits & get_tagger_bit(btag::B, t);
}
bool SelectedJet::in_tag_eta() const {
  return std::abs(Eta()) < jet::TAGGING_ETA_LIM;
}

EventJets::EventJets(const SusyBuffer& buffer, SUSYObjDef& def,
		     unsigned flags, const RunInfo& info):
  m_buffer(&buffer),
  m_flags(flags),
  m_jets_under_uncalibrated_min(0)
{

  assert(buffer.jet_pt);
  assert(buffer.el_cl_eta);
  assert(buffer.el_cl_phi);
  assert(buffer.el_cl_E);

  fill(buffer, def, flags, info);


}
void EventJets::fill(const SusyBuffer& buffer, SUSYObjDef& def,
		     unsigned flags, const RunInfo& info) {
  const int n_jets = buffer.jet_n;
  for (int jet_n = 0; jet_n < n_jets; jet_n++){

    // if (!has_min_pt(jet_n, buffer, jet::UNCALIBRATED_PT_MIN)) {
    //   m_jets_under_uncalibrated_min++;
    // }

    bool is_jet = fill_jet(jet_n, buffer, def, flags, info);

    push_back(new SelectedJet(buffer, def, jet_n, is_jet));

  }
}

EventJets::~EventJets() {
  for (iterator itr = begin(); itr != end(); itr++) {
    delete *itr;
  }
}

// void EventJets::dump_debug(std::ostream& stream) {
//   stream << "n jets under " << jet::UNCALIBRATED_PT_MIN << ": "
// 	 << m_jets_under_uncalibrated_min;
// }

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

    def.FillJet(
      iJet,
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
      buffer.average_int_per_xing(),
      buffer.vx_nTracks);

    def.ApplyJetSystematics(
      iJet,
      buffer.jet_constscale_eta        ->at(iJet),
      flavor_truth_label,
      buffer.average_int_per_xing(),
      buffer.vx_nTracks,
      get_susytools_systematic(info.systematic));

    return def.IsGoodJet(
      iJet,
      buffer.jet_constscale_eta        ->at(iJet),
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
      buffer.RunNumber,
      jet::SUSYTOOLS_PT_CUT,
      jet::SUSYTOOLS_ETA_CUT,
      JetID::VeryLooseBad);

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
    case JESUP: return SystErr::JESUP;
    case JESDOWN: return SystErr::JESDOWN;
    case JER: return SystErr::JER;

      // JES breakdown

#define CP_UD(name, stool_name)				\
      case name ## UP: return SystErr::stool_name ## Up;	\
    case name ## DOWN: return SystErr::stool_name ## Down

      CP_UD(JENP1, EffectiveNP_1_);
      CP_UD(JENP2, EffectiveNP_2_);
      CP_UD(JENP3, EffectiveNP_3_);
      CP_UD(JENP4, EffectiveNP_4_);
      CP_UD(JENP5, EffectiveNP_5_);
      CP_UD(JENP6, EffectiveNP_6_);

      CP_UD(JICALM, EtaIntercalibration_Modelling_);
      CP_UD(JICALS, EtaIntercalibration_StatAndMethod_);

      CP_UD(JSP, SingleParticle_HighPt_);
      CP_UD(JNC, RelativeNonClosure_Pythia8_);
      CP_UD(JPUMU, PileupOffsetTermMu);
      CP_UD(JPUNPV, PileupOffsetTermNPV);
      CP_UD(JPUPT, PileupPtTerm);
      CP_UD(JPURHO, PileupRhoTopology);
      CP_UD(JCB, CloseBy);
      CP_UD(JFLAVCOMP, FlavorCompUncert);
      CP_UD(JFLAVRESP, FlavorResponseUncert);
      CP_UD(JBJES, BJes);


#undef CP_UD
    default:
      return SystErr::NONE;
    }
  }

}

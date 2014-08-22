#ifndef JET_FACTORY_H
#define JET_FACTORY_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <stdexcept>
#include "HforTypeDef.hh"
#include "systematic_defs.hh"
#include "typedefs.hh"
#include "btag_enums.hh"
#include "EventScaleEnums.hh"

class Jet;
class JetBuffer;
class Lepton;
class LeptonBuffer;
class TTree;
class TFile;
class TVector2;
class BtagScaler;
class BtagBuffer;
class TLorentzVector;
class SFBox;
class EventScalefactors;
class SkimCounts;
class EventRecoParameters;

namespace ioflag {
  const unsigned no_flavor            = 1u << 0;
  const unsigned no_truth             = 1u << 1;
  const unsigned has_boson_pt_weight  = 1u << 2;
}


struct MetBuffer
{
  double met;
  double met_phi;
  MetBuffer(TTree* tree, const std::string& prefix);
  MetBuffer(const MetBuffer&) = delete;
  MetBuffer& operator=(const MetBuffer&) = delete;
};

class ObjectFactory
{
public:
  ObjectFactory(const std::string& root_file, int n_jets);
  ~ObjectFactory();
  int entries() const;
  bool is_data() const;
  void entry(int);
  std::vector<Jet> jets() const;
  std::vector<Lepton> leptons() const;
  // TODO: fix this, we're not storing met systematics in the tree any more
  TVector2 met(syst::Systematic = syst::NONE) const;
  ull_t bits() const;
  EventRecoParameters event_reco_parameters() const;
  int leading_cjet_pos() const;
  int subleading_cjet_pos() const;
  hfor::JetType hfor_type() const;
  double event_weight() const;
  float marks_boson_pt_weight() const;
  float boson_pt() const;
  TVector2 boson_child() const;
  const SkimCounts* skim_counts() const;
  EventScalefactors* event_scalefactors() const;
private:
  TTree* m_tree;
  TFile* m_file;
  std::vector<JetBuffer*> m_jet_buffers;
  std::vector<LeptonBuffer*> m_lepton_buffers;

  std::unordered_map<int, MetBuffer*> m_met;

  EventRecoParameters* m_event_reco_parameters;

  int m_hfor_type;

  int m_leading_cjet_pos;
  int m_subleading_cjet_pos;

  // float m_htx;
  float m_mc_event_weight;
  float m_boson_pt_weight;
  float m_boson_pt;

  float m_boson_child_pt;
  float m_boson_child_phi;

  unsigned m_ioflags;
  EventScalefactors* m_evt_sf;

  SkimCounts* m_skim_counts;
};

bool has_higher_pt(const Jet&, const Jet&);


#endif // JET_FACTORY_H

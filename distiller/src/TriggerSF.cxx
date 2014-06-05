#include "TriggerSF.hh"

#include "EventObjects.hh"
#include "Leptons.hh"

#include "SUSYTools/SUSYObjDef.h"
#include "TLorentzVector.h"

TriggerSF get_lepton_trigger_sf(SUSYObjDef& def, const EventObjects& obj){
  std::vector<TLorentzVector> el_tlv;
  std::vector<int> el_tight;
  for (auto el: obj.control_electrons) {
    el_tlv.push_back(*el);
    el_tight.push_back(el->tightpp());
  }
  std::vector<TLorentzVector> mu_tlv;
  std::vector<int> mu_combined;
  for (auto mu: obj.control_muons) {
    mu_tlv.push_back(*mu);
    mu_combined.push_back(mu->combined());
  }

  TriggerSF sf;

#define TRIG_SF(syste)				\
  def.GetLeptonTriggerSF(			\
    obj.prec.random_run,			\
    mu_tlv,					\
    mu_combined,				\
    el_tlv,					\
    el_tight,					\
    "",						\
    syste)

  sf.nominal = TRIG_SF(SystErr::NONE);
  sf.el.up = TRIG_SF(SystErr::ETRIGUP);
  sf.el.down = TRIG_SF(SystErr::ETRIGDOWN);
  sf.mu.up = TRIG_SF(SystErr::MTRIGUP);
  sf.mu.down = TRIG_SF(SystErr::MTRIGDOWN);
  sf.lepton.up   = sf.nominal + sqrt(
    pow(sf.el.up   - sf.nominal, 2) + pow(sf.mu.up   - sf.nominal, 2));
  sf.lepton.down = sf.nominal - sqrt(
    pow(sf.el.down - sf.nominal, 2) + pow(sf.mu.down - sf.nominal, 2));

#undef TRIG_SF

  return sf;

}

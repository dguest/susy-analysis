#include "Met.hh"

#include "SusyBuffer.h"
#include "TVector2.h"
#include "SUSYTools/SUSYObjDef.h"
// #include "Leptons.hh"

namespace { 
  TVector2 get_met(
    const SusyBuffer& buffer, 
    SUSYObjDef& def, 
    const std::vector<int>& muon_idx, 
    SystErr::Syste);
  // TVector2 get_mumet(const TVector2& met, const std::vector<Muon*>& muons); 
}

Mets::Mets(const SusyBuffer& buffer, SUSYObjDef& def, 
	   const std::vector<int>& mudex, 
	   const TVector2& sum_muons) { 
  nominal = get_met(buffer, def, mudex, SystErr::NONE); 
  muon = nominal + sum_muons; 
  nominal_up = get_met(buffer, def, mudex, SystErr::SCALESTUP); 
  muon_up = nominal_up + sum_muons; 
  nominal_down = get_met(buffer, def, mudex, SystErr::SCALESTDOWN); 
  muon_down = nominal_down + sum_muons; 
  nominal_res = get_met(buffer, def, mudex, SystErr::RESOST); 
  muon_res = nominal_res + sum_muons; 
}

namespace { 
  TVector2 get_met(const SusyBuffer& buffer, 
		   SUSYObjDef& def, 
		   const std::vector<int>& mu_mystery_index, 
		   SystErr::Syste systematic) 
  { 
    const int n_el = buffer.el_n; 
    std::vector<int> el_mystery_index; 
    for (int el_n = 0; el_n < n_el; el_n++) { 
      float mystery_float = buffer.el_MET_Egamma10NoTau_wet->at(el_n).at(0); 
      if (mystery_float != 0.0) { 
	el_mystery_index.push_back(el_n); 
      }
    }
  
    TVector2 met = def.GetMET(
      buffer.jet_MET_Egamma10NoTau_wet,
      buffer.jet_MET_Egamma10NoTau_wpx,
      buffer.jet_MET_Egamma10NoTau_wpy,
      buffer.jet_MET_Egamma10NoTau_statusWord,
      el_mystery_index,
      buffer.el_MET_Egamma10NoTau_wet,
      buffer.el_MET_Egamma10NoTau_wpx,
      buffer.el_MET_Egamma10NoTau_wpy,
      buffer.el_MET_Egamma10NoTau_statusWord,
      buffer.MET_Egamma10NoTau_CellOut_etx, //CellOut
      buffer.MET_Egamma10NoTau_CellOut_ety, //CellOut
      buffer.MET_Egamma10NoTau_CellOut_sumet, //CellOut
      buffer.MET_CellOut_Eflow_STVF_etx, 
      buffer.MET_CellOut_Eflow_STVF_ety,
      buffer.MET_CellOut_Eflow_STVF_sumet,		  
      buffer.MET_Egamma10NoTau_RefGamma_etx,
      buffer.MET_Egamma10NoTau_RefGamma_ety,
      buffer.MET_Egamma10NoTau_RefGamma_sumet,
      mu_mystery_index, 
      buffer.mu_staco_ms_qoverp, 
      buffer.mu_staco_ms_theta, 
      buffer.mu_staco_ms_phi, 
      buffer.mu_staco_charge, 
      buffer.mu_staco_energyLossPar,
      buffer.averageIntPerXing, 
      SUSYMet::Default, 
      systematic); 
     
    return met; 
  }

  // TVector2 get_mumet(const TVector2& met, const std::vector<Muon*> muons) { 
  //   TLorentzVector met4(met.Px(), met.Py(), 0, 0); 
  //   for (auto muon = muons.begin(); muon != muons.end(); muon++) { 
  //     met4 += **muon; 
  //   }
  //   return met4.Vect().XYvector(); 
  // }
}

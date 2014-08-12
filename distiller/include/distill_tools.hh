#ifndef DISTILL_TOOLS_HH
#define DISTILL_TOOLS_HH

class SUSYObjDef;
class FakeMetEstimator;
class SelectedJet;
class TVector2;
class BtagCalibration;
class TLorentzVector;
class Muon;
class Electron;

// #include "TChain.h"
#include <vector>
#include <map>
#include <cassert>
#include <cmath>
#include "btag_defs.hh"
#include "constants_distiller.hh"

// --- utility functions

TVector2 get_boson_child_pt(const std::vector<Electron*>&,
			    const std::vector<Muon*>&);

float get_min_jetmet_dphi(const std::vector<SelectedJet*>&,
			  const TVector2& , size_t njet);

TVector2 sum_muon_pt(const std::vector<Muon*>& muons);
TVector2 sum_el_pt(const std::vector<Electron*>& el);

double get_htx(const std::vector<SelectedJet*>&, const size_t x);

double get_energy_weighted_time(const std::vector<SelectedJet*>&,
				size_t n_jets);

// m_ct functions
double get_m_ct(const TLorentzVector& v1, const TLorentzVector& v2);

double get_mctcorr(const TLorentzVector& v1, const TLorentzVector& v2,
		   const std::vector<Electron*>& el,
		   const std::vector<Muon*>& mu,
		   const TVector2& vmet);
double get_mct(const TLorentzVector& v1, const TLorentzVector& v2);

double get_mt(const std::vector<Electron*>& , const std::vector<Muon*>& ,
	      const TVector2& );

double get_mll(const std::vector<Electron*>& , const std::vector<Muon*>&);

template<typename T>
double scalar_sum_pt(const T& objects);

void set_bit(std::vector<SelectedJet*>& jets, unsigned bit);

void calibrate_jets(std::vector<SelectedJet*> jets, const BtagCalibration*);


// ---- templates -----

template<typename T>
double scalar_sum_pt(const T& obj) {
  double sum = 0.0;
  for (auto itr: obj) {
    sum += itr->Pt();
  }
  return sum;
}

#endif

#include "boson_truth_tools.hh"
#include "SusyBuffer.hh"

#include <vector>
#include <cassert>
#include <stdexcept>

#include "TLorentzVector.h"

namespace {
  TLorentzVector get_4vector(const SusyBuffer& buffer, int index);
}

float get_boson_truth_pt(const SusyBuffer& buffer) {

  assert(buffer.mc_status);
  int mc_n = buffer.mc_n;
  TLorentzVector l1;
  TLorentzVector l2;

  bool foundFirst = false;
  bool foundSecond = false;
  for ( int j = 0; j < mc_n; ++j){
    int mc_status = buffer.mc_status->at(j);
    int abs_pdgid = std::abs(buffer.mc_pdgId->at(j));
    if (mc_status == 3 && abs_pdgid >= 11 && abs_pdgid <= 16){

      if (!foundFirst){
	l1 = get_4vector(buffer, j);
	foundFirst = true;
      } else if(!foundSecond){
	l2 = get_4vector(buffer, j);
	foundSecond = true;
      } else {
	return -2.0;
      }
    }
  }
  if (!foundSecond) {
    return -1.0;
  }
  TLorentzVector v = l1 + l2;
  return v.Pt();
}

float get_ttbar_truth_pt(const SusyBuffer& buffer) {
  std::vector<TLorentzVector> tops;
  int mc_n = buffer.mc_n;
  for (int iii = 0; iii < mc_n; iii++) {
    int abs_pdgid = std::abs(buffer.mc_pdgId->at(iii));
    if (abs_pdgid == 6) {
      tops.push_back(get_4vector(buffer, iii));
    }
  }
  if (tops.size() < 2) return -1.0;
  if (tops.size() > 2) return -2.0;
  return (tops.at(0) + tops.at(1)).Pt();
}

BosonError::BosonError(const std::string& what):
  std::range_error(what)
{}

namespace {
  TLorentzVector get_4vector(const SusyBuffer& buffer, int index){
    float pt = buffer.mc_pt->at(index);
    float eta = buffer.mc_pt->at(index);
    float phi = buffer.mc_phi->at(index);
    float m = buffer.mc_m->at(index);
    TLorentzVector v;
    v.SetPtEtaPhiM(pt, eta, phi, m);
    return v;
  }
}

#include "distill_tools.hh"
#include "object_selection.hh"
#include "TLorentzVector.h"
#include <cassert>
#include <utility>

std::pair<size_t,size_t> test_overlap_removal() {
  const std::vector<float> dr_values = {0.1,0.2,0.4, 0.8};
  const std::vector<float> test_etas_abs = {0.05, 0.15, 0.3, 0.6, 1.0};
  std::vector<float> test_etas(test_etas);
  for (auto itr = test_etas_abs.begin(); itr != test_etas_abs.end(); itr++) {
    test_etas.push_back(-(*itr));
  }
  size_t total_survival = 0;
  size_t total_expected = 0;
  for (auto dr = dr_values.cbegin(); dr != dr_values.cend(); dr++) {
    std::vector<TLorentzVector*> masks;
    std::vector<TLorentzVector*> altered;
    masks.push_back(new TLorentzVector( 1.0,0,0,0));
    masks.push_back(new TLorentzVector(-1.0,0,0,0));
    size_t n_expected = 0;
    for (auto eta = test_etas.cbegin(); eta != test_etas.cend(); eta++) {

      TLorentzVector* v1 = new TLorentzVector;
      v1->SetPtEtaPhiE(1.0,*eta, 0, 0);
      altered.push_back(v1);
      if (fabs(*eta) > *dr) n_expected++;

      TLorentzVector* v2 = new TLorentzVector;
      v2->SetPtEtaPhiE(1.0,*eta, *dr, 0);
      altered.push_back(v2);
      n_expected++;

      TLorentzVector* v3 = new TLorentzVector;
      v3->SetPtEtaPhiE(1.0, 0, *eta, 0);
      altered.push_back(v3);
      if (fabs(*eta) > *dr) n_expected++;
    }
    auto survived = object::remove_overlaping(masks, altered, *dr);
    size_t n_survived = survived.size();
    total_survival += n_survived;
    total_expected += n_expected;
  }
  return std::make_pair(total_survival, total_expected);
}

int main(int narg, char* argv[]) {
  auto se = test_overlap_removal();
  printf("overlap test: %i survived, %i expected\n",
	 static_cast<int>(se.first), static_cast<int>(se.second));
}

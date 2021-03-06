#ifndef COPY_FUNCTIONS_HH
#define COPY_FUNCTIONS_HH

namespace outtree {
  class OutTree;
  class Jet;
  class ScaleFactor;
};
class SelectedJet;
class TVector2;
class EventObjects;
class ObjectComposites;

#include "btag_defs.hh"
#include "enums_replacement.hh"
#include <vector>

// ---- copy functions ----
void copy_event(const EventObjects&, JetRep, const ObjectComposites&,
		const TVector2&, outtree::OutTree&);

// the above calls the below

void copy_jet_info(const SelectedJet* , outtree::Jet&);

void copy_scale_factor(const SelectedJet*, outtree::ScaleFactor&,
		       btag::OperatingPoint);

template<typename T, typename B>
void copy_id_vec_to_box(const T& vec, B& box);

void copy_leading_jet_info(const std::vector<SelectedJet*>& signal_jets,
			   outtree::OutTree& out_tree);

void copy_cjet_truth(outtree::OutTree& out_tree,
		     const std::vector<SelectedJet*>& jets);

void copy_met(outtree::OutTree& out, const TVector2& mets);

// ---- templates -----

template<typename T, typename B>
void copy_id_vec_to_box(const T& vec, B& box) {
  float nominal = 1.0;
  float up = 1.0;
  float down = 1.0;
  for (auto itr: vec) {
    nominal *= itr->id_sf();
    up *= itr->id_sf() + itr->id_sf_err();
    down *= itr->id_sf() - itr->id_sf_err();
  }
  box.nominal = nominal;
  box.up = up;
  box.down = down;
}


#endif

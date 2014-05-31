#include "NMinusHist.hh"
#include "Histogram.hh"
#include "hdf_helpers.hh"

#include "H5Cpp.h"

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <limits>

namespace nminus {

  NMinusHist::NMinusHist(const Axis& axis,
			 const std::map<std::string,Window>& selection,
			 unsigned flags):
    m_histogram(new Histogram({axis}, flags)),
    m_name(axis.name),
    m_save_wt2(flags & hist::wt2)
  {
    m_histogram->set_wt_ext(WT2_POSTFIX);
    auto inf = std::numeric_limits<double>::infinity();
    if (!selection.count(axis.name)) {
      m_selection = {-inf, inf};
    } else {
      m_selection = selection.at(axis.name);
    }

    for (const auto& sel: selection) {
      assert(sel.second.min < sel.second.max);
      bool is_this_cut = (sel.first == axis.name);
      bool is_phantom = (sel.second.missing == Window::Missing::PHANTOM);
      if ( !is_this_cut && !is_phantom) {
	m_cuts.push_back(sel);
      }
    }
  }
  NMinusHist::~NMinusHist() {
    delete m_histogram;
  }
  NMinusHist::NMinusHist(NMinusHist&& old):
    m_histogram(0),
    m_selection(std::move(old.m_selection)),
    m_name(std::move(old.m_name)),
    m_save_wt2(old.m_save_wt2),
    m_cuts(std::move(old.m_cuts))
  {
    std::swap(m_histogram, old.m_histogram);
  }
  void NMinusHist::fill(const std::map<std::string, double>& values,
			double weight) {
    if (!values.count(m_name)) return;
    for (const auto& cut: m_cuts) {

      const auto& cut_variable = cut.first;

      // skip cut if the value is missing
      if (!values.count(cut_variable)) {
	switch (cut.second.missing) {
	case Window::Missing::ACCEPT: continue;
	case Window::Missing::REJECT: return;
	case Window::Missing::THROW: throw std::invalid_argument(
	  "required selection " + cut_variable + " is missing");
	default: throw std::logic_error("never go here");
	}
      }
      const auto& selection = cut.second;
      const auto& value = values.at(cut_variable);
      if (selection.min > value || value > selection.max) return;
    }
    m_histogram->fill(values, weight);
  }
  void NMinusHist::write_to(H5::CommonFG& file, std::string name) const {
    if (name.size() == 0) name = m_name;
    m_histogram->write_to(file, name);
    H5::DataSet the_hist(file.openDataSet(name));
    add_selection(the_hist);
    if (m_save_wt2) {
      H5::DataSet sw2_hist(file.openDataSet(name + WT2_POSTFIX));
      add_selection(sw2_hist);
    }
  }

  // ____________________________________________________________________
  // private
  void NMinusHist::add_selection(H5::DataSet& the_hist) const {
    bool is_phantom = (m_selection.missing == Window::Missing::PHANTOM);
    std::string sel_str = is_phantom ? "phantom_selection" : "selection";
    h5::write_attr_vec(the_hist, sel_str + "_min",
		       std::vector<double>{m_selection.min});
    h5::write_attr_vec(the_hist, sel_str + "_max",
		       std::vector<double>{m_selection.max});
  }
}

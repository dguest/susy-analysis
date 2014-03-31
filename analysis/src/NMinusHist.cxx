#include "NMinusHist.hh"
#include "Histogram.hh"
#include "hdf_helpers.hh"

#include "H5Cpp.h"

#include <vector>
#include <map>
#include <string>
#include <limits>
#include <cassert>

namespace nminus {

  Selection::Selection() :
    min(std::numeric_limits<double>::quiet_NaN()),
    max(std::numeric_limits<double>::quiet_NaN()),
    missing(Missing::THROW)
  {
  }
  Selection::Selection(double mn, double mx, Missing miss):
    min(mn), max(mx), missing(miss)
  {
  }

  NMinusHist::NMinusHist(const Axis& axis,
			 const std::map<std::string,Selection>& selection,
			 unsigned flags):
    m_histogram(new Histogram({axis}, flags)),
    m_name(axis.name)
  {
    auto inf = std::numeric_limits<double>::infinity();
    if (!selection.count(axis.name)) {
      m_selection = {-inf, inf};
    } else {
      m_selection = selection.at(axis.name);
    }

    for (const auto& sel: selection) {
      assert(sel.second.min < sel.second.max);
      bool is_this_cut = (sel.first == axis.name);
      bool is_phantom = (sel.second.missing == Selection::Missing::PHANTOM);
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
	case Selection::Missing::ACCEPT: continue;
	case Selection::Missing::REJECT: return;
	case Selection::Missing::THROW: throw std::invalid_argument(
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
    bool is_phantom = (m_selection.missing == Selection::Missing::PHANTOM);
    std::string sel_str = is_phantom ? "phantom_selection" : "selection";
    H5::DataSet the_hist(file.openDataSet(name));
    h5::write_attr_vec(the_hist, sel_str + "_min",
		       std::vector<double>{m_selection.min});
    h5::write_attr_vec(the_hist, sel_str + "_max",
		       std::vector<double>{m_selection.max});
  }
}

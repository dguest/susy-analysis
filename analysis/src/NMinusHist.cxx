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
    max(std::numeric_limits<double>::quiet_NaN()) 
  { 
  }
  Selection::Selection(double mn, double mx): min(mn), max(mx)
  {
  }
  
  NMinusHist::NMinusHist(const Axis& axis, 
			 const std::map<std::string,Selection>& selection) : 
    m_histogram(new Histogram({axis})), 
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
      if (sel.first != axis.name) { 
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
      if (!values.count(cut_variable)) continue;

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
    h5::write_attr_vec(the_hist, "selection_min", 
		       std::vector<double>{m_selection.min}); 
    h5::write_attr_vec(the_hist, "selection_max", 
		       std::vector<double>{m_selection.max}); 
  }
}

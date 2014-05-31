#ifndef NMINUS_HIST_HH
#define NMINUS_HIST_HH

#include <string>
#include <vector>
#include <map>

struct Axis;
class Histogram;

namespace H5 {
  class CommonFG;
  class DataSet;
}
namespace nminus {
  const std::string WT2_POSTFIX = "Wt2";

  struct Window {
    enum class Missing {REJECT, ACCEPT, THROW, PHANTOM};
    Window();
    Window(double, double, Missing = Missing::THROW);
    double min;
    double max;
    Missing missing;
  };
  class NMinusHist {
  public:
    // NOTE: could generalize this by making it an initalizer list
    NMinusHist(const Axis&, const std::map<std::string, Window>&,
	       unsigned flags = 0); // flags are passed to ndhist
    ~NMinusHist();
    NMinusHist(NMinusHist&) = delete;
    NMinusHist(NMinusHist&&);
    NMinusHist operator=(NMinusHist&) = delete;
    void fill(const std::map<std::string, double>& values, double weight);
    void write_to(H5::CommonFG&, std::string name = "") const;
  private:
    void add_selection(H5::DataSet&) const;
    Histogram* m_histogram;
    // NOTE: generalization would also require something new here
    Window m_selection;
    std::string m_name;
    bool m_save_wt2;
    std::vector<std::pair<std::string,Window> > m_cuts;
  };
}

#endif

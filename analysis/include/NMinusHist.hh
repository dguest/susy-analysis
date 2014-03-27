#ifndef NMINUS_HIST_HH
#define NMINUS_HIST_HH

#include <string>
#include <vector>
#include <map>

struct Axis;
class Histogram;

namespace H5 {
  class CommonFG;
}
namespace nminus {

  struct Selection {
    enum class Missing {REJECT, ACCEPT, THROW, PHANTOM};
    Selection();
    Selection(double, double, Missing = Missing::THROW);
    double min;
    double max;
    Missing missing;
  };
  class NMinusHist {
  public:
    // NOTE: could generalize this by making it an initalizer list
    NMinusHist(const Axis&, const std::map<std::string, Selection>&);
    ~NMinusHist();
    NMinusHist(NMinusHist&) = delete;
    NMinusHist(NMinusHist&&);
    NMinusHist operator=(NMinusHist&) = delete;
    void fill(const std::map<std::string, double>& values, double weight);
    void write_to(H5::CommonFG&, std::string name = "") const;
  private:
    Histogram* m_histogram;
    // NOTE: generalization would also require something new here
    Selection m_selection;
    std::string m_name;
    std::vector<std::pair<std::string,Selection> > m_cuts;
  };
}

#endif

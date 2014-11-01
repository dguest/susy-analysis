#include "TTBarReweighting.hh"
#include <stdexcept>

float get_ttbar_reweighting(float pt) {
  // copied from an email from Will, who probably got it from Xiaoxiao
  // or Anna here:
  // https://svnweb.cern.ch/trac/atlasinst/browser/Institutes/Yale/trunk/1LStopBoosted/Root/SelectionTools.cxx#L353
  // They claim they got this from ATLAS-CONF-2013-099 (ttbar diff xsec)
  float pt_gev = pt / 1e3;
  if (pt_gev < 0.0) {
    throw std::domain_error(
      "tried to calculate ttbar pt reweighting for event with negative pt");
  }
  if (pt_gev < 40) {
    return ((1/1.01185 + 1/0.994193)/2);
  } else if (pt_gev < 170) {
    return ((1/1.09592 + 1/1.03448)/2);
  } else if (pt_gev < 340) {
    return ((1/1.40728 + 1/1.31911)/2);
  } else {
    return ((1/1.79938 + 1/1.71078)/2);
  }
}

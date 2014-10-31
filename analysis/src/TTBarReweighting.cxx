#include "TTBarReweighting.hh"

float get_ttbar_reweighting(float pt) {
  // copied from an email from Will, who probably got it from Xiaoxiao
  // or Anna
  if (pt < 40) {
    return ((1/1.01185 + 1/0.994193)/2);
  } else if (pt < 170) {
    return ((1/1.09592 + 1/1.03448)/2);
  } else if (pt < 340) {
    return ((1/1.40728 + 1/1.31911)/2);
  } else {
    return ((1/1.79938 + 1/1.71078)/2);
  }
}

#ifndef PHYSICAL_CONSTANTS_H
#define PHYSICAL_CONSTANTS_H

constexpr double operator"" _TeV (long double en) {
  return en * 1e6;
}
constexpr double operator"" _TeV (unsigned long long en) {
  return en * 1e6;
}

constexpr double operator"" _GeV (long double gev) {
  return gev * 1e3;
}
constexpr double operator"" _GeV (unsigned long long gev) {
  return gev * 1e3;
}
constexpr double operator"" _MeV (long double mev) {
  return mev;
}
constexpr double operator"" _MeV (unsigned long long mev) {
  return mev;
}

const double W_MASS = 80.385_GeV;
const double Z_MASS = 91.188_GeV;


#endif

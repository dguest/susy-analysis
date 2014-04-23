#include "hdf_helpers.hh"


H5::PredType h5::get_type(long long) {
  return H5::PredType::NATIVE_LLONG;
}
H5::PredType h5::get_type(int) {
  return H5::PredType::NATIVE_INT;
}
H5::PredType h5::get_type(double) {
  return H5::PredType::NATIVE_DOUBLE;
}
H5::StrType h5::get_type(const std::string&) {
  return H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);
}

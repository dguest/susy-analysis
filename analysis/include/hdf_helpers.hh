#ifndef HDF_HELPERS_HH
#define HDF_HELPERS_HH

#include "H5Cpp.h"

namespace h5 {
  // attribute adding function
  template<typename M>
  void write_attr(H5::H5Location&, const std::string& name, M val);

  template<typename M>
  void write_attr_vec(H5::H5Location& loc, const std::string& name, M vec);

  // various overloads to use in template
  H5::PredType get_type(long long val);
  H5::PredType get_type(double val);
  H5::PredType get_type(int val);
  H5::StrType get_type(const std::string& val);
}


template<typename M>
void h5::write_attr_vec(H5::H5Location& loc, const std::string& name, M vec)
{
  auto type = get_type(*vec.data());
  hsize_t size = vec.size();
  H5::DataSpace data_space(1, {&size});
  loc.createAttribute(name, type, data_space).write(type, vec.data());
}

template<typename M>
void h5::write_attr(H5::H5Location& loc, const std::string& name, M value)
{
  auto type = get_type(value);
  loc.createAttribute(name, type, H5S_SCALAR).write(type, &value);
}

#endif

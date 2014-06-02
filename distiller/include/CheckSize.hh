#ifndef CHECK_SIZE_HH
#define CHECK_SIZE_HH

#include <string>
#include <stdexcept>

#define CHECK_SIZE(BRANCH, SIZE)\
  CHECK::throw_size(#BRANCH, SIZE, BRANCH->size())



namespace CHECK {
  void throw_size(const std::string& br_name, int expected, int actual);
}


#endif

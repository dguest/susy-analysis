#include "Window.hh"
#include <limits>

namespace nminus {

  Window::Window() :
    min(std::numeric_limits<double>::quiet_NaN()),
    max(std::numeric_limits<double>::quiet_NaN()),
    missing(Missing::THROW)
  {
  }
  Window::Window(double mn, double mx, Missing miss):
    min(mn), max(mx), missing(miss)
  {
  }
}

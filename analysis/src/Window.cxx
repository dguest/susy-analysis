#include "Window.hh"
#include <limits>
#include <stdexcept>

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

  bool accept_missing(const Window& window) {
    switch (window.missing) {
    case Window::Missing::ACCEPT: return true;
    case Window::Missing::REJECT: return false;
    case Window::Missing::THROW: throw std::invalid_argument(
      "required variable is missing");
    default: throw std::logic_error("never go here");
    }
  }
}

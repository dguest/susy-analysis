#ifndef WINDOW_HH
#define WINDOW_HH

namespace nminus {
  struct Window {
    enum class Missing {REJECT, ACCEPT, THROW, PHANTOM};
    Window();
    Window(double, double, Missing = Missing::THROW);
    double min;
    double max;
    Missing missing;
  };

  bool accept_missing(const Window&);
}

#endif

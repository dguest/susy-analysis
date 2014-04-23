#ifndef GRL_HH
#define GRL_HH

#include <string>

namespace Root {
  class TGRLCollection;
}

class Grl
{
public:
  Grl(std::string grl_location = "");
  ~Grl();
  Grl(Grl&) = delete;
  Grl& operator=(Grl&) = delete;
  bool has_lb(int run_number, int lbn);
private:
  Root::TGRLCollection* m_grl;
};


#endif

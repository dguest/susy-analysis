#ifndef BTAG_BUFFER_HH
#define BTAG_BUFFER_HH

class TTree;

#include <string>

enum class TagSF { NOMINAL, UP, DOWN};

class BtagBuffer
{
public:
  BtagBuffer(TTree*, std::string branch);
  double sf(TagSF) const;
private:
  bool m_has_err;
  double m_scale_factor;
  double m_scale_factor_err;
  void set(TTree*, std::string branch, void* branch_address);
};

#endif

#ifndef I_JET_TAG_FILTER_HH
#define I_JET_TAG_FILTER_HH

#include <vector>
class Jet;

class IJetTagFilter
{
public:
  virtual std::vector<Jet> tagged_jets(const std::vector<Jet>&) const = 0;
  virtual ~IJetTagFilter(){};
};

#endif

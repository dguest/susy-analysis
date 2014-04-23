#ifndef BITMAP_CUTFLOW_H
#define BITMAP_CUTFLOW_H

#include <map>
#include <string>
#include <vector>
#include "typedefs.hh"

#include "CutCounter.hh"

class BitmapCutflow
{
public:
  BitmapCutflow(std::string first_name = "n_skimmed_events");
  void add(std::string name, ull_t mask);
  int fill(const ull_t mask);
  std::vector< std::pair<std::string, int> > get() const;
private:
  typedef std::vector<std::pair<ull_t,std::string> > MaskName;
  std::string m_first_name;
  CutCounter m_counter;
  MaskName m_mask_name;
};


#endif // BITMAP_CUTFLOW_H

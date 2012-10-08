#ifndef BITMAP_CUTFLOW_H
#define BITMAP_CUTFLOW_H

#include <map> 
#include <string> 
#include <vector> 

class CutCounter: public std::map<std::string, int> 
{
public: 
  int& operator[](std::string key); 
  std::vector< std::pair<std::string, int> > get_ordered_cuts() const; 
private: 
  std::vector<std::string> m_cuts; 
}; 

class BitmapCutflow 
{ 
public: 
  BitmapCutflow(std::string first_name = "n_events"); 
  void add(std::string name, unsigned mask); 
  int fill(const unsigned mask); 
  std::vector< std::pair<std::string, int> > get() const; 
private: 
  typedef std::vector<std::pair<unsigned,std::string> > MaskName; 
  std::string m_first_name; 
  CutCounter m_counter; 
  MaskName m_mask_name; 
};


#endif // BITMAP_CUTFLOW_H

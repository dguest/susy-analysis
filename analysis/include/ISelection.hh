#ifndef ISELECTION_HH
#define ISELECTION_HH

class EventObjects; 

class ISelection { 
public: 
  virtual bool pass(const EventObjects&) const = 0; 
  virtual ~ISelection() = default; 
}; 

#endif 

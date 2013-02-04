#include <boost/noncopyable.hpp>
#include <vector> 
#include <string>
#include "RunInfo.hh"		// could we remove this struct?  

namespace std { 
  class streambuf; 
}

class StopDistiller : public boost::noncopyable
{ 
public: 
  typedef std::vector<std::pair<std::string, int> > Cutflow; 
  StopDistiller(const RunInfo& info, unsigned flags); 
  Cutflow run_cutflow(std::vector<std::string> in, std::string out);
private: 
  RunInfo m_info; 
  unsigned m_flags; 
  std::streambuf* m_debug_buffer; 
  std::streambuf* m_out_buffer; 
}; 

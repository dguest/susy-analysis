#ifndef BRANCH_BUFFER_HH
#define BRANCH_BUFFER_HH

enum class Save { IF_LISTED, ALWAYS, NEVER };

class TChain;
class ITreeBranch;
class TTree;

// needed for template `set` function
#include "TreeBranch.hh"

#include <set>
#include <string>
#include <vector>

class BranchBuffer {
public:
  BranchBuffer();
  ~BranchBuffer();
  void addInputs(TChain& chain, const std::vector<std::string>& variables);
  std::set<std::string> getExposedInputs() const;
  std::set<std::string> getMissingBranches() const;
  void setPassThrough(TTree&) const;
  void dump() const;

  template<typename T>
  void set(TChain& chain, const std::string& name, T* ptr,
	   Save save = Save::IF_LISTED);

private:
  void setInternal(TChain& chain, const std::string& name, void* dest);
  std::string getBranchClass(TChain& ch, const std::string& name);
  // all "out branches" are set with setPassThrough
  std::map<std::string, ITreeBranch*> m_out_branches;

  // all requested branches (via text file)
  std::set<std::string> m_requested_passthrough;

  // exposed inputs are set to some local buffer (not just passing though)
  std::set<std::string> m_exposed_inputs;
  std::set<std::string> m_missing_inputs;
};

// implementation

template<typename T>
void BranchBuffer::set(TChain& ch, const std::string& name, T* ptr, Save save)
{
  *ptr = 0;
  setInternal(ch, name, ptr);
  if (save == Save::NEVER) return;
  bool listed = (
    save == Save::IF_LISTED && m_requested_passthrough.count(name));
  if (save == Save::ALWAYS || listed ) {
    std::string br_class = getBranchClass(ch, name);
    if (br_class.size() > 0) {
      m_out_branches.insert(
	std::make_pair(name, new ObjBranch<T>(ptr, name, br_class)));
    } else {
      m_out_branches.insert(
	std::make_pair(name, new FlatBranch<T>(ptr, name)));
    }
  }
}

#endif

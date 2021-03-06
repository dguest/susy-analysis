#ifndef ITREE_BRANCH_HH
#define ITREE_BRANCH_HH

class TTree;

class ITreeBranch {
public:
  virtual ~ITreeBranch() {}
  virtual void addToTree(TTree&) = 0;
  virtual void dump() const = 0;
};


#endif

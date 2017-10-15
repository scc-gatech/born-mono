#ifndef TREE_1D_H
#define TREE_1D_H 1
#include "tree_nd.h"
#include "hypercube_float.h"
class tree_1d: public tree{

  public:
  tree_1d(){set_default();}
  tree_1d(tree_1d *par){ set_default(); parent=par;}
  tree_1d(tree *p,tree_loc l, tree_info *in);
  tree_1d(tree_loc l, tree_info *in);
    tree_1d(tree_loc l);
  virtual void add_child(tree_loc l, tree_info *i);
  virtual void rebalance(std::vector<tree*> *branches);
  tree_loc find_mid(std::vector<tree*> *locs);
  ~tree_1d(){}
  virtual void add_limb(){ child.push_back(new tree_1d(this));}
  void split_domain(tree *tree,std::vector<tree*> *locs);
 void split_branches(tree_loc *mid,std::vector<tree*> *locs, 
   std::vector<tree*> *inf,std::vector<tree*> *loc1);
  void add_info(tree_loc *x, float v);
  void compute_back(hypercube_float *match, hypercube_float *tr);
  virtual tree *clone();
  bool jj;
};
#endif

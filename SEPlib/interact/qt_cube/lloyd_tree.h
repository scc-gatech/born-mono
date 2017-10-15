#ifndef LLOYD_TREE_H
#define LLOYD_TREE_H 1
#include "tree_nd.h"
#include "lloyd.h"
#include "hypercube_float.h"
class lloyd_tree_nd:public lloyd{
  public:
  lloyd_tree_nd(){}
  lloyd_tree_nd(hypercube_float *tr, int niter, float pct_delete, float pct_add,tree_nd *myt);
  virtual void partition_space();
  virtual void compute_center();
  virtual void change_regions(const int iter);
  void info_center(hypercube_float *back, tree_loc *base, tree_info_vals *mine);

  hypercube_float *match;
  float pdel,psplit; 
  tree_nd *my_tree;


};
#endif

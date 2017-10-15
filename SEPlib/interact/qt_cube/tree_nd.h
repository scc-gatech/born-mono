#ifndef TREE_ND_H
#define TREE_ND_H 1
#include "tree.h"
#include "hypercube_float.h"

class pt_maps{
  public:
    pt_maps(std::vector<int> x){
      for(int i=0; i < (int)x.size(); i++) ax.push_back(x[i]);
    }
    std::vector<int> ax;
 };
 
class tree_interp{
  public:
   tree_interp(std::vector<bool> use, hypercube_float *array);
   tree_interp(){};
   virtual ~tree_interp(){};
   virtual  float find_val(std::vector<tree*> *branches, tree_loc *loc, bool verb);
   std::vector<pt_maps> my_test;
   std::vector<int> map_to;
   std::vector<tree*> close_trees;
   std::vector<float> dist;
   std::vector<float> vals;
   std::vector<int> mult;
   int ncombos,nsearch;
   hypercube_float *ar;

};


class tree_nd: public tree{

  public:
  tree_nd(){set_default();}
  tree_nd(tree_nd *par){ set_default(); parent=par;}
  tree_nd(tree *p,tree_loc l, tree_info *in,int isp);
  tree_nd(tree_loc l, tree_info *in,int isp);
    tree_nd(tree_loc l);
  virtual void add_child(tree_loc l, tree_info *i);
  virtual void rebalance(std::vector<tree*> *branches);
  tree_loc find_mid(std::vector<tree*> *locs,int *split_axis, tree_loc *par_tree);
  ~tree_nd(){}
  virtual void add_limb(){ child.push_back(new tree_nd(this));}
  void split_domain(tree *tree,std::vector<tree*> *locs,tree_loc *par_loc);
   void split_branches(tree_loc *mid,int split_ax,std::vector<tree*> *locs, 
   std::vector<tree*> *inf,std::vector<tree*> *loc1);
  void add_info(tree_loc *x, float v);
  int find_split_axis(std::vector<tree*> *branches);

  void compute_back(hypercube_float *match, hypercube_float *tr);
  
  
  tree_interp *form_close_dirs(tree_loc *x);
  virtual tree *clone();
  bool jj;
};
#endif

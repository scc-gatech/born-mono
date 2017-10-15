#ifndef TMP_TREE
#define TMP_TREE 1
#include "tree_info.h"
#include "hypercube_float.h"
#include <vector>
#include <map>
class tmp_tree{
 public:
  tmp_tree(){name_tree="reg tree undefined";};
  tmp_tree(tmp_tree *parent, tmp_tree *base,tree_loc *ll, std::vector<int> bef, std::vector<int> end,
     std::vector<int>  sp);
  void basic_tmp_tree_alloc(tmp_tree *parent, tmp_tree *base,tree_loc *ll, std::vector<int> bef, std::vector<int> end,
     std::vector<int>  sp);
  void make_tree_children(std::vector<int> axes, int *loc);
  virtual ~tmp_tree();
  void find_nearest_pt(std::vector<tmp_tree*> *neighs, tree_loc *lc,std::vector<bool> pat,bool verb);
  void find_wind_branches(int *b, int *e, std::vector<tmp_tree*> *branches);
  tmp_tree *find_closest(int *loc);
  void find_neighbors(std::vector<tmp_tree*> *neighs, tree_loc *lc,bool verb);
  void get_branches  (std::vector<tmp_tree*> *branches);
  bool have_tree_children(){if(tree_child.size()==0) return false; return true;}
  void calc_avg(hypercube_float *tr);
  void set_b_e_l(std::vector<int> beg, std::vector<int> end, tree_loc *x);
  void verb(int level);
  void limb_info(int level,int *count);
  float abs_avg(hypercube_float *tr);
  bool single_pt(std::vector<int> ax);
  virtual tmp_tree *clone();
  virtual void init_info(){fprintf(stderr,"In reg tree init\n");}
  void erase_limb(std::string hash_key);
  void set_limb(){
    base->branch_list[hash_key]=this;
  }
  void set_mid(hypercube_float *tr);
  void form_hash_key();
  std::vector<tmp_tree*> tree_child;
  std::vector<int> split_axes;
  std::vector<int> b,e;
  tree_loc *loc;
  tmp_tree *tree_parent;
  tmp_tree *base;
  tree_info *inf;
  int ndiv;
  float val;
  std::string hash_key;
  int npts;
  
  std::string name_tree;
  
  std::map<std::string,tmp_tree*> branch_list;

};
#endif

#ifndef REG_TREE
#define REG_TREE 1
#include "tree_info.h"
#include "hypercube_float.h"
#include <vector>
#include <map>
class reg_tree{
 public:
  reg_tree(){name_tree="reg tree undefined";};
  reg_tree(reg_tree *parent, reg_tree *base,tree_loc *ll, std::vector<int> bef, std::vector<int> end,
     std::vector<int>  sp);
  void basic_reg_tree_alloc(reg_tree *parent, reg_tree *base,tree_loc *ll, std::vector<int> bef, std::vector<int> end,
     std::vector<int>  sp);
  void make_tree_children();
  virtual ~reg_tree();
  void find_nearest_pt(std::vector<reg_tree*> *neighs, tree_loc *lc,std::vector<bool> pat,bool verb);
  void find_wind_branches(int *b, int *e, std::vector<reg_tree*> *branches);
  reg_tree *find_closest(int *loc);
  void find_neighbors(std::vector<reg_tree*> *neighs, tree_loc *lc,bool verb);
  void get_branches  (std::vector<reg_tree*> *branches);
  bool have_tree_children(){if(tree_child.size()==0) return false; return true;}
  void calc_avg(hypercube_float *tr);
  void set_b_e_l(std::vector<int> beg, std::vector<int> end, tree_loc *x);
  void verb(int level);
  void limb_info(int level,int *count);
  float abs_avg(hypercube_float *tr);
  bool single_pt();
  virtual reg_tree *clone();
  virtual void init_info(){fprintf(stderr,"In reg tree init\n");}
  void erase_limb(std::string hash_key);
  void set_limb(){
    base->branch_list[hash_key]=this;
  }
  void set_mid(hypercube_float *tr);
  void form_hash_key();
  std::vector<reg_tree*> tree_child;
  std::vector<int> split_axes;
  std::vector<int> b,e;
  tree_loc *loc;
  reg_tree *tree_parent;
  reg_tree *base;
  tree_info *inf;
  int ndiv;
  float val;
  std::string hash_key;
  int npts;
  
  std::string name_tree;
  
  std::map<std::string,reg_tree*> branch_list;

};
#endif

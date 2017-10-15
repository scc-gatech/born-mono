#ifndef TREE_H
#define TREE_H 1
#include <vector>
#include <assert.h>
#include "tree_info.h"
#include "hypercube_float.h"



class tree{
  public:
  tree(){set_default();}
  
  void clear_info(){ if(inf!=0)delete inf; inf=0;}
  virtual void add_child(std::vector<float> l, tree_info *i){ 
    assert(1==2);
  }
  tree *get_closest(tree_loc *l);
  bool have_children(){ if(child.size()>0) return true; else return false;}
  tree_info *get_info(){return inf;}
  virtual float dist2(tree_loc *p){
    float d=0;
    for(int i=0; i< (int)p->v.size(); i++) d+=(p->v[i]-loc->v[i])*(p->v[i]-loc->v[i]);
    return d;
  }
  tree *get_parent(){
    if(parent==0) return this;
    return parent;
  }
  void set_loc(tree_loc *l){
     if(loc!=0) delete loc;
     loc=new tree_loc(l);
  }
  void set_info(tree_info *in){
     if(inf!=0) delete inf;
     inf=in->clone();
  }
  void clean_tree(bool del_limb=true);
  virtual ~tree(){
    clean_tree();
  }
  void set_default(){ loc=0; inf=0;parent=0;}
  void prune_limb(int ilimb);
  void verb(int level);
  virtual void limb_info(int level,int *count);
  tree_loc find_mid(std::vector<tree_loc>locs);
  virtual void add_limb(){assert(1==0);}
  virtual bool valid_tree(bool verb);
  void  get_infos(std::vector<tree_info*> *infs,bool deep=false);
  void  get_locs(std::vector<tree_loc> *locs);
    void  get_branches(std::vector<tree*> *locs, bool point=false);
  int get_split(){return split;}
  tree *get_close_limb(tree_loc *x, std::vector<bool> less, bool v=false);
  virtual tree *clone(){return 0;}  
  
  tree *get_close_limb(tree_loc *x);
  void split_limb(int isplit, float v);

  void set_split(int s){ split=s;}
  virtual void rebalance(tree_loc *loc){assert(1==0);}
  void helix_to_tree_loc(hypercube_float *back,int i,tree_loc *loc);
  int tree_loc_to_helix(hypercube_float *back, tree_loc *loc){
    int ib=0,nb=1;
    for(int i=0; i < back->get_ndim(); i++){
       ib+=(int)(loc->v[i]*nb);
       nb=nb*back->get_axis(i+1).n;
    //   fprintf(stderr,"CHECK MULT %d %d \n",i,ib);
    }
    return ib;
  }

  tree_loc *loc;
  tree_info *inf;
  int split;
  std::vector<tree *> child;
  tree *parent;
};
#endif

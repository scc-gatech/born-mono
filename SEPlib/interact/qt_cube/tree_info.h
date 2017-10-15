#ifndef TREE_INFO_H 
#define TREE_INFO_H 1
#include<vector>
#include <stdio.h>
#include <assert.h>
class tree_info{
  public:
  tree_info(){};
  virtual tree_info *clone(){assert(0==1); return (tree_info *) 0;}
  virtual ~tree_info(){}
  virtual void set_not_branch(){}
  virtual void verb(int iverb){if(iverb==0){}}
};


class tree_loc{
 public:
  tree_loc(){};
  tree_loc(std::vector<float>l){
    for(int i=0; i < (int)l.size(); i++) v.push_back(l[i]);
  }
  tree_loc(float l){ v.push_back(l);}
  tree_loc(tree_loc *l){
    for(int i=0; i < (int) l->v.size(); i++) v.push_back(l->v[i]);
  }
  std::vector<float> v;
  tree_loc *clone(){
    tree_loc *x=new tree_loc(this);
    return x;
  }
  void verb(int iverb,int split_axis){
    if(iverb>0){
      fprintf(stderr,"LOCATION ");
      if(split_axis==-1)
    //  for(int i=0; i< (int)v.size(); i++) fprintf(stderr,"%f ",v[i]);
         for(int i=0; i< 3; i++) fprintf(stderr,"%f ",v[i]);

      else fprintf(stderr,"%f ",v[split_axis]);
    }
  }
};


class tree_info_vals:public tree_info{
  public:
   tree_info_vals(){};
   void append_info(tree_loc *l, float v) { 
     locs.push_back(*l); vals.push_back(v);
   }
   virtual void verb(int level){if(level==0){}}
   std::vector<tree_loc> locs;
   std::vector<float >  vals;
   tree_info *clone(){
      tree_info_vals *x=new tree_info_vals();
      for(int i=0; i < (int)locs.size(); i++) {
        x->locs.push_back(locs[i]);
        x->vals.push_back(vals[i]);
      }
     x->err=err;
     return x;
   }
   float err;
};

#endif

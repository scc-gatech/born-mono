#include "tree.h"
#include <math.h>

tree *tree::get_closest(tree_loc *p){

  if(have_children()){
    float dist=1e31;
    int iclose=0;
    for(int i=0; i < (int) child.size(); i++){
       float dt=dist2(p);
       if(dt<dist) { dist=dt; iclose=i;}
    }
    return child[iclose]->get_closest(p);
  }
  return this;
}
void tree::helix_to_tree_loc(hypercube_float *back,int ih,tree_loc *loc){
  
  int nb=1;
  for(int i=0; i< back->get_ndim(); i++){
     loc->v[i]=(ih/nb)%back->get_axis(i+1).n;
   //  fprintf(stderr,"SETTING %d %f \n",i,loc->v[i]);
     nb=nb*back->get_axis(i+1).n;
  }
}
void tree::get_infos(std::vector<tree_info*>  *infos,bool deep){
   if(have_children()){
    //  fprintf(stderr,"LOC A %f \n",loc->v[0]);

     for(int i=0; i < (int) child.size(); i++)
       child[i]->get_infos(infos,deep);
   }
   else{
     // fprintf(stderr,"LOC b %f \n",loc->v[0]);

    if(deep) {
      infos->push_back(inf->clone());
      }
    else infos->push_back(inf);
   }
   
 }
 void  tree::get_locs(  std::vector<tree_loc> *locs){
     if(have_children()){
       for(int ic=0; ic < (int) child.size(); ic++) child[ic]->get_locs(locs);
     }
     else locs->push_back(loc);

}

 void  tree::get_branches(  std::vector<tree*> *branches, bool point){
     if(have_children()){
       for(int ic=0; ic < (int) child.size(); ic++) child[ic]->get_branches(branches,point);
     }
     else {
       if(point) {
          branches->push_back(this);
        }
       else branches->push_back(clone());
     }
}
tree_loc tree::find_mid(std::vector<tree_loc>locs){
  
 fprintf(stderr,"DEFINE find_mid later \n");  
 assert(1==0);
}
bool tree::valid_tree(bool v){

  if(loc==0){
     fprintf(stderr,"LOC not assigned \n");
     return false;
  }
  else if(have_children()){
    if(v) fprintf(stderr,"POS %f FOUND %d children (%f %f)\n",loc->v[0],(int)child.size(),
      child[0]->loc->v[0],child[1]->loc->v[0]);
    for(int i=0; i < child.size(); i++){
      bool b=child[i]->valid_tree(v);
      if(!b) {
        fprintf(stderr,"invalid child of %f \n",loc->v[0]);
        return false;
      }
     
    }
  }
  else{
 //   if(!inf){
   //   fprintf(stderr,"info not defined at %f \n",loc->v[0]);
    //  return false;
   // }
  }
  return true;

}
void tree::clean_tree(bool delete_limb){

if(have_children()){
  for(int i=0; i < (int)child.size(); i++){
    if(delete_limb || child[i]->have_children()) {
      child[i]->clean_tree(delete_limb);
      delete child[i];
    }
  }
  child.clear();
  }
  if(inf!=0) delete inf;
  if(loc!=0) delete loc;
  inf=0;
  loc=0;



}
void tree::verb(int level){
  int count=0;
  limb_info(level,&count);


}

void tree::limb_info(int level,int *count){
 if(have_children()){
   for(int i=0; i<(int)child.size(); i++){
     int junk=0;
     child[i]->limb_info(level,&junk);
     *count=*count+junk;
   }
   loc->verb(level,split);
   fprintf(stderr,"split=%d:limb children=%d \n",split,*count);
 }
 else{
   *count=*count+1;
   loc->verb(level,-1);
   fprintf(stderr," branch\n");
   if(level>0 && inf!=0) inf->verb(level);
 }

}

tree *tree::get_close_limb(tree_loc *x, std::vector<bool> less,bool v){
  
  if(!have_children()) return this;
  
  
  
  
  if(less[split]){
    if(child[0]->loc->v[split] > x->v[split]) {
      if(v) fprintf(stderr,"following child0 less condition %d %f %f \n",split,x->v[split],
       child[0]->loc->v[split]);
     return child[0]->get_close_limb(x,less,v);
    }
  }
  if(!less[split]){
     if(v) fprintf(stderr,"following child1 greater condition %d %f %f \n",split,x->v[split],
       child[1]->loc->v[split]);
    if(child[1]->loc->v[split] < x->v[split]) {
      return child[1]->get_close_limb(x,less,v);
    }
  }
  if(v){
    if(less[split]) fprintf(stderr,"Less ");
    else fprintf(stderr,"More ");
    fprintf(stderr,"CHECKING BOTH: split axis=%d look for=%f child0=%f child1=%f \n",
       split,x->v[split],child[0]->loc->v[split],child[1]->loc->v[split]);
  }
  tree *t1=child[0]->get_close_limb(x,less,v);
  tree *t2=child[1]->get_close_limb(x,less,v); 
  if(less[split]){
    if(t2->loc->v[split] < x->v[split]) {
      if(v) fprintf(stderr,"Less condtional both choosing t2 %d %f %f %f \n",
        split,x->v[split],t1->loc->v[split],t2->loc->v[split]);
      return t2;
    }
          if(v) fprintf(stderr,"Less condtional both choosing t1 %d %f %f %f \n",
        split,x->v[split],t1->loc->v[split],t2->loc->v[split]);
    return t1;
  }
  if(t1->loc->v[split] > x->v[split]) {
          if(v) fprintf(stderr,"moore condtional both choosing t1 %d %f %f %f \n",
        split,x->v[split],t1->loc->v[split],t2->loc->v[split]);
    return t1;
  }
        if(v) fprintf(stderr,"More condtional both choosing t2 %d %f %f %f \n",
        split,x->v[split],t1->loc->v[split],t2->loc->v[split]);
  return t2;
  
 
 
 
if(1==2){
  if(!have_children()) return this;
  if(less[split]){
      if(child[0]->loc->v[split] > x->v[split]) {
        if(v) fprintf(stderr,"Choosing child 0 split=%d tree=%f loc=%f %f\n",
           split,child[0]->loc->v[split],x->v[split],child[1]->loc->v[split]);
        return child[0]->get_close_limb(x,less,v);
      }
      if(child[1]->loc->v[split] < x->v[split]){    
       if(v) fprintf(stderr,"Choosing child 1 split=%d tree=%f loc=%f  %f\n",
           split,child[0]->loc->v[split],x->v[split],child[1]->loc->v[split]);
        return child[1]->get_close_limb(x,less,v);
      }
      tree *t1=child[0]->get_close_limb(x,less,v);
     tree *t2=child[1]->get_close_limb(x,less,v); 
  if(v) fprintf(stderr,"Choosing both split=%d tree=%f loc=%f %f  %f\n",
           split,child[0]->loc->v[split],x->v[split],split,child[1]->loc->v[split]);
     if(t2->loc->v[split] <x->v[split]) return t2;
    return t1;
    }
 
  
  if(child[1]->loc->v[split] < x->v[split]) {
   //  fprintf(stderr,"met child 0 %f  < %f x \n",child[0]->loc->v[0],x->v[0]);
     return child[1]->get_close_limb(x,less);
  }
  if(child[0]->loc->v[split] > x->v[split]){
  //   fprintf(stderr,"met child 0 %f  > %f x \n",child[0]->loc->v[0],x->v[0]);
      return child[0]->get_close_limb(x,less);
  }
//  fprintf(stderr,"Entering both because %f (%f %f) \n",x->v[0],child[0]->loc->v[0],child[1]->loc->v[0]);
  tree *t1=child[0]->get_close_limb(x,less);
  tree *t2=child[1]->get_close_limb(x,less); 
//  fprintf(stderr,"met both x=%f (%f %f) \n",x->v[0],t1->loc->v[0],t2->loc->v[0]);
  if(t1->loc->v[split] >x->v[split]) return t1;
 return t2;
 }
}
tree *tree::get_close_limb(tree_loc *x){
  if(!have_children()) return this;

  
  if(x->v[split] <=  child[0]->loc->v[split])
    return child[0]->get_close_limb(x);
  else if(x->v[split] >= child[1]->loc->v[split]) return child[1]->get_close_limb(x);
  tree *t1=child[0]->get_close_limb(x);
  tree *t2=child[1]->get_close_limb(x);
  if(fabs(x->v[split] -
  t1->loc->v[split])
  < fabs(x->v[split] - t2->loc->v[split])) 
    return t1;
  return t2;
}
void tree::prune_limb(int ilimb){
  if(ilimb < 0 || ilimb >= (int)child.size()) {
     fprintf(stderr,"Invalid limb %d %d \n",ilimb,(int)child.size());
     assert(1==0);
  }
  if(child[ilimb]->have_children()){
     child[ilimb]->clean_tree();
  }
  delete child[ilimb];
  child.erase(child.begin()+ilimb);
  
  if(child.size()==1){
  /*
    if(child[0]->have_children()){
       for(int i=0; i < (int)child[0]->child.size(); i++){
         child.push_back(child[0]->child[i]);
         child[i+1]->parent=this;
       }
       for(int i=0; i < (int)child[0]->child.size(); i++) child[0]->child[i]=0; 
       split=child[0]->split;
       child[0]->clean_tree();
       child.erase(child.begin());
      
    }
    else{
    */
      if(child[0]->inf!=0) inf=child[0]->inf->clone();
      child[0]->clean_tree();
      child.clear();
    // }
  }

}
void tree::split_limb(int isplit, float v){
  tree_loc *base=new tree_loc(loc);
  add_limb();
  add_limb();
  int iold,inew;
  if(v < loc->v[isplit]){iold=1; inew=0;}
  else {iold=0; inew=1;}
  
  child[iold]->set_loc(base);
  base->v[isplit]=v; 
  child[inew]->set_loc(base);
  loc->v[isplit]=(child[0]->loc->v[isplit]+child[1]->loc->v[isplit])/2.;
  split=isplit;
  delete base;
}

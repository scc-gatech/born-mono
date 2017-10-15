#include "tree_1d.h"
#include "percentile.h"
#include "seplib.h"
void tree_1d::add_child(tree_loc l, tree_info *i){

  
  child.push_back(new tree_1d(this,l,i));
 
}
tree_1d::tree_1d(tree *p,tree_loc l, tree_info *in){
set_default();
  inf=in->clone();
  loc=new tree_loc(l);
  parent=p;
  set_split(0);

}

tree_1d::tree_1d(tree_loc l, tree_info *in){
set_default();
  inf=in->clone();
  loc=new tree_loc(l);
  parent=0;
    set_split(0);


}
tree_1d::tree_1d(tree_loc l){
set_default();
  
  loc=new tree_loc(l);
  parent=0;
    set_split(0);


}
void tree_1d::rebalance( std::vector<tree*> *branches){
  
//  std::vector<tree*> branches;
  
//  std::vector<tree_loc> locs;
 //  std::vector<tree_info*> infos;
   
   
  // get_locs(&locs);
  //if(do_inf) get_infos(&infos,true);
 // get_branches(&branches);
   tree_loc center=find_mid(branches);

   clean_tree();  //Don't remove branch memory
   set_loc(&center);
   
   

   split_domain(this,branches);



   }
   tree *tree_1d::clone(){
      tree_1d *x=new tree_1d(parent,loc,inf);
     // fprintf(stderr,"CLONING %f %f \n",loc->v[0],x->loc->v[0]);
      return x;   
   }
 void tree_1d::split_domain(tree *myt,std::vector<tree*> *branches){

  tree_loc mid=find_mid(branches);

   if(branches->size()>1){
     std::vector<tree*> branch1,branch2;
     myt->set_loc(&mid);
     split_branches(&mid,branches,&branch1,&branch2);

     myt->add_limb();

     split_domain(myt->child[0],&branch1);

     myt->add_limb();
     myt->split=0;
     split_domain(myt->child[1],&branch2);
     

   }
   else{
    if(myt->loc!=0) delete myt->loc;
    myt->loc=branches->at(0)->loc->clone();
   // fprintf(stderr,"ADDING %f %f \n",myt->loc->v[0],branches->at(0)->loc->v[0]);
    if(myt->inf!=0) delete myt->inf;
    if(branches->at(0)->inf!=0) {
       myt->inf=branches->at(0)->inf->clone();
    }
   }
   
        
 }
void tree_1d::split_branches(tree_loc *mid,std::vector<tree*> *branches, 
   std::vector<tree*> *branch1,std::vector<tree*> *branch2){
   
 

  for(int i=0; i < (int) branches->size(); i++){
   // fprintf(stderr,"CHECK IT %d %f %f \n",i,branches->at(i)->loc->v[0] , mid->v[0]);
    if(branches->at(i)->loc->v[0] < mid->v[0]){
       branch1->push_back(branches->at(i));
       
    
    }
    else{
      branch2->push_back(branches->at(i));
     
    }
  }

}
void tree_1d::add_info(tree_loc *x, float v){
  if(inf==0) inf=new tree_info_vals();
  tree_info_vals *ii=(tree_info_vals*) inf;
  ii->append_info(x,v);
}
void tree_1d::compute_back(hypercube_float *match, hypercube_float *back){
  std::vector<bool> less;
  less.push_back(false);
  for(int i=0; i < (int)back->get_n123(); i++){
 
 
    float lo,hi;
    tree_loc x=tree_loc(1.*i);
    less[0]=true; float pos=get_close_limb(&x,less)->loc->v[0];
    int ip1=(int)(pos+.5);
    lo=match->vals[ip1];
    less[0]=false;  pos=get_close_limb(&x,less)->loc->v[0];
    int ip2=(int)(pos+.5);
    hi=match->vals[ip2];
 
    if(ip2==ip1) back->vals[i]=hi;
    else{
     float f=(float(i)-(float)(ip1))/(float)(ip2-ip1);
     back->vals[i]=(1.-f)*lo+f*hi;
   }
  }
 // srite("junk.H",back->vals,4000);
  //assert(1==2);
 } 
     
tree_loc tree_1d::find_mid(std::vector<tree*> *branches){
   float *buf=new float[branches->size()];
   for(int i=0; i < (int)branches->size(); i++){
      buf[i]=branches->at(i)->loc->v[0];
   //   fprintf(stderr,"BUF IS %d %f \n",i,buf[i]);
    }
   float mid=((float)branches->size()/2.);
   int ib,ie;
   if(branches->size()==1){
      ib=0;ie=0;
   
   }
   else if(branches->size()==2){
     ib=0; ie=1;
   }
   else if(mid-(int)mid > .001) {
     ib=(int) mid;
     ie=ib+1;
   
   }
   else{
      ie=(int) mid;
      ib=ie-1;
   }
   percentile per=percentile();
   float p1=per.find(buf,(int)branches->size(),ib);
  
   float p2=per.find(buf,(int)branches->size(),ie);
   std::vector<float> back;
   back.push_back(0.5*(p1+p2));
   delete []buf;
    // fprintf(stderr,"CHECK SIZE STUF %d %d %d %f\n",ib,ie,(int)branches->size(),back[0]);

   return tree_loc(back);
}

   


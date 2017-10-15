#include "tree_nd.h"
#include "percentile.h"
#include "seplib.h"
void tree_nd::add_child(tree_loc l, tree_info *i){

  
  child.push_back(new tree_nd(this,l,i,0));
 
}
tree_nd::tree_nd(tree *p,tree_loc l, tree_info *in, int isp){
set_default();
  inf=in->clone();
  loc=new tree_loc(l);
  parent=p;
  set_split(isp);

}

tree_nd::tree_nd(tree_loc l, tree_info *in,int isp){
set_default();
  inf=in->clone();
  loc=new tree_loc(l);
  parent=0;
    set_split(isp);


}
tree_nd::tree_nd(tree_loc l){
set_default();
  
  loc=new tree_loc(l);
  //fprintf(stderr,"XXX %f %f \n",l.v[0],l.v[1]);
  parent=0;
    


}
void tree_nd::rebalance( std::vector<tree*> *branches){
  
//  std::vector<tree*> branches;
  
//  std::vector<tree_loc> locs;
 //  std::vector<tree_info*> infos;
   
   
  // get_locs(&locs);
  //if(do_inf) get_infos(&infos,true);
 // get_branches(&branches);
   int split_ax;
  // fprintf(stderr,"IN REBLA \n");
   
   
   std::vector<float> junk;
   for(int i=0; i < branches->at(0)->loc->v.size(); i++) junk.push_back(0);
   for(int i=0; i< branches->size(); i++){
     for(int ia=0; ia < branches->at(0)->loc->v.size(); ia++){
        junk[ia]+=branches->at(i)->loc->v[ia];
     }
   }
   for(int ia=0; ia < branches->at(0)->loc->v.size(); ia++) 
      junk[ia]=junk[ia]/(float)branches->size();
   
   tree_loc mid_loc=tree_loc(junk);
   tree_loc center=find_mid(branches, &split_ax,&mid_loc);

   clean_tree();  //Don't remove branch memory
   set_loc(&center);
   set_split(split_ax);
   
  // fprintf(stderr,"XX1 \n");

   split_domain(this,branches,&center);
   this->parent=0;


   }
   tree *tree_nd::clone(){
      tree_nd *x=new tree_nd(parent,loc,inf,split);
      return x;   
   }
 void tree_nd::split_domain(tree *myt,std::vector<tree*> *branches,tree_loc *base_loc){
  int split_ax;
  tree_loc mid=find_mid(branches,&split_ax,base_loc);
   if(branches->size()>1){
     std::vector<tree*> branch1,branch2;
     myt->set_loc(&mid);
     myt->set_split(split_ax);
     split_branches(&mid,split_ax,branches,&branch1,&branch2);

     myt->add_limb();

 
     split_domain(myt->child[0],&branch1,loc);

     myt->add_limb();
      

     split_domain(myt->child[1],&branch2,loc);
     

   }
   else{
    if(myt->loc!=0) delete myt->loc;
    myt->loc=branches->at(0)->loc->clone();
    fprintf(stderr,"ADDING %f %f \n",myt->loc->v[0],branches->at(0)->loc->v[0]);
    if(myt->inf!=0) delete myt->inf;
    if(branches->at(0)->inf!=0) {
       myt->inf=branches->at(0)->inf->clone();
    }
   }
   
        
 }
void tree_nd::split_branches(tree_loc *mid,int split_ax, std::vector<tree*> *branches, 
   std::vector<tree*> *branch1,std::vector<tree*> *branch2){
   
 

  for(int i=0; i < (int) branches->size(); i++){
   // fprintf(stderr,"CHECK IT %d %f %f \n",i,branches->at(i)->loc->v[0] , mid->v[0]);
    if(branches->at(i)->loc->v[split_ax] < mid->v[split_ax]){
       branch1->push_back(branches->at(i));
       
    
    }
    else{
      branch2->push_back(branches->at(i));
     
    }
  }

}
void tree_nd::add_info(tree_loc *x, float v){
 // if(inf==0) inf=new tree_info_vals();
  tree_info_vals *ii=(tree_info_vals*) inf;
  ii->append_info(x,v);
}

tree_interp::tree_interp(std::vector<bool> use, hypercube_float *array){
    ncombos=1;
    
    ar=array;
    nsearch=1;
    std::vector<int> basic((int)use.size(),0);
    std::vector<int> amap;
    for(int i=0; i < (int) use.size(); i++) {
       ncombos=ncombos*2;
       if(use[i]){
          amap.push_back(i);
          mult.push_back(1);
          nsearch=nsearch*2;
       }
       else mult.push_back(0);
    }
    for(int i=0; i < nsearch;i++){
      int nblock=1;
      for(int idim=0; idim < (int)amap.size(); idim++){
        int cart=((int)i/nblock)%2;
       // fprintf(stderr,"CHECK IT %d %d %d  \n",i,idim,cart,amap[idim]);
        if(cart==0) basic[amap[idim]]=1;
        else basic[amap[idim]]=-1;
        nblock=nblock*2;
      }
      my_test.push_back(pt_maps(basic));
      close_trees.push_back(0);
      dist.push_back(1e31);
    }
   
  }

float tree_interp::find_val(std::vector<tree*> *branches, tree_loc *loc, bool verb){
   
  
   for(int i=0; i < nsearch; i++){
     close_trees[i]=0;
     dist[i]=1e31;
   }
   std::vector<int> comp(loc->v.size(),0);
   for(int ib=0; ib < branches->size(); ib++){
     tree *base=branches->at(ib);
     float dtest=0;
     for(int iax=0; iax < loc->v.size(); iax++){
       if(base->loc->v[iax] > loc->v[iax]) comp[iax]=1;
       else if(base->loc->v[iax] < loc->v[iax]) comp[iax]=-1;
       else comp[iax]=0;
      // if(verb) fprintf(stderr,"COMPARISON CHECK %f %f  %d %d \n",base->loc->v[iax] , loc->v[iax]
      // ,iax,comp[iax]);
       dtest+=mult[iax]*(loc->v[iax]-base->loc->v[iax])*(loc->v[iax]-base->loc->v[iax]);
     // fprintf(stderr,"IN THE LOOP %d %f \n",iax,dtest);
     }
  //if(verb)  fprintf(stderr,"CHECK DTEST %f -%d %d \n",dtest,ib,(int)loc->v.size());
     for(int i=0; i < nsearch; i++){
       bool valid=true;
       for(int iax=0; iax < loc->v.size(); iax++){
    //   if(verb ) fprintf(stderr,"my mult test is %d- %d %d \n",iax,my_test[i].ax[iax],comp[iax]);
         if(my_test[i].ax[iax]*comp[iax] < 0) valid=false;
         
         }
      //   if(valid && verb) fprintf(stderr,"think %f %f is valid %d \n",
        //    base->loc->v[0],base->loc->v[1],i);
         // else if(verb) fprintf(stderr,"invalid condition at %f %f \n",
          //   base->loc->v[0],base->loc->v[1]);
       if(valid && dtest < dist[i]){
         close_trees[i]=base;
         dist[i]=dtest;
        // if(verb)fprintf(stderr,"RESETTING CLOSE %d loc=%f %f dist=%f \n",
         //   i, base->loc->v[0],base->loc->v[1], dist[i]);
       }
     }
   }
 
  float num,den=num=0.;
  for(int i=0; i < nsearch; i++){
    if(close_trees[i]!=0){

      den+=1./(.000001+sqrt(dist[i]));
      int iloc=close_trees[i]->tree_loc_to_helix(ar,close_trees[i]->loc);
      num+=1./(.000001+sqrt(dist[i]))*ar->vals[iloc];
           if(verb) fprintf(stderr,"Found point at %f %f -%d (%f %f %d) \n",
        close_trees[i]->loc->v[0],close_trees[i]->loc->v[1],i,
          1./(.000001+sqrt(dist[i])),ar->vals[iloc],iloc);
    }
  }
 // if(verb) fprintf(stderr,"Point out is %f \n",num/den);
  return num/den;
  
}
/*
tree_interp *tree_nd::form_close_dirs(tree_loc *x){
  std::vector<bool> use;
  for(int i=0; i < (int) x->v->size(); i++) use.push_back(true);
  near_pts *y=new near_pts(use);
  return y;
}
*/
void tree_nd::compute_back(hypercube_float *match, hypercube_float *back){
  

  std::vector<bool> use;
  tree_loc loc2=tree_loc(loc);;
  for(int i=0; i < (int) this->loc->v.size(); i++){
    use.push_back(true);
  }
  tree_interp interp(use,match);
  std::vector<tree*> branches;
  get_branches(&branches,true);

  
  fprintf(stderr,"XXX %d\n",(int)back->get_n123());
  
  for(int i=0; i < (int)back->get_n123(); i++){
  if(i%1000==0) fprintf(stderr,"DO IT %d %d\n",i,(int)back->get_n123());
  helix_to_tree_loc(back,i,&loc2);
    if(i==2+17*32) {
       fprintf(stderr,"CHECK LOC %d %f %f \n",i,loc2.v[0],loc2.v[1]);
    
    }
 // fprintf(stderr,"XXX %d %f \n",i,loc2.v[0]);
    back->vals[i]=interp.find_val(&branches,&loc2,i==2+17*32);
  }
 // assert(1==0);
 verb(2);
 srite("junk.H",back->vals,(int)back->get_n123()*4);
 //assert(1==2);
 } 

int tree_nd::find_split_axis(std::vector<tree*> *branches){


  int ndim=(int)branches->at(0)->loc->v.size();
  
 // fprintf(stderr,"CHECK NDIM %d \n",ndim);
  std::vector<float> sums,vars;
  for(int i=0; i < ndim; i++) {
    sums.push_back(0.);
    vars.push_back(0.);
  }
  for(int ib=0; ib <(int) branches->size(); ib++){
    for(int i=0; i < ndim; i++) sums[i]+=branches->at(ib)->loc->v[i];
  }
  for(int i=0; i < ndim; i++) sums[i]=sums[i]/(float)branches->size();
  for(int ib=0; ib <(int) branches->size(); ib++){
    for(int i=0; i < ndim; i++) 
      vars[i]=(branches->at(ib)->loc->v[i]-sums[i])*(branches->at(ib)->loc->v[i]-sums[i]);
   // fprintf(stderr,"COMPARE %d %f %f %f %f \n",ib,branches->at(ib)->loc->v[0],
    //  branches->at(ib)->loc->v[1],sums[0],sums[1]);
  }
  int imax=0; 
  float maxval=vars[0];
  //fprintf(stderr,"VAR %d %f %f \n",ndim,vars[0],vars[1]);
//  assert(1==0);
  for(int i=1; i < ndim; i++){
    if(vars[i] > maxval) {
       imax=i; maxval=vars[i];
    }
  }
 // fprintf(stderr,"SXX %d \n",(int)loc->v.size());
  //fprintf(stderr,"SPLIT %f %f %d \n",loc->v[0],loc->v[1],imax);
  return imax;
}
     
tree_loc tree_nd::find_mid(std::vector<tree*> *branches, int *splitit,tree_loc *par_tree){
   int split_it=find_split_axis(branches);
   float *buf=new float[branches->size()];
   for(int i=0; i < (int)branches->size(); i++){
      buf[i]=branches->at(i)->loc->v[split_it];
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
   tree_loc x=tree_loc(*par_tree);
   percentile per=percentile();
   float p1=per.find(buf,(int)branches->size(),ib);
  
   float p2=per.find(buf,(int)branches->size(),ie);
   x.v[split_it]=0.5*(p1+p2);

   delete []buf;
   *splitit=split_it;
   return x;
}

   


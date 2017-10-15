#include "lloyd_tree.h"
#include <math.h>
#include "percentile.h"
#include "assert.h"
#include "seplib.h"
lloyd_tree_nd::lloyd_tree_nd(hypercube_float *tr, int ntr, float pct_delete, float pct_add,
  tree_nd *myt){
   psplit=pct_add; 
   pdel=pct_delete;
   match=tr;
   niter=ntr;
   my_tree=myt;
   match=tr;
  }
 void  lloyd_tree_nd::partition_space(){
    axis ax=match->get_axis(1);
    my_tree->clear_info();
    tree_loc loc=tree_loc(my_tree->loc);
    for(int i=0; i< (int)match->get_n123(); i++){
      my_tree->helix_to_tree_loc(match,i,&loc);
      tree_nd *tr=(tree_nd*)my_tree->get_close_limb(&loc);
      fprintf(stderr,"found closest limb for %f %f at %f %f -%d \n",
         loc.v[0],loc.v[1],tr->loc->v[0],tr->loc->v[1],i);
      tr->add_info(&loc,match->vals[i]);
    }
 
  }
void lloyd_tree_nd::compute_center(){
   hypercube_float *calc_back=(hypercube_float*) match->clone_zero();
   
   my_tree->compute_back(match,calc_back);
  // srite("junk.H",calc_back->vals,4000);
   std::vector<tree*> branches;
   my_tree->get_branches(&branches,true);
   for(int ii=0; ii <(int)branches.size(); ii++){
     tree_info_vals *mine=(tree_info_vals*) branches[ii]->inf;
      fprintf(stderr,"CENTER OF %d %d \n",ii,(int)mine->vals.size());
  info_center(calc_back,branches[ii]->loc, mine);
   }
   delete calc_back;
   
  
}

void lloyd_tree_nd::info_center(hypercube_float *back, tree_loc *base, tree_info_vals *mine){
  if(mine->vals.size()==0)  {mine->err=-1.;return;}
  if(mine->vals.size()==1)  {mine->err=0; return;} 
  

  tree_loc change=tree_loc(base);
  float my_min,my_max,sm;
  std::vector<float> diff;
       fprintf(stderr,"a2X 1 \n");

  int it=my_tree->tree_loc_to_helix(match,base);
       fprintf(stderr,"aX3 1 \n");

  diff.push_back(mine->vals[0]-back->vals[it]);
       fprintf(stderr,"2 1 \n");

  my_min=my_max=sm=diff[0];
  float sm2=fabs(back->vals[it]);
     fprintf(stderr,"aXX 1 \n");

  for(int i=1; i < (int) mine->vals.size(); i++){
     int pos=my_tree->tree_loc_to_helix(match,base);
    diff.push_back(mine->vals[i]-back->vals[pos]);
    if(diff[i] > my_max) my_max=diff[i];
    if(diff[i] < my_min) my_min=diff[i];
    sm+=fabs(diff[i]);
    sm2+=fabs(back->vals[pos]);
  }
  
     fprintf(stderr,"b 1 \n");

  if(sm2>0.0) mine->err=sm/sm2;
  else mine->err=0;
  std::vector<float> num;
  float den=0.;
  int ndim= base->v.size();
  for(int i=0; i < ndim; i++) num.push_back(0.);
   
   fprintf(stderr,"XXX 1 \n");
  for(int i=0; i < diff.size(); i++){
    if(sm>0){
      float scale=diff[i]-my_min;
      for(int idim=0; idim< ndim; idim++) num[idim]+=scale*mine->locs[i].v[idim];
      den+=scale;
    }
    else{
      float scale=my_max-diff[i];
      for(int idim=0; idim< ndim; idim++) num[idim]+=scale*mine->locs[i].v[idim];
      den+=scale;
    }
  }
  for(int idim=0; idim < ndim; idim++){
     if(den >.000001)  base->v[idim]=num[idim]/den;
  }
}
void lloyd_tree_nd::change_regions(const int iter){ 
  std::vector<tree_loc*> del_locs;
  std::vector<tree*> branches;
  my_tree->get_branches(&branches);
  int nb=(int)branches.size();
  float *errors=new float[nb];
  for(int ii=0; ii < nb; ii++) {
    tree_info_vals *mine=(tree_info_vals*) branches[ii]->inf;
    errors[ii]=mine->err;
  }
  int ibig=(int)(.90*(float)nb)-1;
  int ismall=(int)(.1*(float)nb);
  if(ismall==0 && branches.size() >2 ) ismall=1;
  if(ibig==nb-1&& branches.size() >2 ) ibig=nb-2;
  //  fprintf(stderr,"CHECK IT %d %d %f %d\n",ismall,ibig,(float)iter/(float)niter,(int)branches.size());

  if((float)iter/(float)niter> .5) ibig=nb-1;
  if((float)iter/(float)niter > .7) ismall=0;
  percentile p=percentile();
  float esplit=p.find(errors,nb,ibig);
  float edel  =p.find(errors,nb,ismall);
  int split_it,delete_it=split_it=0;
  if(edel<.000001) edel=.000001;
  if(esplit < psplit) esplit=psplit;
  if(edel > pdel) edel=pdel;

   //my_tree->verb(2);
//  fprintf(stderr,"RANGE %d-%f %d-%f \n",ismall,edel,ibig,esplit);
  delete [] errors;
  std::vector<tree*> branch_out;
  for(int ii=0; ii <branches.size(); ii++){
    tree_info_vals *mine=(tree_info_vals*) branches[ii]->inf;
    float err=mine->err;
    if(err < edel) { 
       fprintf(stderr,"deleting point %d -%f %f\n",ii,branches[ii]->loc->v[0],err);
    }
    else if(err < esplit){
      branch_out.push_back(branches[ii]);
    }
    else{
    //  fprintf(stderr,"spliting point %d %f \n",ii,branches[ii]->loc->v[0]);
      branch_out.push_back(branches[ii]);
      tree_loc x=tree_loc(branches[ii]->loc);
      for(int i=0; i < x.v.size(); i++) x.v[i]+=.00001;
      branch_out.push_back(new tree_nd(x));
    }
  }
  fprintf(stderr,"THROUGFH X \n");
    my_tree->rebalance(&branch_out);
  fprintf(stderr,"THROUGFH Y\n");

    for(int i=0; i < (int)branches.size();i++) delete branches[i];
    branches.clear();
}

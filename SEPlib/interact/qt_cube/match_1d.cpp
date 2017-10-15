#include "match_1d.h"
#include "seplib.h"
#include "percentile.h"
#include "lloyd_tree.h"




match_1d::match_1d(hypercube_float *ar, float deleteit, float splitit,int nit,int jinit){
  tr=ar;
  del=deleteit;
  split=splitit;
  niter=nit;
  jsamp=jinit;
  fprintf(stderr,"setting niter %d \n",niter);

 }
 
 tree_nd *match_1d::initial_tree(){
   axis ax1=tr->get_axis(1);
      axis ax2=tr->get_axis(2);

   tree_nd *my_tree=new tree_nd();
   float cent1=(float)(int)(ax1.n*0.5);
      float cent2=(float)(int)(ax2.n*0.5);

  tree_loc loc;
  // my_tree->set_loc(&loc);
   std::vector<tree*> branches;
   std::vector<float> xx;
   xx.push_back(0.);
   xx.push_back(0.);
   loc=tree_loc(xx);
   for(int i2=0; i2 < ax2.n; i2+=jsamp){
     xx[1]=i2*1.;
     for(int i1=0; i1< ax1.n; i1+=jsamp){
       xx[0]=i1*1.;
       loc=tree_loc(xx);
       branches.push_back(new tree_nd(&loc));
       branches[branches.size()-1]->inf=new tree_info_vals();
     //  fprintf(stderr,"CHECK SIZE %d %d \n",(int)branches.size(),
      //    (int) branches[branches.size()-1]->loc->v.size());
     }
   }
   /*
   for(int i=0; < ax.n; i++){
     loc=tree_loc((float)i);
     my_tree->find_closest_limb(x)->add_info(x,tr->vals[i]);
   }
   */
    
    //loc.v[0]=500.;
  
   my_tree->rebalance(&branches);

     for(int i=0; i < (int)branches.size(); i++){
     delete branches[i];
   }
   branches.clear();
  my_tree->verb(3);
  //assert(1==0);
 
   return my_tree;
 }
 
 tree_nd *match_1d::make_tree(){
   tree_loc loc;
   tree_nd *my_tree=initial_tree();
    
    
    
   lloyd_tree_nd lloyd=lloyd_tree_nd(tr,niter,del,split,my_tree);
   lloyd.solve_it();
   return my_tree;
  
   }
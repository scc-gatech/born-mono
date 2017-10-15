#include "match_reg.h"
#include <math.h>
#include "seplib.h"
 match_reg::match_reg(std::vector<int> split_axis, hypercube_float *v,int ntr){
    
     tr=v;
     std::vector<int> b,e;
     std::vector<float> avg;
     for(int idim=0; idim < (int)split_axis.size(); idim++){
       axis ax=v->get_axis(split_axis[idim]+1);
       b.push_back(0);
       e.push_back(ax.n);
       avg.push_back(0.5*((float)b[idim]+(float)e[idim]));
     }
     tree_loc loc=tree_loc(avg);
     my_tree=new reg_tree(0,0,&loc,b,e,split_axis);
     niter=ntr;
    // my_tree->calc_avg(tr);
     my_tree->set_mid(tr);
     abs_avg=my_tree->abs_avg(tr);
    
 }
 void match_reg::break_up(float max_err){

    float err=max_break_error(max_err);
    int iter=0;
    while(err > max_err && iter < niter){
      err=max_break_error(max_err);
      iter+=1;
    }
 }
 float match_reg::max_break_error(float max_err){
   std::vector<reg_tree *> branches;
   my_tree->get_branches(&branches);
   std::vector<int> map(tr->get_n123(),0);
   hypercube_float *back=tr->clone();
   
   construct_map_back(branches,&map,back);
   //smooth_back
   std::vector<float> errs(branches.size(),0.);
   float max_e=calc_errors(branches,map,back,&errs);
   split_big_errors(branches,errs,max_err);   
   delete back;
   return max_e;
}
hypercube_float *match_reg::compute_back(){
   std::vector<reg_tree *> branches;
   my_tree->get_branches(&branches);
   std::vector<int> map(tr->get_n123(),0);
   hypercube_float *back=tr->clone();
   construct_map_back(branches,&map,back);
   float *xx=new float[10];
   for(int ib=0; ib < branches.size();ib++){
     reg_tree *tr=branches[ib];
     xx[0]=tr->b[0]; xx[1]=tr->b[1];
     xx[2]=tr->b[0]; xx[3]=tr->e[1];
     xx[4]=tr->e[0]; xx[5]=tr->e[1];
     xx[6]=tr->e[0]; xx[7]=tr->b[1];
     xx[8]=tr->b[0]; xx[9]=tr->b[1];
     srite("loc.H",xx,40);
   }
   fprintf(stderr,"NUMBER BRANCHES=%d \n",branches.size());
  return back;
}

void match_reg::split_big_errors(std::vector<reg_tree*>branches,  std::vector<float> err,
  float emax){
  for(int ib=0; ib < err.size(); ib++){
    if(err[ib]>emax){
     // fprintf(stderr,"IN SPLIT %f %d \n",err[ib],emax);
      branches[ib]->make_tree_children();
      for(int i=0; i < branches[ib]->tree_child.size(); i++){
      //  branches[ib]->tree_child[i]->calc_avg(tr);
          branches[ib]->tree_child[i]->set_mid(tr);

      }
    }
//    else fprintf(stderr,"FAILED SPLIT %d %f \n",ib,err[ib]);
  }
}

float match_reg::calc_errors(std::vector<reg_tree*> branches, std::vector<int> map,
  hypercube_float *back, std::vector<float> *err){
  std::vector<float> count(err->size(),0.);
   for(int i=0; i < map.size(); i++){
   //  err->at(map[i])+=(back->vals[i]-tr->vals[i])*(back->vals[i]-tr->vals[i])/(abs_avg*abs_avg);
     float emax=(back->vals[i]-tr->vals[i])*(back->vals[i]-tr->vals[i])/(abs_avg*abs_avg);
     if(emax >err->at(map[i])) err->at(map[i])=emax;
     count[map[i]]+=1;
    // if(map[i]==15) fprintf(stderr,"XXX %f %f \n",back->vals[i],tr->vals[i]);
   }
   /*
   for(int i=0; i < err->size(); i++){
     if(count[i] >0.) err->at(i)=err->at(i)/count[i];
     reg_tree *tree=branches[i];

   }
   */
   float emax=err->at(0);
   for(int i=0; i < err->size();i++){
       fprintf(stderr,"CHECK ERR %d,%d - %d,%d %f \n"
        ,branches[i]->b[0],branches[i]->b[1],
       branches[i]->e[0],branches[i]->e[1],err->at(i));

     if(emax < err->at(i)) emax=err->at(i);
   }
   return emax;

}
void match_reg::construct_map_back(std::vector<reg_tree*> branches, std::vector<int> *map,
  hypercube_float *back){
  

   for(int ib=0; ib < branches.size(); ib++){
     std::vector<int> block_in,block_out;
     reg_tree *tree=branches[ib];
     
     
    // fprintf(stderr,"BRANCH %d has loc %f,%f and value %f \n",
     //    ib,tree->loc->v[0],tree->loc->v[1],tree->val);
    
     std::vector<reg_tree*> neighs;
     my_tree->find_neighbors(&neighs,tree->loc,ib==1308);
     
 
     block_in.push_back(1);
     block_out.push_back(1);
     std::vector<int> nin,nout;
     for(int i=0; i< tree->b.size(); i++){
       nin.push_back(tree->e[i]-tree->b[i]);
       block_in.push_back(block_in[i]*nin[i]);
       nout.push_back(back->get_axis(i+1).n);      
       block_out.push_back(block_out[i]*nout[i]);
     }
     int ntot=block_in[block_in.size()-1];
   
     for(int i=0; i < ntot; i++){
          std::vector<float> num(neighs.size()+1,0.),den(neighs.size()+1,0.);

       int ih=0;
       float dist=0;
       std::vector<float> mine;
       for(int iax=0; iax < nin.size(); iax++){
         int cart=(i/block_in[iax])%nin[iax];
         ih+=(cart+tree->b[iax])*block_out[iax];
         mine.push_back(cart+tree->b[iax]);
        
       }
               
      
       for(int iax=0; iax < tree->split_axes.size(); iax++){
         int in=0;
         for(in=0; in < neighs.size(); in++){
           float x=(mine[tree->split_axes[iax]]-neighs[in]->loc->v[tree->split_axes[iax]]);
           den[in]+=x*x;
          if(ih==205180) 
             fprintf(stderr,"location %d %f,%f has value %f and is %f away from point (%f,%f) \n",ib,
              neighs[in]->loc->v[0],neighs[in]->loc->v[1],neighs[in]->val,den[in],mine[0],mine[1]);
             
         }
         
         float x=(mine[tree->split_axes[iax]]-tree->loc->v[tree->split_axes[iax]]);
        // fprintf(stderr,"XX %f %d \n",x,tree->split_axes[iax]);
                if(ih==205180)      fprintf(stderr,"center %f,%f has value %f and is %f away from point \n",
              tree->loc->v[0],tree->loc->v[1],tree->val,den[in]);
         den[neighs.size()]+=x*x;
      }
                  

      float nums=tree->val/
        (0.01+den[neighs.size()]);
      float dens=1./(.01+den[neighs.size()]);
      
      for(int in=0; in < neighs.size(); in++) {
        nums+=neighs[in]->val/(.01+den[in]);
        dens+=1./(.01+den[in])  ;
     if(ih==205180) fprintf(stderr,"CHECK SET %f %f %f \n",neighs[in]->val,
1./den[in],
nums/dens);
      }
    
      map->at(ih)=ib;
      back->vals[ih]=nums/dens;
              // back->vals[ih]=tree->val;
//if(ih==205180) fprintf(stderr,"XX %f \n",back->vals[ih]);
    }
   }

 
 }



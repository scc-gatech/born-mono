#include "reg_tree.h"
#include <math.h>


reg_tree::reg_tree(reg_tree *par, reg_tree *bas, tree_loc *ll, std::vector<int> bef, std::vector<int> end,
   std::vector<int> sp){

  basic_reg_tree_alloc(par,base,ll,bef,end,sp);
 name_tree="reg_tree";
}
  void reg_tree::erase_limb(std::string hash_key){
      std::map<std::string,reg_tree*>::iterator iter ;
    base->branch_list.erase(hash_key);
  }
void reg_tree::basic_reg_tree_alloc(reg_tree *par, reg_tree *bas, tree_loc *ll, std::vector<int> bef, std::vector<int> end,
   std::vector<int> sp){
   
   tree_parent=par;
  if(bas==0) base=this;
  else base=bas;
  loc=new tree_loc(ll);
  for(int i=0; i < bef.size(); i++) b.push_back(bef[i]);
  for(int i=0; i < end.size(); i++) e.push_back(end[i]);
  for(int i=0; i < sp.size(); i++) split_axes.push_back(sp[i]);
  inf=0;
  npts=0;
   
   
   }
void reg_tree::make_tree_children(){

std::vector<int> my_split;
 for(int i=0; i < split_axes.size(); i++){
   if(e[split_axes[i]]-b[split_axes[i]]>1){
     my_split.push_back(split_axes[i]);
   }
 }


 ndiv=(int)pow(2.,(float)my_split.size());
  int ntot=b.size();

  std::vector<int> beg(b.size(),0);
  std::vector<int> end(e.size(),0);
  for(int i=0; i < (int)b.size(); i++){
    beg[i]=b[i];
    end[i]=e[i];
  }

  tree_loc x=tree_loc(loc);
  for(int i=0; i < ndiv; i++){
    int nblock=1;
    for(int iax=0; iax < (int)my_split.size(); iax++){
      int cart=((int)i/nblock)%2;
      nblock=nblock*2;
             //  fprintf(stderr,"SETTING IT %d %d %d \n",i,iax,cart);

      if(cart==0){
        
         beg[my_split[iax]]=b[my_split[iax]];
         end[my_split[iax]]=(int)(loc->v[my_split[iax]]);
         
      }
      
      else{
        beg[my_split[iax]]=(int)(loc->v[my_split[iax]]);
        end[my_split[iax]]=e[my_split[iax]];
      }
      x.v[my_split[iax]]=(float)((beg[my_split[iax]]+end[my_split[iax]]))/2.;
    }
    // fprintf(stderr,"CREATING tree_child %d %d-%d %d-%d  %d-%d %f-%f %s\n",
     //     i,beg[0],end[0],beg[1],end[1],beg[2],end[2],x.v[0],x.v[1],name_tree.c_str());
       tree_child.push_back(this->clone());
       tree_child[tree_child.size()-1]->tree_parent=this;

       tree_child[tree_child.size()-1]->set_b_e_l(beg,end,&x);
     // tree_child.push_back(new reg_tree(this,this->base,&x,beg,end,split_axes));
      tree_child[tree_child.size()-1]->init_info();
      base->branch_list[tree_child[tree_child.size()-1]->hash_key]=tree_child[tree_child.size()-1];
//        fprintf(stderr,"TOTAL BRANCHES %d %s\n",(int)base->branch_list.size(),tree_child[tree_child.size()-1]->hash_key.c_str());

    }
  base->erase_limb(hash_key);



}
reg_tree *reg_tree::clone(){
  if(base==0) fprintf(stderr,"Base not set \n");
  reg_tree *x=new reg_tree(this,base, loc,b,e,split_axes);
  return x;
}
void reg_tree::set_b_e_l(std::vector<int> beg, std::vector<int> end, tree_loc *x){
 for(int i=0; i< beg.size(); i++){
   b[i]=beg[i]; e[i]=end[i];
 }
 delete loc;
 loc=new tree_loc(x);
 form_hash_key();
}

bool reg_tree::single_pt(){

  for(int i=0; i< b.size(); i++) {
 // fprintf(stderr,"check sizes %d %d %d \n",i,b[i],e[i]);
    if(e[i]-b[i]>1) return false;
  }
  return true;
}
 void  reg_tree::get_branches(  std::vector<reg_tree*> *branches){
     if(this==base){
       std::map<std::string,reg_tree*>::iterator iter ;
      for ( iter=branch_list.begin(); iter != branch_list.end(); ++iter){
         branches->push_back(iter->second);
       }
       return;
     }
     if(have_tree_children()){
       for(int ic=0; ic < (int) tree_child.size(); ic++) tree_child[ic]->get_branches(branches);
     }
     else {
     //  if(point) {
          branches->push_back(this);
       // }
       //else branches->push_back(clone());
     }
}
void reg_tree::form_hash_key(){

  char temp_ch[30];
  sprintf(temp_ch,"%f",loc->v[0]);
  hash_key=temp_ch;
  for(int i=1; i < loc->v.size(); i++) {
    sprintf(temp_ch,",%f",loc->v[i]);
    hash_key+=temp_ch;
  }
}
reg_tree::~reg_tree(){
  if(tree_parent!=0){
    base->erase_limb(hash_key);
  }
}
float reg_tree::abs_avg(hypercube_float *tr){
  

     std::vector<int> block_in,block_out;
     block_in.push_back(1);
     block_out.push_back(1);
     std::vector<int> nin,nout;
     for(int i=0; i< b.size(); i++){
       nin.push_back(e[i]-b[i]);
       block_in.push_back(block_in[i]*nin[i]);
       nout.push_back(tr->get_axis(i+1).n);      
       block_out.push_back(block_out[i]*nout[i]);
     }
     int ntot=block_in[block_in.size()-1];
    double sm=0;
     for(int i=0; i < ntot; i++){
       int ih=0;
       for(int iax=0; iax < nin.size(); iax++){
         int cart=(i/block_in[iax])%nin[iax];
         ih+=(cart+b[iax])*block_out[iax];
       }
      sm+=fabs(tr->vals[ih]);
    }
    val=sm/(double)ntot;
    return val;
   }
void reg_tree::calc_avg(hypercube_float *tr){
  

     std::vector<int> block_in,block_out;
     block_in.push_back(1);
     block_out.push_back(1);
     std::vector<int> nin,nout;
     for(int i=0; i< b.size(); i++){
       nin.push_back(e[i]-b[i]);
       block_in.push_back(block_in[i]*nin[i]);
       nout.push_back(tr->get_axis(i+1).n);      
       block_out.push_back(block_out[i]*nout[i]);
     }
     int ntot=block_in[block_in.size()-1];
     double sm=0;
     for(int i=0; i < ntot; i++){
       int ih=0;
       for(int iax=0; iax < nin.size(); iax++){
         int cart=(i/block_in[iax])%nin[iax];
         ih+=(cart+b[iax])*block_out[iax];
       }
      sm+=tr->vals[ih];
    }
    val=sm/(double)ntot;
   }
void reg_tree::set_mid(hypercube_float *tr){

       int ih=0;
       int block_out=1;
       for(int iax=0; iax < tr->get_ndim(); iax++){
         ih+=(int)(loc->v[iax]+.5)*block_out;
         block_out=block_out*tr->get_axis(iax+1).n;
       }
     
    
    val=tr->vals[ih];
   }
void reg_tree::find_neighbors(std::vector<reg_tree*> *neighs,tree_loc *lc,bool verb){
  if(tree_child.size()==0) return;
  
  for(int i=0; i<ndiv;i++){ 
    std::vector<bool> pat;
     int nblock=1;
    for(int iax=0; iax < (int)split_axes.size(); iax++){
      int cart=((int)i/nblock)%2;
      nblock=nblock*2;   
      if(cart==0) pat.push_back(false);
      else pat.push_back(true);
   }
   if(verb) {fprintf(stderr,"looking for pattern ");
   if(pat[0]) fprintf(stderr,"T ");
   else fprintf(stderr,"F ");
      if(pat[0]) fprintf(stderr,"T\n");
   else fprintf(stderr,"F\n");
   }
   find_nearest_pt(neighs,lc,pat,verb);
  }
 }

void reg_tree::find_nearest_pt(std::vector<reg_tree*> *neighs, tree_loc *lc,std::vector<bool> pat,bool verb){
 
 std::vector<reg_tree*> tests;
 if(tree_child.size()==0) {
     neighs->push_back(this);
     return;
  }


  if(verb) fprintf(stderr,"Entering location %f %f \n",loc->v[0],loc->v[1]);
  for(int i=0; i < tree_child.size(); i++){
    bool good=true;
    float dist=0;
    for(int iax=0; iax < split_axes.size(); iax++){
      if(!pat[iax]){ //Less case
        if(b[iax] > lc->v[iax]) good=false;
      }
      else if(!pat[iax]){
        if(e[iax] < lc->v[iax]) good=false;
      }
   }
   if(good) tree_child[i]->find_nearest_pt(&tests,lc,pat,verb);
 }
 float dmin;
 int iclose=-1;
 for(int i=0; i < tests.size(); i++){
   bool good=true;
   float dist=0;
   for(int iax=0; iax < split_axes.size(); iax++){
    if(!pat[iax] && tests[i]->loc->v[iax] > lc->v[iax]) good=false;
    else if( pat[iax] && tests[i]->loc->v[iax] < lc->v[iax]) good=false;
    else dist+=(lc->v[iax]-tests[i]->loc->v[iax]+.0001)*(lc->v[iax]-tests[i]->loc->v[iax]+.0001);
  }
  if(good){
    if(verb) fprintf(stderr,"Position %f,%f is %f away and meets criteria \n",
      tests[i]->loc->v[0],tests[i]->loc->v[1],dist);
    if(iclose==-1){
      iclose=i; dmin=dist;
    }
    else if(dist< dmin && dist>.01){
      iclose=i; dmin=dist;
   }
 }
 }
 if(iclose!=-1) neighs->push_back(tests[iclose]);
 return;
}
void reg_tree::find_wind_branches(int *bt, int *et, std::vector<reg_tree*> *branch_sub){
   if(base!=this) assert(1==0);
    std::map<std::string,reg_tree*>::iterator iter ;
   
    for ( iter=branch_list.begin(); iter != branch_list.end(); ++iter){
     bool good=true;
     int i=0;
     while(i<iter->second->b.size() && good){
       if(bt[i] >= iter->second->e[i] || iter->second->b[i] >=et[i]) {
       //  fprintf(stderr,"rejecting %d %d %d %d %d  (%f %f %f) \n",i,bt[i],iter->second->e[i],
        //   iter->second->b[i],et[i],
  // iter->second->loc->v[0],
   //iter->second->loc->v[1],
   //iter->second->loc->v[2]);
        good=false;
       }
       i+=1;
     }
     if(good) branch_sub->push_back(iter->second);
   }
 }
reg_tree *reg_tree::find_closest(int *loc){
  if(tree_child.size()==0) {
    
     return this;
     
     }
  float dmin=0;
  int iclose=0;
  for(int i=0; i < split_axes.size(); i++){
   float x=tree_child[0]->loc->v[split_axes[i]]-loc[split_axes[i]]-.0001;
   dmin+=x*x;
  }
  for(int ic=1; ic < tree_child.size(); ic++){
    float dist=0.;
     for(int i=0; i < split_axes.size(); i++){
       float x=tree_child[ic]->loc->v[split_axes[i]]-loc[split_axes[i]]-.0001;
       dist+=x*x;
     }
     if(dist< dmin) { iclose=ic; dmin=dist;}
    // fprintf(stderr,"looking at child %d %d %d %f (%f %f %f -%f) \n",ic,(int)tree_child.size(),
    // iclose,dmin,
     // tree_child[ic]->loc->v[0],tree_child[ic]->loc->v[1],tree_child[ic]->loc->v[2],dist);
  }
  return  tree_child[iclose]->find_closest(loc);
}
void reg_tree::verb(int level){
  int count=0;
  limb_info(level,&count);


}

void reg_tree::limb_info(int level,int *count){
 if((int)tree_child.size()>0){
   for(int i=0; i<(int)tree_child.size(); i++){
     int junk=0;
     tree_child[i]->limb_info(level,&junk);
     *count=*count+junk;
   }
   loc->verb(level,-1);
   fprintf(stderr,"limb children=%d \n",*count);
      if(level>0 && inf!=0) inf->verb(level);

 }
 else{
   *count=*count+1;
   loc->verb(level,-1);
   fprintf(stderr," branch\n");
   if(level>0 && inf!=0) inf->verb(level);
 }

}

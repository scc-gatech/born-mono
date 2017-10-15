#include "io_func.h"
#include "iostream"
void io_func::set_basics(util *pars, hypercube *h){
     par=pars;
   
     hyper=new hypercube();
     std::vector<axis> axes;
     for(int i=0; i < h->get_ndim(); i++) axes.push_back(h->get_axis(i+1));
     for(int i=h->get_ndim(); i<8; i++) axes.push_back(axis(1));
     hyper->set_axes(axes);
     non_byte=true;
     changed=false;
     isregion=false;
}

    
   
    
void io_func::read_block_float(int *nw, int *fw, float *buf){
  if(nw==0 && fw==0 && buf==0){}
      char errmsg[] = "Undefined read_block_float for this type";
      par->error(errmsg);
 }
void io_func::read_block_byte(int *nw, int *fw, unsigned char *buf){
  if(nw==0 && fw==0 && buf==0){}
      char errmsg[] = "Undefined read_block_byte for this type";
      par->error(errmsg);
   }
void io_func::convert_to_local(int *nwin, int *fwin, int *nwout, int *fwout){
     for(int i=0; i<8; i++){
       nwout[i]=nwin[i];
       fwout[i]=fwin[i];
     }
}
hypercube *io_func::return_hyper(){
  char errmsg[] = "Hypercube not set";
  if(hyper==0) par->error(errmsg);
  return hyper;
}

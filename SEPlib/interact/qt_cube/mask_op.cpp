#include "mask_op.h"

   
mask_float_op::mask_float_op(hypercube_float *mod,std::vector<bool>ms){
 set_domain(mod);
 set_range(mod);
 if(mod->get_n123()!=ms.size())
   fprintf(stderr,"mask and model space not the same size\n");
 mask=&ms;
}
bool mask_float_op::forward(bool add, my_vector *model, my_vector *data){
  if(!check_domain_range(model,data)) {
    fprintf(stderr,"mask_float_op failed \n");
    return false;
  }
  hypercube_float *m=(hypercube_float*) model;
  hypercube_float *d=(hypercube_float*) data;
  if(!add) d->zero();
  for(long long i=0; i < m->get_n123(); i++){
         if(mask->at(i))d->vals[i]+=m->vals[i]*scale;
    
    
   }
  return true;

}
bool mask_float_op::adjoint(bool add, my_vector *model, my_vector *data){

 if(!check_domain_range(model,data)) {
    fprintf(stderr,"mask_float_op failed \n");
    return false;
  }
  hypercube_float *m=(hypercube_float*) model;
  hypercube_float *d=(hypercube_float*) data;
  if(!add) m->zero();
       for(long long i=0; i < m->get_n123(); i++){
      if(mask->at(i))      m->vals[i]+=d->vals[i]*scale;
    
    
   }
  
  return true;


}

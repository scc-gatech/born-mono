#include "helicon.h"

helicon_float::helicon_float(hypercube_float *mod,float_nfilter *f){

  if(!f->check_match(mod->get_n123()))
    fprintf(stderr,"ERROR: helicon_float filter and model not the same size\n");
  set_domain(mod);
  set_range(mod);
  filt=f;

}

bool helicon_float::forward(bool add, my_vector *model, my_vector *data){

 if(!check_domain_range(model,data)) {
    fprintf(stderr,"helicon_float failed \n");
    return false;
 }
 
 hypercube_float *m=(hypercube_float*) model;
 hypercube_float *d=(hypercube_float*) data;
 double w=0;
 if(add) w=1.;
 d->scale_add(w,m,scale);
 long long n123=m->get_n123();
 float_filter *f;
 
 for(long long i=0; i <n123; i++){
   f=filt->get_filt(i);
  // fprintf(stderr,"CHECK SIZE %d %d \n",(int)i,(int)f->filt.size());
   for(int ifilt=0; ifilt< f->filt.size(); ifilt++){
     long long ix=i-f->lag[ifilt];
     if(ix >=0 && ix < n123) d->vals[i]+=m->vals[ix]*f->filt[ifilt]*scale;
   }
 }
 
 return true;
}
bool helicon_float::adjoint(bool add, my_vector *model, my_vector *data){
 if(!check_domain_range(model,data)) {
    fprintf(stderr,"helicon_float failed \n");
    return false;
 }


 hypercube_float *m=(hypercube_float*) model;
 hypercube_float *d=(hypercube_float*) data;
 double w=0;
 if(add) w=1.;
 m->scale_add(w,d,scale);
 long long n123=m->get_n123();
 
 for(long long i=0; i <n123; i++){
   float_filter *f=filt->get_filt(i);
   for(int ifilt=0; ifilt< f->filt.size(); ifilt++){
     long long ix=i-f->lag[ifilt];
     if(ix >=0 && ix < n123) m->vals[ix]+=d->vals[i]*f->filt[ifilt]*scale;
   }
 }
 
return true;

}


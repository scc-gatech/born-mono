#include "polydiv.h"

polydiv_float::polydiv_float(hypercube_float *mod,float_nfilter *f){

  if(!f->check_match(mod->get_n123()))
    fprintf(stderr,"ERROR: polydiv_float filter and model not the same size\n");
  set_domain(mod);
  set_range(mod);
  filt=f;
  t=mod->clone();

}

bool polydiv_float::forward(bool add, my_vector *model, my_vector *data){
 if(!check_domain_range(model,data)) {
    fprintf(stderr,"polydiv_float failed \n");
    return false;
 }

 hypercube_float *m=(hypercube_float*) model;
hypercube_float *d=(hypercube_float*) data;
 
 
// fprintf(stderr,"IN POLYDIV FORWARD \n");

 t->scale_add(0.,m,1.);
 long long n123=m->get_n123();
 
 for(long long i=0; i <n123; i++){
   float_filter *f=filt->get_filt(i);
   for(int ifilt=0; ifilt< f->filt.size(); ifilt++){
     long long ix=i-f->lag[ifilt];
     if(ix >=0 && ix < n123) t->vals[i]-=t->vals[ix]*f->filt[ifilt];
    
     }
 }
 double s;
 if(add) s=1.;

 d->scale_add(s,t,scale);
 
 return true;

}
bool polydiv_float::adjoint(bool add, my_vector *model, my_vector *data){
 if(!check_domain_range(model,data)) {
    fprintf(stderr,"polydiv_float failed \n");
    return false;
 }


 hypercube_float *m=(hypercube_float*) model;
 hypercube_float *d=(hypercube_float*) data;
 t->scale_add(0.,d,1.);
long long n123=m->get_n123();
 for(long long i=n123-1; i >=0; i--){
   float_filter *f=filt->get_filt(i);
   for(int ifilt=0; ifilt< f->filt.size(); ifilt++){
     long long ix=i-f->lag[ifilt];
     if(ix >=0 && ix < n123) t->vals[ix]-=t->vals[i]*f->filt[ifilt];
   }
 }
 double s;
 if(add) s=1.;
 m->scale_add(s,t,scale);
 //d->info("dd",10);
 //0m->info("mm",10);

 return true;

}

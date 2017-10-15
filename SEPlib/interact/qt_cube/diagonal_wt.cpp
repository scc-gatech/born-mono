#include "diagonal_wt.h"

diagonal_wt_float::diagonal_wt_float(hypercube_float *mod,hypercube_float *w){

  if(!w->check_match(mod))
    fprintf(stderr,"ERROR: diagonal_wt_float wt and model not the same size\n");
  set_domain(mod);
  set_range(mod);
  hypercube_float *we=(hypercube_float*) w;
  for(long long i=0; i< we->get_n123(); i++)
    wt.push_back(we->vals[i]);

}

bool diagonal_wt_float::forward(bool add, my_vector *model, my_vector *data){

 if(!check_domain_range(model,data)) {
    fprintf(stderr,"diagonal_wt_float failed \n");
    return false;
 }
 if(!add) data->zero();
 hypercube_float *m=(hypercube_float*) model;
 hypercube_float *d=(hypercube_float*) data;
 long long n123=m->get_n123();
 
 for(long long i=0; i <n123; i++){
   d->vals[i]+=m->vals[i]*scale*wt[i];
  
 }
 //fprintf(stderr,"DIAGONAL FORD %f %f %f \n",scale,wt[0],wt[1]);
 //m->info("MODEL IN",10);
 //d->info("DATA OUT",10);
 
 return true;
}
bool diagonal_wt_float::adjoint(bool add, my_vector *model, my_vector *data){
 if(!check_domain_range(model,data)) {
    fprintf(stderr,"diagonal_wt_float failed \n");
    return false;
 }
  if(!add) model->zero();

 hypercube_float *m=(hypercube_float*) model;
 hypercube_float *d=(hypercube_float*) data;
long long n123=m->get_n123();
  for(long long i=0; i <n123; i++){

   m->vals[i]=+d->vals[i]*scale*wt[i];
 }
return true;

}


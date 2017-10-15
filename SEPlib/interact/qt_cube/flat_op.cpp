#include "flat_op.h"

flat_op::flat_op(hypercube_float *mod,hypercube_float *dat,floatg e){

  
  set_domain(mod);
  set_range(dat);
  a1=dat->get_axis(1);
  a2=dat->get_axis(2);
  a3=dat->get_axis(3);
  eps=e;
  
}

bool flat_op::forward(bool add, my_vector *model, my_vector *data){

 if(!check_domain_range(model,data)) {
    fprintf(stderr,"flat_op failed \n");
    return false;
 }
 if(!add) data->zero();
 hypercube_float *m=(hypercube_float*) model;
 hypercube_float *d=(hypercube_float*) data;
 long long n123=m->get_n123();
 long long i=0;
 for(int i3=0; i3 < a3.n; i3++){
   for(int i2=0; i2 < a2.n; i2++){
     i++;
     for(int i1=1; i1< a1.n; i1++,i++){
       d->vals[i]+=eps*(m->vals[i]-m->vals[i-1]);
     }
   }
 }
 
 i=0;
 for(int i3=0; i3 < a3.n; i3++){
   for(int i2=1; i2 < a2.n; i2++){
     i+=a1.n;
     for(int i1=0; i1< a1.n; i1++,i++){
       d->vals[i+n123]+=m->vals[i]-m->vals[i-a1.n];
     }
   }
 }
 i=0;
 for(int i3=1; i3 < a3.n; i3++){
   i+=a1.n*a2.n;
   for(int i2=0; i2 < a2.n; i2++){
     for(int i1=0; i1< a1.n; i1++,i++){
       d->vals[i+n123*2]+=m->vals[i]-m->vals[i-a1.n*a2.n];
     }
   }
 }
 return true;
}
bool flat_op::adjoint(bool add, my_vector *model, my_vector *data){
 if(!check_domain_range(model,data)) {
    fprintf(stderr,"flat_op failed \n");
    return false;
 }
  if(!add) model->zero();

 hypercube_float *m=(hypercube_float*) model;
 hypercube_float *d=(hypercube_float*) data;
long long n123=m->get_n123();
 long long i=0;
 for(int i3=0; i3 < a3.n; i3++){
   for(int i2=0; i2 < a2.n; i2++){
     i++;
     for(int i1=1; i1< a1.n; i1++,i++){
       m->vals[i]+=eps*d->vals[i];
       m->vals[i-1]-=eps*d->vals[i];
     }
   }
 }
 
 i=0;
 for(int i3=0; i3 < a3.n; i3++){
   for(int i2=1; i2 < a2.n; i2++){
     i+=a1.n;
     for(int i1=0; i1< a1.n; i1++,i++){
       m->vals[i]+=d->vals[i+n123];
       m->vals[i-a1.n]-=d->vals[i+n123];
     }
   }
 }
 i=0;
 for(int i3=1; i3 < a3.n; i3++){
   i+=a1.n*a2.n;
   for(int i2=0; i2 < a2.n; i2++){
     for(int i1=0; i1< a1.n; i1++,i++){
       m->vals[i]+=d->vals[i+n123*2];
       m->vals[i-a1.n*a2.n]-=d->vals[i+n123*2];

     }
   }
 }
 
return true;

}


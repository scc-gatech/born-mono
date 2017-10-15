#include "mat_mult.h"


matmult_float_op::matmult_float_op(int n_1, int n_2, float *a){

 n1=n_1; n2=n_2;
 float_1d *m=new float_1d(n1);
 float_1d *d=new float_1d(n2);
 set_domain(m);
 set_range(d);
 delete d; delete m;
 ar=a;

}
bool matmult_float_op::forward(bool add, my_vector *model, my_vector *data){
  if(!check_domain_range(model,data)) {
    fprintf(stderr,"matmult_float_op failed \n");
    return false;
  }
  float_1d *m=(float_1d*) model;
  float_1d *d=(float_1d*) data;
  if(!add) d->zero();
  for(int i2=0; i2 < n2; i2++){
    for(int i1=0; i1 < n1; i1++){
      d->vals[i1]+=ar[i1+i2*n1]*m->vals[i2]*scale;
    }
    
   }
  return true;

}
bool matmult_float_op::adjoint(bool add, my_vector *model, my_vector *data){

 if(!check_domain_range(model,data)) {
    fprintf(stderr,"matmult_float_op failed \n");
    return false;
  }
  float_1d *m=(float_1d*) model;
  float_1d *d=(float_1d*) data;
  if(!add) m->zero();
    for(int i2=0; i2 < n2; i2++){
    for(int i1=0; i1 < n1; i1++){
      m->vals[i2]+=ar[i1+i2*n1]*d->vals[i1]*scale;
    }
    
   }
  
  return true;


}

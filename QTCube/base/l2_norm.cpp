#include "l2_norm.h"
using namespace SEP;
void l2_norm::calc_coefs(my_vector *res,my_vector *x,my_vector *c0, my_vector *c1,my_vector *c2){ 
   
   
  c0->scale_add(0.,res,.5);
  c0->mult(res);
  c1->scale_add(0.,res,1.);
  c2->set_val(1.);
}

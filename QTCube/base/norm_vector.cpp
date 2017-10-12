#include "norm_vector.h"
using namespace SEP;
     void norm_vector::calc_norm_deriv(my_vector *rt,my_vector *c0, my_vector *c1,my_vector *c2){
      _norm->calc_coefs(this,rt,c0,c1,c2);
    }

#ifndef l2_norm_H
#define l2_norm_H 1
#include "norm.h"
#include<assert.h>
#include <string.h>
#include<my_vector.h>
class l2_norm: public norm{
  public:
  l2_norm(){ strcpy(what,"l2 norm");};
  virtual norm *clone(){return (norm*) new l2_norm();}
  virtual ~l2_norm(){};


  virtual void calc_coefs(my_vector *res,my_vector *x,my_vector *co, my_vector *c1,my_vector *c2);
    


};
#endif

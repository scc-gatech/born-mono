#ifndef l2_norm_H
#define l2_norm_H 1
#include <string.h>
#include "norm.h"
#include<assert.h>
#include<my_vector.h>
namespace SEP{
class l2_norm: public norm{
  public:
  l2_norm(){ strcpy(what,"l2 norm");};
  virtual norm *clone(){return (norm*) new l2_norm();}
  virtual ~l2_norm(){};


  virtual void calc_coefs(SEP::my_vector *res,SEP::my_vector *x,SEP::my_vector *co, SEP::my_vector *c1,SEP::my_vector *c2);
    


};

}
#endif

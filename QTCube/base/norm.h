#ifndef NORM_H
#define NORM_H 1
#include<assert.h>
#include "my_vector.h"
#include <string.h>
namespace SEP{
class norm{
  public:
  norm(){strcpy(what,"default");};
  virtual ~norm(){
   if(1==0){;}; };
  virtual my_vector *return_param_vec(my_vector *vec){ return vec->clone_vec();}
  virtual norm *clone(){assert(1==2);}
  virtual void norm_scale(my_vector *vin, my_vector *vout){ vout->scale_add(0.,vin,1.);}
  virtual void calc_coefs(my_vector *res,my_vector *param,my_vector *co, my_vector *c1,
  my_vector *c2){
   if(res==0 && param==0 && co==0 && c1==0 && c2==0){;};
    assert(1==0);
  }

  char what[88];
  bool rite;

};
}
#endif

#ifndef STEP_H
#define STEP_H 1
#include<norm_vector.h>

class step{
  public:
    step(){};
    virtual void alloc_step(my_vector *mod, norm_vector *dat){  
    }
    virtual bool step_it(int iter, my_vector *x, my_vector *g, norm_vector *rr, norm_vector *gg,double *sc){
    return false;}
  //  virtual void scale_residual(norm_vector *in,norm_vector *out){ out->scale_add(0.,in,1.);}

};

#endif


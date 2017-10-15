#ifndef polydiv_H
#define polydiv_H 1
#include "nfilter.h"
#include "my_operator.h"
#include "hypercube_float.h"
#include "timer.h"
class polydiv_float: public my_operator{
  public:
    polydiv_float(){t=0;}
    polydiv_float(hypercube_float *mod,float_nfilter *f);
    bool forward(bool add, my_vector *model, my_vector *data);
    bool adjoint(bool add, my_vector *model, my_vector *data);
    ~polydiv_float(){ if(t!=0) delete t;}

  float_nfilter *filt;
  hypercube_float *t;
  timer time;
};
#endif

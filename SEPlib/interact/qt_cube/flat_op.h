#ifndef FLAT_OP_H
#define FLAT_OP_H 1
#include "nfilter.h"
#include "my_operator.h"
#include "hypercube_float.h"
class flat_op: public my_operator{
  public:
    flat_op(){}
    flat_op(hypercube_float *mod,hypercube_float *dat,float e);
    bool forward(bool add, my_vector *model, my_vector *data);
    bool adjoint(bool add, my_vector *model, my_vector *data);
    ~flat_op(){}
    axis a1,a2,a3;
    float eps;
};
#endif

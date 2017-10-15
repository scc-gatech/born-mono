#ifndef HELICON_H
#define HELICON_H 1
#include "nfilter.h"
#include "my_operator.h"
#include "hypercube_float.h"
class helicon_float: public my_operator{
  public:
    helicon_float(){}
    helicon_float(hypercube_float *mod,float_nfilter *f);
    bool forward(bool add, my_vector *model, my_vector *data);
    bool adjoint(bool add, my_vector *model, my_vector *data);
    ~helicon_float(){}

  float_nfilter *filt;

};
#endif

#ifndef MASK_OP_H
#define MASK_OP_H 1
#include "hypercube_float.h"
#include "my_operator.h"
class mask_float_op: public my_operator{
  public:
    mask_float_op(){basic_init_op();}
    mask_float_op(hypercube_float *mod, std::vector<bool> *ms);
    virtual bool forward(bool add, my_vector *model, my_vector *data);
    virtual bool adjoint(bool add, my_vector *model, my_vector *data);

  private:
    std::vector<bool> *mask;


};
#endif

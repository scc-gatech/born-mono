#ifndef I_OP_H
#define I_OP_H 1
#include "my_operator.h"
#include "norm_vector.h"
class i_op: public my_operator{
  public:
  i_op(norm_vector *mod, norm_vector *dat);
  virtual bool forward(bool add, my_vector *model, my_vector *data);
  virtual bool adjoint(bool add, my_vector *model, my_vector *data);






};
#endif

#ifndef MATMULT_OP_H
#define MATMULT_OP_H 1
#include "float_1d.h"
#include "my_operator.h"
class matmult_float_op: public my_operator{
  public:
    matmult_float_op(){basic_init_op();}
    matmult_float_op(int n1, int n2, float *a);
    virtual bool forward(bool add, my_vector *model, my_vector *data);
    virtual bool adjoint(bool add, my_vector *model, my_vector *data);

  private:
    float *ar;
    int nm,nd;


};
#endif

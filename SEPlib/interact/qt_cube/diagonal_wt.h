#ifndef DIAGONAL_WT_H
#define DIAGONAL_WT_H 1
#include "nfilter.h"
#include "my_operator.h"
#include "hypercube_float.h"
class diagonal_wt_float: public my_operator{
  public:
    diagonal_wt_float(){}
    diagonal_wt_float(hypercube_float *mod,hypercube_float *w);
    bool forward(bool add, my_vector *model, my_vector *data);
    bool adjoint(bool add, my_vector *model, my_vector *data);
    ~diagonal_wt_float(){wt.clear();}
    void reset_wt(hypercube_float *ff){
      for(int i=0; i < wt.size(); i++) wt[i]=ff->vals[i];
     }

    std::vector<float> wt;

};
#endif

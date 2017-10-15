#ifndef NORM_VECTOR_H
#define NORM_VECTOR_H 1
#include <string>
#include "norm.h"
#include "l2_norm.h"
#include <stdio.h>
#include <assert.h>
#include "my_vector.h"
class norm_vector: public my_vector{

  public:
    norm_vector(){ perc=0.; _norm=new l2_norm();}
  
    void set_percent(float p){
      perc=p/100.;
    }
    void set_norm(norm *newnorm){
      if(_norm!=0)    delete _norm;
      if(newnorm!=0)  _norm=newnorm->clone();
      else _norm=new l2_norm();
    }
    virtual void norm_scale(my_vector *norm_out){assert(1==2);if(norm_out==0){}}
    virtual void calc_norm_deriv(my_vector *rw,my_vector *c0, my_vector *c1,my_vector *c2);
    virtual my_vector *return_norm_param(){ return _norm->return_param_vec(this);}
    virtual ~norm_vector(){ delete _norm;}
  
    float perc;
      norm *_norm;
    





};
#endif

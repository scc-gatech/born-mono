#ifndef NORM_VECTOR_H
#define NORM_VECTOR_H 1
#include <string>
#include "norm.h"
#include "l2_norm.h"
#include <stdio.h>
#include <assert.h>
#include "my_vector.h"
#include <memory>
namespace SEP{
class norm_vector: public my_vector{

  public:
    norm_vector(){ perc=0.; 
       std::shared_ptr<l2_norm> x(new l2_norm());
         _norm=x;
    }
  
    void set_percent(float p){
      perc=p/100.;
    }
    void set_norm(std::shared_ptr<SEP::norm> newnorm){
       if(!newnorm){
         std::shared_ptr<l2_norm> x(new l2_norm());
         _norm=x;
       }
       else _norm=newnorm;

    }
    virtual void norm_scale(SEP::my_vector *norm_out){assert(1==2);if(norm_out==0){;};}
    virtual void calc_norm_deriv(SEP::my_vector *rw,SEP::my_vector *c0, SEP::my_vector *c1,SEP::my_vector *c2);
    virtual SEP::my_vector *return_norm_param(){ return _norm->return_param_vec(this);}
    virtual ~norm_vector(){}
  
    float perc;
      std::shared_ptr<SEP::norm> _norm;
    





};

}
#endif

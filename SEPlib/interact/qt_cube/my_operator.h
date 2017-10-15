#ifndef MY_OPERATOR_H
#define MY_OPERATOR_H 1
extern "C" {
#include "sep3d.h"
}
#include<norm_vector.h>
class my_operator{
  public:
    my_operator(){basic_init_op();}
    ~my_operator(){
     if(domain!=0) delete domain;
      if(range!=0) delete range;
      domain=0;
      range=0; 
      strcpy(name,"MY OPERATOR");
    };
    void basic_init_op(){
      domain=0; range=0; solver_own=false; scale=1.;
    }
    bool check_domain_range(my_vector *dom, my_vector *range);
    virtual bool forward(bool add, my_vector *model, my_vector *data){return false;}
    virtual bool adjoint(bool add, my_vector *model, my_vector *data){return false;}
    void set_domain(norm_vector *dom);
    void set_range(norm_vector *ran);
    virtual void hessian(my_vector *in, my_vector *out);
    bool   dot_test(bool verb);
    virtual void set_scale(const double s){scale=s;}
    virtual norm_vector *range_vec(norm_vector *v1, 
      norm_vector *v2){ seperr("can not request range vec of non-combo op\n");return 0;}
    virtual norm_vector *domain_vec(norm_vector *v1, 
      norm_vector *v2){ seperr("can not request domain vec of non-combo op\n");return 0;}


    char name[1024];
    float scale;
    norm_vector *domain,*range;
    bool solver_own;

};
#endif

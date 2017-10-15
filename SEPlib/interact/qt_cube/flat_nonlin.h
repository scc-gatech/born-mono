#ifndef FLAT_NONLIN_H
#define FLAT_NONLIN_H 1
#include "flat_op.h"
#include "solver_basic.h"
#include "hypercube_mask.h"
#include "cgstep.h"
class flat_nonlin{
  public:
    flat_nonlin(){ solv=0;st=0;};
    flat_nonlin(hypercube_float *dips, hypercube_float *tau,float eps,
      std::vector<long long> fixed, int iref);
    void update_tau(int non_line, int lin, hyercube_float *tau);
    hypercube_float *create_data( hypercube_float *dips, hypercube_float *tau, 
  int iloc2);
    
    ~flat_nonlin(){
        if(solv!=0) delete solv; //Will also delete operators
        if(st!=0) delete st;
     }
    
    hypercube_float *dips;
    cgstep *st;
    lin_solver_basic *solv;

  
    


};



#endif
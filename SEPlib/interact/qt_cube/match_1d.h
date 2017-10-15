#ifndef MATCH_1D_H
#define MATCH_1D_H 1
#include "tree_nd.h"
#include "float_1d.h"
#include "hypercube_float.h"



class match_1d{
 public:
   match_1d(){}
   match_1d(hypercube_float *ar,float deleit, float splitit,int nit=10000,int jini=5);
   tree_nd *make_tree();
   tree_nd *initial_tree();

   
   float del,split;
   int niter;
   int jsamp;
   hypercube_float *tr;
   

   

};
#endif

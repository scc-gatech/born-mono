#ifndef FLOAT_1D_H
#define FLOAT_1D_H
#include<hypercube_float.h>

namespace SEP{
class float_1d:public hypercube_float{
  
   public:
     float_1d(){} //Default constructor
     float_1d(const SEP::axis &a1);
     float_1d(const SEP::axis &a1, const  float *array);
     float_1d(const int n1);
     float_1d(const int n1,const  float *array);
     virtual ~float_1d(){};
   private:
     void base_init_1df(const SEP::axis &a1) ;
  
  
  
  };
  
}
#endif
  



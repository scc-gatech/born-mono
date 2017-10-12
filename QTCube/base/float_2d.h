#ifndef FLOAT_2D_H
#define FLOAT_2D_H 1
#include<hypercube_float.h>

namespace SEP{
class float_2d:public hypercube_float{
  
   public:
     float_2d(){} //Default constructor
     float_2d(const SEP::axis a1, const SEP::axis a2);
     float_2d(const float_2d *x);
     float_2d(const std::shared_ptr<float_2d>x);
     float_2d(const SEP::axis a1, const SEP::axis a2, const float *array);
     float_2d(const int n1,const  int n2);
     float_2d(const int n1,const  int n2,const  float *array);
     virtual ~float_2d(){};
     void base_init_2df(const SEP::axis a1,const SEP::axis a2);

  
  
  
  
  };
}
#endif



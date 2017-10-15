#ifndef FLOAT_2D_H
#define FLOAT_2D_H 1
#include<hypercube_float.h>


class float_2d:public hypercube_float{
  
   public:
     float_2d(){} //Default constructor
     float_2d(axis a1, axis a2);
     float_2d(axis a1, axis a2, float *array);
     float_2d(int n1, int n2);
     float_2d(int n1, int n2, float *array);
     virtual ~float_2d(){};
     void base_init_2df(axis a1,axis a2);

  
  
  
  };
#endif



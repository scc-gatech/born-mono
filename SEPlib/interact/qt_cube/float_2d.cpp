#include<float_2d.h>


float_2d::float_2d(axis a1, axis a2)
{
  base_init_2df(a1,a2);
}
float_2d::float_2d(axis a1, axis a2, float *array)
{
 this->base_init_2df(a1,a2);
 this->set(array);
 }
 void float_2d::base_init_2df(axis a1, axis a2)
 {

   std::vector<axis> axes;
   axes.push_back(a1); axes.push_back(a2);
  
   this->init_ndf(axes);
   }
  float_2d::float_2d(int n1, int n2)
  {
    axis a1=axis(n1);
    axis a2=axis(n2); 
    this->base_init_2df(a1,a2);
  }
  float_2d::float_2d(int n1, int n2, float *array)
  {
    
    axis a1=axis(n1);
    axis a2=axis(n2); 
    this->base_init_2df(a1,a2);
    this->set(array);
  }


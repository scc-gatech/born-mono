#include<float_2d.h>

using namespace SEP;
float_2d::float_2d(const axis a1, const axis a2)
{
  base_init_2df(a1,a2);
}
float_2d::float_2d(const axis a1, const axis a2,const  float *array)
{
 this->base_init_2df(a1,a2);
 this->set(array);
 }
 float_2d::float_2d(const float_2d *x){
   axis a1=x->getAxis(1),a2=x->getAxis(2);
   base_init_2df(a1,a2);
   set(&x->vals[0]);
 }
  float_2d::float_2d(const std::shared_ptr<float_2d> x){
   axis a1=x->getAxis(1),a2=x->getAxis(2);
   base_init_2df(a1,a2);
   set(&x->vals[0]);
 }
 
 void float_2d::base_init_2df(const axis a1,const  axis a2)
 {

   std::vector<axis> axes;
   axes.push_back(a1); axes.push_back(a2);
  
   this->init_ndf(axes);
   }
  float_2d::float_2d(const int n1, const int n2)
  {
    axis a1=axis(n1);
    axis a2=axis(n2); 
    this->base_init_2df(a1,a2);
  }
  float_2d::float_2d(const int n1,const  int n2, const float *array)
  {
    
    axis a1=axis(n1);
    axis a2=axis(n2); 
    this->base_init_2df(a1,a2);
    this->set(array);
  }


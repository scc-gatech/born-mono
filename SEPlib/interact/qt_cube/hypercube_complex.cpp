#include<hypercube_complex.h>


hypercube_complex::hypercube_complex(std::vector<axis> axes){
  this->init_nd(axes);
   

}
hypercube_complex::hypercube_complex(std::vector<axis> axes, std::complex<float> *vals){

  this->init_nd(axes);
  this->set(vals);

}

void hypercube_complex::set(std::complex<float> *array){

       memcpy((void*)this->vals,(const void*) array, 
         sizeof(std::complex<float>)*this->get_n123());

}

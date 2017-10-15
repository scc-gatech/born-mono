#ifndef HYPERCUBE_COMPLEX_H
#define HYPERCUBE_COMPLEX_H 1
//#include<cg.h>
#include<axis.h>
#include<complex>
#include<hypercube.h>



class hypercube_complex: public hypercube
{

  public:
    hypercube_complex()
       {this->vals=0;}//Default
       
    hypercube_complex(std::vector<axis> axes);
    hypercube_complex(std::vector<axis> axes, std::complex<float> *vals);
    void allocate(){
      if(this->vals!=0) deallocate();
      this->vals=new std::complex<float>[this->get_n123()];
      this->vals=0;
     }
     void deallocate(){
       if(this->vals!=0) delete []this->vals;
     }
     ~hypercube_complex(){
       this->deallocate();
      }
  
    virtual void scale(double factor) {
      for(int i=0; i < this->get_n123(); i++) this->vals[i]*=factor;
    }
  
    void set(std::complex <float> *array);

    void set_val(std::complex <float> val){
      for(int i=0; i < this->get_n123(); i++) 
         this->vals[i]=val;
    }
    
    std::complex<float> *vals;
    
    private:

 };
 
 #endif
 




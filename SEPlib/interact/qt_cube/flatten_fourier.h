#ifndef FLATTEN_FOURIER_H
#define FLATTEN_FOURIER_H 1
   extern "C" {

#include<fftw3.h>
void fftw_test2_(int *n1, int *n2, int *n3,int *sp, float *temp, long long *p1,long long *p2);
}
#include<flatten_op.h>
class flatten_fourier{
  public:
    flatten_fourier(){mult=0;}
    flatten_fourier(hypercube_float *delta, flatten_op *oper,int n_1, int n_2, int n_3, 
    float e,int il);
    void create_mult();
    void inverse(hypercube_float *mod, hypercube_float *dat, hypercube_float *delta);



    ~flatten_fourier(){ 
      if(mult!=0){
        fftwf_destroy_plan( planF);
        fftwf_destroy_plan( planI);
        delete [] mult; mult=0;
      }
     }
    float *return_mult(){return mult;}
  private:
    int n1,n2,n3;
    long long n123;
    int iloc;
    float *mult;
    float eps_t;
    fftwf_plan planF,planI;
    flatten_op *op;

};
#endif

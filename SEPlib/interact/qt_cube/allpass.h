
#ifndef ALLPASS_H
#define ALLPASS_H 1
#include<hypercube_float.h>
class allpass{
  public:
  
    allpass(){};
    allpass(hypercube *hyper,int nw, float *pp);
    
    void deriv(hypercube_float *in, hypercube_float *out,int j);
    void pass(hypercube_float *in, hypercube_float *out,int j);
    void do_deriv_1(float *in, float *out, int i, int j){
    	float p=dat[i];
    	out[i]+=(in[i-1+j]-in[i+1])*(3.-2.*p)/12.+
    	        (in[i+j]  -in[i]  )*p/3.+
    	        (in[i+1+j]-in[i-1])*(-3.-2.*p)/12.;
    }
    void do_deriv_2(float *in, float *out, int i, int j){};
    void do_deriv_3(float *in, float *out, int i, int j){}; 
    void do_pass_2(float *in, float *out, int i, int j){};
    void do_pass_3(float *in, float *out, int i, int j){}; 
    void do_pass_1(float *in, float *out, int i, int j){
    	float p=dat[i];
    	out[i]+=(in[i-1+j]-in[i+1])*(1.-p)*(2.-p)/12.+
    	        (in[i+j]  -in[i]  )*(2.-p)/6.+
    	        (in[i+1+j]-in[i-1])*(1.+p)*(2.+p)/12.;
    }
   int n1,n2,n3,nw;
   float *dat;



};

#endif
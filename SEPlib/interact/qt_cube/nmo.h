#ifndef NMO_H
#define NMO_H 1
#include "sinc_bob.h"
class nmo{
  public:
  nmo(){sinc=0;}
  nmo(int n_t, float d_t, float t_0, int n_s, float d_s, float s_0);
  void nmo_it(float *ss, float h, float *in, float *out);
  void dnmo_it(float *dd, float hin, float hout, float *in, float *out);
    void dnmo2_it(float *dd, float hin, float hout, float *in, float *out);
    void dnmo3_it(float *dd, float hin, float hout, float *in, float *out);

  ~nmo() {if(sinc==0) {delete sinc;} sinc=0;}

  int nt,ns;
  float dt,ot,ds,os;
  sinc_bob *sinc;

};

#endif
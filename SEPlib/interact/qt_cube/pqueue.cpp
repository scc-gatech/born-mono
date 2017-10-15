
#include "pqueue.h"
#include <stdio.h>

pqueue::pqueue(int n){
  x=new float*[(n+1)];
  xn=x;
  x1=x+1;

}
void pqueue::delete_pqueue(){
   if(x!=0) delete [] x;
   x=0;
}
void pqueue::pqueue_insert(float *v){
  float **xi, **xp;
  unsigned int p;

  xi = ++xn;
  *xi = v;
  p = (unsigned int) (xn-x);
  for (p >>= 1; p > 0; p >>= 1) {
    xp = x + p;
    if (*v > **xp) break;
    *xi = *xp; xi = xp;
  }
  *xi = v; 
 }

float *pqueue::pqueue_extract(){

  unsigned int c;
  int n;
  float *v, *t;
  float **xi, **xc;

  v = *x1;
  *(xi = x1) = t = *(xn--);
  n = (int) (xn-x);
  for (c = 2; c <= (unsigned int)n; c <<= 1) {
    xc = x + c;
    if (c <(unsigned int) n && **xc > **(xc+1)) {
      c++; xc++;
    }
    if (*t <= **xc) break;
    *xi = *xc; xi = xc;
  }
  *xi = t;
  return v;

}

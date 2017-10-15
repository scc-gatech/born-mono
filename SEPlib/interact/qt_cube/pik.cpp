#include "pik.h"


pik::pik(float *is,int te,int ex){
  for(int i=0; i < 8; i++) floc[i]=is[i];
  type=te;
  extra=ex;
}

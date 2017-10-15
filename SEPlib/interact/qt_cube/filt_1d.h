#ifndef FILT_1D_H
#define FILT_1D_H 1
#include "nfilter.h"
class deriv_filt: public float_filt_1d{
public:
  deriv_filt(){set_def();}
  deriv_filt(int n);
};
class deriv2_filt: public float_filt_1d{
 public:
  deriv2_filt(){set_def();}
  deriv2_filt(int n);
};
class decay_filt: public float_filt_1d{
public:
  decay_filt(){set_def();}
  decay_filt(int n,float f);
};

#endif

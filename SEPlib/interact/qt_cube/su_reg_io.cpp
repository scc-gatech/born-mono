#include "su_reg_io.h"


su_reg_io_float::su_reg_io_float(QString tagit,hypercube* d, bool sw, int in,util *pars){
  set_trace_basics(tagit,d,0,240,sw,pars,in);
   non_byte=true;
}

#include "segy_reg_io.h"


segy_reg_io_float::segy_reg_io_float(QString tagit,hypercube* d, bool sw, int in,util *pars){
  set_trace_basics(tagit,d,3600,240,sw,pars,in);
   non_byte=true;
}

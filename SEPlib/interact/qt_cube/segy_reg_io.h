#ifndef SEGY_REG_IO_H
#define SEGY_REG_IO_H 1
#include "trace_io.h"


class segy_reg_io: public trace_io{


  public:
 
   segy_reg_io(){ }
   ~segy_reg_io(){  }

   
};

class segy_reg_io_float: public segy_reg_io{
  public:
    segy_reg_io_float(QString file, hypercube *d, bool sw, int in, util *pars);


};

#endif

#ifndef SU_REG_IO_H
#define SU_REG_IO_H 1
#include "trace_io.h"


class su_reg_io: public trace_io{


  public:
 
   su_reg_io(){ }
   ~su_reg_io(){  }

   
};

class su_reg_io_float: public su_reg_io{
  public:
    su_reg_io_float(QString file, hypercube *d, bool sw, int in, util *pars);


};

#endif

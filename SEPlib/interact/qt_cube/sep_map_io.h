#ifndef sep_map_io_H
#define sep_map_io_H 1
#include "io_func.h"
#include "sreg.h"



class sep_map_io_float: public sep_reg_io{
  public:
    sep_map_io_float(QString tag, util *pars);
    

};
class sep_map_io_byte: public sep_reg_io{
  public:
    sep_map_io_byte(QString tag, util *pars);
  
};
#endif

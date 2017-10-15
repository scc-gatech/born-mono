#ifndef SEP_REG_MMAP_IO_H
#define SEP_REG_MMAP_IO_H 1
#include "io_func.h"
#include "sreg.h"

class sep_reg_mmap_io: public io_func{


  public:
 
   sep_reg_mmap_io(){ fdes=0;}
   ~sep_reg_mmap_io(){  if(fdes!=0) delete fdes;}
   void set_sep_basics(QString tagit,  util *pars);
    char tag[1024];
    QString in_path;
    sreg *fdes;
    char *map;
   
};

class sep_reg_mmap_io_float: public sep_reg_mmap_io{
  public:
    sep_reg_mmap_io_float(QString tag, util *pars);
    virtual void read_block_float(int *nw, int *fw, float *buf);
    virtual void read_block_byte(int *nw, int *fw, unsigned char *buf);

};
class sep_reg_mmap_io_byte: public sep_reg_mmap_io{
  public:
    sep_reg_mmap_io_byte(QString tag, util *pars);
    virtual void read_block_byte(int *nw, int *fw, unsigned char *buf);
    virtual void read_block_float(int *nw, int *fw, float *buf);
   
};
#endif

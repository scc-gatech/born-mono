#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H 1
#include "buffer.h"
class byte_buffer: public buffer{
  public:
    byte_buffer(){};
    
    byte_buffer(util *p, hypercube *h,io_func *i,int in,int *nw, int *fw);
   
    
    virtual ~byte_buffer(){ delete [] cbuf;};

  unsigned char *get_char_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
  float *get_float_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
  void read_buffer(int *nwin, int *fwin, int *nwout, int *fwout,int ndim, int *nloop);
  virtual void calc_histo();
  virtual float get_value(orient_cube *pos);


 

  private:
    unsigned char *cbuf;


};
#endif

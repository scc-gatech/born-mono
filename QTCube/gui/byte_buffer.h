#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H 1
#include "buffer.h"
namespace SEP{
class byte_buffer: public buffer{
  public:
    byte_buffer(){};
    
    byte_buffer(std::shared_ptr<SEP::paramObj>p, std::shared_ptr<SEP::hypercube>h,std::shared_ptr<SEP::io_func>i,int in,
      std::vector<int>&nw, std::vector<int>&fw);
   
    
    virtual ~byte_buffer(){ delete [] cbuf;};
  unsigned char *get_char_data(int n, long long *ind);
  float *get_float_data(int n, long long *ind);

  unsigned char  *get_char_data(std::shared_ptr<SEP::orient_cube>pos, int n, long long *mp);
  unsigned char *get_char_data(std::shared_ptr<SEP::orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
  float *get_float_data(std::shared_ptr<SEP::orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
  void read_buffer(std::vector<int>&nwin, std::vector<int>&fwin, std::vector<int>&nwout, std::vector<int>&fwout,int ndim, std::vector<int>&nloop);
  virtual void calc_histo();
  virtual float get_value(std::shared_ptr<SEP::orient_cube>pos);


 

  private:
    unsigned char *cbuf;


};
}
#endif

#ifndef UTIL_H
#define UTIL_H 1
#include<qstring.h>
#include "paramObj.h"
#include<vector>
#include<memory>
namespace SEP{
class util{

public:
  util(std::shared_ptr<SEP::paramObj> pars){ par=pars;}
  static QString string_from_int_array(int n,int *ar);
  static QString string_from_float_array(int n,float *ar);
  static int *int_array_from_string(QString str);
  static std::vector<int> int_vector_from_string(QString com);
  static float *float_array_from_string(QString str);


  void swap_float_bytes(int n, float *buf);
 

  void error(const char *e){par->error(e);}
  void set_float_clip( float *buf, int inum,long long nelem, float *bc, float *ec,int swap=0);
  void convert_to_byte(float *fbuf, long long foff, unsigned char *cbuf, long long coff,
    long long nelem, float bclip, float eclip);
  void convert_to_float(float *fbuf, long long foff, unsigned char *cbuf, long long coff,
    long long nelem, float bclip, float eclip);

private:
  std::shared_ptr<SEP::paramObj> par;

};
}
#endif

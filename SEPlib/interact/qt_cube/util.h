#ifndef UTIL_H
#define UTIL_H 1
#include<qstring.h>
#include "param_func.h"
#include<vector>
class util{

public:
  util(param_func *pars){ par=pars;}
  static QString string_from_int_array(int n,int *ar);
  static QString string_from_float_array(int n,float *ar);
  static int *int_array_from_string(QString str);
  static float *float_array_from_string(QString str);
  QString param_string(QString par, int inum);
  QString param_string(QString par, int inum, QString def);
  int param_int(QString par, int inum);
  int param_int(QString par, int inum, int def);
  void swap_float_bytes(int n, float *buf);
  float param_float(QString par, int inum);
  float param_float(QString par, int inum, float def);
  std::vector<int>param_ints(QString par, int inum, std::vector<int> def);
  std::vector<int>param_ints(QString par, int inum);
  void error(char *e){par->error(e);}
  void set_float_clip( float *buf, int inum,long long nelem, float *bc, float *ec,int swap=0);
  void convert_to_byte(float *fbuf, long long foff, unsigned char *cbuf, long long coff,
    long long nelem, float bclip, float eclip);
  void convert_to_float(float *fbuf, long long foff, unsigned char *cbuf, long long coff,
    long long nelem, float bclip, float eclip);
 float get_float(char *p,float def){
    return par->get_float(p,def);
  }
  int get_int(char *p,int def){
    return par->get_int(p,def);
  }
  std::string  get_string(const char *p, const char *def){
    return par->get_string(p,def);
  
  }
private:
  param_func *par;

};
#endif

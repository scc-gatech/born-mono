#ifndef GENERIC_C_IO_H
#define GENERIC_C_IO_H 1
#include"genericIO.h"

extern "C"{
 void initializeIO(int argc,  const int *lens,const  char *aa);
 void openRegFile(const char *ioName, const char *name, const char *usg);
 void openIrregFile(const char *ioName, const char *name, const char *usg);
 void closeIO(const char *ioName);
 void getParamInt(const char *ioName, const char *name, int *val,const  int def);
 void getParamFloat(const char *ioName, const char *name, float *val, const int def);
 void getParamString(const char *ioName, const char *name, char *val,const  int def);
 void getParamBool(const char *ioName, const char *name, int *val,const  int def);
 void getParamInts(const char *ioName, const char *name, const int num, int *val, const int def);
 void getParamFloats(const char *ioName, const char *name,const int num, float *val,const  int defs);

 void getFileInt(const char *ioName, const char *tag, const char *name, int *val,const  int def);
 void getFileFloat(const char *ioName, const char *tag,const  char *name, float *val,const  int def);
 void getFileString(const char *ioName, const char *tag, const char *name, char *val, const int def);
 void getFileBool(const char *ioName, const char *tag,  const char *name, int *val,const  int def);
 void getFileInts(const char *ioName, const char *tag,  const char *name,const int num, int *val,const  int def);
 void getFileFloats(const char *ioName, const char *file, const char *name, const int num,float *val,const  int defs);

 void putFileInt(const char *ioName, const char *tag,const  char *name,const  int val);
 void putFileFloat(const char *ioName, const char *tag,const  char *name,const  float val);
 void putFileString(const char *ioName, const char *tag,  const char *name,const  char *val);
 void putFileBool(const char *ioName, const char *tag,  const char *name,const  int val);
 void putFileInts(const char *ioName, const char *tag,  const char *name,const  int num,const int *val);
 void putFileFloats(const char *ioName, const char *file, const char *name, const int num,const float *val);

 void readFloatStream(const char *ioName, const char *tag, const long long npts, float *array);
 void writeFloatStream(const char *ioName, const char *tag, const long long npts,const  float *array);
 void readFloatWindow(const char *ioName, const char *tag, const int ndim,const  int *nw,const  int *fw,const  int *jw, float *ar);
 void writeFloatWindow(const char *ioName, const char *tag, const int ndim,const  int *nw, const int *fw,const  int *jw,const  float *ar);

 void getDefaultIOName(char *ioName);
 void readComplexStream(const char *ioName, const char *tag, const long long npts, float _Complex *array);
 void writeComplexStream(const char *ioName, const char *tag, const long long npts,const  float  _Complex *array);
 void readComplexWindow(const char *ioName, const char *tag, const int ndim,const  int *nw,const  int *fw,const  int *jw, float  _Complex *ar);
 void writeComplexWindow(const char *ioName, const char *tag, const int ndim,const  int *nw, const int *fw,const  int *jw,const  float  _Complex *ar);


 void readDescription(const char *ioName, const char *tag);
 void writeDescription(const char *ioName, const char *tag);
 void getDataType(const char *ioName, const char *tag, char *typ);
 void setDataType(const char *ioName, const char *tag,const  char *typ);

 void getHyper(const char *ioName, const char *tag,const int ndimin, int *ndimout, int *n, float *o, float *d,  int *nlens, char *label);
 void setHyper(const char *ioName, const char *tag,const int ndim, const int *n, const float *o, const float *d,const int *nlens, const  char *label);
 void cError( const char *ioName, const char *err);
 std::vector<std::string> fromChar1D(const std::vector<int> &nlens, const char *val);
 void toChar1D(const std::vector<std::string>  in, const int nargs, std::vector<int> &nlens, char *val);
}
#endif 
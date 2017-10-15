#ifndef BUFFER_H
#define BUFFER_H 1
#include "hypercube.h"
#include "io_func.h"
#include "orient_cube.h"
#include "param_func.h"
#include "util.h"
class io_store{
  public:
  io_store(int *f){
    for(int i=0; i<8; i++) fs.push_back(f[i]);
  }
  std::vector<int> fs;

};
class buffer{

  public:
    buffer(){ set_null();}
    
    void set_null();
  
    void set_basics(util *p, hypercube *h,io_func *i,int in);
    virtual ~buffer(){
       clean_up();
    }
    void clean_up();
  
    bool hold_slice(orient_cube *pos, int iax1, int iax2,bool *data_contains);
    virtual void read_hyper(int *nw, int *fw){
      if(fw==0 || nw==0){}
      char errmsg[] = "Read hyper unimplemented";
      par->error(errmsg);
    }
  void buffer_basics(param_func *p);
  //long long *form_index_map(orient_cube *pos, int iax1, int f1, int e1, int iax2,
   // int f2, int e2);
      long long *form_index_map(orient_cube *pos, int iax1, int iax2,
       int f1, int e1, int f2, int e2);
void set_hold(bool *h);
  virtual  unsigned char *get_char_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){
       char errmsg[] = "Get char data undefined";
       par->error(errmsg);
       if(pos==0 && iax1==0 && f1==0 && e1==0 && iax2==0 && f2==0 && e2==0){}
       return 0;   
   }
  virtual  float *get_float_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){
       char errmsg[] = "Get float data undefined";
       par->error(errmsg);
       if(pos==0 && iax1==0 && f1==0 && e1==0 && iax2==0 && f2==0 && e2==0){}
       return 0;
  }
 
  void build_axis_map(axis ain, axis aout, int iax);
  void calc_read_loop(int *nwin, int *fwin,int *nwout, int *fwout, int *nloop, int *ndim);
  virtual void read_buffer(int *nwin, int *fwin,int *nwout, int *fwout,int ndim, int *nloop){ 
   if(nwin==0|| fwin==0 || nloop==0 || ndim==0 || nwout==0 || fwout==0){}}
  int resize_buffer(int *nwin,int *fwin,int *nwout,int *fwout,int ndim,long long off,
    unsigned char *tbuf,unsigned char *cbuf,int ssz);
  void calc_get_pars(orient_cube *pos, int iax1, int iax2, long long *j1, long long *j2,
   long long *first);
  void window_to_local(int *nbuf, int *fbuf);
  virtual void calc_histo(){
    char errmsg[] = "Calc histo must be overridden";
    par->error(errmsg);
  }

  float *return_histo();
  virtual float get_value(orient_cube *pos){if(pos==0){} return 0.;}
  long long point_to_local(position *pos);
  virtual bool hold_point(float *pos,bool *use_axis);
  void return_hold(bool *h){ memcpy(h,hold,8*sizeof(bool));}

  void calc_resize(int *fio, int *nio, int *fbuf, int *nbuf);

  hypercube *hyper_io,*hyper_buf;
  io_func *io;
  util *par;
  bool hold[8];
  bool resamp_1;
  long long skip[8];
  int  loc[8];
  int inum;
  int **ii;
  bool same;
  float histo[256];
  long long n123_view,n123_buf;
  float bclip,eclip;
   std::vector<axis> aios ,abufs;



};
#endif

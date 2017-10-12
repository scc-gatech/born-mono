#ifndef BUFFER_H
#define BUFFER_H 1
#include "hypercube.h"
#include "io_func.h"
#include "orient_cube.h"
#include "util.h"
namespace SEP{
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
  
    void set_basics(std::shared_ptr<SEP::paramObj>p, std::shared_ptr<SEP::hypercube>h,std::shared_ptr<io_func>i,int in);
    virtual ~buffer(){
       clean_up();
    }
    void clean_up();
  
    bool hold_slice(std::shared_ptr<orient_cube>pos, int iax1, int iax2,bool *data_contains);
    virtual void read_hyper(int *nw, int *fw){
      if(fw==0 || nw==0){;};
      _par->error("Read hyper unimplemented");
    }
  void buffer_basics(SEP::paramObj *p);
  //long long *form_index_map(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,
   // int f2, int e2);
      long long *form_index_map(std::shared_ptr<orient_cube>pos, int iax1, int iax2,
       int f1, int e1, int f2, int e2);
       long long *grid_to_index(int n, long long *ind);
void set_hold(bool *h);
  virtual  unsigned char *get_char_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){
       _par->error("Get char data1 undefined");
       if(pos==0 && iax1==0 && f1==0 && e1==0 && iax2==0 && f2==0 && e2==0){;};
       return 0;   
   }
   /*
     virtual  unsigned char *get_char_data(std::shared_ptr<orient_cube>pos, int n, long long *buf){
       _par->error("Get char data2 undefined");
       if(n==0 && buf==0&&pos==0){;};
       return 0;   
   }
   */
  virtual  float *get_float_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){
       _par->error("Get float data undefined");
       if(pos==0 && iax1==0 && f1==0 && e1==0 && iax2==0 && f2==0 && e2==0){;};
       return 0;
  }
 
  void build_axis_map(SEP::axis ain, SEP::axis aout, int iax);
  void calc_read_loop(std::vector<int>&nwin, std::vector<int>&fwin,std::vector<int> &nwout,
   std::vector<int> &fwout, std::vector<int> &nloop, int&ndim);
  virtual void read_buffer(int *nwin, int *fwin,int *nwout, int *fwout,int ndim, int *nloop){ 
   if(nwin==0|| fwin==0 || nloop==0 || ndim==0 || nwout==0 || fwout==0){;};}
  int resize_buffer(std::vector<int> &nwin,std::vector<int> &fwin,std::vector<int> &nwout,
  std::vector<int> &fwout,int ndim,long long off,
    unsigned char *tbuf,unsigned char *cbuf,int ssz);
  void calc_get_pars(std::shared_ptr<orient_cube>pos, int iax1, int iax2, long long *j1, long long *j2,
   long long *first);
  void window_to_local(std::vector<int>nbuf, std::vector<int>fbuf);
  virtual void calc_histo(){
    _par->error("Calc histo must be overriten");
  }

  float *return_histo();
  virtual float get_value(std::shared_ptr<orient_cube>pos){if(pos==0){;}return 0.;}
  long long point_to_local(std::shared_ptr<position>pos);
  virtual bool hold_point(float *pos,bool *use_axis);
  void return_hold(bool *h){ memcpy(h,hold,8*sizeof(bool));}

  void calc_resize(std::vector<int> &fio, std::vector<int> &nio, std::vector<int> &fbuf, 
  std::vector<int> &nbuf);

  std::shared_ptr<SEP::hypercube>hyper_io,hyper_buf;
  std::shared_ptr<io_func>io;
  std::shared_ptr<util>_util;
  std::shared_ptr<SEP::paramObj> _par;
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
   std::vector<SEP::axis> aios ,abufs;



};
}
#endif

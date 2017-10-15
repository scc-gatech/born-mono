#ifndef IO_FUNC_H
#define IO_FUNC_H 1
#include "param_func.h"
#include "hypercube.h"
#include "util.h"
 #include <QTreeWidgetItem>

 #include <qobject.h>



class io_func: public QObject{
  Q_OBJECT

  signals:
	    void actionDetected(std::vector< QString> text);
  public:
    io_func(){hyper=0;};
    void set_basics(util *pars, hypercube *h);
    
    virtual ~io_func(){
       if(hyper!=0) delete hyper;
    }
    
    virtual void read_block_float(int *nw, int *fw, float *buf);
    virtual void read_block_byte(int *nw, int *fw,  unsigned char *buf);
    virtual void convert_to_local(int *nwin,int *fwin, int *nwout, int *fwout);
    void return_clips(float *b, float *e){
      *b=bclip; *e=eclip;
    }
    virtual void read_headers(){};
    hypercube *return_hyper();
    bool not_byte(){ return non_byte;}
    bool io_changed(){return changed;}
    void set_clip(float *dat,int inum, long long nelem,int swap=0){
     par->set_float_clip(dat,inum,nelem,&bclip,&eclip,swap);
  }
  
  void convert_to_byte(float *flt, long long foff, unsigned char *tbuf, long long coff,
    long long n123){
      par->convert_to_byte(flt,foff,tbuf,coff,n123,bclip,eclip);
  }
  virtual void get_grid_loc(int *in){if(in==0){}}
  virtual void get_io_loc(int *in){if(in==0){} }
  virtual std::vector<QStringList> get_header(long long inum){
  if(inum==0){}
  std::vector<QStringList> send;
  return send;
}
  virtual std::vector<QString> get_header_val(long long inum){
  if(inum==0){}
  std::vector<QString> send;
  return send;
}

  virtual void add_pt_action(int *iloc){if(iloc==0){}}
  virtual void del_pt_action(int *iloc){if(iloc==0){}}
     bool am_region(){return isregion;}
    
    hypercube *hyper;
    util *par;
    float bclip,eclip;
    bool non_byte;
    bool changed;
    bool isregion;
    



};
#endif

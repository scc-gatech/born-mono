#ifndef dataset_H
#define dataset_H 1
#include "io_func.h"
#include "orient_cube.h"
#include "buffer.h"
#include "util.h"
#include <assert.h>
#include "bar_pt.h"
 #include <qobject.h>
 #include<map>
 #include <QTreeWidgetItem>
#include <iostream>

namespace SEP{
class dataset: public QObject{
  Q_OBJECT
  signals:
	    void actionDetected(std::vector< QString> text);
  public:
    dataset(){}
    void set_basics(std::string titl,QString name,std::shared_ptr<SEP::hypercube>g,std::shared_ptr<io_func>i, 
    std::shared_ptr<SEP::paramObj> p,int in,int im=1);
     virtual ~dataset(){ 
        clean_bufs();
     }
     void clean_bufs(){
      //for(int i=0; i < (int)buf.size(); i++) delete buf[i];
      buf.clear();
    }

    virtual std::shared_ptr<SEP::hypercube> return_io_hyper(){return io->return_hyper();}
    virtual unsigned char *get_char_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, 
    int e1, int iax2,
      int f2, int e2);
      virtual unsigned char *get_char_data(std::shared_ptr<orient_cube>pos,int iax1,int iax2,int n, long long *index);
    virtual float *get_float_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
    virtual float get_value(std::shared_ptr<orient_cube>pos);
     virtual std::vector<QStringList> get_header(std::shared_ptr<orient_cube>pos){
       long long inum=get_trace_num(pos);
       return io->get_header(inum);
     }
      virtual std::vector<QString> return_header_keys(){
       return io->return_header_keys();
     }
         std::vector<QString> virtual get_header_vals(std::shared_ptr<orient_cube> pos){
       long long inum=get_trace_num(pos);
       return io->get_header_val(inum);
     }
     QString return_axis_name(int iax);
    long long get_trace_num(std::shared_ptr<orient_cube>pos);
    void  snap_location(float *floc, int single, QString stype);
    int check_load_buffer(std::shared_ptr<orient_cube>pos, int iax1,  int iax2);
    int find_buffer(std::shared_ptr<orient_cube>pos);
    virtual void afterInitialization(){;}
    virtual std::shared_ptr<buffer> create_buffer(std::shared_ptr<orient_cube>pos, int iax1, int iax2){
      if(pos==0 || iax1==0 || iax2==0) {;};
      return 0;
    }
    QString validate_view(QString name, orient_cube pos);
    SEP::axis get_grid_axis(int iax){
      return grid->getAxis(iax);
    }
    SEP::axis get_io_axis(int iax){
      return io->return_hyper()->getAxis(iax);
    }
   QString get_bcolor(){
      return bcolor;
    }
   QString get_type(){ return data_type;}
   QString get_ecolor(){
    return ecolor;
    }
    void set_bcolor(QString col){
      bcolor=col;
  }
  void set_ecolor(QString col){
    ecolor=col;
  }
  void set_display_axis(int iax,QString x);
    
    int return_axes_display(int iax) {return axes_display[iax];}
    std::vector<float> return_axis_value(int iax,std::vector<long long> positions);
    
    
    QString data_type;
    QString return_histogram();
    void build_conv();
    int add_pt(float pct,float clp);
    void del_pt(int ipt);
    QString return_clip();
    QString data_updated(){ return "none";}
    QString get_name(){return name;}
    std::shared_ptr<SEP::hypercube>return_grid(){return grid;}
    virtual float get_minval(){return buf[0]->bclip;}
    virtual float get_maxval(){return buf[0]->eclip;}
    QString get_values(){return QString(values.c_str());}
    QString get_title_short(){return title_short;}
    virtual void delete_dataset(std::shared_ptr<orient_cube>pos, int iax1, int iax2);
    bool valid_display(int iax, int *order, std::vector<int> ns);
    virtual bool match_grid();
    virtual void set_contains();
    QString validate_view(QString nm, std::shared_ptr<orient_cube>pos);
    void  get_pt(int ipt, float *pct, float *clp);
    bool get_data_contain(int iax){ return data_contains[iax];}
    virtual void add_pt_action(int *iloc){io->add_pt_action(iloc);}
    virtual void del_pt_action(int *iloc){io->del_pt_action(iloc);}
    virtual void get_io_loc(int *iloc){ io->get_io_loc(iloc);}
    virtual bool am_region(){ return io->am_region();}
    QString set_pts(float *pcts);
    void clear_pts();
    std::shared_ptr<SEP::hypercube> grid;
    std::vector<std::shared_ptr<buffer>> buf;
    bool data_contains[8],display_axis[8];
    std::shared_ptr<io_func> io;
    std::shared_ptr<SEP::paramObj> par;
    int nmax_buf,inum;
    unsigned char conv[256];
    std::vector<bar_pt>  clip;
    std::vector<int> axes_display;
    QString name;
    bool pickable;
    QString restrict;
    long long n123;
    bool moveable;
    std::string values;
    QString bcolor,ecolor;
    bool show_picks;
    bool update_pick;
    QString title,title_short;
        std::vector<int> axes_dispay;



};

}

#endif

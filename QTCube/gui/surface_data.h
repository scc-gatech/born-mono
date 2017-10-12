#ifndef surface_DATA_H
#define surface_DATA_H 1
#include "pick_draw.h"
#include "pick_new.h"
#include "dataset.h"
#define FUNKY_VAL -123456
namespace SEP{
class surface_slice{
  public:
    surface_slice(std::shared_ptr<orient_cube> pos, int iax1, int iax2,int isingle,QString display,
    QString color, std::shared_ptr<dataset> dat, std::shared_ptr<pick_draw>pk);
    void form_amp_slice(std::shared_ptr<orient_cube>ori,std::shared_ptr<picks_vec> list, std::shared_ptr<dataset>dat);
    void form_depth_slice(std::shared_ptr<orient_cube> ori,std::shared_ptr<picks_vec> list,float o,float d);
    bool have_slice(QString display, QString color, int isingle,std::shared_ptr<orient_cube>pos, int iax1, int iax2);
    float *return_slice( std::shared_ptr<orient_cube>pos,int f1, int e1, int f2, int e2);
    ~surface_slice(){delete_surf_slice();}
    void delete_surf_slice(){
       delete [] slice;
    }
    QString  display;
    int iax1,iax2,iloc[8];
    int n1,n2;
    int isingle;
    float *slice;
    QString color;

};

class surface_data: public dataset{
  
   public:
surface_data(std::shared_ptr<SEP::hypercube> g, QString nm, QString typ,
std::shared_ptr<dataset>dat,std::shared_ptr<SEP::paramObj>p,std::shared_ptr<SEP::pick_draw>picks, QString col,int in);
void set_single(std::shared_ptr<SEP::orient_cube>pos, int is);
  void create_buffer(std::shared_ptr<SEP::orient_cube>pos);
  void update_surface();

virtual unsigned char *get_char_data(std::shared_ptr<SEP::orient_cube >pos,int iax1,int iax2, int n, long long *index);
   float *get_float_data(std::shared_ptr<SEP::orient_cube>pos,int iax1,int iax2, int n, long long *index);

     virtual unsigned char *get_char_data(std::shared_ptr<SEP::orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
    virtual float *get_float_data(std::shared_ptr<SEP::orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
    virtual float get_value(std::shared_ptr<SEP::orient_cube>pos);
    virtual float get_minval();
    virtual float get_maxval();
    void set_dataset(std::shared_ptr<SEP::orient_cube >pos,QString nm,std::shared_ptr<SEP::dataset>dat);
    void set_display(std::shared_ptr<SEP::orient_cube >pos,QString d);

     virtual std::vector<QStringList> get_header(std::shared_ptr<SEP::orient_cube>pos){
       long long inum=get_trace_num(pos);
       return dat->io->get_header(inum);
     }
         std::vector<QString> virtual get_header_vals(std::shared_ptr<SEP::orient_cube>pos){
       long long inum=get_trace_num(pos);
       return dat->io->get_header_val(inum);
     }
    void form_histo(std::shared_ptr<SEP::orient_cube>pos);
    unsigned char *float_to_byte(int n,float *tb);
    float  *check_load_slice(std::shared_ptr<SEP::orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
    void set_color(std::shared_ptr<SEP::orient_cube>pos,QString col);
    QString get_dataset(){ return data_name;}
    QString return_histogram();
    QString get_display(){ return display;}
    QString get_color(){ return pick_col;}
    virtual std::shared_ptr<SEP::hypercube> return_io_hyper(){return dat->return_io_hyper();}
    virtual void get_io_loc(int *iloc){ dat->io->get_io_loc(iloc);}
     virtual bool am_region(){ return false;}
     std::shared_ptr<SEP::pick_draw> pk;
     QString pick_col;
     float *histo;
     int isingle;
     long long nbuf,nrem;
     QString display;
     std::shared_ptr<SEP::dataset> dat;
     std::shared_ptr<SEP::orient_cube>my_pos;
     QString data_name;
     int nslices;
     std::vector<std::shared_ptr<SEP::surface_slice>> slices;
     float minv, maxv;
     float *buf;
     int my_orient;

};
}

#endif

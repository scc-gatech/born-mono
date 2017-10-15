#ifndef surface_DATA_H
#define surface_DATA_H 1
#include "pick_draw.h"
#include "dataset.h"
#define FUNKY_VAL -123456

class surface_slice{
  public:
    surface_slice(orient_cube *pos, int iax1, int iax2,int isingle,QString display,
    QString color, dataset *dat, pick_draw *pk);
    void form_amp_slice(orient_cube *ori,picks_vec *list, dataset *dat);
    void form_depth_slice(orient_cube *ori,picks_vec *list,float o,float d);
    bool have_slice(QString display, QString color, int isingle,orient_cube *pos, int iax1, int iax2);
    float *return_slice( orient_cube *pos,int f1, int e1, int f2, int e2);
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
surface_data(hypercube *g, QString nm, dataset *dat,param_func *p,pick_draw *picks, QString col,int in);
void set_single(orient_cube *pos, int is);
 
   
     virtual unsigned char *get_char_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
    virtual float *get_float_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
    virtual float get_value(orient_cube *pos);
    virtual float get_minval();
    virtual float get_maxval();
    void set_dataset(orient_cube *pos,QString nm,dataset *dat);
    void set_display(orient_cube *pos,QString d);

     virtual std::vector<QStringList> get_header(orient_cube *pos){
       long long inum=get_trace_num(pos);
       return dat->io->get_header(inum);
     }
         std::vector<QString> virtual get_header_vals(orient_cube *pos){
       long long inum=get_trace_num(pos);
       return dat->io->get_header_val(inum);
     }
    void form_histo(orient_cube *pos);
    unsigned char *float_to_byte(int n,float *tb);
    float  *check_load_slice(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
    void set_color(orient_cube *pos,QString col);
    QString get_dataset(){ return data_name;}
    QString return_histogram();
    QString get_display(){ return display;}
    QString get_color(){ return pick_col;}
    virtual hypercube *return_io_hyper(){return dat->return_io_hyper();}
    virtual void get_io_loc(int *iloc){ dat->io->get_io_loc(iloc);}
     virtual bool am_region(){ return false;}
     pick_draw *pk;
     QString pick_col;
     float *histo;
     int isingle;
     QString display;
     dataset *dat;
     QString data_name;
     int nslices;
     std::vector<surface_slice*> slices;
     float minv, maxv;

};


#endif

#ifndef SLICE_H 
#define SLICE_H 1
#include <qpainter.h>
#include<dataset.h>
#include<float.h>
#include<orient_cube.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <vector>
#include "pick_new.h"
#include "pairs_new.h"

class slice{
  
 public:
  
  slice(){zero_slice();}
 
  void set_draw_params(QFontMetrics *fm,orient_cube *pos, int iax1, int iax2, int bx, int ex, int by, int ey,
    int dopos,bool at, bool ab, bool al, bool ar,float  sh1, float sh2, bool rev1, bool rev2,
    bool verb=false);
  
  void update_pos(std::vector<QString> *com, int ix,int iy, orient_cube *pos);
  void set_overlay(){
    is_overlay=true;
  }
  bool in_slice(int ix,int iy);
  void update_range(std::vector<QString> *com,
     int ix_old,int iy_old, int ix, int iy, orient_cube *pos);
void get_data_loc(int ix, int iy,int *id1);
    void get_data_loc(orient_cube *pos, int ix, int iy,int *loc);
int  *form_pixel_slice_map();

 bool get_image_pos_sample(float ix, float iy, int *id1, int *id2);
  void get_image_pos(float x, float y, int *id1, int *id2,bool ignore_rev=false);
  void get_shifted_image_pos_from_sam_fract(float ix, float iy, int *id1, int *id2);
void get_shifted_image_pos_from_sample(int ipos, int *id1, int *id2);

bool get_shifted_image_pos_from_sample(int ix, int iy, int *id1, int *id2);
  void get_shifted_image_pos_from_loc(float fx, float fy, int *ix, int *iy,bool ignore_rev=false);
  void get_image_pos_pct(float fx, float fy, int *ix, int *iy);
  void get_slice_axes(orient_cube *pos, int *i1, int *i2, int *i3);
  float nice_number (float d);
  int get_iax1(){return iax1;}
  int get_iax2(){return iax2;}
    
    QString string_from_array(int n,int *ar);
    int *array_from_string(QString str);
  virtual void draw_slice(QPainter *painter,dataset *dat,QPen *pen,
  orient_cube *pos,  bool ov,bool draw_grid){ if(painter==0 && dat==0 && 
    pen==0 && pos==0&& ov &&draw_grid){}}
  int  optimal_with_font(int n, float min, float max,
                         /*@out@*/ float *onum, /*@out@*/ float *dnum);

       int get_orient_num(){ return my_pos->get_orient_num();}

   void set_picks(int idelta,picks_vec *pks){
     my_pos->set_picks(iax1,iax2,idelta,pks);
   }
   int get_n1_slice(){return n1_slice;}
   int get_n2_slice(){return n2_slice;}
   pairs_new *return_pick_locs(QString col,picks_vec *pk );
   int get_i3(){return i3_axis;}
   virtual slice *clone(){ return 0;};
   virtual ~slice(){delete_slice();}
   QString name;
	  int ntic1,ntic2;

   orient_cube *my_pos;
   dataset *my_dat;
   int bx,ex,iax1,iax2,by,ey,xb,xe,yb,ye;
   int i3_axis;
   float ox,oy,dx,dy;
   float sx,sy;
   axis axes[2];
   QFontMetrics *fm;
   int b_1,b_2,e_1,e_2,boundary;
   bool reverse1,reverse2;
   bool do_axes,rev_1,rev_2;
   int do_pos;
   bool axis_left,axis_right,axis_top,axis_bot;
   float power(float f, int ie);
   float noice_number(float d);
   int  optimal_scale(int nsz, float min, float max, float *onum,  float *dnum);
   void draw_axes(QPainter *painter,orient_cube *pos,bool draw_grid);
   void draw_pos(QPainter *painter, orient_cube *pos);
   void delete_slice();
   void zero_slice();
 //  std::vector<QString> *return_hash(){return &hash_map;}
   int *get_pixel_to_slice_map(int *n1, int *n2);
   long long *get_index_map_ptr(int idelta);
   long long *get_slice_to_grid_map();
   orient_cube *get_orient(){return ps;}
   virtual void set_bcolor(QString c){bcolor=c;}
  virtual void set_ecolor(QString c){ ecolor=c;}
  QString get_bcolor(){ return bcolor;}
  QString get_ecolor(){ return ecolor;}
  bool is_raster;
  bool is_overlay;
  QString bcolor,ecolor;
  orient_cube *ps;
  long long *slice_to_grid_map;
  int *pixel_to_slice_map;
  int *temp_map;
  int n1_pixel,n1_slice,n2_slice;

};



#endif


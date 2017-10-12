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
namespace SEP{
class slice{
  
 public:
  
  slice(){zero_slice();}
 
  void set_draw_params(std::shared_ptr<QFontMetrics >fm,std::shared_ptr<orient_cube>pos, int iax1, int iax2, int off,int bx, int ex, int by, int ey,
    int dopos,bool at, bool ab, bool al, bool ar,float  sh1, float sh2, bool rev1, bool rev2,
    bool verb=false);
  
  void update_pos(std::vector<QString> *com, int ix,int iy, std::shared_ptr<orient_cube>pos);
  void set_overlay(){
    is_overlay=true;
  }
  bool in_slice(int ix,int iy);
  void update_range(std::vector<QString> *com,
     int ix_old,int iy_old, int ix, int iy, std::shared_ptr<orient_cube>pos);
void get_data_loc(int ix, int iy,int *id1);
    void get_data_loc(std::shared_ptr<orient_cube>pos, int ix, int iy,int *loc);
 void form_pixel_slice_map();

 bool get_image_pos_sample(float ix, float iy, int *id1, int *id2);
  void get_image_pos(float x, float y, int *id1, int *id2,bool ignore_rev=false);
  void get_shifted_image_pos_from_sam_fract(float ix, float iy, int *id1, int *id2);
void get_shifted_image_pos_from_sample(int ipos, int *id1, int *id2);
void get_shifted_axis_pos1( bool left,float pct,int *ix, int *iy);
void get_shifted_axis_pos2( bool top, float pct,int *ix, int *iy);
void get_data_pos(int ix,int iy,float *pos);

bool get_shifted_image_pos_from_sample(int ix, int iy, int *id1, int *id2);
  void get_shifted_image_pos_from_loc(float fx, float fy, int *ix, int *iy,bool ignore_rev=false);
  void get_image_pos_pct(float fx, float fy, int *ix, int *iy);
  void get_slice_axes(std::shared_ptr<orient_cube>pos, int *i1, int *i2, int *i3);
  float nice_number (float d);
  int get_iax1(){return iax1;}
  int get_iax2(){return iax2;}
    std::vector<long long> get_multi_locs(int ix_old,int iy_old, int ix, int iy);
    QString string_from_array(int n,int *ar);
    int *array_from_string(QString str);
  virtual void draw_slice(QPainter *painter,std::shared_ptr<dataset>dat,QPen *pen,
  std::shared_ptr<orient_cube>pos,  bool ov,bool draw_grid){ if(painter==0 && !dat&& 
    pen==0 && !pos&& ov &&draw_grid){;};}
  std::vector<float>  optimal_with_font(int n, float min, float max);
  std::vector<float>  optimal_with_font(int n,int mn,int mx,std::vector<float> values,std::vector<float> *tic_vals);

       int get_orient_num(){ return my_pos->get_orient_num();}

   void set_picks(int idelta,std::shared_ptr<picks_vec>pks){
     my_pos->set_picks(iax1,iax2,idelta,pks);
   }
   int get_n1_slice(){return n1_slice;}
   int get_n2_slice(){return n2_slice;}
   std::shared_ptr<pairs_new>return_pick_locs(QString col,std::shared_ptr<picks_vec>pk );
   int get_i3(){return i3_axis;}
   int get_offset(){return offset;}
   virtual std::shared_ptr<slice> clone(){ return nullptr;};
   virtual ~slice(){delete_slice();}
   QString name;
	  int ntic1,ntic2;

   std::shared_ptr<orient_cube>my_pos;
   std::shared_ptr<dataset>my_dat;
   int bx,ex,iax1,iax2,by,ey,xb,xe,yb,ye;

   int b1_grid,e1_grid,b2_grid,e2_grid;
   int i3_axis;
   float ox,oy,dx,dy;
   float sx,sy;
   SEP::axis axes[2];
   std::shared_ptr<QFontMetrics >fm;
   int b_1,b_2,e_1,e_2,boundary;
   bool reverse1,reverse2;
   bool do_axes,rev_1,rev_2;
   int do_pos;
   bool axis_left,axis_right,axis_top,axis_bot;
   float power(float f, int ie);
   float noice_number(float d);
   int  optimal_scale_linear(int nsz, float min, float max, float *onum,  float *dnum);
   void draw_axes(QPainter *painter,std::shared_ptr<orient_cube>pos,bool draw_grid);
   void draw_pos(QPainter *painter, std::shared_ptr<orient_cube>pos);
   void delete_slice();
   void zero_slice();
 //  std::vector<QString> *return_hash(){return &hash_map;}
   int *get_pixel_to_slice_map(int *n1, int *n2);
   long long *get_index_map_ptr(int idelta);
   long long *get_slice_to_grid_map();
   std::shared_ptr<orient_cube>get_orient(){return ps;}
   virtual void set_bcolor(QString c){bcolor=c;}
  virtual void set_ecolor(QString c){ ecolor=c;}
  QString get_bcolor(){ return bcolor;}
  QString get_ecolor(){ return ecolor;}
  bool is_raster;
  bool is_overlay;
  QString bcolor,ecolor;
  std::shared_ptr<orient_cube>ps;
  long long *slice_to_grid_map;
  int *pixel_to_slice_map;
  int n1_pixel,n1_slice,n2_slice;
  int offset;

};
}


#endif


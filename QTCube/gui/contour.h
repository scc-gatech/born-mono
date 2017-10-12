
#include <qpainter.h>
#include<dataset.h>
#include<float.h>
#include<orient_cube.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <vector>
#include "slice.h"
#ifndef contour_H
#define contour_H 1
namespace SEP{
class box{
  public:
   box(){};
   box(int b_x,int b_y,int e_x,int e_y){bx=b_x; by=b_y; ey=e_y; ex=e_x;}
   bool boxes_overlap(box *other);
   box(int ix,int iy,QString txt, float rad, std::shared_ptr<QFontMetrics> fm);
   int bx,ex,by,ey;
   int shiftx,shifty;

};
class contour: public slice
{
  
 public:
  
  contour(){set_contour_color("black");  
  is_raster=false;num_lines=5; dc_set=false;c0_set=false; c0=0; dc=1.; };
 
 	void Paintcontours(int n2, int n1, QPainter *painter, float *b,float minv, float maxv);

  virtual void draw_slice(QPainter *painter,std::shared_ptr<dataset>dat,QPen *pen,
  std::shared_ptr<orient_cube>pos,  bool ov,bool draw_grid);
  std::shared_ptr<slice>clone();
  int cconnect (float *z, int n1, int n2,float  c, int *ix, int *iy,bool *south_mark,
  bool *west_mark,QPolygon *pts);
  int contour_val ( QPainter *painter,float *z, bool *west_mark, bool *south_mark, int n1, int n2,float c);
  float delta (float a, float b, float c);

  void set_number_lines(int iline){num_lines=iline;}
  int get_number_lines(){return num_lines;}
  void set_c0(float c_0){ c0=c_0; c0_set=true;}
  void set_dc(float d_c){ dc=d_c; dc_set=true;}
  double get_dc(){return dc;}
  double get_c0(){return c0;}
  void draw_contour(float c, QPainter *painter, QPolygon *poly);
  void get_shifted_image_pos_fract(float ix, float iy, int *id1, int *id2);
  void set_contour_color(QString color);
  QString return_contour_color();
  QColor contourColor;

  int num_lines;
  QString my_color;
  float c0,dc;
  bool c0_set,dc_set;
  int f1,f2;
  std::vector<box> boxes;

};

}
#endif


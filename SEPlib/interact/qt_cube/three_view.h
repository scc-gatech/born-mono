#include<view.h>
#include<slice.h>
#include<dataset.h>
#include<vector>
#include "multiple.h"
#include<qfontmetrics.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>
#ifndef THREE_VIEW_H
#define THREE_VIEW_H 1
class three_view: public multiple
{

public:
  three_view(){}
  //required view constructors
  std::vector<QString> keyReleaseEvent( QKeyEvent *e,orient_cube *pos ){
    return key_release_multiple(e,pos);
  }
  std::vector<QString> keyPressEvent(QKeyEvent *e, orient_cube *pos){
    return key_press_multiple(e,pos);
  }
  //Left 

  std::vector<QString> l_mouse_d(float x, float y, orient_cube *pos, mouse_func *func);
    //std::vector<QString> l_mouse_m(float x, float y, orient_cube *pos, mouse_func *func);

 // std::vector<QString> l_mouse_r(float x, float y, orient_cube *pos, mouse_func *func);
   virtual void resize_view(int ix,int iy);
        virtual void resize_view_beg(int ix,int iy);
    virtual void resize_view_end(int ix,int iy);
  ~three_view(){};
   virtual void get_corner_loc(int *ix,int *iy);
   virtual void set_corner_loc(int ix, int iy);
   virtual void get_pcts(float *pt,float *pb){ *pt=pct_top;*pb=pct_front;}
   virtual void set_pcts(float pt, float pb){ pct_top=pt; pct_front=pb;}
   void pct_from_xy(int ix, int iy,float *pf, float *pt);
  float pct_top,pct_front;
  int bsmall;

   private:



};
#endif

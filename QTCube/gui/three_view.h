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
namespace SEP{
class three_view: public multiple
{

public:
  three_view(){}
  //required view constructors
  std::vector<QString> keyReleaseEvent( QKeyEvent *e,std::shared_ptr<orient_cube>pos ){
    return key_release_multiple(e,pos);
  }
  std::vector<QString> keyPressEvent(QKeyEvent *e, std::shared_ptr<orient_cube>pos){
    return key_press_multiple(e,pos);
  }
  //Left 

  virtual std::vector<QString> left_mouse_down(float x, float y, std::shared_ptr<SEP::orient_cube>pos, std::shared_ptr<SEP::mouse_func>func);
  virtual std::vector<QString> left_mouse_move(float x, float y, std::shared_ptr<SEP::orient_cube>pos, std::shared_ptr<SEP::mouse_func>func);
  virtual std::vector<QString> left_mouse_release(float x, float y, std::shared_ptr<SEP::orient_cube>pos, std::shared_ptr<SEP::mouse_func>func);

    //std::vector<QString> l_mouse_m(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>func);

 // std::vector<QString> l_mouse_r(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>func);
   virtual void resize_view(int ix,int iy);
        virtual void resize_view_beg(int ix,int iy);
    virtual void resize_view_end(int ix,int iy);
  ~three_view(){};
   virtual void get_corner_loc(int *ix,int *iy){ get_corner_loc(ix,iy,pct_front,pct_top);}
  virtual void get_corner_loc(int *ix,int *iy, float p1, float p2, float p3);
  void get_corner_loc(int *ix, int *iy, float pfront, float ptop);

   virtual void set_corner_loc(int ix, int iy);
   virtual void get_pcts(float *pt,float *pb){ *pt=pct_top;*pb=pct_front;}
   virtual void set_pcts(float pt, float pb){ pct_top=pt; pct_front=pb;}
   void pct_from_xy(int ix, int iy,float *pf, float *pt);
  float pct_top,pct_front;
  int bsmall;

   private:



};
}
#endif

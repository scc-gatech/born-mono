#include<three_view.h>
#include<ctime>
//Added by qt3to4:
#include <QPolygon>
#include <QMouseEvent>
#include "util.h"

using namespace SEP;
void three_view::resize_view_beg(int ix,int iy){
  int cx,cy;
  this->get_corner_loc(&cx,&cy);

   corner_sel=0;
  int dist=(ix-cx)*(ix-cx) +(iy-cy)*(iy-cy);
   if(dist < 900 && ix >=cx && iy <= cy) {
     corner_sel=1;
   }
 
}
void three_view::resize_view(int ix, int iy){
  if(corner_sel==1){
    set_corner_loc(ix,iy);
  }
  

}
void three_view::resize_view_end(int ix,int iy){
  if(corner_sel==1){
    set_corner_loc(ix,iy);
    corner_sel=0;
  }
}

std::vector<QString> three_view::left_mouse_move(float x,float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>func){
  if(corner_sel==1){

    if(  TIME.elapsed() >50){
      TIME.restart();
    int ix,iy;
    this->convert_to_local_loc(x,y,&ix,&iy);
    com[1]="orient";
    com[2]="vsize";
    com[3]=QString::number(ix);
    com[4]=QString::number(iy);
    return com;
  }
  else{
     com[1]="none";
     return com;
  
  }
  }
  else {
    return l_mouse_m_view(x,y,pos,func);
  }

}
std::vector<QString> three_view::left_mouse_release(float x,float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>func){
 
 
 if(corner_sel==1){
    int ix,iy;
    this->convert_to_local_loc(x,y,&ix,&iy);
    com[1]="orient";
    com[2]="vsize";
    com[3]=QString::number(ix);
    com[4]=QString::number(iy);
    corner_sel=0;
    return com;
  }
  else  return l_mouse_r_view(x,y,pos,func);


}


std::vector<QString> three_view::left_mouse_down(float x, float y, std::shared_ptr<orient_cube>pos,std::shared_ptr<mouse_func>func){
   int ix,iy;
   int cx,cy;
     com[1]="none";
   this->convert_to_local_loc(x,y,&ix,&iy);
   this->get_corner_loc(&cx,&cy);

   corner_sel=0;

  int dist=(ix-cx)*(ix-cx) +(iy-cy)*(iy-cy);
   if(dist < 900 && ix >cx && iy < cy) {
     corner_sel=1;
      TIME.start();

     return com;
   }
   else return  l_mouse_d_view(x,y,pos,func);
     
   return com;
}
void three_view::get_corner_loc(int *ix,int *iy, float p1, float p2, float p3){
   get_corner_loc(ix,iy,p2/(p2+p3),p3/(p1+p3));
  pct_front=p2/(p2+p3);
  pct_top=p3/(p1+p3);

}


void three_view::get_corner_loc(int *ix, int *iy, float pfront, float ptop) {

     float xrange=(ex-bx-bsmall-bound_l-bound_r);
     float yrange=(ey-by-bsmall-bound_t-bound_b);
     *ix=(int)(xrange*pfront+bsmall/2+bound_l+bx);
    *iy=(int)(yrange*ptop+bsmall/2+bound_t+by);

     
  }
  

void three_view::set_corner_loc(int ix,int iy) {
      pct_from_xy(ix,iy,&pct_top,&pct_front);
  }
void three_view::pct_from_xy(int ix, int iy,float *pt, float *pf){
     float xrange=(ex-bx-bsmall-bound_l-bound_r);
     float yrange=(ey-by-bsmall-bound_t-bound_b);
    *pf=(ix-bsmall/2-bx-bound_l)/xrange;
    *pt=(iy-bsmall/2-by-bound_t)/yrange;

}

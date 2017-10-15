#include<three_view.h>
#include<ctime>
//Added by qt3to4:
#include <QPolygon>
#include <QMouseEvent>
#include "util.h"


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
std::vector<QString> three_view::l_mouse_d(float x, float y, orient_cube *pos,mouse_func *func){
   int ix,iy;
   int cx,cy;
    TIME.start();
     com[1]="none";
   this->convert_to_local_loc(x,y,&ix,&iy);
   this->get_corner_loc(&cx,&cy);

   corner_sel=0;

  int dist=(ix-cx)*(ix-cx) +(iy-cy)*(iy-cy);
   if(dist < 900 && ix >cx && iy < cy) {
     corner_sel=1;
     return com;
   }
   else{
     for(int islice=0; islice <  (int)slices.size(); islice++){
      if(slices[islice]->in_slice(ix,iy)==true)
        func->l_mouse_d(slices,islice,ix,iy,pos,&com,draw_o);
    }
     
   }
   return com;
}


void three_view::get_corner_loc(int *ix, int *iy) {
     float xrange=(ex-bx-bsmall-bound_l-bound_r);
     float yrange=(ey-by-bsmall-bound_t-bound_b);
 *ix=(int)(xrange*pct_front+bsmall/2+bound_l+bx);
    *iy=(int)(yrange*pct_top+bsmall/2+bound_t+by);

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

#include "mouse_zoom.h"
using namespace SEP;
mouse_zoom::mouse_zoom(){

status="Zoom: L-zoom(reset) M-navigate R-add pick";
ivalid_down=-1;
}
void mouse_zoom::l_mouse_d(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   range_down(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_zoom::l_mouse_m(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){  
   range_move(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_zoom::l_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
  update_range(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_zoom::m_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
 
   update_pos(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_zoom::m_mouse_d(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, 
  std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   TIME.start();
 com->at(1)="none";
    if(ivalid_down==-5) non_slice_move(ix,iy,com,draw_o);

 if((int)slices.size()==0 && islice==0 && ix==0 && iy==0 && pos==0);
/* 
 if(pos==0 && draw_o==0 && (int)slices.size()==0);
  if(islice==-1) non_slice_down(ix,iy,com);
  */
}
void mouse_zoom::m_mouse_m(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
     com->at(1)="none";
   if(islice==-1) non_slice_down(ix,iy,com);
   else{
  if(TIME.elapsed() > 100) {
    update_pos(slices,islice,ix,iy,pos,com,draw_o);
    TIME.restart();
  }
  }
  /*
  if(pos==0  && islice==0 && (int)slices.size()==0);
   
   if(ivalid_down==-5) non_slice_move(ix,iy,com,draw_o);
*/

}
void mouse_zoom::r_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){

   add_pt(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_zoom::l_mouse_double(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
reset(slices,islice,ix,iy,pos,com,draw_o);
}

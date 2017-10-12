#include "mouse_region.h"
using namespace SEP;
mouse_region::mouse_region(){

status="Pick: L-add region M-navigate R-delete region";
ivalid_down=-1;

}
void mouse_region::l_mouse_d(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   range_down(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_region::l_mouse_m(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   range_move(slices,islice,ix,iy,pos,com,draw_o);
}
void  mouse_region::l_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
  add_multi_picks(slices,islice,ix,iy,pos,com,draw_o);
}

void mouse_region::r_mouse_d(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   range_down(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_region::r_mouse_m(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   range_move(slices,islice,ix,iy,pos,com,draw_o);
}
void  mouse_region::r_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
  delete_multi_picks(slices,islice,ix,iy,pos,com,draw_o);
}

void mouse_region::m_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
  update_pos(slices,islice,ix,iy,pos,com,draw_o);
}




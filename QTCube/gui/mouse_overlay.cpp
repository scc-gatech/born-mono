#include "mouse_overlay.h"
using namespace SEP;
mouse_overlay::mouse_overlay(){

status="Pick: L-draw line M-navigate R-draw hyperbola";
ivalid_down=-1;

}
void mouse_overlay::l_mouse_d(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   range_down(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_overlay::l_mouse_m(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   line_move(slices,islice,ix,iy,pos,com,draw_o);
   if(ivalid_down==islice){
      float pnew[2],pold[2];
      slices[islice]->get_data_pos(ix,iy,pnew);
      slices[islice]->get_data_pos(down_x,down_y,pold);
      label="Line slope: "+QString::number((pnew[1]-pold[1]) /(pnew[0]-pold[0]));      
   
   }

}
void  mouse_overlay::l_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
    redraw_it(slices,islice,ix,iy,pos,com,draw_o);

}

void mouse_overlay::r_mouse_d(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   range_down(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_overlay::r_mouse_m(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
   hyper_move(slices,islice,ix,iy,pos,com,draw_o);
   if(ivalid_down==islice){
      float pnew[2],pold[2];
      slices[islice]->get_data_pos(ix,iy,pnew);
      slices[islice]->get_data_pos(down_x,down_y,pold);
      float slow2=(pnew[0]*pnew[0]-pold[0]*pold[0])/(pnew[1]*pnew[1]-pold[1]*pold[1]);
      float t02=(pnew[0]*pnew[0]-slow2*pnew[1]*pnew[1]);
      
      label="Hyperbola:  T0="+QString::number(sqrtf(t02))+" Velocity="+QString::number(sqrtf(1./slow2));   
   
   }
}
void  mouse_overlay::r_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
    redraw_it(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_overlay::m_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
  update_pos(slices,islice,ix,iy,pos,com,draw_o);
}




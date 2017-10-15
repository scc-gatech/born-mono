#include "mouse_region.h"

mouse_region::mouse_region(){

status="Region: L-Add dividing line (finish) M-Move (reset)  R-Delete Window";
ivalid_down=-1;

}

void mouse_region::l_mouse_r(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
   add_region_pt(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_region::l_mouse_double(std::vector<slice*> slices, int islice, int ix, int iy,
  orient_cube *pos, std::vector<QString> *com,draw_other *draw_o){
  fprintf(stderr,"in mouse release \n");
   finish_region_pt(slices,islice,ix,iy,pos,com,draw_o);
}

void mouse_region::r_mouse_m(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
   range_move(slices,islice,ix,iy,pos,com,draw_o);
  
}
void mouse_region::r_mouse_r(std::vector<slice*> slices, int islice, int ix, int iy,
orient_cube *pos, std::vector<QString> *com,draw_other *draw_o){
   com->at(1)="none";
  if(TIME.elapsed() > 100) {
    region_delete(slices,islice,ix,iy,pos,com,draw_o);
    TIME.restart();
  }
}
void mouse_region::r_mouse_d(std::vector<slice*> slices, int islice, int ix, int iy,
orient_cube *pos, std::vector<QString> *com,draw_other *draw_o){
 TIME.start();
 if(draw_o==0);
 com->at(1)="none";
 if((int)slices.size()==0 && islice==0 && ix==0 && iy==0 && pos==0);
}
void mouse_region::m_mouse_double(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
reset(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_region::m_mouse_r(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
   update_pos(slices,islice,ix,iy,pos,com,draw_o);
}
void mouse_region::m_mouse_d(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
   if((int) slices.size()==0 && pos==0 && draw_o==0);
   if(islice==-1) non_slice_down(ix,iy,com);
}
void mouse_region::m_mouse_m(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
  if((int)slices.size()==0 && islice==0 && pos==0  && draw_o==0);
   if(ivalid_down==-5) non_slice_move(ix,iy,com,draw_o);

}

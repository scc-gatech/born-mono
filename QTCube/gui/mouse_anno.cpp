#include "mouse_anno.h"
using namespace SEP;
mouse_anno::mouse_anno(){

status="Annotate: L-Add annotation M-Move annotation R-Deleta annotation";

}

void mouse_anno::m_mouse_d(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
 TIME.start();
down_x=ix;
down_y=iy;
 com->at(1)="annotate"; com->at(2)="move"; com->at(3)=QString::number(down_x)+":"+QString::number(down_y)+
   ":"+QString::number(ix)+":"+QString::number(iy);
   }
void mouse_anno::m_mouse_m(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
 com->at(1)="none";
   if(islice==-1) non_slice_down(ix,iy,com);
   else{
  if(TIME.elapsed() > 100) {
 com->at(1)="annotate"; com->at(2)="shift"; com->at(3)=QString::number(down_x)+":"+QString::number(down_y)+
   ":"+QString::number(ix)+":"+QString::number(iy);
    TIME.restart();
  }
  }
}
void  mouse_anno::m_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
  com->at(1)="annotate"; com->at(2)="finish"; com->at(3)=QString::number(down_x)+":"+QString::number(down_y)+
   ":"+QString::number(ix)+":"+QString::number(iy);

}
void mouse_anno::l_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
 com->at(1)="annotate"; com->at(2)="finish"; com->at(3)=QString::number(down_x)+":"+QString::number(down_y)+
   ":"+QString::number(ix)+":"+QString::number(iy);

}
void mouse_anno::l_mouse_m(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
 
 com->at(1)="none";
   if(islice==-1) non_slice_down(ix,iy,com);
   else{
  if(TIME.elapsed() > 100) {
 com->at(1)="annotate"; com->at(2)="update"; com->at(3)=QString::number(down_x)+":"+QString::number(down_y)+
   ":"+QString::number(ix)+":"+QString::number(iy);
    TIME.restart();
  }
  }
  
}
void mouse_anno::l_mouse_d(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
 TIME.start();
down_x=ix;
down_y=iy;
 com->at(1)="annotate"; com->at(2)="add"; com->at(3)=QString::number(down_x)+":"+QString::number(down_y)+
   ":"+QString::number(ix)+":"+QString::number(iy);

 if((int)slices.size()==0 && islice==0 && ix==0 && iy==0 && pos==0);
}
void mouse_anno::r_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o){
 com->at(1)="annotate"; com->at(2)="remove"; com->at(3)==QString::number(ix)+":"+QString::number(iy);
   }
   
  
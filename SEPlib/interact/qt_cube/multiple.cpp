#include<multiple.h>
#include<ctime>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPolygon>


int multiple::in_slice(int ix, int iy){
  for(int i=0;i < (int)slices.size(); i++){
    if (slices[i]->in_slice(ix,iy)) return i;
  }
  
  return -1;

}

std::vector<QString> multiple::left_mouse_release_multiple(float x, float y, orient_cube *pos,\
  QMouseEvent * event)  {
      int ix,iy;
      if(event==0 ){}
    this->convert_to_local_loc(x,y,&ix,&iy);
      int cur_slice=this->in_slice(ix,iy);
      if(valid && cur_slice==slice_down &&cur_dat->moveable){
         valid=false;
         slices[cur_slice]->update_range(&com,down_x,down_y,ix,iy,pos);

         return com;
      }
      valid=false;
      com[1]="none";return(com);
   }
std::vector<QString> multiple::left_mouse_down_multiple(float x, float y, orient_cube *pos,QMouseEvent * event){
  valid=false;
  if(pos==0 && event==0){}
 
  TIME.start();
  this->convert_to_local_loc(x,y,&down_x,&down_y);

  slice_down=in_slice(down_x,down_y);
  if(slice_down>=0) valid=true;
  com[1]="none";return com;
 }
std::vector<QString> multiple::left_mouse_move_multiple(float x, float y, orient_cube *pos,QMouseEvent * event){
      int ix,iy;
        if(pos==0 && event==0){}
      this->convert_to_local_loc(x,y,&ix,&iy);

      int cslice=in_slice(ix,iy);
      if(valid && cslice==slice_down  && TIME.elapsed()>200 &&cur_dat->moveable){
        TIME.restart();
        draw_o->objs.push_back(new draw_line(down_x,down_y,ix,iy));
       /*
       boxes.push_back(QPolygon(5));
        boxes[0].putPoints(0, 5,  //top
                       down_x,down_y,down_x,iy,
                       ix,iy,ix,down_y,down_x,down_y);
 */
        com[1]="redraw"; com[2]="only";
      }
      else com[1]="none";
      return com;

  }
std::vector<QString> multiple::key_release_multiple( QKeyEvent *e,orient_cube *pos ){
  return key_press_view(e,pos);
}
std::vector<QString> multiple::key_press_multiple( QKeyEvent *e,orient_cube *pos ){
  if(e==0){}
 return key_press_view(e,pos);
}
std::vector<QString> multiple::mid_mouse_release_multiple(float x, float y, orient_cube *pos,QMouseEvent * event){
  int ix,iy;
  if(event==0){}
  this->convert_to_local_loc(x,y,&ix,&iy);
  int icur=this->in_slice(ix,iy);
  if(icur >=0 && cur_dat->moveable){
    slices[icur]->update_pos(&com,ix,iy,pos);
    return com;
    
  }
  com[1]="none"; return(com);
}

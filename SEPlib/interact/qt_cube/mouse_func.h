#ifndef MOUSE_FUNC_H
#define MOUSE_FUNC_H 1
#include "slice.h"
#include "draw_other.h"
#include <qdatetime.h>
class mouse_func{

  public:
  
  mouse_func(){ivalid_down=-4;down_x=0;down_y=0;};
  virtual ~mouse_func(){};
  void do_noth(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
  
  virtual void l_mouse_d(std::vector<slice*> slices, int islice, int ix, int iy, 
    orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    return(do_noth(slices,islice,ix,iy,pos,  com,draw_o));
  }
  virtual void l_mouse_m(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
     
    return(do_noth(slices,islice,ix,iy,pos,  com,draw_o));
  }
  virtual void l_mouse_r(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    return(do_noth(slices,islice,ix,iy,pos,  com,draw_o));
  }
  
  virtual void m_mouse_d(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    return(do_noth(slices,islice,ix,iy,pos,com,draw_o));
  }
  virtual void m_mouse_m(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
  
    return(do_noth(slices,islice,ix,iy,pos,com,draw_o));
  }
  virtual void m_mouse_r(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    return(do_noth(slices,islice,ix,iy,pos,com,draw_o));
  }
  
  virtual void r_mouse_d(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    return(do_noth(slices,islice,ix,iy,pos,com,draw_o));
  }
  virtual void r_mouse_m(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    
    return(do_noth(slices,islice,ix,iy,pos,  com,draw_o));
  }
  virtual void r_mouse_r(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    return(do_noth(slices,islice,ix,iy,pos,com,draw_o));
  }
    virtual void r_mouse_double(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    return(do_noth(slices,islice,ix,iy,pos,com,draw_o));
  }
   virtual void l_mouse_double(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    return(do_noth(slices,islice,ix,iy,pos,com,draw_o));
  }
  virtual void m_mouse_double(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    return(do_noth(slices,islice,ix,iy,pos,com,draw_o));
  }
void del_pt(std::vector <slice*>slices,int islice,int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o,bool move=false);
void update_pos(std::vector <slice*>slices, int islice,int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
void  update_range(std::vector<slice*>slices,int islice, int ix,int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
void range_down(std::vector<slice*>slices,int islice, int ix,int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
void range_move(std::vector<slice*>slices,int islice, int ix,int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
void add_pt(std::vector <slice*>slices,int islice,int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
void reset(std::vector <slice*>slices,int islice,int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
void move_pt(std::vector<slice*>slices,int islice, int ix,int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
void non_slice_move(int ix, int iy,std::vector<QString> *com,draw_other *draw_o);
void non_slice_down(int ix, int iy,std::vector<QString> *com);

  int ivalid_down;
  int down_x,down_y;
  QTime TIME;
  double down_time;
  QString status;
  std::vector<int> coord1,coord2;

  
};

#endif

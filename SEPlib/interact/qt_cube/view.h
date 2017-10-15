#include <qwidget.h>
#include <qimage.h>
#include <qmatrix.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include "orient_cube.h"
#include "slice.h"
#include "dataset.h"
#include  "mode.h"
#include "mouse_func.h"
#include "raster.h"
#include <qdatetime.h>
#include "draw_other.h"
#include <vector>
#include <qstring.h>
#include "pick_draw.h"
#ifndef VIEW_H 
#define VIEW_H 1

class draw_what{
  public:
   draw_what(){
      draw_bar=false;
      draw_picks=true;
      draw_grid=false;
      draw_pos=100;
      draw_axes=true;
      draw_title=true;
      draw_region=false;
   }
   bool draw_bar;
  bool draw_picks;
  bool draw_grid;
  int draw_pos;
  bool draw_axes;
  bool draw_title;
  bool draw_region;
  
};

class view{

  public:
    view(){};
 
    virtual ~view(){};
    void set_size(int b_x,int e_x, int b_y, int e_y);
    int get_bx();
    int get_by();
    int get_ex();
    int get_ey();
    virtual void viewit(QPainter *painter,QPen *pen, slice *fact,
     dataset *dat, orient_cube *pos,int bx, int ex, int by, int ey, draw_what *draws,
       bool overlay){
        if(painter==0 && pen==0&& fact==0 && dat==0&& pos==0 && bx==ex && by==ey &&
          draws==0 && overlay){}
       };
    
 
    
    //VIRTUAL FUNCTIONS
    //RIGHT
    virtual std::vector<QString> right_mouse_release(float x, float y, orient_cube *pos,QMouseEvent * event){
      return right_mouse_release_view(x,y,pos,event);
    }
    virtual std::vector<QString> right_mouse_move(float x, float y, orient_cube *pos,QMouseEvent * event){
      return right_mouse_move_view(x,y,pos,event);
    }
    virtual std::vector<QString> right_mouse_down(float x, float y, orient_cube *pos,QMouseEvent * event){
      return right_mouse_down_view(x,y,pos,event);
    }
    //MID
    virtual std::vector<QString> mid_mouse_release(float x, float y, orient_cube *pos,QMouseEvent * event){
      return mid_mouse_release_view(x,y,pos,event);
    }
    virtual std::vector<QString> mid_mouse_move(float x, float y, orient_cube *pos,QMouseEvent * event){
      return mid_mouse_move_view(x,y,pos,event);
    }
    virtual std::vector<QString> mid_mouse_down(float x, float y, orient_cube *pos,QMouseEvent * event){
      return mid_mouse_down_view(x,y,pos,event);
    }
    //LEFT
        virtual std::vector<QString> left_mouse_down(float x, float y, orient_cube *pos,QMouseEvent * event){
      return left_mouse_down_view(x,y,pos,event);
    }
    virtual std::vector<QString> left_mouse_release(float x, float y, orient_cube *pos,QMouseEvent * event){
      return left_mouse_release_view(x,y,pos,event);
    }
    virtual std::vector<QString>  left_mouse_move(float x, float y, orient_cube *pos,QMouseEvent * event){
      return left_mouse_move_view(x,y,pos,event);
    }
    virtual std::vector<QString> keyPressEvent( QKeyEvent *e , orient_cube *pos){
      return key_press_view(e,pos);
    }
    virtual std::vector<QString> keyReleaseEvent( QKeyEvent *e, orient_cube *pos ){
      return key_release_view(e,pos);
    }
    
    
    
    
    std::vector<QString> do_noth(float x, float y, orient_cube *pos);
    
    virtual std::vector<QString> r_mouse_d(float x, float y, orient_cube *pos, mouse_func *mode);
   virtual  std::vector<QString> r_mouse_m(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> r_mouse_r(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> r_mouse_d_e(float x, float y, orient_cube *pos,mouse_func *mode, 
       draw_other *draw_o){ 
      if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }
    virtual std::vector<QString> r_mouse_m_e(float x, float y, orient_cube *pos,
      mouse_func *mode, draw_other *draw_o){ 
       if(mode==0|| draw_o==0){}
      return(do_noth(x,y,pos));
    }
    virtual std::vector<QString> r_mouse_r_e(float x, float y, orient_cube *pos,
      mouse_func *mode, draw_other *draw_o){ 
       if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }
    virtual void resize_view(int ix,int iy){if(ix==iy){}}
        virtual void resize_view_beg(int ix,int iy){if(ix==iy){}} 
    virtual void resize_view_end(int ix,int iy){if(ix==iy){}} 

    virtual std::vector<QString> m_mouse_d(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> m_mouse_m(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> m_mouse_r(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> m_mouse_d_e(float x, float y, orient_cube *pos, mouse_func *mode,draw_other *draw_o){ 
       if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }
    virtual std::vector<QString> m_mouse_m_e(float x, float y, orient_cube *pos,mouse_func *mode,draw_other *draw_o){ 
       if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }
    virtual std::vector<QString> m_mouse_r_e(float x, float y, orient_cube *pos,mouse_func *mode,draw_other *draw_o){ 
       if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }
      
    virtual std::vector<QString> l_mouse_d(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> l_mouse_m(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> l_mouse_r(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> l_mouse_d_e(float x, float y, orient_cube *pos,mouse_func *mode,draw_other *draw_o){ 
       if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }
    virtual std::vector<QString> l_mouse_m_e(float x, float y, orient_cube *pos,mouse_func *mode,draw_other *draw_o){ 
       if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }
    virtual std::vector<QString> l_mouse_r_e(float x, float y, orient_cube *pos,mouse_func *mode,draw_other *draw_o){ 
       if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }
  
    virtual std::vector<QString> l_mouse_double(float x, float y, orient_cube *pos, mouse_func *mode);
   virtual  std::vector<QString> m_mouse_double(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> r_mouse_double(float x, float y, orient_cube *pos, mouse_func *mode);
    virtual std::vector<QString> l_mouse_double_e(float x, float y, orient_cube *pos,mouse_func *mode, 
    draw_other *draw_o){ 
       if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }
       virtual std::vector<QString> m_mouse_double_e(float x, float y, orient_cube *pos,mouse_func *mode,draw_other *draw_o){ 
       if(mode==0 || draw_o==0){}
      return(do_noth(x,y,pos));
    }   
    virtual std::vector<QString> r_mouse_double_e(float x, float y, orient_cube *pos,mouse_func *mode, 
      draw_other *draw_o){ 
       if(mode==0 || draw_o==0) {}
      return(do_noth(x,y,pos));
    }
    
    //DEFAULT BEHAVIOR
    std::vector<QString> right_mouse_release_view(float x, float y, orient_cube *pos,QMouseEvent * event){
       if(x==y && pos==0 && event==0) {}
     return do_noth();}
    std::vector<QString> right_mouse_down_view(float x, float y, orient_cube *pos,QMouseEvent * event){
     if(x==y && pos==0 && event==0) {}
   return do_noth();}
    std::vector<QString> right_mouse_move_view(float x, float y, orient_cube *pos,QMouseEvent * event){
        if(x==y && pos==0 && event==0) {}
      return do_noth();}
    std::vector<QString> mid_mouse_release_view(float x, float y, orient_cube *pos,QMouseEvent * event){
       if(x==y && pos==0 && event==0) {}
     return do_noth();}
    std::vector<QString> mid_mouse_down_view(float x, float y, orient_cube *pos,QMouseEvent * event){
       if(x==y && pos==0 && event==0) {}
     return do_noth();}
    std::vector<QString> mid_mouse_move_view(float x, float y, orient_cube *pos,QMouseEvent * event){
       if(x==y && pos==0 && event==0) {}
     return do_noth();}
    std::vector<QString> left_mouse_release_view(float x, float y, orient_cube *pos,QMouseEvent * event){
       if(x==y && pos==0 && event==0) {}
     return do_noth();}
    std::vector<QString> left_mouse_down_view(float x, float y, orient_cube *pos,QMouseEvent * event){
       if(x==y && pos==0 && event==0) {}
     return do_noth();}
     
     
    std::vector<QString> left_mouse_move_view(float x, float y, 
      orient_cube *pos,QMouseEvent * event){
         if(x==y && pos==0 && event==0) {}
        return do_noth();}
    std::vector<QString> key_press_view( QKeyEvent *e , orient_cube *pos);
    std::vector<QString> key_release_view( QKeyEvent *e, orient_cube *pos );
    void set_basics(pick_draw *_pks,draw_other *dr){ 
    trspace=false;
      draw_o=dr;
      pk=_pks;
     }
    std::vector<QString> do_noth(){com[1]="none";return com;}
    void set_new_font(QFontMetrics *f){fm=f; 
    }
    
    void delete_slices();
    virtual void get_pcts(float *pt,float *pb){ *pt=0;*pb=0;}
    virtual void set_pcts(float pt, float pb){ if(pt==pb){} }
    void clear_mouse(){old_mx=-1; old_my=-1;valid=false;}
    void draw_bar(int xshift,QPainter *painter,QPen *pen, raster *fact, 
    unsigned char *conv, bool draw_title);
    void draw_title(int xshift,QPainter *painter,QPen *pen,QString title);
    void render_it(QPainter *painter,QPen *pen, slice *fact, 
  dataset *dat, orient_cube *pos, bool overlay, draw_what *draw);
  int get_corner_sel(){ return corner_sel;}
  void set_corner_sel(int c){corner_sel=c;}
  void set_title_pos(QPainter *painter, dataset *dat);

    void convert_to_local_loc(float x,float y, int *ix, int *iy);
    QFontMetrics *fm;
    pick_draw *pk;
    mode *mym;
 
     int bx,ex,by,ey,old_mx,old_my,down_x,down_y,baxes,bound_t,bound_b,bound_l,bound_r;
 QTime TIME;
 double down_time;
 bool valid;
 dataset *cur_dat;
 
   std::vector<slice*> slices;
 draw_other *draw_o;
 std::vector<QString> com; 
 QString iview;
  int cx_t,cy_t;
  bool title_bot;
  bool trspace;
  int corner_sel;
  private:
 
 
 };
 #endif


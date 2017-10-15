#include<view.h>
#include<slice.h>
#include<dataset.h>
#include<vector>
#include<qfontmetrics.h>
#include <QMouseEvent>
#include <QKeyEvent>
#ifndef MULTIPLE_H
#define MULTIPLE_H 1
class multiple: public view
{

public:
  multiple(){}

  int in_slice(int ix, int iy);
  
  //required view constructors
  std::vector<QString> keyReleaseEvent( QKeyEvent *e,orient_cube *pos ){
    return key_release_multiple(e,pos);
  }
  std::vector<QString> keyPressEvent(QKeyEvent *e, orient_cube *pos){
    return key_press_multiple(e,pos);
  }
  //Left 
  std::vector<QString> left_mouse_release(float x, float y, orient_cube *pos,QMouseEvent * event){
    return left_mouse_release_multiple(x,y,pos,event);
  }
  std::vector<QString> left_mouse_down(float x, float y, orient_cube *pos,QMouseEvent * event){
   return left_mouse_down_multiple(x,y,pos,event);
  }
  std::vector<QString> left_mouse_move(float x, float y, orient_cube *pos,QMouseEvent * event){
   return left_mouse_move_multiple(x,y,pos,event);
  }
  //Middle
  std::vector<QString> mid_mouse_release(float x, float y, orient_cube *pos,QMouseEvent * event){
    return mid_mouse_release_multiple(x,y,pos,event);
  }
  std::vector<QString> mid_mouse_down(float x, float y, orient_cube *pos,QMouseEvent * event){
   return mid_mouse_down_multiple(x,y,pos,event);
  }
  std::vector<QString> mid_mouse_move(float x, float y, orient_cube *pos,QMouseEvent * event){
   return mid_mouse_move_multiple(x,y,pos,event);
  }
  //Right
  std::vector<QString> right_mouse_release(float x, float y, orient_cube *pos,QMouseEvent * event){
    return right_mouse_release_multiple(x,y,pos,event);
  }
  std::vector<QString> right_mouse_down(float x, float y, orient_cube *pos,QMouseEvent * event){
   return right_mouse_down_multiple(x,y,pos,event);
  }
  std::vector<QString> right_mouse_move(float x, float y, orient_cube *pos,QMouseEvent * event){
   return right_mouse_release_multiple(x,y,pos,event);
  }
  
  
  std::vector<QString> left_mouse_release_multiple(float x, float y, orient_cube *pos,QMouseEvent * event);
  std::vector<QString> left_mouse_down_multiple(float x, float y, orient_cube *pos,QMouseEvent * event);
  std::vector<QString> left_mouse_move_multiple(float x, float y, orient_cube *pos,QMouseEvent * event);
  std::vector<QString> mid_mouse_release_multiple(float x, float y, orient_cube *pos,QMouseEvent * event);
  
  std::vector<QString> mid_mouse_down_multiple(float x, float y, orient_cube *pos,QMouseEvent * event){
    return mid_mouse_down_view(x,y,pos,event);
  }
  std::vector<QString> mid_mouse_move_multiple(float x, float y, orient_cube *pos,QMouseEvent * event){
    return mid_mouse_move_view(x,y,pos,event);
  }
  std::vector<QString> right_mouse_release_multiple(float x, float y, orient_cube *pos,QMouseEvent * event){
    return right_mouse_release_view(x,y,pos,event);
  }
  std::vector<QString> right_mouse_down_multiple(float x, float y, orient_cube *pos,QMouseEvent * event){
    return right_mouse_down_view(x,y,pos,event);
  }
  std::vector<QString> right_mouse_move_multiple(float x, float y, orient_cube *pos,QMouseEvent * event)
  {
    return right_mouse_move_view(x,y,pos,event);
  }
  std::vector<QString> key_release_multiple(QKeyEvent *e, orient_cube *pos);
  std::vector<QString> key_press_multiple(QKeyEvent *e, orient_cube *pos);
  ~multiple(){
    delete_slices();
  }
  void draw_picks(QPainter *painter, orient_cube *pos);

 
  int slice_down;
  private:

};
#endif

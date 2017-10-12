#include<view.h>
#include<slice.h>
#include<dataset.h>
#include<vector>
#include<qfontmetrics.h>
#include <QMouseEvent>
#include <QKeyEvent>
#ifndef MULTIPLE_H
#define MULTIPLE_H 1
namespace SEP{
class multiple: public view
{

public:
  multiple(){}

  int in_slice(int ix, int iy);
  
  //required view constructors
  std::vector<QString> keyReleaseEvent( QKeyEvent *e,std::shared_ptr<SEP::orient_cube>pos ){
    return key_release_multiple(e,pos);
  }
  std::vector<QString> keyPressEvent(QKeyEvent *e, std::shared_ptr<SEP::orient_cube>pos){
    return key_press_multiple(e,pos);
  }
  
  std::vector<QString> key_release_multiple(QKeyEvent *e, std::shared_ptr<SEP::orient_cube>pos);
  std::vector<QString> key_press_multiple(QKeyEvent *e, std::shared_ptr<SEP::orient_cube>pos);
  ~multiple(){
    delete_slices();
  }
  void draw_picks(QPainter *painter, std::shared_ptr<SEP::orient_cube>pos);

 
  int slice_down;
  private:

};
}
#endif

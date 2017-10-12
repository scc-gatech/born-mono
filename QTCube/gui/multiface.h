#include<slice.h>
#include<dataset.h>
#include<vector>
#include<view.h>
#include<qfontmetrics.h>
#ifndef multi_face_H
#define multi_face_H 1
namespace SEP{
class multi_face: public view
{

public:
  multi_face(){}
  multi_face(QString view,std::shared_ptr<QFontMetrics >f_m,std::shared_ptr<SEP::pick_draw> _pks,std::shared_ptr<SEP::draw_other> dr,int g1,int g2, int j);
  void viewit(QPainter *pm,QPen *pen, std::shared_ptr<SEP::slice>fact,std::shared_ptr<dataset>dat,std::shared_ptr<SEP::orient_cube> pos,
  int bx, int ex, int by, int ey,std::shared_ptr<SEP::draw_what>drawit,bool overlay);

  ~multi_face(){  }
  
   private:
    int grid1,grid2;
    int j3;
    int bsmall;


};
}
#endif

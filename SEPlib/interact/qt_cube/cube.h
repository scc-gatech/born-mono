#include<view.h>
#include<slice.h>
#include<dataset.h>
#include<vector>
#include<qfontmetrics.h>
#include "three_view.h"
#ifndef CUBE_H
#define CUBE_H 1
class cube: public three_view
{

public:
  cube(){}
  cube(QString view,QFontMetrics *f_m,pick_draw *_pks,mode *mm,draw_other *dr);
  void viewit(QPainter *pm,QPen *pen, slice *fact,dataset *dat,orient_cube *pos,
  int bx, int ex, int by, int ey,draw_what *draws,bool overlay);
  ~cube(){}

};
#endif

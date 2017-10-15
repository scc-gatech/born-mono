#include<view.h>
#include<slice.h>
#include<dataset.h>
#include<vector>
#include "orient_cube.h"
#include<qfontmetrics.h>
#include "three_view.h"
#ifndef CUBE_CUT_H
#define CUBE_CUT_H 1
class cube_cut: public three_view
{

public:
  cube_cut(){ori=0;}
  cube_cut(QString view,QFontMetrics *f_m,pick_draw *_pks,mode *mm,draw_other *dr);
  void viewit(QPainter *pm,QPen *pen, slice *fact,dataset *dat,orient_cube *pos,
  int bx, int ex, int by, int ey, draw_what *draws,bool overlay);
  void delete_orient();
  ~cube_cut(){ delete_orient();}
  orient_cube *ori_f,*ori_s,*ori_t,*ori;

};
#endif

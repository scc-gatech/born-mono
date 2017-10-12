#include<view.h>
#include<slice.h>
#include<dataset.h>
#include<vector>
#include "orient_cube.h"
#include<qfontmetrics.h>
#include "three_view.h"
#ifndef CUBE_CUT_H
#define CUBE_CUT_H 1
namespace SEP{
class cube_cut: public three_view
{

public:
  cube_cut(){ori=0;}
  cube_cut(QString view,std::shared_ptr<QFontMetrics >f_m,std::shared_ptr<SEP::pick_draw>_pks,std::shared_ptr<SEP::draw_other>dr,std::vector<float> proportions);
  void viewit(QPainter *pm,QPen *pen, std::shared_ptr<SEP::slice>fact,std::shared_ptr<SEP::dataset>dat,std::shared_ptr<SEP::orient_cube>pos,
  int bx, int ex, int by, int ey, std::shared_ptr<SEP::draw_what>draws,bool overlay);
  void delete_orient();
  ~cube_cut(){ delete_orient();}
  std::shared_ptr<SEP::orient_cube>ori_f,ori_s,ori_t,ori;

};
}
#endif

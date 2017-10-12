
#include <qpainter.h>
#include<dataset.h>
#include<float.h>
#include<orient_cube.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <vector>
#include "slice.h"
#ifndef WIGGLE_H
#define WIGGLE_H 1
namespace SEP{
class wiggle: public slice
{
  
 public:
  
  wiggle(){set_wiggle_color("black"); set_pos_color("black"); set_neg_color("none");  is_raster=false;};
 
 	void PaintWiggles(int n2, int n1, QPainter *painter, unsigned char *buf);

  virtual void draw_slice(QPainter *painter,std::shared_ptr<SEP::dataset>dat,QPen *pen,
  std::shared_ptr<orient_cube>pos,  bool ov,bool draw_grid);
  std::shared_ptr<slice>clone();
  
  void set_wiggle_color(QString color);
  QString return_wiggle_color();
    void set_neg_color(QString color);
  QString return_neg_color(); 
  void set_pos_color(QString color);
  QString return_pos_color();
  
  QString pos_color,neg_color,wiggle_color;
  QColor wiggleColor,posColor,negColor;

};
}
#endif


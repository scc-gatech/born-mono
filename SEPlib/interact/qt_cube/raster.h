#include <qimage.h>
#include <stdlib.h>
#include<string>
#include <qvector.h>
#include <qrgb.h>
#include "slice.h"
#ifndef RASTER_H
#define RASTER_H 1

class raster: public slice{

 public:
  raster(){};
  QImage * makeImage(unsigned char * uc, int nx, int ny);
  QVector<QRgb> scale;
    void set_bcolor(QString c);
  void set_ecolor(QString c);
  QString get_bcolor(){ return bcolor;}
  QString get_ecolor(){ return ecolor;}
  QString bcolor,ecolor;
  virtual void draw_slice(QPainter *painter,dataset *dat,QPen *pen,
  orient_cube *pos,  bool ov,bool draw_grid);
  void change_map(int i,QString col);
  virtual slice *clone();
  slice *clone_alpha(int alpha);


};
#endif


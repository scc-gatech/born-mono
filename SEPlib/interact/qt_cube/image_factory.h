#include <qimage.h>
#include <stdlib.h>
#include<string>
#include <qvector.h>
#include <qrgb.h>
#include <QPainter>
#ifndef IMAGE_FACTORY_H
#define IMAGE_FACTORY_H 1

class image_factory{

 public:
  image_factory(){};
  QImage * makeImage(unsigned char * uc, int nx, int ny);
  QVector<QRgb> scale;
  QColor wiggleColor;
    void set_bcolor(QString c);
  void set_ecolor(QString c);

  
  void change_map(int i,QString col);
  void set_wiggle_color(QString color);
  void set_contour_color(QString color);
  //void grab_wiggle_color(
 // bool return_wiggle(){ return am_wiggle;}
  bool am_wiggle;
  bool am_contour;
   
};

class draw_wiggle: public image_factory{
  public:
  draw_wiggle(){ am_wiggle=true;}


};

#endif


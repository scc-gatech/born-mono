#include <qimage.h>
#include <stdlib.h>
#include<string>
#include <qvector.h>
#include <qrgb.h>
#include "slice.h"
#ifndef RASTER_H
#define RASTER_H 1
namespace SEP{
class raster: public slice{

 public:
  raster(){};
  QImage * makeImage( unsigned char * uc, const int nx, int ny,const int xsize, const int ysize);
  QVector<QRgb> scale;
    void set_bcolor(QString c);
  void set_ecolor(QString c);
  void resampleBand(const unsigned char *in, const int fin,const  int jin, const int nin,
unsigned char *out, const int fout, const int jout, const int nout, const int resamp,int *space) const;
  QString get_bcolor(){ return bcolor;}
  QString get_ecolor(){ return ecolor;}
  QString bcolor,ecolor;
  virtual void draw_slice(QPainter *painter,std::shared_ptr<SEP::dataset>dat,QPen *pen,
  std::shared_ptr<SEP::orient_cube>pos,  bool ov,bool draw_grid);
  void change_map(int i,QString col);
  virtual std::shared_ptr<slice> clone();
  virtual std::shared_ptr<slice>clone_alpha(int alpha);


};
}
#endif


#ifndef COLOR_TABLE_H
#define COLOR_TABLE_H 1
#include "raster.h"
namespace SEP{
class simple_color: public raster
{
  public:
  simple_color(){};
  void fill_it(int *reg, int *green, int *blue,int alpha);
  float fill_color(int *sc,int x,int ncol);



};

class grey:public simple_color
{
public:
grey(int alpha)
{
name="grey";
  int red[4],green[4],blue[4];
  red[0]=green[0]=blue[0]=0;
  red[1]=green[1]=blue[1]=127;
  red[2]=green[2]=blue[2]=128;
  red[3]=green[3]=blue[3]=255;
  this->fill_it(red,green,blue,alpha);
}
};
class greyc:public simple_color
{
public:
greyc(int alpha)
{
name="greyc";
  int red[4],green[4],blue[4];
  red[0]=green[0]=blue[0]=0;
  red[1]=green[1]=blue[1]=127;
  red[2]=green[2]=blue[2]=128;
  red[3]=green[3]=blue[3]=255;
  this->fill_it(red,green,blue,alpha);
   scale[0]=scale[1]=scale[254]=scale[255]=scale[2]=scale[3]=scale[4]=scale[253]=scale[252]=scale[251]=qRgba(255,0,0,alpha);
}
};

class flag:public simple_color
{
public:
flag(int alpha)
{
name="flag";
  int blue[] = {255, 255, 255, 0};
  int green[]={255,255,255,0};
  int red[]={0,255,255,255,255};


  this->fill_it(red,green,blue,alpha);
}
};



class cbl: public simple_color
{
public:
cbl(int alpha)
{
name="cbl";
int blue[] = {0,0,0,255};
  int green[]={0,0,0,0};
  int red[]={255,0,0,0};


  this->fill_it(red,green,blue,alpha);
}
};
class patriotic: public simple_color{
  public:
   patriotic(int alpha){
     name="patriotic";
     int blue[]={128,255,255,0};
     int green[]={0,255,255,0};
     int red[]={0,255,255,128};
     fill_it(red,green,blue,alpha);
  }
 };
class ava:   public simple_color{
public:

  ava(int alpha);

};
class cgsi: public simple_color
{
public:
cgsi(int alpha)
{

name="cgsi";
  int blue[] = {0,255,255,0};
  int green[]={0,255,255,0};
  int red[]={0,255,255,255};


  this->fill_it(red,green,blue,alpha);
}
};
class rainbow: public simple_color
{
public:
rainbow(int alpha);
};
class jet: public simple_color
{
public:
jet(int alpha);
};
class color_cycle: public simple_color
{
public:
color_cycle(int alpha);
};

class user_color: public simple_color{
  public:
    user_color(int alpha);
    void set_red(int *reg);
    void set_green(int *green);
    void set_blue(int *blue);
};
}
#endif

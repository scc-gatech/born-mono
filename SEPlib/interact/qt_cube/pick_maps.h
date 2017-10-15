#include "pikset.h"
#include<map>
#include<vector>
#include <qpainter.h>
#include <qpixmap.h>
#ifndef PICK_MAPS_H
#define PICK_MAPS_H 1

class pick_maps
{
  public:
    pick_maps();
    void change_active(QString name);
    float*    return_pick_line(int n, int *ipos,float *scale,float *os,float *ds,int single,int iax);
    void build_int_line(std::vector<pik> pts, int f, int n, float *ar,int single,
    float osingle, float dsingle, int isort, float osort, float dsort){
         return active->build_int_line(pts,f,n,ar,single,osingle,dsingle,isort,osort,dsort);
      }
    QString active_name;

    std::map<QString,pikset*> groups;
    pikset *active;

};
#endif

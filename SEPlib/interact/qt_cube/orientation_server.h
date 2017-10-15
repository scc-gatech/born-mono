#include <math.h>
#include <QString>
#include <map>
#include <set>
#include <list>
#include "axis.h"
#include "position.h"
#include "pick_new.h"
#ifndef ORIENTSERVER_H
#define ORIENTSERVER_H 1
#define MAX_MAPS 100

class converter{
  public:
  converter(int *in,axis *ax,float a,float *cen,axis *una);
 pick_new *convert_pick( pick_new *in);
  pick_new *convert_back_pick( pick_new *in);
  void rotate_pt_back(float x1, float x2, float *y1, float *y2);

  void rotate_pt(float x1, float x2, float *y1, float *y2);
  private:
    axis a1,a2;
    int rot_ax[2];
    axis ax_rot[2];
    float rot_cen[2];
    float ang;
};
    
class orientation_server{
  public:
  orientation_server(position *pos);
  int get_new_num(){ num+=1; active_list.insert(num);
   return num;}
  int get_new_num(int iold,int *in, axis *ax, float a, float *cen);
   pick_new *convert_pick(int oc,pick_new *pk);
   pick_new *convert_back_pick(int oc,pick_new *pk);

  position *myp;
  std::set<int> *return_active(){ return &active_list;}
  std::map<int,converter*> converters;
  private:
  std::set<int> active_list;
  int num;
};

#endif

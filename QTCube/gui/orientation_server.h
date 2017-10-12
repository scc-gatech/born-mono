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
namespace SEP{
class converter{
  public:
  converter(int *in,std::vector<SEP::axis> ax,float a,float *cen,std::vector<SEP::axis>una);
  std::shared_ptr<pick_new >convert_pick( std::shared_ptr<pick_new >in);
  std::shared_ptr<pick_new >convert_back_pick( std::shared_ptr<pick_new >in);
  void rotate_pt_back(float x1, float x2, float *y1, float *y2);

  void rotate_pt(float x1, float x2, float *y1, float *y2);
  private:
    SEP::axis a1,a2;
    int rot_ax[2];
    std::vector<SEP::axis> ax_rot;
    float rot_cen[2];
    float ang;
};
    
class orientation_server{
  public:
  orientation_server(std::shared_ptr<position >pos);
  int get_new_num(){  num+=1; active_list.insert(num);return num;}
   int get_new_inst(){inst+=1; return inst;}
  int get_new_num(int iold,int *in, std::vector<SEP::axis>ax, float a, float *cen);
   std::shared_ptr<pick_new >convert_pick(int oc,std::shared_ptr<pick_new>pk);
   std::shared_ptr<pick_new >convert_back_pick(int oc,std::shared_ptr<pick_new >pk);

  std::shared_ptr<position >myp;
  std::set<int> *return_active(){ return &active_list;}
  std::map<int,std::shared_ptr<converter>> converters;
  private:
  std::set<int> active_list;
  int num;
  int inst;
};
}

#endif

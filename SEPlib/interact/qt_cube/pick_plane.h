#ifndef PICK_PLANE_H 
#define PICK_PLANE_H 1
#include "pickset.h"
#include <QPainter>
#include "orient_cube.h"
#include "slice.h"
class pick_plane{
  public:
    pick_plane(std::map<QString, picks*> *all,int i1, int i2);
    ~pick_plane(){};
    void draw(QPainter *painter,int ndist, std::vector<pick_plane*>*neighbors,
      std::vector<int> *dist, orient_cube *pos,slice *slc,QString draw_what);
    void add_pt(QString col, long long hash){ maps.find(col)->second->add_pt(hash);}
    void draw_line(pairs *points, QPainter *painter,orient_cube *pos, slice *slc, 
      int sz, int idist);
    pairs get_pts_sort(int iax1, int iax2, float *o, float *d,int ival,
    int isort, QString col, orient_cube *pos=0);
    pairs get_pts_sort_le(int iax1, int iax2, float *o, float *d,int ival,int isort, 
      QString col,orient_cube *pos=0);
    void draw_extra(pairs *points, QPainter *painter,orient_cube *pos, slice *slc, int sz);

    void draw_points(pairs *points, QPainter *painter,orient_cube *pos, slice *slc, int sz);
    long long find_closest(orient_cube *pos, QString col, int *iloc,int extra=-99){ 

      return maps.find(col)->second->find_closest(pos,iax1,iax2,iloc[iax1],iloc[iax2],extra);
    }
    std::vector<long long> find_picks(int iax1, int iax2,QString col,orient_cube *pos=0){

      return maps.find(col)->second->all_picks(pos,iax1,iax2);
    }
    std::vector<long long> shift_pick_window(QString col, int *beg, int *end, int extra){
        return maps.find(col)->second->shift_pick_window(beg,end,extra);
    }
    void del_pt(QString col, long long hash){ maps.find(col)->second->del_pt(hash);}
    void set_dist(int d){ndist=d;}
    pairs subsample(axis a1,int iax1,int beg1, int end1, 
    axis a2,int iax2, int beg2, int end2,QString col,int ilim=-1){
      return maps[col]->subsample(a1,iax1,beg1,end1,a2,iax2,beg2,end2,ilim);
      
    }
    std::vector<int> dist;
    std::map<QString, pickset*> maps;

    int ndist,iax1,iax2;

};
#endif

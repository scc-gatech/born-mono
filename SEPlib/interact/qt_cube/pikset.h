#include "pik.h"
#include <vector>
#include <string>
#include "qcolor.h"

#ifndef PIKSET_H
#define PIKSET_H 1
class pikset
{
  public:
    pikset(){};
    pikset(QString nm, QColor con,int sing,int rg, int ln){
      cur_col=con; 
      name=nm;
      single=sing;
      dist=rg;
      line=ln;
    }

   
    std::vector <pik>  get_pts(float *fbeg, float *fend, int ilim=-1);
    std::vector <pik>  get_pts_sort(float *ibeg, float *iend, int single,float *ds,int ilim=-1);
    QColor get_color(){return cur_col;}
    void add_pt(float *floc,int tp,int ex);
    bool del_pt(float *floc,int iax1, int iax2,float *ds);
    void delete_pt(float *floc,float *ds);
    void find_closest(float *iloc,float *scale,int *iclose, float *dist,float *ds);
    float *return_pick_line(int n, int *ipos,float *scale,float *os, float *ds,
      int ax,int single);
    void build_int_line(std::vector<pik> pts, int f, int n, float *ar,int single,
      float osingle, float dsingle, int isort, float osort, float dsort);
    int get_single(){ return single;}
    void change_single(int ising){ single=ising;}
     int get_dist(){ return dist;}
    void set_dist(int ising){ dist=ising;}
     int get_line(){return line;}
     void set_line(int ln){line=ln;}
   void delete_picks(int ipick);
   


    std::vector<pik> pts;
    QString name;
   private:
    QColor cur_col;
    int single;
    int dist;
    int line;



};
#endif

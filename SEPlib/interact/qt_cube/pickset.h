#ifndef PICKSET_H
#define PICKSET_H 1
#include "pick.h"
#include "axis.h"
#include "slice.h"
#include "pairs.h"
class pickset{
  public:
    pickset(picks *a){all=a;}
    int get_single(){ return all->get_single();}
    int get_display(){ return all->get_display();}
    void set_display(int type){ all->set_display(type);}
    int get_sz(){return all->get_sz();}
    QColor get_color(){ return all->get_color();}
    void add_pt(long long hs){ hash.push_back(hs);}
    void del_pt(long long hs);
    long long find_closest(orient_cube *pos,int iax1, int iax2, int i1, int i2,int extra);
    std::vector<long long> shift_pick_window(int *beg, int *end,int extra); 
    
    pairs subsample(slice *slc, bool *rot,int ilim=-1);
    
    pairs subsample(axis a1,int iax1,int beg1, int end1, 
      axis a2,int iax2, int beg2, int end2,int ilim=-1);
      
      
     pairs get_pts_sort(orient_cube *pos,int iax1, int iax2, float *o, float *d,int ival,int isort);
          pairs get_pts_sort_le(orient_cube *pos, int iax1, int iax2, float *o, float *d,int ival,int isort);

    std::vector<long long> all_picks(orient_cube *pos, int iax1, int iax2);
    picks *all;
    std::vector<long long> hash;
};
#endif

#include "pikset.h"
#include "slice.h"
#include<map>
#include<vector>
#include <qpainter.h>
#include <qpixmap.h>
#include <pick_maps.h>
#include "dataset.h"
#ifndef PICK_GROUPS_H
#define PICK_GROUPS_H 1

class pick_groups
{
  public:
    pick_groups(pick_maps *map);
    
    void change_active(QString name);
    void set_range(int irange);
    void increase_size(){sz++;}
    void decrease_size(){sz--;}
    void set_extra(int ex){extra_state=ex;}
    int  get_extra(){return extra_state;}
    void change_single(int iax);
    void draw_line();
    void draw_pts();
    void set_draw(QString drawing){ draw_what=drawing;}
    void draw(QPainter *painter, orient_cube *pos, slice *slc,dataset *dat);
    void draw_line(pikset *piks,QPainter *painter,orient_cube *pos, slice *slc, int idist,
     float *os, float *ds, dataset *dat);
    void draw_pts(pikset *piks,QPainter *painter, orient_cube *pos, slice *slc, int idist,
      float *os, float *ds,dataset *dat);
    void read_file(const QString &text);
    void write_file(const QString &text);
    void delete_pt(float *floc,float *ds){
      mp->active->delete_pt(floc,ds);    
    }
    QString get_draw_what(){return draw_what;}
    QString get_active_name(){ return mp->active_name;}
    int get_single(){return mp->active->get_single();}
        int get_range(){return mp->active->get_dist();}
    void add_pt(float *floc,int type){ mp->active->add_pt(floc,type,extra_state); changed=false;}
    bool del_pt(float *floc,int iax1,int iax2,float *ds){ 
      bool  aa= mp->active->del_pt(floc,iax1,iax2,ds);
       if(aa) changed=true;
      return aa;
    }
    int get_line(){return mp->active->get_line();}
    bool get_changed(){ 
      bool c=changed; changed=false; return c;}
         pick_maps *mp;
  private:
  
    int sz;
    QString draw_what;
    int iax1,iax2,iax3;
    float beg[8],end[8];
    int ns[8];
    axis opa;
    bool changed;
    int extra_state;
   
};
#endif

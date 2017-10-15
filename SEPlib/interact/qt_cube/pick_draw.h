#ifndef pick_draw_H
#define pick_draw_H 1
#include <vector>
#include "orientation_server.h"
#include <QColor>
#define DISPLAY_LINE 0
#define DISPLAY_POINTS 1
#define DISPLAY_EXTRA 2
#define MAX_ORIENTS 3
#include "my_colors.h"
#include<map>
#include <qpainter.h>
#include <qpixmap.h>
#include "pairs_new.h"
#include "slice.h"
#include "pick_plane_new.h"
#include "pick_bucket.h"
class pick_orient{
  public:
    pick_orient(){;}
    pick_orient(position *pos,int o);

    pick_planes_new *planes;
    pick_bucket *buck;
    ~pick_orient(){ delete buck; delete planes;}
    int oc;
};
class pick_param{

  public:
    pick_param();
    pick_param(int si,int di,int s,QColor c){
      single=si; display_type=di; sz=s; col=c;
    }
    int get_single(){return single;}
    void set_single(int sing){ single=sing;}
    int get_display(){return display_type;}
    void set_display(int type){ display_type=type;}
    void increase_size(){sz++;}
    void decrease_size(){sz--;}
    int get_sz(){return sz;}
    void draw(QPainter *painter, int iax1, int iax2, pairs_new *myp, float rat);
    void draw_points(QPainter *painter,  pairs_new  *myp);
    void draw_line(QPainter *painter,  pairs_new *myp,float rat);
  private:
    int single;
    int display_type;
    int sz;
    QColor col;



};

class pick_draw{
  public:
   pick_draw();
   void set_server(orientation_server *s);
   void set_position(position *pos);
   void set_extra(int ex){ extra=ex;}
   int get_extra(){ return extra;}
   void increase_size();
   void decrease_size();
   void set_dist(int id){ dist=id;}
   int get_dist(){return dist;}
   int get_single();
   void set_single(int single);
   void set_display(int dis);
   int get_display();
   void change_active_col(QString name){
     active_col=name;
   }
   QString get_active_col(){ return active_col;}
   QString get_draw_what(){return draw_what;}
   void set_draw_what(QString c){ draw_what=c;}
   void delete_pick_draw();
   void update_slice_maps(int *iloc, int *inew, int iax1, int iax2, int iax3, slice *slc);
   QString get_active_name(){ return active_col;}
   void int_to_float(const int *iloc, float *floc);
   void read_file(const QString &text);
   void write_file(const QString &text);
   void add_pt(int *iloc, int type,QString col="None",int extra=-99);
   void del_pt(orient_cube *pos, int *floc, int iax1, int iax2);
   void delete_picks(orient_cube *pos, int ival, QString col="None");
   void delete_pick_vals(int ival, QString col); 
   void float_to_int(const float *floc, int *iloc);
   std::map<long long,pick_new*> return_all(QString col){return buck->return_all(col);}
   picks_vec *return_all_picks(int orient_num, QString col);
   picks_vec *return_all_plane(int *iloc, int iax1, int iax2, QString col);
   picks_vec *return_all_plane_single(int *iloc, int iax1, int iax2, QString col);
   void draw(QPainter *painter, slice *slc);
   void clear_picks(orient_cube *pos,int *iloc,QString interval);
   void del_pick_orients(pick_new *pk);
   
   pairs_new *get_pts_sort(orient_cube *orient_num,int *iloc, int i1,int isort, QString col);   
   picks_vec *create_load_plane(orient_cube *pos,int iax1,int iax2, int *iloc);
   picks_vec *return_iloc_based( int *iloc,  QString col);
   pairs_new *get_pts_sort_le(orient_cube *pos, int iax1, int iax2,int ival, int isort,QString col);
   void ignore_axis(int i){ buck->ignore_axis(i);}
   void set_move(int im){ bulk=im;}
   int get_move(){return bulk;}
   void check_create_oc(orient_cube *pos);
   ~pick_draw(){
     delete_pick_draw();
   }
   
  
  private:
   std::map<int,pick_orient*> orient_planes;
   std::list<int> orients;
   pick_planes_new *planes;
   pick_bucket *buck;
   std::map<QString,pick_param*> all;
   QString draw_what;
   QString active_col;
   int extra;
   int dist;
   int bulk;
   position *myp;
   my_colors myc;
   float o[8],d[8];
   int dig[8];
   int n[8];
  orientation_server *serv;
};


#endif




/*
picking();


  pairs_new get_pts_sort(int *iloc,int iax1, int iax2, int isort, int ival,QString col="None");
  pairs_new get_pts_sort(orient_cube *pos,slice *slc,int ival,int isort, QString col="None");
  pairs_new get_pts_sort_le(int *iloc,int iax1, int iax2, int isort, int ival,QString col="None");
   pairs_new get_pts_sort_le(orient_cube *pos,int *iloc,int iax1, int iax2, int isort, int ival,QString col="None");
 pairs_new get_pts_sort_le(orient_cube *pos,slice *slc,int ival,int isort, QString col="None");
 std::vector<pick*> all_picks(orient_cube *pos,int *iloc, int iax1, int iax2, QString col);
 std::vector<pick*> all_picks(int *iloc, int iax1, int iax2, int ising,QString col);

  private:
    std::vector<plane_contain> planes;
    std::map<QString,picks*> all;
    QString draw_what;
    int ndist;
    int n[8],blk[8];
    float o[8],d[8],dig[8];
    bool changed;
    int extra_state;
    int bulk;
    QString  active_col;

};
*/

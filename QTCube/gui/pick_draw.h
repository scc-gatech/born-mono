#ifndef pick_draw_H
#define pick_draw_H 1
#include <vector>
#include "orientation_server.h"
#include <QColor>
#define DISPLAY_LINE 0
#define DISPLAY_POINTS 1
#define DISPLAY_EXTRA 2
#define DISPLAY_TEXT 3
#define MAX_ORIENTS 3
#include "my_colors.h"
#include<map>
#include <qpainter.h>
#include <qpixmap.h>
#include "pairs_new.h"
#include "slice.h"
#include "pick_plane_new.h"
#include "pick_bucket.h"
#include "my_fonts.h"
namespace SEP{
class pick_orient{
  public:
    pick_orient(){;}
    pick_orient(std::shared_ptr<position>pos,int o);

    std::shared_ptr<SEP::pick_planes_new >planes;
    std::shared_ptr<SEP::pick_bucket >buck;
    ~pick_orient(){ }
    int oc;
};
class pick_param{

  public:
    pick_param();
    pick_param(int si,int di,int s,QColor c,QString fn){
      single=si; display_type=di; sz=s; col=c; font_name=fn; 
      std::shared_ptr<SEP::my_fonts> m(new my_fonts());
      myf=m;
          }
    int get_single(){return single;}
    void set_single(int sing){ single=sing;}
    int get_display(){return display_type;}
    void set_display(int type){ display_type=type;}
    void increase_size(){sz++;}
    void decrease_size(){sz--;}
    int get_sz(){return sz;}
    void draw(QPainter *painter, int iax1, int iax2, std::shared_ptr<SEP::pairs_new >myp, float rat);
    void draw_points(QPainter *painter,  std::shared_ptr<SEP::pairs_new >myp,bool txt);
    void draw_line(QPainter *painter,  std::shared_ptr<SEP::pairs_new >myp,float rat);
  private:
    int single;
    int display_type;
    int sz;
    QColor col;
    QString font_name;
    std::shared_ptr<SEP::my_fonts> myf;



};

class pick_draw{
  public:
   pick_draw();
   void set_server(std::shared_ptr<SEP::orientation_server >s);
   void set_position(std::shared_ptr<SEP::position >pos);
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
   void update_slice_maps(int *iloc, int *inew, int iax1, int iax2, int iax3, std::shared_ptr<SEP::slice>slc);
   QString get_active_name(){ return active_col;}
   void int_to_float(const int *iloc, float *floc);
   void read_file(const QString &text);
   void write_file(const QString &text);
      void add_rotated_pt(int orient_num,int *iloc, int type,QString col="None",int extra=-99,QString txt="None");
   void del_nearest(std::shared_ptr<SEP::orient_cube>pos, int *floc, int iax1, int iax2);
   void add_pt(int *iloc, int type,QString col="None",int extra=-99,QString txt="None");
   void del_pt(std::shared_ptr<SEP::orient_cube>pos, int *floc, int iax1, int iax2);
   void delete_picks(std::shared_ptr<SEP::orient_cube>pos, int ival, QString col="None");
   void delete_pick_vals(int ival, QString col); 
   void float_to_int(const float *floc, int *iloc);
   std::map<long long,std::shared_ptr<SEP::pick_new>> return_all(QString col){return buck->return_all(col);}
   std::shared_ptr<SEP::picks_vec>return_all_picks(std::shared_ptr<SEP::orient_cube>pos, QString col);
   std::shared_ptr<SEP::picks_vec>return_all_plane(int *iloc, int iax1, int iax2, QString col);
   std::shared_ptr<SEP::picks_vec>return_all_plane_single(int *iloc, int iax1, int iax2, QString col);
   void draw(QPainter *painter, std::shared_ptr<SEP::slice>slc);
   void clear_picks(std::shared_ptr<SEP::orient_cube>pos,int *iloc,QString interval);
   void del_pick_orients(std::shared_ptr<SEP::pick_new>pk);
   
   std::shared_ptr<pairs_new >get_pts_sort(std::shared_ptr<SEP::orient_cube>orient_num,int *iloc, int i1,int isort, QString col);   
   std::shared_ptr<picks_vec >create_load_plane(std::shared_ptr<SEP::orient_cube>pos,int iax1,int iax2, int *iloc);
   std::shared_ptr<picks_vec >return_iloc_based( int *iloc,  QString col);
  std::shared_ptr< pairs_new >get_pts_sort_le(std::shared_ptr<SEP::orient_cube>pos, int iax1, int iax2,int ival, int isort,QString col);
   void add_multi_picks(std::vector<long long> locs);
   void delete_multi_picks(std::vector<long long> locs);
   void set_pick_text(QString t){ active_txt=t; fprintf(stderr,"Setting text %s \n",active_txt.toAscii().constData());}
   void ignore_axis(int i){
     if(buck) fprintf(stderr,"see axis \n");
     else fprintf(stderr,"Can not see bucket \n");
    buck->ignore_axis(i);}
   void set_move(int im){ bulk=im;}
   int get_move(){return bulk;}
   void check_create_oc(std::shared_ptr<SEP::orient_cube>pos);
 
   
  
  private:
   std::map<int,std::shared_ptr<SEP::pick_orient>> orient_planes;
   std::list<int> orients;
   std::shared_ptr<SEP::pick_planes_new >planes;
   std::shared_ptr<SEP::pick_bucket>buck;
   std::map<QString,std::shared_ptr<SEP::pick_param>> all;
   QString draw_what;
   QString active_col;
   QString active_txt;
   QString font_name;

   int extra;
   int dist;
   int bulk;
   std::shared_ptr<SEP::position> myp;
   std::shared_ptr<SEP::my_colors> myc;
   std::shared_ptr<SEP::my_fonts > myf;
   float o[8],d[8];
   int dig[8];
   int n[8];
  std::shared_ptr<SEP::orientation_server>serv;
};

}
#endif




/*
picking();


  pairs_new get_pts_sort(int *iloc,int iax1, int iax2, int isort, int ival,QString col="None");
  pairs_new get_pts_sort(std::shared_ptr<orient_cube>pos,std::shared_ptr<slice>slc,int ival,int isort, QString col="None");
  pairs_new get_pts_sort_le(int *iloc,int iax1, int iax2, int isort, int ival,QString col="None");
   pairs_new get_pts_sort_le(std::shared_ptr<orient_cube>pos,int *iloc,int iax1, int iax2, int isort, int ival,QString col="None");
 pairs_new get_pts_sort_le(std::shared_ptr<orient_cube>pos,std::shared_ptr<slice>slc,int ival,int isort, QString col="None");
 std::vector<pick*> all_picks(std::shared_ptr<orient_cube>pos,int *iloc, int iax1, int iax2, QString col);
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

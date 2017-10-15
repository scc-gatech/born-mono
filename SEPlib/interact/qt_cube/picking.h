#ifndef PICKING_H
#define PICKING_H 1
#include "position.h"
#include "dataset.h"
#include "plane_contain.h"

class picking{
  public:

  picking();


  void set_position(position *pos);
  void draw(QPainter *painter, orient_cube *pos, slice *slc,dataset *dat);
  void create_planes();
  int get_dist(){return ndist;}
  void find_neighbors(int i1, int i2, int *ilocs, std::vector<pick_plane*>*neigh,
  std::vector<int> *dist);
  void set_dist(int id){ndist=id;}
  void increase_size();
  void decrease_size();
  void set_ative(QString name){ active_col=name;}
  void change_active(QString name){active_col=name;}
  void set_extra(int ex){ extra_state=ex;}
  int get_extra(){return extra_state;}

  QString get_draw_what(){return draw_what;}
  void set_draw_what(QString c){ draw_what=c;}
  QString get_active_name(){ return active_col;}
  int get_display(){return all.find(active_col)->second->get_display();}
  void set_display(int type){ all.find(active_col)->second->set_display(type);}
  bool get_changed(){ bool c=changed; changed=false; return c;}
  int get_single(){ return all.find(active_col)->second->get_single();}
  void set_single(int is);
  void add_pt(int *iloc, int type,QString col="None",int extra=-99);
    void add_pt(float *floc, int type,QString col="None",int extra=-99);

  void del_pt(orient_cube *pos, int *floc, int iax1, int iax2);
  void delete_picks(orient_cube *pos, int ival, QString col="None");
  void delete_pick_list(QString col, std::vector<long long> hashes);
  void ignore_axis(int iax){dig[iax]=0;}
  void float_to_int(const float *floc, int *iloc);
  void int_to_float(const int *iloc, float *floc);
  void read_file(const QString &text);
  void write_file(const QString &text);
  void delete_pick_vals(int ival, QString col="None"); 
  pick_plane  *find_plane(int i1, int i2,int *ilocs);
  void clear_picks(orient_cube *pos, int iax1 ,int iax2, QString col="None");
   plane_contain *find_plane_contain(int i1, int i2);
  pairs get_pts_sort(int *iloc,int iax1, int iax2, int isort, int ival,QString col="None");
  pairs get_pts_sort(orient_cube *pos,slice *slc,int ival,int isort, QString col="None");
  pairs get_pts_sort_le(int *iloc,int iax1, int iax2, int isort, int ival,QString col="None");
   pairs get_pts_sort_le(orient_cube *pos,int *iloc,int iax1, int iax2, int isort, int ival,QString col="None");

 pairs get_pts_sort_le(orient_cube *pos,slice *slc,int ival,int isort, QString col="None");
 std::vector<pick*> all_picks(orient_cube *pos,int *iloc, int iax1, int iax2, QString col);
 std::vector<pick*> all_picks(int *iloc, int iax1, int iax2, int ising,QString col);
 void shift_pick_window(orient_cube *pos,int *iloc, int iax1, int iax2,int *idelta);
 std::map<long long,pick> *return_all(QString col){return all[col]->return_all();}

 void clear_picks(int *iloc, int iax1 ,int iax2, QString col);
 void set_move(int m){ bulk=m;}
 int get_move(){ return bulk;}
 

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

#endif

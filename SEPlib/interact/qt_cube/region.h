#ifndef REGION_H
#define REGION_H
#include "hypercube.h"
#include "buffer.h"
#include "tmp_tree.h"
#include "my_colors.h"
#include "io_func.h"
#include "slice.h"
#include <QPainter>
#include <QPen>
class region_info: public tree_info{
  public:
  region_info(){set=false;}
  void set_val(int iv){ val=iv; set=true;}
  void del_val(){ set=false;}
  virtual region_info *clone(){assert(0==1);}
  virtual void verb(int iverb);
  virtual void set_not_branch(){set=false;}
  int val;
  bool set;
  
  
};
class region_tree: public io_func, public tmp_tree{
  public:
  region_tree(){}
  region_tree(hypercube *grid);
  bool same_loc(int *loc);
  bool different_loc(int *loc,std::vector<int> *lcs);
    bool update_cur();
  tmp_tree *clone();
   int get_num();
    void init_info();
    int return_oldest();

  region_info *inf2;
  int inum;
  int tot_num;

};
class region: public buffer{

  public:
    region(){}
    region(hypercube *grid);
    unsigned char *get_char_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
    float *get_float_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2);
    virtual float get_value(orient_cube *pos);
   void find_wind_branches(orient_cube *pos,int iax1, int f1, int e1, int iax2, int f2,
     int e2, std::vector<tmp_tree*> *branches);  
    void add_pt(int  *pos,int val,std::vector<int>ax);
    void add_pt(orient_cube *pos, int val,std::vector<int> ax){
      add_pt(pos->loc,val,ax);
    }
      virtual bool hold_point(float *pos,bool *use_axis){
        if(pos==0 && use_axis[0]) ; return true;}

    QString get_primary(){return primary_c;}
    QString get_second(){ return secondary_c;}
    void set_primary(QString col){
       primary_c=col;
       primary=colors.cnum[col];
    }
        void set_second(QString col){
       secondary_c=col;
       secondary=colors.cnum[col];
    }
    QString get_horiz_string(){
      if(_up==0) return "none";
      if(_up<0) return "down";
      return "up";
    
    }
        QString get_vert_string(){
      if(_left==0) return "none";
      if(_left<0) return "right";
      return "left";
    
    }
   void set_vert_string(QString col){
      if(col.contains("none")>0) _left=0;
       else if(col.contains("right")>0) _left=-1;
       else _left=1;
    
    }
       void set_horiz_string(QString col){
      if(col.contains("none")>0) _up=0;
       else if(col.contains("down")>0) _up=-1;
       else _up=1;
    
    }
    void  draw_pts(QPainter *painter, QPen *pen);
    
    void add_pt(int *pos,std::vector<int> ax){ add_pt(pos,primary,ax);}
    void del_pt(orient_cube*pos,bool update_it=true);
    void delete_region_tree(region_tree *nearest,bool update_it=true);
    void delete_range(orient_cube *pos, 
    int iax1, int b1, int e1, int iax2 ,int b2, int e2);
    void partition_region(orient_cube *pos, int iax1, std::vector<int> coord1,
  int iax2,std::vector<int> coord2);
   void form_continuous(std::vector<int> coord1,std::vector<int> coord2,
     std::vector<int> *use1,std::vector<int> *use2,int *beg1,int *beg2,int *end1,int *end2);
 void draw_region_pts(QPainter *painter, orient_cube *pos, slice *slice);
 void add_boundaries(int l1,int l2, int n1, int n2, int beg1, int beg2, int **map,
  int **sum ,std::vector<int>*ose1,  std::vector<int> *ose2);
    //void set_val(int v){cur_val=v;}
    void init_info();
    region_tree *tr;
    std::vector<int> build1,build2;
    int primary,secondary;
    QString primary_c,secondary_c;
    int _up,_left;
    my_colors colors;
    int inum;
  //  int return_val(){return cur_val;}

};

#endif 

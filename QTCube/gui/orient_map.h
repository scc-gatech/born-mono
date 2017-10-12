#include <math.h>
#include <map>
#include <vector>
#include "axis.h"
#include "pick_new.h"
#include "my_colors.h"
#ifndef ORIENT_MAP_H
#define ORIENT_MAP_H 1
namespace SEP{
class pick_holder{
  public:
    pick_holder(){;}
    void add_pick(long long hsh){ hash.push_back(hsh);}
    void del_pick(long long hsh);
    int return_size(){ return (int)hash.size();}
    ~pick_holder(){hash.clear();}
    std::vector<long long> hash;
    

};
class orient_map{
 public:
  orient_map(bool rot, int iax1,  int iax2,int *rot_ax,  std::vector<SEP::axis> ax_rot, 
     int **rot_to_reg_1,  int **rot_to_reg_2, int *beg,
     int *iloc, int *end,int *ns,bool r1, bool r2,std::vector<int> m1d,int sax,int bs, int es);

  void calc_get_pars(int iax1, int iax2, long long *j1, long long *j2,
  long long *first);
   orient_map(){map=0;}
 //  ~rot_map(){ sample_to_dat.clear();}
  void create_samp_dat_map();
  
  std::map<long long,int> *return_samp_dat_map(int *n1);
  std::map<long long,int> sample_to_dat;
  void form_index_map();
  void find_loc(int *iloc,int id1, int id2);
  long long *get_index_map_ptr();
  void orient_data_loc(int *cur,int *iloc);
  std::vector<int> return_picks_index(QString col);
  std::map<long long, int> *get_map_to_index(){ return &map_to_index;}
  ~orient_map(){ if(map!=0) delete [] map; map=0;  picks.clear(); map_to_index.clear();}
  void set_picks(std::shared_ptr<picks_vec>pks);
  void add_pick(QString col,long long index);
  void del_pick(QString col,long long index);
  bool check_same(int *iloc, int f1, int f2, int e1, int e2);
  void rotation_to_grid_loc(int *loc);
  void shift_data_image(float *iloc);
  bool samp_exist;
  long long *map;
  std::map<long long, int>  map_to_index;
  std::map<QString,pick_holder>  picks;
   int **rot_to_reg_1,**rot_to_reg_2;
   int beg[8],end[8],ns[8];
   SEP::axis ax_rot[2];
   int rot_ax[2];
   int iax1, iax2;
   int iloc[8];
   int **locs;
   int nsz;
   int n1,n2;
   std::vector<int> map_1d;
   int shift_ax;
   int b_s,e_s;
   long long skip[8];
   bool rotate;
   bool rev1,rev2;


};

}
#endif

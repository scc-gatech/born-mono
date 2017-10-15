#ifndef PLANE_CONTAIN_H
#define PLANE_CONTAIN_H 1
#include<pick_plane.h>
class plane_contain{
  public:
    plane_contain(int *n, int iax1, int iax2, std::map<QString,picks*> *all);
    void alloc_plane(int *n, int i1, int i2, std::map<QString,picks*> *all);
    void add_pt(QString col,int *iloc, long long hash);
    void del_pt(QString col, int *iloc, long long hash);
    void form_neighbors(int *iloc,int ndist, std::vector<pick_plane*> *neigh,
    std::vector<int> *dist);
    void compress(int *iloc, int iax1, int iax2, int *ipos);
     std::vector<long long> find_picks(QString col, int *iloc, int ising,
       orient_cube *pos=0);
   

    int nsz[6],nall[8];
    pick_plane *******tot;
    int iax1,iax2;

  

};
#endif

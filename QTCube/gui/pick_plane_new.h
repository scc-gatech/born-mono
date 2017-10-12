#ifndef PICK_PLANE_NEW_H
#define PICK_PLANE_NEW_H 1
#include "pick_new.h"
#include <map>
#include <memory>
#include "orientation_server.h"
#define MAX_PLANES 200
namespace SEP{
class pick_plane_new{
  public:
    pick_plane_new(){ ;}
    pick_plane_new(int *iloc,std::shared_ptr<SEP::picks_vec>pck);
    void del_pick(std::shared_ptr<SEP::pick_new >pk);
    void add_pick(std::shared_ptr<SEP::pick_new >pk);
    inline bool in_plane(int *iloc);
    bool same_plane(int *loc);
    std::shared_ptr<picks_vec>return_picks();

    ~pick_plane_new(){
        picks.clear();
    }
    std::set<std::shared_ptr<SEP::pick_new> > picks;
    int iloc[8];
};

class pick_planes_new{
  public:

    pick_planes_new(){}
    void add_pick(std::shared_ptr<SEP::pick_new>pk);
    void del_pick(std::shared_ptr<SEP::pick_new>pk);
    bool map_exists(int *iloc);
    std::shared_ptr<SEP::picks_vec>return_picks(int *iloc);
    void add_plane(int *iloc, std::shared_ptr<SEP::picks_vec>pck);
    void delete_pick_planes_new();
  private:
    std::list<SEP::pick_plane_new > planes;
};
}
#endif

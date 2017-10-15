#include "pick_plane_new.h"
#include <assert.h>
pick_plane_new::pick_plane_new(int *loc, picks_vec *pks){
 memcpy(iloc,loc,sizeof(int)*8);

 for(int i=0; i < pks->return_size(); i++)
   add_pick(pks->return_pick(i));

}
void pick_plane_new::del_pick(pick_new *pk){
   picks.erase(pk);
   fprintf(stderr,"new size is %d \n",(int)picks.size());
}
void pick_plane_new::add_pick(pick_new *pk){

   picks.insert(pk);

}
bool pick_plane_new::in_plane(int *il){
  if(iloc[0]!=il[0]&& iloc[0]!=-1) return false;
  if(iloc[1]!=il[1]&& iloc[1]!=-1) return false;
  if(iloc[2]!=il[2]&& iloc[2]!=-1) return false;
  if(iloc[3]!=il[3]&& iloc[3]!=-1) return false;
  if(iloc[4]!=il[4]&& iloc[4]!=-1) return false;
  if(iloc[5]!=il[5]&& iloc[5]!=-1) return false;
  if(iloc[6]!=il[6]&& iloc[6]!=-1) return false;
  if(iloc[7]!=il[7]&& iloc[7]!=-1) return false;
  return true;
}
void pick_planes_new::add_pick(pick_new *pk){
  for(std::list<pick_plane_new>:: iterator p=planes.begin(); p!=planes.end(); p++){
    if((*p).in_plane(pk->iloc)) {
      p->add_pick(pk);
      
      }
  }
}
void pick_planes_new::del_pick(pick_new *pk){
  for(std::list<pick_plane_new>:: iterator p=planes.begin(); p!=planes.end(); p++){
    if((*p).in_plane(pk->iloc)) {
       (*p).del_pick(pk);
    }
  }
}
void pick_planes_new::add_plane(int *iloc,picks_vec *pks){
  if(planes.size()==MAX_PLANES){
    if(planes.size()==MAX_PLANES){
       //Drop the last 10 
       for(int i=0; i< 10; i++) planes.pop_back();
    }
  }
  planes.push_front(pick_plane_new(iloc,pks)); 
}
void pick_planes_new::delete_pick_planes_new(){
   planes.clear();
}
bool pick_planes_new::map_exists( int *iloc){
  for(std::list<pick_plane_new>::iterator it=planes.begin(); it!=planes.end(); ++it){
    if(it->same_plane(iloc)) return true;
 }
 return false;

}
bool pick_plane_new::same_plane( int *loc){
   
  if(iloc[0]!=loc[0] && loc[0]!=-1) return false;
  if(iloc[1]!=loc[1] && loc[1]!=-1) return false;
  if(iloc[2]!=loc[2] && loc[2]!=-1) return false;
  if(iloc[3]!=loc[3] && loc[3]!=-1) return false;
  if(iloc[4]!=loc[4] && loc[4]!=-1) return false;
  if(iloc[5]!=loc[5] && loc[5]!=-1) return false;
  if(iloc[6]!=loc[6] && loc[6]!=-1) return false;
  if(iloc[7]!=loc[7] && loc[7]!=-1) return false;
  return true;

}    
picks_vec *pick_planes_new::return_picks(int *iloc){
     for(std::list<pick_plane_new>::iterator it=planes.begin(); it!=planes.end(); ++it){
        if(it->same_plane(iloc)) {
           return it->return_picks();
        }
    }
    assert(1==2);
    return (picks_vec *) 0;
}
picks_vec *pick_plane_new::return_picks(){
  picks_vec *p=new picks_vec();
  for(std::set< pick_new *>:: iterator pk=picks.begin(); pk!=picks.end(); ++pk){
     p->add_pick(*pk);
  }
  return p;
}

#ifndef PICK_BUCKET_H
#define PICK_BUCKET_H 1
#include <map>
#include <string>
#include <vector>
#include <set>
#include <QColor>
#include "position.h"
#include "pick_new.h"
#include "my_colors.h"
namespace SEP{
class pick_bucket{
  public:
   pick_bucket(){ setit=6;}
   pick_bucket(std::shared_ptr<position> pos);
   std::map<int,std::shared_ptr<picks_vec>>return_picks_from_range(int ip,std::vector<int> keys,int *iloc );
   std::shared_ptr<pick_new>add_pick(int *iloc,long long p, int te, QString col, int ex,QString txt);
   std::shared_ptr<pick_new>add_pick(int *iloc, int te, QString col, int ex,QString t){
     long long i=loc_to_index(iloc);
      return add_pick(iloc,i,te,col,ex,t);
   }
 long long loc_to_index(int *iloc){
   long long ind=0;
   for(int i=0; i < 8; i++){ 
    if(dig[i]) ind+=block[i]*iloc[i];
   }
   return ind;
 }   
void index_to_loc(long long ind,int *iloc){
   for(int i=7; i >=0; i--){ 
    if(dig[i])  {iloc[i]=ind/block[i]; ind-=block[i]*iloc[i];}
   }
 }   
 void print();
 std::shared_ptr<picks_vec  >return_pick_plane(int *iloc, QString col="None");
   void delete_bucket();
   std::shared_ptr<SEP::pick_new >get_pick(long long p, QString col);
   std::shared_ptr<SEP::picks_vec >picks_from_vec(std::vector<int> *bs, QString col);
   std::shared_ptr<SEP::picks_vec  >get_pick_type(int typ,QString col);
   void del_pick(long long p, QString col);
   long long find_nearest(long long p, int iax1, int iax2,QString col);
   inline int bucket_num(long long p);
   inline void get_index(long long p, int *index);
   int get_nbuckets(){return buckets.size();}
   std::shared_ptr<picks_new >get_bucket(int i){return buckets[i];}
   inline int bucket_num(int *index);
   void ignore_axis(int iax){ dig[iax]=false;}
    std::map<long long,std::shared_ptr<pick_new>> return_all(QString col);
    
    std::shared_ptr<picks_vec>return_all_picks(QString col);
    ~pick_bucket(){
    void delete_bucket();
  }
 private:
    int delta[8],nb[8];
    int block_b[8];
    int next[8];
    long long block[8];
    std::vector<std::shared_ptr<picks_new>> buckets;
    std::shared_ptr<position >myp;
    std::shared_ptr<my_colors >myc;
    bool dig[8];
    int setit;

};

}
#endif

#include "pick.h"

pick::pick(int *is,long long p, int te,int ex){
  memcpy(iloc,is,sizeof(int)*8);
  pos=p;
  type=te;
  extra=ex;
}
long long picks::form_key(int *iloc){
  long long key=iloc[0]*blk[0]+iloc[1]*blk[1]+iloc[2]*blk[2]+iloc[3]*blk[3]+
    iloc[4]*blk[4]+iloc[5]*blk[5]+iloc[6]*blk[6]+iloc[7]*blk[7];
  return key;
}
void picks::delete_pick(long long hash){
//fprintf(stderr,"ERASING %d \n",(int)hash);
  bucket2.erase(hash);
}
void picks::key_to_loc(long long hash, int *iloc){
    long long v=hash;
     for(int i=7; i >=0 ;i--) {
       iloc[i]=v/blk[i]; v=v-iloc[i]*blk[i];
      }
}
std::vector<long long> picks::find_picks(int ival){
  std::vector<long long> list;
  std::map<long long, pick>::const_iterator i;
  for ( i=bucket2.begin(); i !=bucket2.end();  ++i){
 // fprintf(stderr,"CHECK IT %d %d %d - %d \n",i->second.iloc[0],
//  i->second.iloc[1],i->second.iloc[2],i->second.type);
     if(ival==i->second.type) list.push_back(i->first);
  }
  //fprintf(stderr,"have %d found %d \n",
  //  (int)bucket2.size(),(int)list.size());
  return list;
}
std::vector<pick*> picks::all_picks(std::vector<long long> *list){
  std::vector<pick*> out;
  for(int i=0; i<(int)list->size(); i++){
     out.push_back(&bucket2[list->at(i)]);
   }
  return out;
}
long long picks::add_pick(int *iloc, int ty, int ex,bool *delt){
  long long hash=form_key(iloc);
  *delt=false;
  long long pos; loc_to_pos(iloc,&pos);
  if(bucket2.count(hash)==1) *delt=true;
 // fprintf(stderr,"adding point %d %d %d - %d (%d)\n",iloc[0],iloc[1],iloc[2],ty,
 // (int)hash);
  bucket2[pos]= pick(iloc,pos,ty,ex);
  return hash;
}
void picks::set_ns(int *ns){
  blk[0]=1;
  for(int i=1; i<8; i++) blk[i]=blk[i-1]*ns[i-1];
}
picks::picks(QColor c){

  single=0;
  display_type=DISPLAY_POINTS;
  sz=2;
  col=c;

}
void picks::loc_to_pos(int *loc, long long *pos){
  *pos=0;
  for(int i=0; i < 8; i++) *pos+=loc[i]*blk[i];
}
void picks::pos_to_loc(long long pos, int *loc){
  for(int i=7; i >=0; i--){
     loc[i]=pos/blk[i];
     pos-=loc[i]*blk[i];
   }
}

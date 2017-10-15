#include "pick_new.h"


pick_new::pick_new(int *loc,long long p, int te, QString c, int ex){
  pos=p;
  type=te;
  col=c;
  extra=ex;
  memcpy(iloc,loc,sizeof(int)*8);
}
 picks_vec *picks_new::get_parse_picks(QString col,int type,int extra){
 picks_vec *buck=new picks_vec();
 get_parse_picks(buck,col,type,extra);
 return buck;
}
picks_new::picks_new(position *pos){
setit=2;
myp=pos;
   bucket.clear();
}
void pick_new::pick_delete(){

}
void picks_new::get_parse_picks(picks_vec *buck,QString col,int typ,int extra){

  std::vector<pick_new*>:: iterator /*iend=buck->picks.end(),*/ it;
  int nend=buck->picks.size();
  for(int i=0; i < (int)bucket.size(); i++) buck->add_pick(bucket[i]);

  if(typ!=-10){
    for(int i=buck->return_size()-1;i>=nend;i--){
      pick_new *p=buck->return_pick(i);
      if(p->type!=typ){
        buck->picks.erase(buck->picks.begin()+i);
      }
    }
  }

  if(!col.contains("None")){
    for(int i=buck->return_size()-1;i>=nend;i--){
      pick_new *p=buck->return_pick(i);
      if(p->col!=col){
        buck->picks.erase(buck->picks.begin()+i);
      }
    }
  }   


  if(extra!=EXTRA_DEFAULT){
    for(int i=buck->return_size()-1;i>=nend;i--){
      pick_new *p=buck->return_pick(i);
      if(p->extra!=extra){
        buck->picks.erase(buck->picks.begin()+i);
      }
    }
  }

}
void pick_new::print(){
  fprintf(stderr,"%d %d %d %d %d %d %d %d = %lld \n",
    iloc[0],iloc[1],iloc[2],iloc[3],iloc[4],iloc[5],iloc[6],iloc[7],pos);
    fprintf(stderr,"COL=%s type=%d extra=%d \n",
      col.toAscii().constData(),type,extra);
   
}
pick_new *picks_new::get_pick(long long p,QString col){
   for(std::vector<pick_new*>::iterator it=bucket.begin(); it !=bucket.end(); ++it){
     if(p==(*it)->pos && (*it)->col.contains(col)){
        return (*it);
     }
   }
   return 0;
}
void picks_new::del_pick(long long p,  QString col){
   for(std::vector<pick_new*>::iterator it=bucket.begin(); it !=bucket.end(); ++it){
     if(p==(*it)->pos && (*it)->col.contains(col)){
        delete *it;
        bucket.erase(it);
        return;
     }
   }
}
void picks_new::print(int i){
  if((int)bucket.size()>0){
     fprintf(stderr,"total picks in bucket %d is %d \n",i,(int)bucket.size());
    for(int ii=0; ii < (int)bucket.size(); ii++) bucket[ii]->print();
  }


}
pick_new *picks_new::add_pick(int *iloc,long long p,int te, QString col,int ex){
     del_pick(p,col);  
     bucket.push_back(new pick_new(iloc,p,te,col,ex));
     pick_new *pp=bucket[bucket.size()-1];
     return pp;
  }
void picks_new::delete_pick_type(int typ){
  for(std::vector<pick_new*>::iterator it=bucket.begin(); it !=bucket.end(); ++it){
     if((*it)->type==typ) {
        delete  *it;
        bucket.erase(it);
        
        }
  }

}
void picks_new::delete_picks(){
  
  for(std::vector<pick_new*>::iterator it=bucket.begin(); it !=bucket.end(); ++it){
        delete  *it;
        bucket.erase(it);        
  }
bucket.clear();
}

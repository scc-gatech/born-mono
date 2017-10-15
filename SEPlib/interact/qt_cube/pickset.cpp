#include "pickset.h"


pairs pickset::subsample(slice *sc, bool *rot, int ilim){


  orient_cube *pos=sc->get_orient();
   int iax1=sc->iax1, iax2=sc->iax2;
   axis a1=pos->get_orient_axis(sc->iax1);
   axis a2=pos->get_orient_axis(sc->iax2);

   *rot=pos->get_rotate();
   //fprintf(stderr,"CHECK AXES %d %d \n",iax1,iax2);
  if(!*rot){

    int   b1=pos->get_beg(iax1), e1=pos->get_end(iax1); 
    int b2=pos->get_beg(iax2), e2=pos->get_end(iax2);
   // fprintf(stderr,"Sending to other %d %d %d %d %d %d \n",iax1,iax2,b1,b2,e1,e2);
    return subsample(a1,iax1,b1,e1,a2,iax2,b2,e2,ilim);
  }
 pairs out;
 if(all->bucket2.size()==0) return out;
   int n1;
   std::map<long long,int> *s_to_d=pos->get_rot_map(iax1,iax2,&n1);
  // std::map<int, long long> *d_to_s=&sc->dat_to_sample;
   int b,e;
  pos->get_axis_range(sc->iax1, &b, &e);
   //sc->return_maps(s_to_d,d_to_s);
// int n1=e-b;
 //  fprintf(stderr,"SZ 1 %d %d %d \n",(int)d_to_s->size(),
  // (int)s_to_d->size(),(int)all->bucket2.size());
   if(s_to_d->size() < all->bucket2.size()){
      std::map<long long, int>::const_iterator i;
       for ( i=s_to_d->begin(); i != s_to_d->end(); ++i){
         if(all->bucket2.count(i->first) >0){
            int iloc=i->second;
            int i2=iloc/n1, i1=iloc-n1*i2;
            out.add_point(i1*a1.d+a1.o,i2*a2.d+a2.o,all->bucket2[i->first].type,
               all->bucket2[i->first].extra);
         }
       }
   }
   else{
  
       std::map<long long,pick>::const_iterator i;
       for ( i=all->bucket2.begin(); i !=all->bucket2.end(); ++i){
         if(s_to_d->count(i->first) >0){
           int iloc=s_to_d->find(i->first)->second;
            int i2=iloc/n1, i1=iloc-n1*i2;
            out.add_point(i1*a1.d+a1.o,i2*a2.d+a2.o,i->second.type,i->second.extra);
         }
       }
   
   }
 
  return out;
}
pairs pickset::subsample(axis a1,int iax1,int beg1, int end1, axis a2, int iax2, int beg2,
int end2, int ilim){


  pairs out;
  int   b1=beg1, b2=beg2, e1=end1, e2=end2;
  if(end1<beg1) {b1=end1; e1=beg1;}
  if(end2<beg2) {b2=end2; e2=beg2;}
   float o1=a1.o,o2=a2.o,d1=a1.d,d2=a2.d;

  for(int i=0; i < (int)hash.size(); i++){
      pick *pk=&all->bucket2[hash[i]];
   
      if(pk->iloc[iax1] >= b1 && pk->iloc[iax1] <= e1 && pk->iloc[iax2] >= b2 && 
        pk->iloc[iax2]<=e2 &&(ilim ==-1 || ilim== pk->type)) {
          out.add_point(pk->iloc[iax1]*d1+o1,pk->iloc[iax2]*d2+o2,pk->type,pk->extra);


          }
    }
  return out;
}

 std::vector<long long> pickset::all_picks(orient_cube *pos, int iax1, int iax2){
       std::vector<long long> ret;
       
    bool rot=false;
    if(pos!=0) rot=pos->get_rotate();
  if(rot){
     bool first=true;
       int n1;
        std::map<long long,int> *s_to_d=pos->get_rot_map(iax1,iax2,&n1);

     long long dist;

   if(s_to_d->size() < all->bucket2.size()){
      std::map<long long, int>::const_iterator i;
       for ( i=s_to_d->begin(); i != s_to_d->end(); ++i){
         if(all->bucket2.count(i->first) >0) ret.push_back(i->first);
        
        }
   }
   else{
       std::map<long long,pick>::const_iterator i;
       for ( i=all->bucket2.begin(); i !=all->bucket2.end(); ++i){
         if(s_to_d->count(i->first) >0) ret.push_back(i->first);
       }
    }
  
  }
  else{
      for(int i=0; i < (int)hash.size(); i++) ret.push_back(hash[i]);
  }
 
      return ret;





}

void pickset::del_pt(long long hs){
  for(int i=0; i < (int)hash.size(); i++){
    if(hash[i]==hs){
       hash.erase(hash.begin()+i);
       return;
    }
  }
}
std::vector<long long> pickset::shift_pick_window(int *beg, int *end, int extra){
    std:: vector<long long> hashes;
    for(int i=0; i < (int)hash.size(); i++){
      long long hsh=hash[i];
      int il=0;
      bool valid=false;
      if(extra==all->bucket2[hsh].extra) valid=true;
      while(valid && il <8){
        int ip=all->bucket2[hsh].iloc[il];
        if(ip < beg[il] || ip>end[il]){
           valid=false;
           }
        il++;
      }
      if(valid) hashes.push_back(hsh);
  }
  return hashes;
}

long long pickset::find_closest(orient_cube *pos, int iax1, int iax2, int i1, int i2,int extra){  
  long long ret=-1;
  if(pos->get_rotate()){
     bool first=true;
       int n1;
        std::map<long long,int> *s_to_d=pos->get_rot_map(iax1,iax2,&n1);

     long long dist;

   if(s_to_d->size() < all->bucket2.size()){
      std::map<long long, int>::const_iterator i;
       for ( i=s_to_d->begin(); i != s_to_d->end(); ++i){
         if(all->bucket2.count(i->first) >0){
            int iloc=i->second;
            int i2=iloc/n1, i1=iloc-n1*i2;
            long long hsh=i->first;
            int dc=   (i1-all->bucket2[hsh].iloc[iax1])*(i1-all->bucket2[hsh].iloc[iax1])+
        (i2-all->bucket2[hsh].iloc[iax2])*(i2-all->bucket2[hsh].iloc[iax2]);
            if(dist < dc || first && (extra==i1-all->bucket2[hsh].extra || extra<0)) {
              dist=dc;
             ret=hsh;
              first=true;
            }
       
         }
       }
   }
   else{
       std::map<long long,pick>::const_iterator i;
       for ( i=all->bucket2.begin(); i !=all->bucket2.end(); ++i){
         if(s_to_d->count(i->first) >0){
         
           int iloc=s_to_d->find(i->first)->second;
            int i2=iloc/n1, i1=iloc-n1*i2;
           long long hsh=i->first;
            int dc=   (i1-all->bucket2[hsh].iloc[iax1])*(i1-all->bucket2[hsh].iloc[iax1])+
        (i2-all->bucket2[hsh].iloc[iax2])*(i2-all->bucket2[hsh].iloc[iax2]);
            if(dist < dc || first) {
              dist=dc;
             ret=hsh;
              first=true;
            }
         }
       }
   
   }
  
  }
  else{
    if((int)hash.size()==0) return -1;

    ret=hash[0];
    int dist=
     (i1-all->bucket2[ret].iloc[iax1])*(i1-all->bucket2[ret].iloc[iax1])+
     (i2-all->bucket2[ret].iloc[iax2])*(i2-all->bucket2[ret].iloc[iax2]);
    for(int i=1; i < (int)hash.size(); i++){
    long long hsh=hash[i];
      int dc=   (i1-all->bucket2[hsh].iloc[iax1])*(i1-all->bucket2[hsh].iloc[iax1])+
        (i2-all->bucket2[hsh].iloc[iax2])*(i2-all->bucket2[hsh].iloc[iax2]);
      if(dc < dist) {
         dist=dc;
         ret=hash[i];
      }
    }
  }
  return ret;


}

pairs pickset::get_pts_sort_le(orient_cube *pos,int iax1, int iax2,float *o, float *d,int ival,int isort){

 pairs out;
 bool rot=false;
 if(pos!=0) rot=pos->get_rotate();
 if(rot){
   int n1;
        std::map<long long,int> *s_to_d=pos->get_rot_map(iax1,iax2,&n1);
   if(s_to_d->size() < all->bucket2.size()){
      std::map<long long, int>::const_iterator i;
       for ( i=s_to_d->begin(); i != s_to_d->end(); ++i){
         if(all->bucket2.count(i->first) >0){
            int iloc=i->second;
            int i2=iloc/n1, i1=iloc-n1*i2;
            
            long long hsh=i->first;
            
             pick *pk=&all->bucket2[hsh];
   if(pk->type<=ival ) {
   out.add_point(
    i1*d[0]+o[0],i2*d[1]+o[1],pk->type,pk->extra);
  }

    
       
         }
       }
   }
   else{
       std::map<long long,pick>::const_iterator i;
       for ( i=all->bucket2.begin(); i !=all->bucket2.end(); ++i){
         if(s_to_d->count(i->first) >0){
         
           int iloc=s_to_d->find(i->first)->second;
           long long hsh=i->first;
                        int i2=iloc/n1, i1=iloc-n1*i2;

             pick *pk=&all->bucket2[hsh];
   if(pk->type<=ival ) {

   out.add_point(
     i1*d[0]+o[0],i2*d[1]+o[1],pk->type,pk->extra);
  }

         }
       }
   
   }
  
  }
 else{
 for(int i=0; i < (int)hash.size(); i++){
   pick *pk=&all->bucket2[hash[i]];
   if(pk->type<=ival ) {
   out.add_point(
     pk->iloc[iax1]*d[0]+o[0],pk->iloc[iax2]*d[1]+o[1],pk->type,pk->extra);
  }

}
}
  if(isort==iax1) out.sort1();
  else out.sort2();
  return out;

}
pairs pickset::get_pts_sort(orient_cube *pos,int iax1, int iax2,float *o, float *d,int ival,int isort){

 pairs out;
 bool rot=false;
 if(pos!=0) rot=pos->get_rotate();
 if(rot){
     int n1;
        std::map<long long,int> *s_to_d=pos->get_rot_map(iax1,iax2,&n1);
   if(s_to_d->size() < all->bucket2.size()){
      std::map<long long, int>::const_iterator i;
       for ( i=s_to_d->begin(); i != s_to_d->end(); ++i){
         if(all->bucket2.count(i->first) >0){
            int iloc=i->second;
            int i2=iloc/n1, i1=iloc-n1*i2;
            
            long long hsh=i->first;
            
             pick *pk=&all->bucket2[hsh];
   if(pk->type==ival || ival==-1) {
   out.add_point(
    i1*d[0]+o[0],i2*d[1]+o[1],pk->type,pk->extra);
  }

    
       
         }
       }
   }
   else{
       std::map<long long,pick>::const_iterator i;
       for ( i=all->bucket2.begin(); i !=all->bucket2.end(); ++i){
         if(s_to_d->count(i->first) >0){
         
           int iloc=s_to_d->find(i->first)->second;
           long long hsh=i->first;
                        int i2=iloc/n1, i1=iloc-n1*i2;

             pick *pk=&all->bucket2[hsh];
   if(pk->type==ival || ival==-1) {
   out.add_point(
     i1*d[0]+o[0],i2*d[1]+o[1],pk->type,pk->extra);
  }

         }
       }
   
   }
  
  }
 else{
 for(int i=0; i < (int)hash.size(); i++){
   pick *pk=&all->bucket2[hash[i]];
   if(pk->type<=ival ) {
   out.add_point(
     pk->iloc[iax1]*d[0]+o[0],pk->iloc[iax2]*d[1]+o[1],pk->type,pk->extra);
  }

}
}
  if(isort==iax1) out.sort1();
  else out.sort2();
  return out;

}


 #include "pick_bucket.h"
 #include <assert.h>
 #include <math.h>
 #define BUCKET_SIZE 20
 using namespace SEP;
 pick_bucket::pick_bucket(std::shared_ptr<position>pos){
   int n123=1;
   block[0]=1;
   block_b[0]=1;
   for(int i=0; i < 8; i++){
     dig[i]=true;
     
     next[i]=pos->getAxis(i).n;
     if(next[i]==1){ 
     nb[i]=1; delta[i]=1;}
     else{
       delta[i]=BUCKET_SIZE;
       nb[i]=(int)((ceilf((float)next[i]/(float)delta[i])));
     }

     if(i>0) {
       block[i]=block[i-1]*next[i-1];
       block_b[i]=block_b[i-1]*nb[i-1];
     }
     n123=n123*nb[i];
  }
  
  for(int i=0; i < n123; i++){
     std::shared_ptr<picks_new> x(new picks_new(pos));
      buckets.push_back(x);
    }
  myp=pos;
  std::shared_ptr<my_colors> c(new my_colors());
  myc=c;
 }



 std::map<int,std::shared_ptr<picks_vec> > pick_bucket::return_picks_from_range(int ip,std::vector<int> keys,int *iloc){
 
   std::vector<int> grab;
   //We may want to parallelize this
   int iax1=-1, iax2=-1;
   int n1=0,n2=0;
   int buck_base=0;  

  for(int i=0; i < 8; i++){ 
     if(iloc[i]==-1 && i!=ip){
       if(iax1==-1) {iax1=i; n1=nb[i];}
       else{
         assert(iax2==-1);
         iax2=i; n2=nb[i];
       }
      }
      else{
      if(ip!=i){
        buck_base+=block_b[i]*(int)(iloc[i]/BUCKET_SIZE);
      }
  }
  }
   for(std::vector<int>::iterator i=keys.begin(); i!=keys.end(); i++){
     for(int i2=0; i2<n2; i2++){
        int buck_use=buck_base+(int)((*i/BUCKET_SIZE))*block_b[ip]+i2*block_b[iax2];
        for(int i1=0; i1 < n1; i1++){

          grab.push_back(buck_use);
          buck_use+=block_b[iax1];
        }
    }
}

   std::map<int,std::shared_ptr<picks_vec>> pick_out;
   for(auto i=keys.begin(); i!=keys.end(); i++){
     std::shared_ptr<picks_vec> x(new picks_vec());
     pick_out[*i]=x;
   }
   std::shared_ptr<picks_vec>myp=picks_from_vec(&grab,"None");

  for(int i=0; i < myp->return_size(); i++){

    std::shared_ptr<pick_new> p=myp->return_pick(i);
    if(pick_out.count(p->iloc[ip])==1)   pick_out[p->iloc[ip]]->add_pick(p);

  }

  return pick_out;
}
std::shared_ptr<picks_vec>pick_bucket::return_pick_plane(int *iloc, QString col){

   int iax1=-1, iax2=-1,n1,n2;
   int buck_base=0;  
  std::vector<int> grab;
for(int i=0; i < 8; i++){ 
     if(iloc[i]==-1){
       if(iax1==-1) {iax1=i; n1=nb[0];}
       else{
         assert(iax2==-1);
         iax2=i; n2=nb[i];
      }
       buck_base+=block_b[i]/BUCKET_SIZE;
    }
}
    for(int i2=0; i2<n2; i2++){
        int buck_use=buck_base+i2*block_b[iax2];
        for(int i1=0; i1 < n1; i1++){
          grab.push_back(buck_use);
          buck_use+=block_b[iax1];
        }
    }
  std::shared_ptr<picks_vec>picks=picks_from_vec(&grab,col);
  return picks;
}
std::shared_ptr<pick_new >pick_bucket::add_pick(int *iloc,long long p, int te, QString col, int ex,QString t){
 
  std::shared_ptr<pick_new> pp=buckets[bucket_num(p)]->add_pick(iloc,p,te,col,ex,t);
  return pp;
}
void pick_bucket::print(){
int count=0;
  for(int i=0; i < (int)buckets.size(); i++){
//    buckets[i]->print(i);
    count+=buckets[i]->return_size();
 }
  fprintf(stderr,"total picks=%d \n",count);
}
std::shared_ptr<pick_new >pick_bucket::get_pick(long long p,QString col){
   return buckets[bucket_num(p)]->get_pick(p,col);


}
void pick_bucket::del_pick(long long p, QString col){
  buckets[bucket_num(p)]->del_pick(p,col);
}
long long pick_bucket::find_nearest(long long p, int iax1, int iax2, QString col){
  int index[8],send[8];
  get_index(p,index);
  std::vector<int> bs;
  memcpy(send,index,8*sizeof(int));
  bs.push_back(bucket_num(index));
  int b=BUCKET_SIZE;
  send[iax1]=std::max(0,index[iax1]-b); send[iax2]=std::max(index[iax2]-b,0); bs.push_back(bucket_num(send));
  send[iax2]=index[iax2]; bs.push_back(bucket_num(send));
  send[iax2]=std::min(next[iax2],index[iax2]+b); bs.push_back(bucket_num(send));
  send[iax1]=index[iax1]; bs.push_back(bucket_num(send));
  send[iax1]=std::min(next[iax1],index[iax1]+b); bs.push_back(bucket_num(send));
  send[iax2]=index[iax2]; bs.push_back(bucket_num(send));
  send[iax2]=std::max(0,index[iax2]-b); bs.push_back(bucket_num(send));
  send[iax1]=index[iax1]; bs.push_back(bucket_num(send));
  std::shared_ptr<picks_vec>picks=picks_from_vec(&bs,col);
  long long near=-1;
  int dmin=99999;
  int loc[8];
  
  for(int i=0; i< (int)picks->return_size(); i++){
    myp->index_to_loc(picks->return_pick(i)->pos,loc);
    bool valid=true;
    int j=0; 
    while( j < 8 && valid){
      if(iax1!=j && iax2!=j){
        if(loc[j]!=myp->get_loc(j)) valid=false;
      }
      j++;
    }
    if(valid){
      int dist=(myp->get_loc(iax1)-loc[iax1])*(myp->get_loc(iax1)-loc[iax1])+
         (myp->get_loc(iax2)-loc[iax2])*(myp->get_loc(iax2)-loc[iax2]);    
      if(dmin > dist){
         dmin=dist;
         near=picks->return_pick(i)->pos;
      }
    }
  }

}
std::shared_ptr<picks_vec>pick_bucket::picks_from_vec(std::vector<int> *bs, QString col){
  std::shared_ptr<picks_vec>picks(new picks_vec());
  //Probably want to parallelize this
  
  for(std::vector<int>::iterator it=bs->begin(); it !=bs->end() ; ++it){
    assert(*it < (int)buckets.size() && *it >=0);
    if(buckets[*it]->return_size()>0){
       buckets[*it]->get_parse_picks(picks,col);
    }
  }
  return picks;
}
inline void pick_bucket::get_index(long long p, int *index){
    #pragma loop unroll
    for(int i=7;i>=0;i--){
      index[i]=p/block[i]; 
      p-=index[i]*block[i];
      if(!dig[i]) index[i]=-1;
   }

}
int pick_bucket::bucket_num(long long p){
    int iout=0;
    #pragma loop unroll
    for(int i=7;i>=0;i--){
      int index=p/block[i]; 
      p-=index*block[i];
      if(!dig[i]) index=0;
      iout+=index/delta[i]*block_b[i];      
   //   fprintf(stderr,"INDEX CALC %lld %d %d %lld %d %d  \n",po,i,index,block[i],block_b[i],index/delta[i]*block_b[i]);
   }
   //assert(index!=1188000);
   return iout;
}
int pick_bucket::bucket_num(int *index){
   int iout=0;
    #pragma loop unroll
    for(int i=7;i>=0;i--){
      iout+=index[i]/delta[i]*block_b[i];
   }
   return iout;
}
void  pick_bucket::delete_bucket(){
      
      buckets.clear();
   }
 std::shared_ptr<picks_vec>pick_bucket::get_pick_type(int typ,QString col){
  std::shared_ptr<picks_vec>pk(new picks_vec());
  for(int i=0; i <(int) buckets.size(); i++) buckets[i]->get_parse_picks(pk,col,typ);
  return pk;
}
std::map<long long,std::shared_ptr<pick_new>> pick_bucket::return_all(QString col){
  std::map<long long, std::shared_ptr<pick_new>> mp;

  for(int i=0; i< (int) buckets.size(); i++){
    std::shared_ptr<picks_vec>b(new picks_vec());
    buckets[i]->get_parse_picks(b,col);

    for(int j=0; j < (int)b->return_size(); j++){
      mp[b->return_pick(j)->pos]=b->return_pick(j);
    }
  }
  return mp;
} 
std::shared_ptr<picks_vec>pick_bucket::return_all_picks(QString col){
  std::shared_ptr<picks_vec>p(new picks_vec());
  for(int i=0; i< (int) buckets.size(); i++){
    buckets[i]->get_parse_picks(p,col);
  }
  return p;
} 

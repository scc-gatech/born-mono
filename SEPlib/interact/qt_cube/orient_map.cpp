#include "orient_map.h"
#include <math.h>
#include <utility>
#include <string.h>
#include "seplib.h"
#include "assert.h"



orient_map::orient_map( bool rot, int i_ax1,  int i_ax2, int *rotax,  axis *axrot, 
     int **rr1,  int **rr2, int *begs,
     int *i_loc, int *ends,  int *n_s,bool r1, bool r2,int *mp, int sa,int bs,
     int es){

   map=0;
   map_1d=mp;
   shift_ax=sa;
   rev1=r1;rev2=r2;
   memcpy(beg,begs,8*sizeof(int));
   memcpy(end,ends,8*sizeof(int));
   memcpy(ns,n_s,8*sizeof(int));
   memcpy(iloc,i_loc,8*sizeof(int));
   rotate=rot;
   skip[0]=1;
   for(int i=0; i <7;i++){
     skip[i+1]=skip[i]*ns[i];
     
   }
    b_s=bs; e_s=es;
    iax1=i_ax1;
   iax2=i_ax2;
 
   rot_to_reg_1=rr1;
   rot_to_reg_2=rr2;
   for(int i=0; i < 2; i++){
     rot_ax[i]=rotax[i];
     ax_rot[i].n=axrot[i].n;
     ax_rot[i].o=axrot[i].o;
     ax_rot[i].d=axrot[i].d;
   }
   map=0;
  samp_exist=0;
 }
std::vector<int> orient_map::return_picks_index(QString col){
  std::vector<int> out;
  int n=picks[col].return_size();
  for(int i=0; i< n; i++){
    out.push_back(map_to_index[picks[col].hash[i]]);
  }
  return out;

}
void orient_map::set_picks(picks_vec *pks){
  for(int i=0; i < pks->return_size(); i++){
    pick_new *p=pks->return_pick(i);
    picks[p->col].add_pick(p->pos);
  }
}
void orient_map::add_pick(QString col, long long index){
  
  picks[col].add_pick(index);

}
void orient_map::del_pick(QString col, long long index){
  picks[col].del_pick(index);
}
void pick_holder::del_pick(long long index){
  std::vector<long long>::iterator i=hash.begin();
 while(i !=hash.end() && *i!=index) i++;
  if(i==hash.end()){
    fprintf(stderr,"internal error: attempt to delete pick that doesn't exist");
    assert(1==2);
  }
 hash.erase(i);

}
long long *orient_map::get_index_map_ptr(){
  if(map==0) form_index_map();
  return map;
}
std::map<long long,int> *orient_map::return_samp_dat_map(int *n){

if(!samp_exist) create_samp_dat_map();

*n=n1;
return &sample_to_dat;

}
void orient_map::create_samp_dat_map(){


 samp_exist=true;
// if(!rotate) return;


if(1==1){
  for(int i2=0; i2 < n2; i2++){
    for(int i1=0; i1 < n1; i1++){
      if(rev1){
        if(rev2) sample_to_dat[map[i1+i2*n1]]=(n2-i2-1)*n1+n1-i1-1;
        else sample_to_dat[map[i1+i2*n1]]=i2*n1+n1-i1-1;
      }
      else{
        if(rev2)  sample_to_dat[map[i1+i2*n1]]=i1+(n2-i2-1)*n1;
        else sample_to_dat[map[i1+i2*n1]]=i1+i2*n1;
    }
  }
 }

}
if(1==3){

 
    
 long long blks[8];

 blks[0]=1;

 long long add[8];
 add[0]=iloc[0];
 for(int i=1; i < 8; i++) {
   blks[i]=blks[i-1]*ns[i-1];
   add[i]=iloc[i]*blks[i];
 }

int b1=beg[iax1],e1=end[iax1],b2=beg[iax2],e2=end[iax2];

int n1=abs(e1-b1);
/*
typedef std::pair<long long,int>s_d_p;
typedef std::pair<int, long long> d_s_p;
*/

if( (iax1 ==rot_ax[0] && iax2==rot_ax[1])  || (iax1==rot_ax[1] && iax2==rot_ax[0])){

  int ia,ib;
  int *i_1,*i_2,i1,i2,r1,r2;
  int b_1,b_2,e_1,e_2;
  i1=1; r1=-1; i2=10; r2=-10;
  if(iax1==rot_ax[0]){
     ia=iax1;
     ib=iax2;
     if(rev1) i_1=&r1;
     else   i_1=&i1;
     if(rev2) i_2=&r2;
     else i_2=&i2;
     
     b_1=b1; b_2=b2;e_2=e2;e_1=e1;
  }
  else{
     b_1=b2; b_2=b1;e_2=e1;e_1=e2;
    ia=iax2; ib=iax1;
    if(rev1) i_1=&r2;
    else i_1=&i2;
    if(rev2) i_2=&r1;
    else i_2=&i1;
//    i_1=&i2; i_2=&i1;
  }
  for(i2=0; i2 < ax_rot[1].n; i2++){
     r2=ax_rot[1].n-i2-1;
    for(i1=0; i1 < ax_rot[0].n; i1++){
       r1=ax_rot[0].n-i1-1;

      if(rot_to_reg_2[i2][i1] >= b_2 && rot_to_reg_2[i2][i1] < e_2 &&
        rot_to_reg_1[i2][i1] >= b_1 && rot_to_reg_1[i2][i1] < e_1){

       add[ib]=rot_to_reg_2[i2][i1]*blks[ib];
       add[ia]=rot_to_reg_1[i2][i1]*blks[ia];
       long long v1=add[0]+add[1]+add[2]+add[3]+add[4]+add[5]+add[6]+add[7];
       int pos=*i_1+n1*(*i_2);         
       sample_to_dat[v1]=pos;               

    }
   
    }
  }

 }
 
 else if(iax1==rot_ax[0]){ //Rotated first axis= first axis

 int irot2=iloc[rot_ax[1]],b_1=b1,e_1=e1,b_2=beg[rot_ax[1]],e_2=end[rot_ax[1]];
   for(int ireg2=b2; ireg2 < e2; ireg2++){
     for(int i1=0; i1 < ax_rot[0].n; i1++){
       int ireg1=rot_to_reg_1[irot2][i1];
       int irege=rot_to_reg_2[irot2][i1];
       if(ireg1 >= b_1 && ireg1< e_1 && irege>=b_2 && irege<e_2){
         add[iax1]=ireg1*blks[iax1];
         add[iax2]=ireg2*blks[iax2];
         add[rot_ax[1]]=irege*blks[rot_ax[1]];
         long long v1=add[0]+add[1]+add[2]+add[3]+add[4]+add[5]+add[6]+add[7];
         int pos=i1+n1*ireg2;
         sample_to_dat[v1]=pos;
       }
     }
   }
 }
 else if(iax1==rot_ax[1]){ //Rotated second axis= first axis

 int irot1=iloc[rot_ax[0]],b_1=b1,e_1=e1,b_2=beg[rot_ax[0]], e_2=end[rot_ax[0]];
   for(int ireg2=b2; ireg2 < e2; ireg2++){
     for(int i1=0; i1 < ax_rot[1].n; i1++){
       int irege=rot_to_reg_1[i1][irot1];
       int ireg1=rot_to_reg_2[i1][irot1];
       if(ireg1 >= b_1 && ireg1< e_1 && irege>=b_2 && irege<e_2){
         add[iax1]=ireg1*blks[iax1];
         add[iax2]=ireg2*blks[iax2];
         add[rot_ax[0]]=irege*blks[rot_ax[0]];
         long long v1=add[0]+add[1]+add[2]+add[3]+add[4]+add[5]+add[6]+add[7];
         int pos=i1+n1*i1;
         sample_to_dat[v1]=pos;
       }
     }
   }
 }
 else if(iax2==rot_ax[0]){ //Rotated first axis= second axis

 int irot2=iloc[rot_ax[1]],b_2=b2,e_2=e2,b_1=beg[rot_ax[1]], e_1=end[rot_ax[1]];
  for(int i2=0; i2 < ax_rot[0].n; i2++){
    for(int ireg1=b1; ireg1 < e1; ireg1++){
       int ireg2=rot_to_reg_1[irot2][i2];
       int irege=rot_to_reg_2[irot2][i2];
       if(ireg2 >= b_2 && ireg2< e_2 && irege>=b_1 && irege<e_1){
         add[iax1]=ireg1*blks[iax1];
         add[iax2]=ireg2*blks[iax2];
         add[rot_ax[1]]=irege*blks[rot_ax[1]];
         long long v1=add[0]+add[1]+add[2]+add[3]+add[4]+add[5]+add[6]+add[7];
         int pos=ireg1+n1*i2;
         sample_to_dat[v1]=pos;
       }
     }
   }
 }
 else if(iax2==rot_ax[1]){ //Rotated second axis= second axis

 int irot1=iloc[rot_ax[0]],b_2=b2,e_2=e2,b_1=beg[rot_ax[0]], e_1=end[rot_ax[0]];
  for(int i2=0; i2 < ax_rot[1].n; i2++){
    for(int ireg1=b1; ireg1 < e1; ireg1++){
       int irege=rot_to_reg_1[i2][irot1];
       int ireg2=rot_to_reg_2[i2][irot1];
       if(ireg2 >= b_2 && ireg2< e_2 && irege>=b_1 && irege<e_1){
         add[iax1]=ireg1*blks[iax1];
         add[iax2]=ireg2*blks[iax2];
         add[rot_ax[0]]=irege*blks[rot_ax[0]];
         long long v1=add[0]+add[1]+add[2]+add[3]+add[4]+add[5]+add[6]+add[7];
         int pos=ireg1+n1*i2;
         sample_to_dat[v1]=pos;
       }
     }
   }
 } 
}

}
void orient_map::find_loc(int *iloc,int id1, int id2){


 long long v=map[id1+id2*n1];
  iloc[7]=(int)(v/skip[7]); v=v-skip[7]*iloc[7];
  iloc[6]=(int)(v/skip[6]); v=v-skip[6]*iloc[6];
  iloc[5]=(int)(v/skip[5]); v=v-skip[5]*iloc[5];
  iloc[4]=(int)(v/skip[4]); v=v-skip[4]*iloc[4];
  iloc[3]=(int)(v/skip[3]); v=v-skip[3]*iloc[3];
  iloc[2]=(int)(v/skip[2]); v=v-skip[2]*iloc[2];
  iloc[1]=(int)(v/skip[1]); 
  iloc[0]=(int)(v-skip[1]*iloc[1]);

  if(map_1d!=0){
    iloc[shift_ax]=map_1d[map[id1+id2*n1]];
  
  }
  
  //if(map_1d!=0){
   // iloc[iax_1d]=map_1d[iloc[iax1]+iloc[iax2]*n1];
  // }
}
bool orient_map::check_same(int *i_loc, int f1, int e1, int f2, int e2){
   for(int i=0; i < 8; i++){
     if(i==iax1){
        if(f1!=beg[iax1] || e1!=end[iax1]){
          return false;
          
          }
     }
     else if(i==iax2){
             if(f2!=beg[iax2] || e2!=end[iax2]) {

       return false;
         
         }
     }
     else if(iloc[i]!=i_loc[i]) {
       return false;
       }
  }
  return true;
  
}
void orient_map::form_index_map(){
 
 
  int f1=beg[iax1],e1=end[iax1];
  int f2=beg[iax2],e2=end[iax2];


  
  long long j1,j2,jax1=0,jax2=0;
  long long first;
  calc_get_pars(iax1,iax2,&j1,&j2,&first);
   n1=abs(e1-f1);
   n2=abs(e2-f2);
  nsz=n1*n2;

     int dir1=1,dir2=1;
  if(rev1){
      dir1=f1; f1=e1-1; e1=dir1; dir1=-1;
   }
   if(rev2){
     dir2=f1; f2=e2-1; e2=dir2; dir2=-1;
   }
  // if(f1>e1) {dir1=-1; f1-=1;}
  // if(f2>e2) {dir2=-1; f2-=1;}
   map=new long long [n1*n2];
  int i=0;
  //Impiclict assumption that the rotated axis is held....
 if(!rotate){
   /*int ilast;*/

     for(int i2=0; i2 < n2; i2++){
          for(int i1=0; i1 < n1; i1++,i++){
            /*ilast=*/map[i]=(dir1*i1+f1)*j1+(dir2*i2+f2)*j2+first;
            
            
          }
        }
 

 }
 else{
 /*
    if(rev1){
      dir1=f1; f1=e1-1; e1=dir1; dir1=-1;
   }
   if(rev2){
      dir2=f1; f2=e2-1; e2=dir2; dir2=-1;
   }
   */
   int **m1=rot_to_reg_1,**m2=rot_to_reg_2;
   int ia1,ia2;
   if(iax1 < iax2){ ia1=iax1; ia2=iax2; }
   else { ia1=iax2; ia2=iax1;}
   int b1=beg[rot_ax[0]],b2=beg[rot_ax[1]],e1=end[rot_ax[0]],e2=end[rot_ax[1]];

   if(ia1==rot_ax[0] && ia2==rot_ax[1]){
   
    
     int **t;
     if(iax1>iax2){ t=m1; m1=m2; m1=t;}
     if(ia1==iax1){
        for(int i2=0; i2 < n2; i2++){
          for(int i1=0; i1 < n1; i1++,i++){
            int ib=m2[i2*dir2+f2][i1*dir1+f1];
            int ia=m1[dir2*i2+f2][dir1*i1+f1];
            
            if(ia<b1 || ia>= e1 || ib <b2 || ib>= e2) map[i]=-1;
            else{
              int i_2=ib;
              int i_1=ia;
               map[i]=i_1*j1+i_2*j2+first;   
            }
          }
         
      }
      }
      else{
        for(int i2=0; i2 < n2; i2++){
          for(int i1=0; i1 < n1; i1++,i++){
            int ib=m1[i1*dir1+f1][i2*dir2+f2];
            int ia=m2[i1*dir1+f1][i2*dir2+f2];
        //    if(ib<b1 || ib>= e1 || ia <b2 || ia>= e2) map[i]=-1;           
          //  else{
              int i_2=ib;
              int i_1=ia;
               map[i]=i_1*j1+i_2*j2+first;   

          //  }
          }
         
      }
      
      
      }
      }
      else{
        first=0;
        for(int iax=0; iax < 8; iax++){
         if(iax==rot_ax[0]) jax1=skip[iax];
         else if(iax==rot_ax[1]) jax2=skip[iax];
         else if(iax2==iax) j2=skip[iax];
         else if(iax1==iax) j1=skip[iax];
         else{

               first=first+skip[iax]*iloc[iax];

            
          }
        }
        if(iax1==rot_ax[0] || iax1==rot_ax[1]){
        if(iax1==rot_ax[0]){
             int   ir1=iloc[rot_ax[1]];
      //       int   ioth=rot_ax[1];
          
            for(int i2=0; i2 < n2; i2++){
              for(int i1=0; i1 < n1; i1++,i++){
                int ib=m2[ir1][i1*dir1+f1];
                int ia=m1[ir1][i1*dir1+f1];
                if(ia<b1 || ia>= e1 || ib < b2 || ib>=e2) {
                
                   map[i]=-1;
                   }
                else{
                 int i_1=ia;
                 int i_2=i2*dir2+f2;
                 int i_3=ib;
                 map[i]=first+jax1*i_1+j2*i_2+i_3*jax2;
                }
              }
            }
          }
          else{
         
             int  ir1=iloc[rot_ax[0]];
         //    int  ioth=rot_ax[0];
            
        
            for(int i2=0; i2 < n2; i2++){
              for(int i1=0; i1 < n1; i1++,i++){
                int ib=m2[i1*dir1+f1][ir1];
                int ia= m1[i1*dir1+f1][ir1];
              if(ia<b1 || ia>= e1 || ib < b2 || ib>=e2) map[i]=-1;
                else{
                 int i_1=ib;
                 int i_2=i2*dir2+f2;
                 int i_3=ia;
                 map[i]=first+jax2*i_1+i_2*j2+i_3*jax1;
                }
              }
            }
          
       }
      
          
       }
       else{
          if(iax2==rot_ax[0]){
             int   ir1=iloc[rot_ax[1]];
      //       int   ioth=rot_ax[1];
            for(int i2=0; i2 < n2; i2++){
              for(int i1=0; i1 < n1; i1++,i++){
               int ia=m1[ir1][i2*dir2+f2];
               int ib=m2[ir1][i2*dir2+f2];
               if(ia<b1 || ia>= e1 || ib < b2 || ib>=e2) map[i]=-1;
                else{
                 int i_2=ia;
                 int i_1=i1*dir1+f1;
                 int i_3=ib;
                 map[i]=first+i_1*j1+i_2*jax1+i_3*jax2;
                }
               
              }
            }
          }
          else{
             int  ir1=iloc[rot_ax[0]];
         //    int  ioth=rot_ax[0];
            for(int i2=0; i2 < n2; i2++){
              for(int i1=0; i1 < n1; i1++,i++){
                int ia=m1[i2*dir2+f2][ir1];
                int ib=m2[i2*dir2+f2][ir1];
                if(ia<b1 || ia>= e1 || ib < b2 || ib>=e2) map[i]=-1;
                else{
                 int i_2=ib;
                 int i_1=i1*dir1+f1;
                 int i_3=ia;
                 map[i]=first+i_1*j1+i_2*jax2+i_3*jax1;
                }
              }
            }
          
       }
      
       
       }
       
       }
      
  }
  if(map_1d!=0){


    for(int i=0; i < n1*n2; i++){ 
      if(map[i]>=0){
       
        if(map_1d[map[i]] < b_s || map_1d[map[i]] >= e_s) map[i]=-1;
        //if(i%n1 == n1-1 && iax1==0 && iax2==1) fprintf(stderr,"EDGE %d %d %d %d \n",
         // b_s,e_s,
          //    i%n1,map_1d[map[i]]);
      }
    }
    for(int i=0; i < n1*n2;i++) map_to_index[map[i]]=i;
  }
  //return map;
  }
void orient_map::rotation_to_grid_loc(int *loc){

   if(map_1d==0) return;
   if(iax1!=0 && iax2!=0) return;
   long long ib=loc[1]*skip[1]+loc[2]*skip[2]+loc[3]*skip[3]+loc[4]*skip[4]+loc[5]*skip[5]+loc[6]*skip[7]+loc[7]*skip[7];
   for(int i=0; i  < ns[0]-1; i++,ib++){
     if(map_1d[ib] <=  loc[0] && map_1d[ib+1]>=loc[0]) {
       if(map_1d[ib+1]-loc[0] > loc[0]-map_1d[ib]){
         loc[0]=i;
       }
       else{
          loc[0]=i+1;
   }
       return;
    }
}
       
}
void orient_map::orient_data_loc(int *cur,int *ipos){
     
   
    if(map_1d!=0) {
       int i1=ipos[iax1]-beg[iax1];
       int i2=ipos[iax2]-beg[iax2];
       int i_1=cur[iax1]-beg[iax1];
       int i_2=cur[iax2]-beg[iax2];
       if(rev1) {i_1=n1-i_1-1;i1=n1-i1-1;}
       if(rev2) {i_2=n2-i_2-1;i2=n2-i2-1;}
  
     
       if(iax1 ==0 || iax2==0 || 0==0)       ipos[shift_ax]=map_1d[map[i1+i2*n1]];
       else{ 
         //Figure out our current position in the flattened space
        //int ifind=cur[0]; //Current position in un-flattend space
        //long long ibef=map[i1+i2*n1]-cur[0]; //The location in unflattened space at z=0
       
         //Shift along flattened space to update position
       
       /*
          //Neeed to make sure that we end up at the same flatened time locaiton
          //Find the same flattend place???
          int ifind=map_1d[map[i_1+i_2*n1]];
          int ibeg=map[i1+i2*n1]-cur[0]; //Begining of the trace
          int i=1; int ibef=map_1d[ibeg];
          

          if(map_1d[ibef] > ifind){
            ipos[0]=0;
            fprintf(stderr,"returning begin \n");
            return;
          }
          int iend;
          while(i < ns[0]){
            iend=map_1d[ibeg+i];
            if(iend > ifind){
               fprintf(stderr,"returning in the middle %d %d %d %d\n",i,ibef,iend,ifind);
               if(iend-ifind < ifind-ibef) ipos[0]=iend;
               else ipos[0]=ibef;
               return;
            }
            ibef=iend;
            i++;
          }
          ipos[0]=iend;
          return;
       }
       */
    }
}
}
void orient_map::shift_data_image(float *pct){

 if(shift_ax<0 || (shift_ax!=iax1 && shift_ax!=iax2)) return;
int i1a=std::max(0,std::min(n1-1,(int)(pct[0]*n1)));
int i2a=std::max(0,std::min(n2-1,(int)(pct[1]*n2)));
if(map[i1a+i2a*n1]==-1) return;
int inew=map_1d[map[i1a+i2a*n1]];
if(iax1==shift_ax){
   pct[0]=(float)(inew-beg[0])/(float)(end[0]-beg[0]);
   if(rev1) pct[0]=1.-pct[0];
}
else{
  pct[1]=(float)(inew-beg[0])/(float)(end[0]-beg[0]);
     if(rev2) pct[1]=1.-pct[1];

}
return;


int iuse=1/*,iother=0*/;
if(iax1==shift_ax) {iuse=0;/*iother=1;*/}

 int i1=(int)(pct[0]*n1),i2=(int)(pct[1]*n2);
 if(i1>=n1) i1=n1-1;
 if(i2>=n2) i2=n2-1;
 if(shift_ax==iax1){
 int ibeg=i2*n1;
 
 //Find the valid (not -1 portion of the map at this location in i2,i3,...
 while(map[ibeg] < 0 && ibeg< i2*n1+n1-1) ibeg++; 
 if(ibeg==i2*n1+n1-1) return;
 int iend=i2*n1+n1-1;
 while(map[iend]<0 && iend >i2*n1) iend--;
   int bef=map_1d[map[ibeg]];
   int enf=map_1d[map[iend]];

 if(pct[iuse]>.99999) return;

//If the map is positive direction
 if(bef<enf){  

   float ilook=pct[iax1]*n1+beg[iax1]; //This is the value we are looking for
    if(bef >= ilook) { //If the value is before what we have in our map (shouldn't happen) return boundary 
       pct[iuse]=.01;
       return;
     }
    
     
    if(1==3){
     for(int i=ibeg+1; i < iend; i++){
       int end=map_1d[map[i]];
       if(ilook >=bef && ilook < end){
         pct[0]=(float)((ilook-bef)/(float)(end-bef)+
            (float)(i-i2*n1))/(float)n1;
         return;
       }
       }
      bef=enf;
     }
     pct[0]=.9999;
  }
  
  //Map in negative direction
  else{

   if(pct[0]>.9999){
     pct[0]=.999; return;
   }
   float ilook=(1.-pct[0])*n1+beg[iax1]; //look for
   if(enf >= ilook){
     pct[0]=.01;
     return;
   }
   for(int i=iend-1; i >=ibeg ; i--){
      int  beg=map_1d[map[i]];
      if(ilook >= enf && ilook < beg){
        pct[0]=(float)((ilook-enf)/(float)(beg-enf)+(float)(i-i2*n1))/(float)n1;
        return;
       }    
       enf=beg;
    } 
    pct[0]=.9999;

  }

  return;
 }
   float ilook=pct[1]*n2+beg[iax2]; //look for
 //  int bef=(map[i1]-(int)(map[i1]/
 //     skip[iax2+1])*skip[iax2+1])/skip[iax2];
 int bef=map_1d[map[i1]];
   if(bef >= ilook) {
     pct[1]=0.01;
     return;
   }
   for(int i=1; i < n2; i++){
  //   int end=(map[i1+n1*i]-
 //      (int)(map[i1+n1*i]/skip[iax2+1])*skip[iax2+1])/skip[iax2];
  int end=map_1d[map[i1+n1*i]];
  if(ilook >=bef && ilook < end){
        pct[1]=(float)((ilook-bef)/(float)(end-bef)+(float)i)/(float)n2;
      
        return;
     }
     bef=end;
   }
   pct[1]=.9999;

   return;

}
void orient_map::calc_get_pars(int iax1, int iax2, long long *j1, long long *j2, long long *first){
 *first=0;
 


 
 long long ipos=0;
 for(int i=0; i < 8; i++){
   if(i>0)ipos=ipos+skip[i]*iloc[i];
   if(i==iax1) *j1=skip[i];
   else if(i==iax2) *j2=skip[i];
   else{
       *first=*first+
         skip[i]*
          iloc[i];
   }
  }
  if(map_1d!=0 && iax1!=0 && iax2!=0){
    int iuse=0,iold;
    if(map_1d[ipos]>= iloc[0])iuse=0;
    else{
     iold=map_1d[ipos];
     int i=1;
     bool found=false;
     int n1=ns[0];
     while(i< n1 -1 && !found){
       if(map_1d[ipos+i] > iloc[0]){
        found=true;
        if(map_1d[ipos+i]-iloc[0] < iloc[0]-iold) iuse=i;
        else iuse=i-1;
       }
       iold=map_1d[ipos+i];
       i++;
     }
     if(!found) {
       iuse=n1-1;
     }
    }

    *first=*first-skip[0]*iloc[0]+skip[0]*iuse;
  
  }
  
 }

 

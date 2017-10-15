#include "buffer.h"
#include <iostream>
#include <stdlib.h>

void buffer::set_basics(util *p,hypercube *h,io_func *i,int in){
  par=p;
  hyper_buf=h;
  io=i;
  hyper_io=io->return_hyper();
  std::vector<axis> aios =hyper_io->return_axes(8);
  std::vector<axis> abufs =hyper_buf->return_axes(8);

   
  
  if(hyper_io->same_size(hyper_buf)) same=true;
 else{
   
    same=false;
 }
     aios =hyper_io->return_axes(8);
     abufs =hyper_buf->return_axes(8);

  
    ii=new int*[8];

    resamp_1=true;
    for(int i=0; i < 8; i++){
      ii[i]=new int[abufs[i].n];
      build_axis_map(aios[i],abufs[i],i);
      if(i>0 && aios[i].n != abufs[i].n) resamp_1=false;
      
    }
      
  
  inum=in;
  
}
void buffer::build_axis_map(axis aio, axis abuf, int iax){

  for(int i=0; i < abuf.n; i++){
    float pout=i*abuf.d+abuf.o;
    float fin=(int)((pout-aio.o)/aio.d+.5);
    if(fin < 0.){
      ii[iax][i]=0; 
    }
    else if(fin>=aio.n-1){
      ii[iax][i]=aio.n-1;
    }
    else{
      ii[iax][i]=(int) fin; 
    }
    //fprintf(stderr,"constructing axis map %d %d %d \n",iax,i,ii[iax][i]);
  }

}
bool buffer::hold_slice(orient_cube *cube, int ax1, int ax2,bool *data_contains){

  if(io->changed) return false;

  if(!hold[ax1] || !hold[ax2]) return false;

  for(int i=0; i < 8; i++){


    if(!hold[i] && loc[i] != cube->get_loc(i) && data_contains[i]) {

     return false;
    }
  }
  return true;
}
void buffer::set_null(){
   ii=0;
}
void buffer::clean_up(){
  if(ii!=0){
    for(int i=0; i < 8; i++) delete []ii[i];
    delete [] ii;
  }
  set_null();
}
void buffer::calc_read_loop(int *nwview, int *fwview,int *nwio, int *fwio, int *nloop,int *ndim){
long long big=1;

  int nloc[8],floc[8];
   aios=hyper_io->return_axes(8);
   abufs=hyper_buf->return_axes(8);
  //Convert to read parameters to the disk file?? range
  for(int i=0; i< 8;i++){
    if(aios[i].n==1){ //I don't contaio this axis
      nloc[i]=1; floc[i]=0;
    }
    else if(nwview[i]==1){//Reading a single plane 
      float fmin=abufs[i].o+fwview[i]*abufs[i].d;
      floc[i]=(int)((fmin-aios[i].o)/aios[i].d+.5);
      if(floc[i] < 0) floc[i]=0;
      if(floc[i] > aios[i].n-1) floc[i]=aios[i].n-1;
      nloc[i]=1;
    
    }
    else{  //Reset to my coordinates
      float fmin=abufs[i].o;
      float fmax=fmin+abufs[i].d*(abufs[i].n-1);
      int imin=(int)((fmin-aios[i].o)/aios[i].d+.5); 
      if(imin<0) imin=0; if(imin>aios[i].n-1) imin=aios[i].n-1;
      int imax=(int)((fmax-aios[i].o)/aios[i].d+.5); 

      if(imax<0) imax=0; if(imax>aios[i].n-1) imax=aios[i].n-1;
    floc[i]=imin; nloc[i]=imax-imin+1;
    }
  
  }

  //We want to read in less than 50 million element sections 
  //(needs to be greater than the largest n1*n2 someone would read)
  int iax=0;
  long long maxs=50*1000*1000;  
  bool found=false;
  while(iax < 7 && !found){
    big=big*(long long)nloc[iax];
    if(big > maxs){
      found=true;
      big=big/(long long)nloc[iax];
    }
    else iax+=1;
  }

  if(!found){ //We are reading in everything
    for(int i=0; i < 8; i++){
      nwio[i]=nloc[i]; fwio[i]=floc[i];
      nloop[i]=1;
    }
    *ndim=8;
  }
  else{
    for(int i=0; i < iax; i++){
      nwio[i]=nloc[i]; fwio[i]=floc[i];
      nloop[i]=1; 
    }
    for(int i=iax; i<8; i++){
      nwio[i]=1; fwio[i]=0;
      nloop[i]=nloc[i];
    }
    *ndim=iax;
  }

 }
int buffer::resize_buffer(int *njunk,int *fjunk,int *nwio,int *fwio,int ndim, long long off,
    unsigned char *bufin,unsigned char *bufout,int ssz){
    
    if(ndim==0){}
    int nput=0;
    if(njunk==0 && fjunk==0) {}
  if(same){

     long long n123_view=1;
    
    for(int i=0; i < 8; i++) n123_view=n123_view*(long long)nwio[i]; 
    nput=n123_view;
    
    memcpy(bufout+off*ssz,bufin,ssz*n123_view);
  }
  else{
    int nwview[8],fwview[9];
    calc_resize(fwio,nwio,fwview,nwview);


    int nuse[8];
    for(int i=0; i < 8; i++) nuse[i]=nwview[i];
    long long iin,iout;

    int mult[8]; mult[0]=1;
    int shift[8];
    for(int i=1; i < 8; i++) mult[i]=mult[i-1]*nwio[i-1];



    iout=off;
    
   //   fwio[0],fwio[1],fwio[2],nwio[0],nwio[1],nwio[2]);
    //  fwview[0],fwview[1],fwview[2],nwview[0],nwview[1],nwview[2]);
    for(int i7=0; i7 < nuse[7]; i7++){
     shift[7]=ii[7][i7]*mult[7];
     for(int i6=0; i6< nuse[6]; i6++){
      shift[6]=shift[7]+ii[6][i6]*mult[6];
      for(int i5=0; i5< nuse[5]; i5++){
       shift[5]=shift[6]+ii[5][i5]*mult[5];
       for(int i4=0; i4<nuse[4]; i4++){
        shift[4]=shift[5]+ii[4][i4]*mult[4];
        for(int i3=0; i3<nuse[3]; i3++){
         shift[3]=shift[4]+ii[3][i3]*mult[3];
         for(int i2=0; i2<nuse[2]; i2++){
          shift[2]=shift[3]+ii[2][i2]*mult[2];
          for(int i1=0; i1<nuse[1]; i1++){
           shift[1]=shift[2]+ii[1][i1]*mult[1];
           for(int i0=0; i0<nuse[0]; i0++){
             iin=shift[1]+ii[0][i0];
             nput+=1;
             memcpy((bufout+(iout+nput)*(long long)ssz),(bufin+iin*(long long)ssz),ssz);
  
           }
          }
         }
        }
       }
      }
     }
    }
  }  
   return nput; 
    
}
long long *buffer::form_index_map(orient_cube *pos, int iax1,int iax2, 
 int f1, int e1, int f2, int e2){



long long *bb=pos->get_index_map_ptr(iax1,iax2,f1,e1,f2,e2,0);
 
 long long *in=new long long[abs((e1-f1)*(e2-f2))];
 memcpy(in,bb,abs((e1-f1)*(e2-f2))*sizeof(long long));
 if(same){
    return in;
  }
// int n=abs((pos->get_beg(iax1)-pos->get_end(iax1))*
// (pos->get_beg(iax2)-pos->get_end(iax2)));
 
 int n=abs((e1-f1)*(e2-f2));
 
 long long *out=new long long [n];
 if(resamp_1 && 3==3){
 int nin=pos->get_axis(0).n;
 int nout=skip[1];
 for(int i=0; i < n; i++){
 
   if(in[i]==-1) out[i]=-1;
   else{
   long long i2=in[i]/nin,i1=in[i]-i2*nin;
   out[i]=i2*nout+ii[0][i1];
 }
 }

  delete [] in;
     return out;
  }

 //We are in the case where we have different sized buffer than axes (this
 //could be done better by not assuming 8 day
 

 
 long long kip[8];
 kip[0]=1;
 for(int i=0; i <7; i++) {
   kip[i+1]=kip[i]*pos->get_axis(i).n;
   }
   
   for(int i=0; i < n; i++){
   long long v=in[i];
   int i7=ii[7][(int)(v/kip[7])]; v=v-kip[7]*(int)(v/kip[7]);
   int i6=ii[6][(int)(v/kip[6])]; v=v-kip[6]*(int)(v/kip[6]);
   int i5=ii[5][(int)(v/kip[5])]; v=v-kip[5]*(int)(v/kip[5]);
   int i4=ii[4][(int)(v/kip[4])]; v=v-kip[4]*(int)(v/kip[4]);
   int i3=ii[3][(int)(v/kip[3])]; v=v-kip[3]*(int)(v/kip[3]);

   int i2=ii[2][(int)(v/kip[2])]; v=v-kip[2]*(int)(v/kip[2]);
   int i1=ii[1][(int)(v/kip[1])]; 
   int i0=ii[0][(int)(v-kip[1]*(int)(v/kip[1]))];

   out[i]=i0+skip[1]*i1+skip[2]*i2+i3*skip[3]+i4*skip[4]+
             i5*skip[5]+skip[6]*i6+i7*skip[7];

 }
 delete [] in;
 return out;
}
/*
long long *buffer::form_index_map(orient_cube *pos, int iax1, int f1, int e1, int iax2,
    int f2, int e2){
 
  long long j1,j2,jax1=0,jax2=0;
  long long first;
  calc_get_pars(pos,iax1,iax2,&j1,&j2,&first);
  int n1=abs(e1-f1);
  int n2=abs(e2-f2);


     int dir1=1,dir2=1;
   if(f1>e1) {dir1=-1; f1-=1;}
   if(f2>e2) {dir2=-1; f2-=1;}
  long long *map=new long long [n1*n2];
  int i=0;
  //Impiclict assumption that the rotated axis is held....
 if(!pos->get_rotate()){
   if(same){
     for(int i2=0; i2 < n2; i2++){
          for(int i1=0; i1 < n1; i1++,i++){
            map[i]=(dir1*i1+f1)*j1+(dir2*i2+f2)*j2+first;
          }
        }
      }
      else{
        for(int i2=0; i2 < n2; i2++){
          int i_2=ii[iax2][dir2*i2+f2];
          for(int i1=0; i1 < n1; i1++,i++){
            int i_1=ii[iax1][f1+i1*dir1];
              map[i]=first+i_1*j1+i_2*j2;
          }
        }
      }
 }
 else{
   int **m1=pos->rot_to_reg_1,**m2=pos->rot_to_reg_2;
   int ia1,ia2;
   if(iax1 < iax2){ ia1=iax1; ia2=iax2; }
   else { ia1=iax2; ia2=iax1;}
     int b1,b2,e1,e2;
  b1=pos->get_beg(pos->rot_ax[0]);
  b2=pos->get_beg(pos->rot_ax[1]);
  e1=pos->get_end(pos->rot_ax[0]);
  e2=pos->get_end(pos->rot_ax[1]);
   if(ia1==pos->rot_ax[0] && ia2==pos->rot_ax[1]){
   
    
     int **t;
     if(iax1>iax2){ t=m1; m1=m2; m1=t;}
     if(ia1==iax1){
        for(int i2=0; i2 < n2; i2++){
          for(int i1=0; i1 < n1; i1++,i++){
            int ib=m2[i2*dir2+f2][i1*dir1+f1];
            int ia=m1[dir2*i2+f2][dir1*i1+f1];
            
            if(ia<b1 || ia>= e1 || ib <b2 || ib>= e2) map[i]=-1;
            else{
              int i_2=ii[iax2][ib];
              int i_1=ii[iax1][ia];
               map[i]=i_1*j1+i_2*j2+first;   
           //   fprintf(stderr,"FORMING MAP %d %d :%d %d: %d %d -%d\n",i1,i2,
              //  m2[i2*dir2+f2][i1*dir1+f1],m1[i2*dir2+f2][i1*dir1+f1],i_1,i_2,(int)map[i]);
             // map[i]=i_1*j1+i_2*j2+first;     
            }
          }
         
      }
      }
      else{
        for(int i2=0; i2 < n2; i2++){
          for(int i1=0; i1 < n1; i1++,i++){
            int ib=m1[i1*dir1+f1][i2*dir2+f2];
            int ia=m2[i1*dir1+f1][i2*dir2+f2];
       //     fprintf(stderr,"what the ia ib (%d %d %d) (%d %d %d) \n",
       //      b1,ia,e1,b2,ib,e2);
            if(ib<b1 || ib>= e1 || ia <b2 || ia>= e2) map[i]=-1;           
            else{
              int i_2=ii[iax2][ib];
              int i_1=ii[iax1][ia];
               map[i]=i_1*j1+i_2*j2+first;   
           //   fprintf(stderr,"FORMING MAP %d %d :%d %d: %d %d -%d\n",i1,i2,
              //  m2[i2*dir2+f2][i1*dir1+f1],m1[i2*dir2+f2][i1*dir1+f1],i_1,i_2,(int)map[i]);
             // map[i]=i_1*j1+i_2*j2+first;     
            }
          }
         
      }
      
      
      }
      }
      else{
  //      fprintf(stderr,"in one axis rotated \n");
        first=0;
        for(int iax=0; iax < 8; iax++){
         if(iax==pos->rot_ax[0]) jax1=skip[iax];
         else if(iax==pos->rot_ax[1]) jax2=skip[iax];
         else if(iax2==iax) j2=skip[iax];
         else if(iax1==iax) j1=skip[iax];
         else{
           if(hold[iax]){
             if(same){
               first=first+skip[iax]*pos->get_loc(iax);
             }
             else{
               first=first+skip[iax]*ii[iax][pos->get_loc(iax)];
             }
            }
          }
        }
      //  fprintf(stderr,"constructed js %d %d %d %d \n",j1,j2,jax1,jax2);
        if(iax1==pos->rot_ax[0] || iax1==pos->rot_ax[1]){
        if(iax1==pos->rot_ax[0]){
             int   ir1=pos->get_loc(pos->rot_ax[1]);
             int   ioth=pos->rot_ax[1];
          
            for(int i2=0; i2 < n2; i2++){
              for(int i1=0; i1 < n1; i1++,i++){
                int ib=m2[ir1][i1*dir1+f1];
                int ia=m1[ir1][i1*dir1+f1];
                if(ia<b1 || ia>= e1 || ib < b2 || ib>=e2) {
                
                   map[i]=-1;
             //       if(i1==1)fprintf(stderr,"map is 0 %d %d  \n",i2,i2*dir2+f2);
                   }
                else{
                 int i_1=ii[iax1][ia];
                 int i_2=ii[iax1][i2*dir2+f2];
                 int i_3=ii[ioth][ib];
                 map[i]=first+jax1*i_1+j2*i_2+i_3*jax2;
                 
                 
                 
                 
               //  map[i]=first+i_1*j2+i_2*jax2+i_3*jax1;
                }
              }
            }
          
          
          
          
          }
          else{
           // fprintf(stderr,"displaying second rotated axis \n");
         
             int  ir1=pos->get_loc(pos->rot_ax[0]);
             int  ioth=pos->rot_ax[0];
            
        
            for(int i2=0; i2 < n2; i2++){
              for(int i1=0; i1 < n1; i1++,i++){
                int ib=m2[i1*dir1+f1][ir1];
                int ia= m1[i1*dir1+f1][ir1];
              if(ia<b1 || ia>= e1 || ib < b2 || ib>=e2) map[i]=-1;
                else{
                 int i_1=ii[iax2][ib];
                 int i_2=ii[iax1][i2*dir2+f2];
                 int i_3=ii[ioth][ia];
                 map[i]=first+jax2*i_1+i_2*j2+i_3*jax1;
                 
                 
                 
                 
                 
                 
              //   map[i]=first+i_1*j2+i_2*jax2+i_3*jax1;
                }
              }
            }
          
       }
      
          
       }
       else{
          if(iax2==pos->rot_ax[0]){
             int   ir1=pos->get_loc(pos->rot_ax[1]);
             int   ioth=pos->rot_ax[1];
        //  fprintf(stderr,"displaying first rotated axis %d %d (%d)\n",ir1,ioth,n2);
          
            for(int i2=0; i2 < n2; i2++){
              for(int i1=0; i1 < n1; i1++,i++){
               int ia=m1[ir1][i2*dir2+f2];
               int ib=m2[ir1][i2*dir2+f2];
               if(ia<b1 || ia>= e1 || ib < b2 || ib>=e2) map[i]=-1;
                else{
                 int i_2=ii[iax2][ia];
                 int i_1=ii[iax1][i1*dir1+f1];
                 int i_3=ii[ioth][ib];
                 map[i]=first+i_1*j1+i_2*jax1+i_3*jax2;
               // if(i1==1) fprintf(stderr,"map is %d [%d] (%d*%d %d*%d %d*%d) %d \n",
                //  m1[ir1][i2*dir2+f2],i2,
                  //i_1,j1,i_2,jax1,i_3,jax2,map[i]);  
                }
               
              }
            }
          
          
          
          
          }
          else{
           // fprintf(stderr,"displaying second rotated axis \n");
         
             int  ir1=pos->get_loc(pos->rot_ax[0]);
             int  ioth=pos->rot_ax[0];
            
        
            for(int i2=0; i2 < n2; i2++){
              for(int i1=0; i1 < n1; i1++,i++){
                int ia=m1[i2*dir2+f2][ir1];
                int ib=m2[i2*dir2+f2][ir1];
                if(ia<b1 || ia>= e1 || ib < b2 || ib>=e2) map[i]=-1;
                else{
                 int i_2=ii[iax2][ib];
                 int i_1=ii[iax1][i1*dir1+f1];
                 int i_3=ii[ioth][ia];
                 map[i]=first+i_1*j1+i_2*jax2+i_3*jax1;
                }
              }
            }
          
       }
      
       
       }
       
       }
     
  }
  return map;
    
 }
 */
void buffer::calc_get_pars(orient_cube *pos, int iax1, int iax2, long long *j1, long long *j2,
  long long *first){
 

 *first=0;
 for(int i=0; i < 8; i++){
   if(i==iax1) *j1=skip[i];
   else if(i==iax2) *j2=skip[i];
   else{
     if(hold[i]){
       if(same){
       *first=*first+
         skip[i]*
         pos->get_loc(i);
       
         }
         else{
          *first=*first+
         skip[i]*
          ii[i][pos->get_loc(i)];
         }
     }
   }
  }
  
 }
 void buffer::set_hold(bool *h){
   for(int i=0; i < 8; i++) hold[i]=h[i];
 }
 void buffer::window_to_local(int *nwview, int *fwview){

   n123_view=1;
   n123_buf=1;
   long long j=1;
   for(int i=0; i < 8; i++){
     int nwio=hyper_io->get_axis(i+1).n;
     n123_view=n123_view*(long long)nwview[i];
     n123_buf=n123_buf*(long long)nwio;
     skip[i]=j;
     
     if(nwview[i] >1){
       hold[i]=true;
      // j=j*nwview[i];
       j=j*nwio;
      }
      else{
        hold[i]=false;
        loc[i]=fwview[i];
      }
    }
   }
 long long buffer::point_to_local(position *pos){
   long long iloc=0;
   
   if(same){
     for(int i=0; i < 8; i++){
       if(hold[i]) iloc=iloc+(long long)pos->get_loc(i)*skip[i];
     }
   }
   else{
     for(int i=0; i < 8; i++){
       if(hold[i]) iloc=iloc+(long long)ii[i][pos->get_loc(i)]*skip[i];
     }
   }
   return iloc;
 }
 bool buffer::hold_point(float *pos_loc, bool *use_axis){
   for(int i=0; i <8; i++){
      
     if(!hold[i] && pos_loc[i] !=loc[i] && use_axis[i] && hyper_buf->get_axis(i+1).n>1){
     
       std::cout << "FAILING "<<i<<" hold="<<hold[i]<<" "<<pos_loc[i]<<"!="<<loc[i]<<" "<<use_axis[i]<<std::endl;
      // std::cout << "FAILING "<<hold[i]<<" "<<pos->get_loc(i)<<" "<<loc[i]<<" " << use_axis[i]<<endl;
     return false;
     }
   }
   return true;
 }
 float *buffer::return_histo(){
   float *tmp=new float[256];
   memcpy((void*)tmp,(const void*)histo,256*sizeof(float));
   return tmp;
 }
void buffer::calc_resize(int *fio, int *nio, int *fbuf, int *nbuf){



  for(int i=0;i < 8; i++){  
    nbuf[i]=nio[i];
    fbuf[i]=fio[i];
  }
  
if(!same){
  for(int iax=0; iax < 8; iax++){
  bool first=true;
  if(hold[iax]){ 
   for(int i=0; i < abufs[iax].n; i++){
     if(ii[iax][i] >= fio[iax] && ii[iax][i] < fio[iax]+nio[iax]){
       if(first){
         first=false;
         nbuf[iax]=0;
         fbuf[iax]=i;
       } 
       nbuf[iax]+=1;

     }

   }

 }
  }
}

}


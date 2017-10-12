#include "float_buffer.h"
#include<iostream>
#include<stdlib.h>
using namespace SEP;
//Initialize and read in buffer
float_buffer::float_buffer(std::shared_ptr<paramObj>p, std::shared_ptr<hypercube>h,std::shared_ptr<io_func>i,int in,std::vector<int>&nwbuf, std::vector<int>&fwbuf){
 
    set_basics(p,h,i,in);

     window_to_local(nwbuf,fwbuf);
    fbuf=new float [n123_buf];

    int ndim;
    std::vector<int> nwio(8,1),fwio(8,0),nloop(8,1);
    calc_read_loop(nwbuf,fwbuf,nwio,fwio,nloop,ndim);
    read_buffer(nwbuf,fwbuf,nwio,fwio,ndim,nloop);
float minv, maxv;
          io->return_clips(&minv,&maxv);

 }
 
 void float_buffer::read_buffer(std::vector<int>&nwbuf, std::vector<int>&fwbuf,std::vector<int>&nwio, std::vector<int>&fwio, int ndim ,std::vector<int>&nloop){
 

   long long nblock=1,off=0;
   long long block=1;
   for(int i=0; i< ndim; i++) block=block*(long long) nwbuf[i];
   for(int i=0; i< ndim; i++) nblock=nblock*(long long)nwio[i];
   long long count=0;
   std::vector<int>fsend(8,0);fsend[0]=fwbuf[0];        fsend[1]=fwbuf[1];
   for(int i7=0; i7 < nloop[7];i7++){     fsend[7]=fwbuf[7]+i7;
    for(int i6=0; i6 < nloop[6];i6++){   fsend[6]=fwbuf[6]+i6;
     for(int i5=0; i5 < nloop[5];i5++){   fsend[5]=fwbuf[5]+i5;
      for(int i4=0; i4 < nloop[4];i4++){  fsend[4]=fwbuf[4]+i4;
       for(int i3=0; i3 < nloop[3];i3++){ fsend[3]=fwbuf[3]+i3;
        for(int i2=0; i2 < nloop[2];i2++){fsend[2]=fwbuf[2]+i2;
         for(int i1=0; i1 < nloop[1];i1++){fsend[1]=fwbuf[1]+i1;

          io->read_block_float(nwio,fsend,(float*)(fbuf+count*nblock));
       //   off+=resize_buffer(nwbuf,fwbuf,nwio,fsend,ndim,
        //   off,(unsigned char*)tbuf,(unsigned char*)fbuf,(int)sizeof(float));
          off+=nblock;
          count+=1;

        }
       }
      }
     }
    }
   }      
 }
 if(off >10000000) off=10000000;
 if(io->not_byte()){
   fprintf(stderr,"CHECM ME =%d d=%d \n",inum,off);
   io->set_clip(fbuf,inum,off);
   io->return_clips(&bclip,&eclip);
  
 }

 //srite("x22.H",fbuf,1000*250*4);
 calc_histo();
}
 unsigned char *float_buffer::get_char_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2, int f2, int e2){
      if(!hold[iax1] || !hold[iax2]) _par->error("Internal error don't hold axes requested");
      int n1=abs(e1-f1), n2=abs(e2-f2);
         if(pos->get_rotate() && (!hold[pos->rot_ax[0]] || !hold[pos->rot_ax[1]])){
      fprintf(stderr,"Must hold rotated axes. Defaulting to no rotation.\n");
      pos->set_no_rotate();
   }
   long long *index=form_index_map(pos,iax1,iax2,f1,e1,f2,e2);

      
       unsigned char *out=get_char_data(n1*n2,index);
       delete [] index;
       return out;
    
    
    }

unsigned char *float_buffer::get_char_data(int n, long long *index){
            unsigned char *out=new unsigned char[n];
     float minv,maxv;
      io->return_clips(&minv,&maxv);
      float  j=maxv-minv;
    for(int i=0 ; i < n; i++){
      if(index[i] <0) {
         out[i]=0;
      }
      else{
        int  b=(int)(255*(fbuf[index[i]]-minv)/j);
        if(b<0) b=0; if(b>255) b=255;
        out[i]=b;
      }
   }
  
  //  delete [] index;
    return out; 
  }   
   
float *float_buffer::get_float_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,
    int f2, int e2){
    if(!hold[iax1] || !hold[iax2]) _par->error("Internal error don't hold axes requested");
       if(pos->get_rotate() && (!hold[pos->rot_ax[0]] || !hold[pos->rot_ax[1]])){
      fprintf(stderr,"Must hold rotated axes. Defaulting to no rotation.\n");
      pos->set_no_rotate();
   }
        int n1=abs(e1-f1), n2=abs(e2-f2);

      long long *index=form_index_map(pos,iax1,iax2,f1,e1,f2,e2);
      float *out=new float[n1*n2];
    float minv,maxv;
    io->return_clips(&minv,&maxv);
    for(int i=0 ; i < n1*n2; i++){
      if(index[i] <0) {
         out[i]=minv;
      }
      else{
        out[i]=fbuf[index[i]];
      }
   }
    
    delete [] index;
  
    return out;

  }

 void float_buffer::calc_histo(){
   
   long long count[256];
   for(int i=0; i < 256; i++) count[i]=0;
   io->return_clips(&bclip,&eclip);
   float j=eclip-bclip;
   int b;
   for(long long i=0; i < n123_view; i++){
      b=(int)(255*(fbuf[i]-bclip)/j+.5);
      if(b<0) b=0;
      else if(b>255) b=255;
      count[b]++;
   }
   long long mym=0;
   for(int i=0; i < 256; i++){
     if(count[i] > mym) mym=count[i];
   }
   for(int i=0; i < 256; i++){
     histo[i]=(float)count[i]/(float)mym;
   }
 }

 float float_buffer::get_value(std::shared_ptr<orient_cube>pos){
   return fbuf[point_to_local(pos)];
 
 }

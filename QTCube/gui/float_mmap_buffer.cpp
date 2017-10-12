#include "float_mmap_buffer.h"
#include<iostream>
using namespace SEP;
//Initialize and read in buffer
float_mmap_buffer::float_mmap_buffer(std::shared_ptr<paramObj>p, std::shared_ptr<hypercube>h,std::shared_ptr<io_func>i,int in,
std::vector<int>&nwbuf, std::vector<int>&fwbuf){
 
    set_basics(p,h,i,in);

     window_to_local(nwbuf,fwbuf);
     //fbuf=new float [n123_buf];
    fbuf=(float*)std::static_pointer_cast<sep_reg_mmap_io>(i)->map; 

    std::vector<int> nwio(8,1),fwio(8,1),nloop(8,1);
    int ndim;
    calc_read_loop(nwbuf,fwbuf,nwio,fwio,nloop,ndim);

    swap=p->getInt(std::string("swap")+std::to_string(inum),0);

    read_buffer(nwbuf,fwbuf,nwio,fwio,ndim,nloop);


   // io->return_clips(&minv,&maxv);
  

 }
 
 void float_mmap_buffer::read_buffer(std::vector<int>&nwbuf, std::vector<int>&fwbuf,std::vector<int>&nwio, std::vector<int>&fwio, int ndim ,std::vector<int>&nloop){
 
   if(nwbuf[0]==0 && fwbuf[0]==0 && fwio[0]==0 && ndim==0 && nwio[0]==0 && nloop[0]==0){;}
   long long n=4;
   for(int i=0; i < 8; i++)
     n=(long long) hyper_io->getAxis(i+1).n*n;
     
     if(n > 50*1000*1000)n=50*1000*1000;
   io->set_clip(fbuf,inum,n,swap);
   io->return_clips(&bclip,&eclip);
 //calc_histo();
}
 unsigned char *float_mmap_buffer::get_char_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1,
  int iax2, int f2, int e2){
      if(!hold[iax1] || !hold[iax2]) _par->error("Internal error don't hold axes requested");
         if(pos->get_rotate() && (!hold[pos->rot_ax[0]] || !hold[pos->rot_ax[1]])){
      fprintf(stderr,"Must hold rotated axes. Defaulting to no rotation.\n");
      pos->set_no_rotate();
   }

      int n1=abs(e1-f1), n2=abs(e2-f2);
      unsigned char *out=new unsigned char[n1*n2];
      form_index_map(pos,iax1,iax2,f1,e1,f2,e2);
       long long *index=form_index_map(pos,iax1,iax2,f1,e1,f2,e2);
      float *tmpf=new float [n1*n2];
          float minv,maxv;
    io->return_clips(&minv,&maxv);
          float j=(maxv-minv)/255;

    for(int i=0 ; i < n1*n2; i++){
       if(index[i]<0) tmpf[i]=0;
        tmpf[i]=fbuf[index[i]];
      }

       if(swap==1) _util->swap_float_bytes(n1*n2,tmpf);

     for(int i=0; i < n1*n2; i++){
     //fprintf(stderr,"INDEX VAL %ld %f \n",index[i],tmpf[i]);
          if(index[i]<0) out[i]=0;
         else{
           int b=(int)((tmpf[i]-minv)/j+.5);
     //if(i==1000) fprintf(stderr,"IN HERE 3 %f %ld %d %f %f\n",tmpf[i],index[i],b,
     //tmpf[i]-minv,j);
           
          if(b<0) out[i]=0;
          else if(b >255) out[i]=255;
          else out[i]=b;
         }
    }
    delete [] tmpf;
   // delete [] index;
 
    return out; 
  }   
   
float *float_mmap_buffer::get_float_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,
    int f2, int e2){
    if(!hold[iax1] || !hold[iax2]) _par->error("Internal error don't hold axes requested");
       if(pos->get_rotate() && (!hold[pos->rot_ax[0]] || !hold[pos->rot_ax[1]])){
      fprintf(stderr,"Must hold rotated axes. Defaulting to no rotation.\n");
      pos->set_no_rotate();
   }
     int n1=abs(e1-f1), n2=abs(e2-f2);
      float *out=new float[n1*n2];
      long long *index=form_index_map(pos,iax1,iax2,f1,e1,f2,e2);
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

 void float_mmap_buffer::calc_histo(){
   
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

 float float_mmap_buffer::get_value(std::shared_ptr<orient_cube>pos){
   return fbuf[point_to_local(pos)];
 
 }

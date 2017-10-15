#include "incore_data.h"
#include "float_buffer.h"
#include "float_mmap_buffer.h"
#include "byte_buffer.h"
#include "sregf.h"
incore_data_float::incore_data_float(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int
mmap,int im){
    mmap_it=mmap;

  set_basics(title,nm,g,i,p,in,im);

    datas=io->return_hyper();
 
}

buffer *incore_data_float::create_buffer(orient_cube *pos, int iax1, int iax2){

   int nw[8],fw[8];
   std::vector<axis> axes=grid->return_axes(8);
   for(int i=0; i< 8; i++){
     nw[i]=axes[i].n;
     fw[i]=0;
   }

   if(iax1==0 && iax2==0 && pos==0){}
   if(mmap_it==1){
      float_mmap_buffer *b=new float_mmap_buffer(par,grid,io,inum,nw,fw);
      return b;
   }
    float_buffer *b=new float_buffer(par,grid,io,inum,nw,fw);
     return b;
}

incore_data_byte::incore_data_byte(std::string title,QString nm, hypercube *g,io_func *i, param_func *p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
  datas=io->return_hyper();

}
buffer *incore_data_byte::create_buffer(orient_cube *pos, int iax1, int iax2){
 
 
   int nw[8],fw[8];
   std::vector<axis> axes=grid->return_axes(8);
   for(int i=0; i< 8; i++){
     nw[i]=axes[i].n;
     fw[i]=0;
   }

   char param[256],def[256];
   strcpy(def,"NONE");
   sprintf(param,"%s.map_1",name.ascii());
  
   std::string val=par->get_string(param,def);
   if(val!="NONE"){
      sregf fl=sregf(val);
     int ns[8]; pos->get_ns(ns);
     long long sz=1;
     for(int i=0; i < 8; i++) sz=sz*(long long)ns[i];
     if(sz!=fl.get_n123()) {
       char errmsg[] = "file and grid size don't match";
       par->error(errmsg);
     }
     char var[] = "oversamp";
     float oversamp=par->get_float(var,1.);
     hypercube_float buf=hypercube_float(&fl);
     map=new int[sz];
     fl.read_all(val,&buf);
     long long i=0;
     for(long long i2=0; i2 < sz/(long long)ns[0]; i2++){
       for(int i1=0 ;   i1 < ns[0]; i1++,i++){
      if(1==1){
         map[i]=(int)(oversamp*buf.vals[i]);
         if(map[i] < 0) map[i]=0;
         if(map[i]>= ns[0]) map[i]=ns[0]-1;

      }
      if(1==3){  
        int iref=1;
         map[i]=(int)(-buf.vals[i]+buf.vals[iref*(long long)ns[0]+i1]+.5+i1);
      //   if(i2==40336) fprintf(stderr,"CHECK vAL %d %f %f %d \n",i1,buf.vals[i],
       //   buf.vals[iref*(long long)ns[0]+i1],map[i]);
        // map[i]=map[i]/2;
         if(map[i] < 0) map[i]=0;
         if(map[i]>= ns[0]) map[i]=ns[0]-1;
         // float v=map[i];
        }
        
        
       }
     }
     pos->set_one_shift(0,map);
   }
   if(iax1==0 && iax2==0 && pos==0){}
    byte_buffer *b=new byte_buffer(par,grid,io,inum,nw,fw);
   return b;
}

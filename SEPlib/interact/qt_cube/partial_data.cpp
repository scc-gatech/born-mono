#include "partial_data.h"
#include "float_buffer.h"
#include "byte_buffer.h"


partial_data_float::partial_data_float(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
  

}
buffer *partial_data_float::create_buffer(orient_cube *pos, int iax1, int iax2){
 
   int nw[8],fw[8];
   std::vector<axis> axes=grid->return_axes(8);
   for(int i=0; i< 8; i++){
     if(!data_contains[i]){

       nw[i]=1;
       fw[i]=0;
     }
     else{

       nw[i]=axes[i].n;
       fw[i]=0;
     }
   }
   
   if(iax1==0 && iax2==0 && pos==0){}
    float_buffer *b=new float_buffer(par,grid,io,inum,nw,fw);
   
   return b;
}

partial_data_byte::partial_data_byte(std::string title,QString nm, hypercube *g,io_func *i, param_func *p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
  
}
void partial_data::set_contains(){

 
  datas=io->return_hyper();
  std::vector<axis> adat=io->return_hyper()->return_axes(8);
  std::vector<axis> axes=grid->return_axes(8);
     for(int i=0; i < 8; i++){
       if(axes[i].n>1 && adat[i].n==1) data_contains[i]=false;
        if(axes[i].n>1 && adat[i].n==1) display_axis[i]=false;
     }



}
buffer *partial_data_byte::create_buffer(orient_cube *pos, int iax1, int iax2){

    int nw[8],fw[8];
   std::vector<axis> axes=grid->return_axes(8);
   for(int i=0; i< 8; i++){
     if(!data_contains[i]){

       nw[i]=1;
       fw[i]=0;
     }
     else{

       nw[i]=axes[i].n;
       fw[i]=0;
     }
   }
   if(iax1==0 && iax2==0 && pos==0){}
    byte_buffer *b=new byte_buffer(par,grid,io,inum,nw,fw);
   return b;
}

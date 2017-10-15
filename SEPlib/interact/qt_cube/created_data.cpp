#include "created_data.h"
#include "float_buffer.h"
#include "byte_buffer.h"
#include <iostream>

created_data_float::created_data_float(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
 
}
void created_data::set_contains(){
  datas=io->return_hyper();
    std::vector<axis> adat=io->return_hyper()->return_axes(8);
    std::vector<axis> axes=grid->return_axes(8);
     for(int i=0; i < 8; i++){
       if(axes[i].n>1 && adat[i].n==1) {
         data_contains[i]=display_axis[i]=false;
       }
       else {
         data_contains[i]=display_axis[i]=true;
      }
     }

}

bool created_data::match_grid(){
  hypercube *iod=io->return_hyper();
  bool valid=true;
  for(int i=0; i < 8 ; i++){
    if(data_contains[i]){
       float bg,eg,bd,ed;
       axis ga=grid->get_axis(i+1);
       axis da=iod->get_axis(i+1);
       if(da.n >1){      
         bg=ga.o; eg=ga.o+ga.d*(ga.n-1);
         bd=da.o; ed=da.o+da.d*(da.n-1);
       
         if(((bg-bd)/ga.d) >.01){
            fprintf(stderr,"Axis %d data axis (%f) begins before grid axis (%f) \n",
              i+1,bd,bg);
            valid=false;
         }
         if(((ed-eg)/ga.d) > .01) {
            fprintf(stderr,"Axis %d data axis (%f) ends after grid axis (%f) \n",
              i+1,ed,eg);
            valid=false;
         }
      }
    }
 }
 return valid;
}
buffer *created_data_float::create_buffer(orient_cube *pos, int iax1, int iax2){
 
   int nw[8],fw[8];
   std::vector<axis> axes=grid->return_axes(8);
   /*bool hold[8];*/
   for(int i=0; i< 8; i++){
     /*hold[i]=true;*/
     if(!display_axis[i]){

       nw[i]=1;
       fw[i]=pos->get_loc(i);
      
     }
     else{

       nw[i]=axes[i].n;
       fw[i]=0;
     }
   }
   
   if(iax1==0 && iax2==0 && pos==0){}
    float_buffer *b=new float_buffer(par,grid,io,inum,nw,fw);
   // b->set_hold(hold);
   

   return b;
}

created_data_byte::created_data_byte(std::string title,QString nm, hypercube *g,io_func *i, param_func *p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);

}
buffer *created_data_byte::create_buffer(orient_cube *pos, int iax1, int iax2){

    int nw[8],fw[8];
    /*bool hold[8];*/
   std::vector<axis> axes=grid->return_axes(8);
   for(int i=0; i< 8; i++){
     /*hold[i]=true;*/
     if(!display_axis[i]){

       nw[i]=1;
       fw[i]=pos->get_loc(i);
     }
     else{

       nw[i]=axes[i].n;
       fw[i]=0;
     }
   }
   if(iax1==0 && iax2==0 && pos==0){}
    byte_buffer *b=new byte_buffer(par,grid,io,inum,nw,fw);
   //   b->set_hold(hold);

   return b;
}

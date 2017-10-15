#include "part_semb_data.h"
#include "float_buffer.h"
#include "byte_buffer.h"
#include "interval_pick.h"

part_semb_data_byte::part_semb_data_byte(std::string title,QString nm,hypercube *g,io_func *i,
  int i_t,int i_off, int i_mes, pick_draw *_pk, QString c1, QString c2,param_func *p, int in,int imaa){
  set_basics(title,nm,g,i,p,in,imaa);
  datas=io->return_hyper();
  it=i_t; im=i_mes;ioff=i_off;
  pks=_pk; col1=c1; col2=c2;
    dumb=new orient_cube(grid); 
   for(int i=0; i < 8; i++) {
    axis a=grid->get_axis(i+1);
    ds[i]=a.d;
  }
  
}
 void part_semb_data_byte::update_vel(int *loc){
   for(int i=0; i < 8; i++) dumb->set_loc(i,loc[i]);
   
   
   interval_pick::form_interval_vel(dumb,grid,ds,pks,col1,col2,im,it,ioff);
}


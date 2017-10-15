#include "outcore_data.h"
#include "float_buffer.h"
#include "byte_buffer.h"

void outcore_data::hold_pattern(orient_cube *pos, int iax1, int iax2, bool *hold){

 for(int i=0; i < 8; i++) hold[i]=false;
 int order[8];
 pos->get_orders(order);
 hold[iax1]=hold[iax2]=true;
 if(iax1 !=order[0] && iax2!=order[0]) hold[order[0]]=true;
 else if(iax1!=order[1] && iax2!=order[1]) hold[order[1]]=true;
 else hold[order[2]]=true;
 
}
void outcore_data::delete_dataset(orient_cube *pos, int iax1, int iax2){

   bool hold[8],hh[8];
   hold_pattern(pos,iax1,iax2,hold);
   bool found=false;
   
   int i=0;
   while(i< (int) buf.size() && !found){

     buf[i]->return_hold(hh);
     bool test=true;
     std::vector<axis> axes=buf[i]->hyper_io->return_axes(8);
     for(int j=0; j< 8; j++) if(hold[j]!=hh[j] && axes[j].n >1) test=false;


     if(test){
       delete buf[i];
       buf.erase(buf.begin()+i);
       found=true;
          fprintf(stderr,"deleting dataset %d of %d \n",i,(int)buf.size());

     }
     i+=1;
   }
   if(!found){
      fprintf(stderr,"deleting dataXset 0 of %d \n",(int)buf.size());

     delete buf[0];
     buf.erase(buf.begin());
   }
}

outcore_data_float::outcore_data_float(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
    datas=io->return_hyper();

}


buffer *outcore_data_float::create_buffer(orient_cube *pos, int iax1, int iax2){
 
   int nw[8],fw[8];
   bool hold[8];
   
   hold_pattern(pos,iax1,iax2,hold);
   std::vector<axis> axes=grid->return_axes(8);

   
   for(int i=0; i < 8; i++){
     if(!hold[i]){
       nw[i]=1;
       fw[i]=pos->get_loc(i);
     }
     else{
       nw[i]=axes[i].n;
       fw[i]=0;
     }
     }
     

    float_buffer *b=new float_buffer(par,grid,io,inum,nw,fw);
   

   return b;
}

outcore_data_byte::outcore_data_byte(std::string title,QString nm, hypercube *g,io_func *i, param_func *p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
  datas=io->return_hyper();

}
buffer *outcore_data_byte::create_buffer(orient_cube *pos, int iax1, int iax2){
 
 
   int nw[8],fw[8];
   std::vector<axis> axes=grid->return_axes(8);
   bool hold[8];
      hold_pattern(pos,iax1,iax2,hold);

   for(int i=0; i< 8; i++){
   if(!hold[i]){
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

   return b;
}

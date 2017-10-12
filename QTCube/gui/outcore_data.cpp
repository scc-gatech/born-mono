#include "outcore_data.h"
#include "float_buffer.h"
#include "byte_buffer.h"
using namespace SEP;
void outcore_data::hold_pattern(std::shared_ptr<orient_cube>pos, int iax1, int iax2, bool *hold){

 for(int i=0; i < 8; i++) hold[i]=false;
 int order[8];
 pos->get_orders(order);
 hold[iax1]=hold[iax2]=true;
 if(iax1 !=order[0] && iax2!=order[0]) hold[order[0]]=true;
 else if(iax1!=order[1] && iax2!=order[1]) hold[order[1]]=true;
 else hold[order[2]]=true;
 
}
void outcore_data::delete_dataset(std::shared_ptr<orient_cube> pos, int iax1, int iax2){

   bool hold[8],hh[8];
   hold_pattern(pos,iax1,iax2,hold);
   bool found=false;
   
   int i=0;
   while(i< (int) buf.size() && !found){

     buf[i]->return_hold(hh);
     bool test=true;
     std::vector<axis> axes=buf[i]->hyper_io->getAxes();
          for(int i=axes.size(); i < 8; i++) axes.push_back(axis(1));

     for(int j=0; j< 8; j++) if(hold[j]!=hh[j] && axes[j].n >1) test=false;


     if(test){
       buf.erase(buf.begin()+i);
       found=true;
          fprintf(stderr,"deleting dataset %d of %d \n",i,(int)buf.size());

     }
     i+=1;
   }
   if(!found){
      fprintf(stderr,"deleting dataXset 0 of %d \n",(int)buf.size());

     buf.erase(buf.begin());
   }
}

outcore_data_float::outcore_data_float(std::string title,QString nm,std::shared_ptr<hypercube>g,std::shared_ptr<io_func>i, std::shared_ptr<paramObj>p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
    datas=io->return_hyper();

}


std::shared_ptr<buffer> outcore_data_float::create_buffer(std::shared_ptr<orient_cube>pos, int iax1, int iax2){
 
   std::vector<int> nw(8,1),fw(8,0); 
   bool hold[8];
   
   hold_pattern(pos,iax1,iax2,hold);
   std::vector<axis> axes=grid->getAxes();
   for(int i=axes.size(); i<8; i++) axes.push_back(axis(1));
   
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
     

    std::shared_ptr<float_buffer>b(new float_buffer(par,grid,io,inum,nw,fw));
   

   return b;
}

outcore_data_byte::outcore_data_byte(std::string title,QString nm, std::shared_ptr<hypercube>g,std::shared_ptr<io_func >i,std::shared_ptr< paramObj>p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
  datas=io->return_hyper();

}
std::shared_ptr<buffer>outcore_data_byte::create_buffer(std::shared_ptr<orient_cube>pos, int iax1, int iax2){
 
 
    std::vector<int> nw(8,1),fw(8,1);
   std::vector<axis> axes=grid->getAxes();
        for(int i=axes.size(); i < 8; i++) axes.push_back(axis(1));

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

   if(iax1==0 && iax2==0 && pos==0);
    std::shared_ptr<byte_buffer> b(new byte_buffer(par,grid,io,inum,nw,fw));

   return b;
}

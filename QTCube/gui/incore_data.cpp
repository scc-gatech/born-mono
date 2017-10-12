#include "incore_data.h"
#include "float_buffer.h"
#include "float_mmap_buffer.h"
#include "byte_buffer.h"
using namespace SEP;
incore_data_float::incore_data_float(std::string title,QString nm,std::shared_ptr<hypercube >g,std::shared_ptr<io_func >i, std::shared_ptr<paramObj >p, int in,int
mmap,int im){
    mmap_it=mmap;

  set_basics(title,nm,g,i,p,in,im);

    datas=io->return_hyper();
 
}

buffer *incore_data_float::create_buffer(orient_cube *pos, int iax1, int iax2){

    std::vector<int> nw(8,1),fw(8,1);
   std::vector<axis> axes=grid->getAxes(8);
   for(int i=0; i< 8; i++){
     nw[i]=axes[i].n;
     fw[i]=0;
   }

   if(iax1==0 && iax2==0 && pos==0){;}
  
    float_buffer *b=new float_buffer(par,grid,io,inum,nw,fw);
     return b;
}

incore_data_byte::incore_data_byte(std::string title,QString nm, std::shared_ptr<hypercube >g,std::shared_ptr<io_func >i, std::shared_ptr<paramObj >p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
  datas=io->return_hyper();

}
std::shared_ptr<buffer>incore_data_byte::create_buffer(std::shared_ptr<orient_cube >pos, int iax1, int iax2){
 
 
    std::vector<int> nw(8,1),fw(8,1);
   std::vector<axis> axes=grid->getAxes(8);
   for(int i=0; i< 8; i++){
     nw[i]=axes[i].n;
     fw[i]=0;
   }

  
   
   if(iax1==0 && iax2==0 && pos==0){;}
    std::shared_ptr<byte_buffer> b(new byte_buffer(par,grid,io,inum,nw,fw));
   return b;
}

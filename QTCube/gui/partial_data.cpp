#include "partial_data.h"
#include "float_buffer.h"
#include "byte_buffer.h"

using namespace SEP;
partial_data_float::partial_data_float(std::string title,QString nm,std::shared_ptr<hypercube>g,std::shared_ptr<io_func>i, std::shared_ptr<paramObj>p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
  

}
std::shared_ptr<buffer> partial_data_float::create_buffer(std::shared_ptr<orient_cube>pos, int iax1, int iax2){
 
    std::vector<int> nw(8,1),fw(8,1);
   std::vector<axis> axes=grid->getAxes();
   for(int i=axes.size(); i < 8; i++) axes.push_back(axis(1));
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
   
   if(iax1==0 && iax2==0 && pos==0){;}
    std::shared_ptr<float_buffer>b(new float_buffer(par,grid,io,inum,nw,fw));
   
   return b;
}

partial_data_byte::partial_data_byte(std::string title,QString nm, std::shared_ptr<hypercube>g,std::shared_ptr<io_func>i, std::shared_ptr<paramObj>p, int in,int im){
  set_basics(title,nm,g,i,p,in,im);
  
}
void partial_data::set_contains(){

 
  datas=io->return_hyper();
  std::vector<axis> adat=io->return_hyper()->getAxes();
  std::vector<axis> axes=grid->getAxes();
       for(int i=axes.size(); i < 8; i++) adat.push_back(axis(1));

     for(int i=axes.size(); i < 8; i++) axes.push_back(axis(1));
     for(int i=0; i < 8; i++){
       if(axes[i].n>1 && adat[i].n==1) data_contains[i]=false;
        if(axes[i].n>1 && adat[i].n==1) display_axis[i]=false;
     }



}
std::shared_ptr<buffer> partial_data_byte::create_buffer(std::shared_ptr<orient_cube>pos, int iax1, int iax2){

    std::vector<int> nw(8,1),fw(8,1);
   std::vector<axis> axes=grid->getAxes();
      for(int i=axes.size(); i < 8; i++) axes.push_back(axis(1));
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
   if(iax1==0 && iax2==0 && pos==0){;}
    std::shared_ptr<byte_buffer>b(new byte_buffer(par,grid,io,inum,nw,fw));
   return b;
}
